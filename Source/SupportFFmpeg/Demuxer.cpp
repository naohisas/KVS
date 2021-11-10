/*****************************************************************************/
/**
 *  @file   Demuxer.cpp
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#include "Demuxer.h"
#include <kvs/Message>
#include <kvs/Math>
#include <kvs/MutexLocker>


#if FF_API_CONVERGENCE_DURATION
#define KVS_FFMPEG_DEMUXER__WARNING_OFF FF_DISABLE_DEPRECATION_WARNINGS
#define KVS_FFMPEG_DEMUXER__WARNING_ON  FF_ENABLE_DEPRECATION_WARNINGS
#else
#define KVS_FFMPEG_DEMUXER__WARNING_OFF
#define KVS_FFMPEG_DEMUXER__WARNING_ON
#endif

namespace kvs
{

namespace ffmpeg
{

/*===========================================================================*/
/**
 *  @brief  Open format context and decoder.
 *  @param  filename [in] filename of movie data
 *  @return true, if the context and decoder are opened successfully
 */
/*===========================================================================*/
bool Demuxer::open( const std::string& filename )
{
    kvs::MutexLocker locker( &m_mutex );

    m_format_context.openInput( filename, m_error );
    if ( m_error )
    {
        kvsMessageError() << "Cannot open " << filename << "." << std::endl;
        return false;
    }

    m_format_context.findStreamInfo( m_error );
    if ( m_error )
    {
        const auto& e = m_error;
        const auto m = e.message();
        kvsMessageError() << "Cannot find stream: " << e << ", " << m << std::endl;
        return false;
    }

    if ( !this->find_stream() )
    {
        kvsMessageError() << "Cannot find video stream." << std::endl;
        return false;
    }

    if ( !this->setup_decoder() )
    {
        kvsMessageError() << "Cannot setup video decoder." << std::endl;
        return false;
    }

    const auto dst_width = m_decoder.width();
    const auto dst_height = m_decoder.height();
    const av::PixelFormat dst_pix_fmt = AV_PIX_FMT_RGB24;
    m_rescaler = av::VideoRescaler( dst_width, dst_height, dst_pix_fmt );
    m_nframes = this->number_of_frames();

    return true;
}

/*===========================================================================*/
/**
 *  @brief  Close the format context and decoder.
 */
/*===========================================================================*/
void Demuxer::close()
{
    if ( m_format_context.isOpened() ) { m_format_context.close(); }
    if ( m_decoder.isOpened() ) { m_decoder.close(); }
}

/*===========================================================================*/
/**
 *  @brief  Grab current decoded frame and a frame indicator is move to next frame.
 *  @return true if the frame is grabbed successfully
 */
/*===========================================================================*/
bool Demuxer::grab()
{
    kvs::MutexLocker locker( &m_mutex );

    if ( !m_format_context.isOpened() ) { return false; }
    if ( !m_stream.isValid() ) { return false; }

    m_frame_pts = av::NoPts;

    int trials = 0;
    const int max_trials = 1 << 9;
    bool valid = false;
    while ( !valid )
    {
        auto packet = m_format_context.readPacket( m_error );
        if ( m_error ) { valid = false; break; }

        bool flush_packet = false;
        if ( !packet.isComplete() )
        {
            packet = av::Packet();
            packet.setStreamIndex( m_stream_index );
            flush_packet = true;
        }

        if ( packet.streamIndex() != m_stream_index )
        {
            if ( ++trials > max_trials ) { break; }
            continue;
        }

        KVS_FFMPEG_DEMUXER__WARNING_OFF;
        m_current_frame = m_decoder.decode( packet, m_error );
        if ( m_error ) { valid = false; break; }
        KVS_FFMPEG_DEMUXER__WARNING_ON;

        if ( m_current_frame.isValid() )
        {
            if ( m_frame_pts == av::NoPts )
            {
                const auto pts = packet.pts().timestamp();
                const auto dts = packet.dts().timestamp();
                m_frame_pts = pts != av::NoPts && pts != 0 ? pts : dts;
            }

            valid = true;
        }
        else
        {
            if ( flush_packet ) { valid = false; break; }
            if ( ++trials > max_trials ) { break; }
        }
    }

    if ( valid ) { m_next_frame_index++; }

    if ( valid && m_frame_index0 < 0 )
    {
        m_frame_index0 = this->pts_to_frame( m_frame_pts );
    }

    return valid;
}

/*===========================================================================*/
/**
 *  @brief  Retrieve current rescaled frame.
 *  @return rescaled frame
 */
/*===========================================================================*/
av::VideoFrame Demuxer::retrieve()
{
    return m_current_frame.isValid() ?
        m_rescaler.rescale( m_current_frame, m_error ) :
        av::VideoFrame();
}

/*===========================================================================*/
/**
 *  @brief  Find video stream from the format context.
 *  @return true if the video stream was found
 */
