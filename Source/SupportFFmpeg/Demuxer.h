/*****************************************************************************/
/**
 *  @file   Demuxer.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#pragma once
#include <kvs/Mutex>
#include <SupportFFmpeg/AvCpp/av.h>
#include <SupportFFmpeg/AvCpp/avutils.h>
#include <SupportFFmpeg/AvCpp/formatcontext.h>
#include <SupportFFmpeg/AvCpp/stream.h>
#include <SupportFFmpeg/AvCpp/codeccontext.h>
#include <SupportFFmpeg/AvCpp/codec.h>
#include <SupportFFmpeg/AvCpp/packet.h>
#include <SupportFFmpeg/AvCpp/frame.h>
#include <SupportFFmpeg/AvCpp/videorescaler.h>
#include <SupportFFmpeg/AvCpp/pixelformat.h>


namespace kvs
{

namespace ffmpeg
{

/*===========================================================================*/
/**
 *  @brief  Demuxer class.
 */
/*===========================================================================*/
class Demuxer
{
    struct LogQuiet { LogQuiet() { av_log_set_level( AV_LOG_QUIET ); } };

private:
    LogQuiet m_log_quiet{}; ///< quiet log message
    mutable kvs::Mutex m_mutex; ///< mutex
    std::error_code m_error{}; ///< error code
    av::FormatContext m_format_context{}; ///< format context
    av::Stream m_stream{}; ///< input video stream
    av::VideoDecoderContext m_decoder{}; ///< decoder
    av::VideoRescaler m_rescaler{}; ///< rescaler
    av::VideoFrame m_current_frame{}; ///< current frame (decoded frame)
    int64_t m_nframes = 0; ///< total number of frames
    int64_t m_frame_pts = av::NoPts; ///< time-stamp (pts) of current frame
    int64_t m_frame_index0 = -1; ///< index of first frame
    int64_t m_next_frame_index = 0; ///< index of next decoding frame
    int m_stream_index = -1; ///< video stream index

public:
    Demuxer() = default;
    Demuxer( const std::string& filename ) { this->open( filename ); }
    ~Demuxer() { this->close(); }

    const av::FormatContext& formatContext() const { return m_format_context; }
    const av::VideoDecoderContext& decoder() const { return m_decoder; }
    const av::Stream& stream() const { return m_stream; }
    const av::VideoRescaler& rescaler() const { return m_rescaler; }

    int64_t numberOfFrames() const { return m_nframes; }
    int64_t nextFrameIndex() const { return m_next_frame_index == 0 ? 1 : m_next_frame_index; }
    int64_t currentFrameIndex() const { return this->nextFrameIndex() - 1; }
    bool isLastFrame() const { return this->currentFrameIndex() == m_nframes - 1; }

    void seek( const int64_t index ) { this->seek_by_index( index ); }
    void seek( const int index ) { this->seek_by_index( int64_t( index ) ); }
    void seek( const size_t index ) { this->seek_by_index( int64_t( index ) ); }
    void seek( const double sec ) { this->seek_by_sec( sec ); }
    void seek( const float sec ) { this->seek_by_sec( double( sec ) ); }

    bool open( const std::string& filename );
    void close();
    bool grab();
    av::VideoFrame retrieve();

private:
    bool find_stream();
    bool setup_decoder();
    int64_t number_of_frames() const;
    int64_t pts_to_frame( const int64_t pts ) const;
    int64_t frame_to_pts( const int64_t frame ) const;
    void seek_by_index( const int64_t index );
    void seek_by_sec( const double sec );
};

} // end of namespace ffmpeg

} // end of namespace kvs
