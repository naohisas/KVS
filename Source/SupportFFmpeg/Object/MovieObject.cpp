/*****************************************************************************/
/**
 *  @file   MovieObject.cpp
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#include "MovieObject.h"


namespace kvs
{

namespace ffmpeg
{

/*===========================================================================*/
/**
 *  @brief  Return the current frame as a kvs::ColorImage.
 *  @return current frame
 */
/*===========================================================================*/
const kvs::ColorImage MovieObject::currentImage() const
{
    if ( m_buffer.size() > 0 )
    {
        return kvs::ColorImage( this->width(), this->height(), m_buffer );
    }

    return kvs::ColorImage( this->width(), this->height() );
}

/*===========================================================================*/
/**
 *  @brief  Jumps to the frame specified by the index and grabs the frame.
 *  @param  index [in] frame index
 *  @return true on success
 */
/*===========================================================================*/
bool MovieObject::jumpToFrame( const size_t index )
{
    if ( this->currentFrameIndex() == kvs::Int64(index) ) { return true; }
    this->seekToFrame( index );
    return this->grabFrame();
}

/*===========================================================================*/
/**
 *  @brief  Grabs and retrieves the current frame.
 *  @return true on success
 */
/*===========================================================================*/
bool MovieObject::grabFrame()
{
    if ( m_demuxer.grab() )
    {
        auto frame = m_demuxer.retrieve();
        if ( frame )
        {
            const auto* buffer_data = frame.data();
            const auto buffer_size = frame.bufferSize();
            m_buffer = Buffer( buffer_data, buffer_size );
            return true;
        }
    }
    return false;
}

/*===========================================================================*/
/**
 *  @brief  Reads the movie file and grab the top frame.
 *  @param  filename [in] filename of the movie data
 *  @return true on success
 */
/*===========================================================================*/
bool MovieObject::read( const std::string& filename )
{
    if ( m_demuxer.open( filename ) )
    {
        return this->grabFrame();
    }

    return false;
}

} // end of namespace ffmpeg

} // end of namespace kvs