/*===========================================================================*/
bool Demuxer::find_stream()
{
    const auto nstreams = m_format_context.streamsCount();
    for ( size_t i = 0; i < nstreams; ++i )
    {
        auto stream = m_format_context.stream(i);
        if ( stream.mediaType() == AVMEDIA_TYPE_VIDEO )
        {
            m_stream = stream;
            m_stream_index = int(i);
            return true;
        }
    }
    return false;
}

/*===========================================================================*/
/**
 *  @brief  Setup decoder.
 *  @return true if the decoder is setup successfully
 */
/*===========================================================================*/
bool Demuxer::setup_decoder()
{
    if ( !m_stream.isValid() ) { return false; }

    m_decoder = av::VideoDecoderContext( m_stream );

    av::Codec codec = av::findDecodingCodec( m_decoder.raw()->codec_id );
    m_decoder.setCodec( codec );
    m_decoder.setRefCountedFrames( true );
    m_decoder.open( {{"threads","auto"}}, m_error );
    if ( m_error ) { return false; }

    return true;
}

/*===========================================================================*/
/**
 *  @brief  Returns number of frames
 *  @return number of frames
 */
/*===========================================================================*/
int64_t Demuxer::number_of_frames() const
{
    const int64_t nframes = m_stream.raw()->nb_frames;
    if ( nframes > 0 ) { return nframes; }

    const auto d = m_format_context.duration().seconds();
    const auto duration = kvs::Math::IsZero( d ) ? m_stream.duration().seconds() : d;
    const auto frame_rate = m_stream.frameRate().getDouble();
    return static_cast<int64_t>( kvs::Math::Floor( duration * frame_rate + 0.5 ) );
}

/*===========================================================================*/
/**
 *  @brief  Returns frame index calculated by the specified timestamp (PTS).
 *  @param  pts [in] timestamp
 *  @return frame index
 */
/*===========================================================================*/
int64_t Demuxer::pts_to_frame( const int64_t pts ) const
{
    if ( pts == av::NoPts ) { return 0; }
    const auto tb = m_stream.timeBase().getDouble();
    const auto fps = m_stream.frameRate().getDouble();
    const auto st = m_stream.startTime().timestamp();
    const auto sec = ( pts - ( st == av::NoPts ? 0 : st ) ) * tb;
    return static_cast<int64_t>( fps * sec + 0.5 );
}

/*===========================================================================*/
/**
 *  @brief  Returns timestamp (PTS) calculated by the specified frame index.
 *  @param  index [in] frame index
 *  @return timestamp
 */
/*===========================================================================*/
int64_t Demuxer::frame_to_pts( const int64_t frame ) const
{
    const auto tb = m_stream.timeBase().getDouble();
    const auto fps = m_stream.frameRate().getDouble();
    const auto sec = static_cast<double>( frame ) / fps;
    const auto st = m_stream.startTime().timestamp();
    return static_cast<int64_t>( sec / tb + 0.5 ) + ( st == av::NoPts ? 0 : st );
}

/*===========================================================================*/
/**
 *  @brief  Seeks frame specified by frame index
 *  @param  index [in] frame index
 */
/*===========================================================================*/
void Demuxer::seek_by_index( const int64_t index )
{
    auto frame_index = kvs::Math::Min( index, m_nframes );
    int64_t delta = 16;

    if ( m_frame_index0 < 0 && m_nframes > 1 )
    {
        this->grab();
    }

    while ( true )
    {
        const int64_t frame_index_temp = kvs::Math::Max( frame_index - delta, int64_t(0) );
        const int64_t time_stamp = this->frame_to_pts( frame_index_temp );

        if ( m_nframes > 1 )
        {
            const auto flag = AVSEEK_FLAG_BACKWARD;
            m_format_context.seek( time_stamp, m_stream_index, flag, m_error );
        }
        avcodec_flush_buffers( m_decoder.raw() );

        if ( frame_index > 0 )
        {
            this->grab();

            if ( frame_index > 1 )
            {
                m_next_frame_index = this->pts_to_frame( m_frame_pts ) - m_frame_index0;

                if ( m_next_frame_index < 0 || m_next_frame_index > frame_index - 1 )
                {
                    if ( frame_index_temp == 0 || delta >= INT_MAX/4 ) { break; }
                    delta = delta < 16 ? delta * 2 : delta * 3 / 2;
                    continue;
                }

                while ( m_next_frame_index < frame_index - 1 )
                {
                    if ( !this->grab() ) { break; }
                }

                m_next_frame_index++;
                break;
            }
            else
            {
                m_next_frame_index = 1;
                break;
            }
        }
        else
        {
            m_next_frame_index = 0;
            break;
        }
    }
}

/*===========================================================================*/
/**
 *  @brief  Seeks frame specified by time in sec.
 *  @param  sec [in] time
 */
/*===========================================================================*/
void Demuxer::seek_by_sec( const double sec )
{
    const auto index =  static_cast<int64_t>( sec * m_stream.frameRate().getDouble() );
    this->seek_by_index( index );
}

} // end of namespace ffmpeg

} // end of namespace kvs
