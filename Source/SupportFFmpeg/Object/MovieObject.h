/*****************************************************************************/
/**
 *  @file   MovieObject.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#pragma once
#include <string>
#include <kvs/ObjectBase>
#include <kvs/Module>
#include <kvs/ValueArray>
#include <kvs/ColorImage>
#include <SupportFFmpeg/Demuxer.h>


namespace kvs
{

namespace ffmpeg
{

/*===========================================================================*/
/**
 *  @brief  Movie object class.
 */
/*===========================================================================*/
class MovieObject : public kvs::ObjectBase
{
    kvsModule( kvs::ffmpeg::MovieObject, Object );
    kvsModuleBaseClass( kvs::ObjectBase );

public:
    using Buffer = kvs::ValueArray<kvs::UInt8>;

private:
    kvs::ffmpeg::Demuxer m_demuxer{}; ///< demuxer (decoder)
    Buffer m_buffer{}; ///< RGB buffer of current frame

public:
    MovieObject() = default;
    MovieObject( const std::string& filename ) { this->read( filename ); }

    double frameRate() const { return m_demuxer.stream().frameRate().getDouble(); }
    size_t numberOfFrames() const { return static_cast<size_t>( m_demuxer.numberOfFrames() ); }
    size_t width() const { return static_cast<size_t>( m_demuxer.decoder().width() ); }
    size_t height() const { return static_cast<size_t>( m_demuxer.decoder().height() ); }
    bool isLastFrame() const { return m_demuxer.isLastFrame(); }
    kvs::Int64 currentFrameIndex() const { return m_demuxer.currentFrameIndex(); }
    const Buffer& currentBuffer() const { return m_buffer; }
    const kvs::ColorImage currentImage() const;

    bool jumpToFrame( const size_t index );
    bool jumpToNextFrame() { return this->grabFrame(); }
    void seekToFrame( const size_t index ) { m_demuxer.seek( index ); }
    bool grabFrame();
    bool read( const std::string& filename );
};

} // end of namespace ffmpeg

} // end of namespace kvs
