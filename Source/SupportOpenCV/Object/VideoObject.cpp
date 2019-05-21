/*****************************************************************************/
/**
 *  @file   VideoObject.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: VideoObject.cpp 1385 2012-12-04 03:25:29Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#include "VideoObject.h"


namespace kvs
{

namespace opencv
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new VideoObject class.
 */
/*===========================================================================*/
VideoObject::VideoObject():
    m_type( kvs::opencv::VideoObject::Color24 )
{
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new VideoObject class.
 *  @param  device_id [in] device ID
 */
/*===========================================================================*/
VideoObject::VideoObject( const int device_id )
{
    if ( !this->createCaptureDevice( device_id ) )
    {
        kvsMessageError( "Cannot create capture device from device:%d.", device_id );
        return;
    }

    if ( !this->initialize( m_device ) )
    {
        kvsMessageError( "Cannot initialize video object from device:%d.", device_id );
        return;
    }
}

/*===========================================================================*/
/**
 *  @brief  Shallow copy.
 *  @param  other [in] other video object
 */
/*===========================================================================*/
void VideoObject::shallowCopy( const VideoObject& other )
{
    BaseClass::operator=( other );
    m_device = other.m_device;
    m_type = other.m_type;
    m_width = other.m_width;
    m_height = other.m_height;
    m_nchannels = other.m_nchannels;
}

/*===========================================================================*/
/**
 *  @brief  Create capture device specified by 'device_id'
 *  @param  device_id [in] device ID
 *  @return true, if the reading process is done successfully
 */
/*===========================================================================*/
bool VideoObject::createCaptureDevice( const int device_id )
{
    if ( !m_device.create( device_id ) )
    {
        kvsMessageError( "Cannot create capture device from device:%d.", device_id );
        return false;
    }

    return true;
}

/*===========================================================================*/
/**
 *  @brief  Read video file specified by 'filename'
 *  @param  filename [in] filename
 *  @return true, if the reading process is done successfully
 */
/*===========================================================================*/
bool VideoObject::createCaptureDevice( const std::string& filename )
{
    if ( !m_device.create( filename ) )
    {
        kvsMessageError( "Cannot create capture device from %s.", filename.c_str() );
        return false;
    }

    return true;
}

/*===========================================================================*/
/**
 *  @brief  Initialize the video object.
 *  @param  device [in] capture device
 *  @return true, if the video object is initialized successfully
 */
/*===========================================================================*/
bool VideoObject::initialize( const kvs::opencv::CaptureDevice& device )
{
    const IplImage* frame = device.queryFrame();
    if ( !frame )
    {
        kvsMessageError("Cannot query a new frame from the capture device.");
        return false;
    }

    m_width = static_cast<size_t>( frame->width );
    m_height = static_cast<size_t>( frame->height );
    m_nchannels = static_cast<size_t>( frame->nChannels );

    const int depth = frame->depth;
    if ( depth != IPL_DEPTH_8U )
    {
        kvsMessageError("The depth of the grabbed image isn't 'IPL_DEPTH_8U'.");
        return false;
    }

    m_type = m_nchannels == 1 ? Gray8 : Color24;

    return true;
}

} // end of namespace opencv

} // end of namespace kvs
