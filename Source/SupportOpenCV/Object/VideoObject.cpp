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
VideoObject::VideoObject( void ):
    m_device_id( CV_CAP_ANY ),
    m_type( kvs::opencv::VideoObject::Color24 )
{
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new VideoObject class.
 *  @param  device_id [in] device ID
 */
/*===========================================================================*/
VideoObject::VideoObject( const int device_id ):
    m_device_id( device_id )
{
    if ( !this->initialize( device_id ) )
    {
        kvsMessageError("Cannot initialize a capture device (%d).", device_id);
    }
}

/*===========================================================================*/
/**
 *  @brief  Destructs the VideoObject class.
 */
/*===========================================================================*/
VideoObject::~VideoObject( void )
{
}

/*===========================================================================*/
/**
 *  @brief  Returns the object type.
 *  @return object type
 */
/*===========================================================================*/
kvs::ObjectBase::ObjectType VideoObject::objectType( void ) const
{
    return( kvs::ObjectBase::Image );
}

/*===========================================================================*/
/**
 *  @brief  Returns the device ID.
 *  @return device ID
 */
/*===========================================================================*/
const int VideoObject::deviceID( void ) const
{
    return( m_device_id );
}

/*===========================================================================*/
/**
 *  @brief  Returns the capture device.
 *  @return capture device
 */
/*===========================================================================*/
const kvs::opencv::CaptureDevice& VideoObject::device( void ) const
{
    return( m_device );
}

/*===========================================================================*/
/**
 *  @brief  Returns the pixel type.
 *  @return pixel type
 */
/*===========================================================================*/
const VideoObject::PixelType VideoObject::type( void ) const
{
    return( m_type );
}

/*===========================================================================*/
/**
 *  @brief  Returns the capture width.
 *  @return capture width
 */
/*===========================================================================*/
const size_t VideoObject::width( void ) const
{
    return( m_width );
}

/*===========================================================================*/
/**
 *  @brief  Returns teh capture height.
 *  @return capture height
 */
/*===========================================================================*/
const size_t VideoObject::height( void ) const
{
    return( m_height );
}

/*===========================================================================*/
/**
 *  @brief  Returns the number of channels.
 *  @return number of channels
 */
/*===========================================================================*/
const size_t VideoObject::nchannels( void ) const
{
    return( m_nchannels );
}

/*===========================================================================*/
/**
 *  @brief  Initialize the video object.
 *  @param  device_id [in] device ID
 *  @return true, if the video object is initialized successfully
 */
/*===========================================================================*/
const bool VideoObject::initialize( const size_t device_id )
{
    if ( !m_device.create( device_id ) )
    {
        kvsMessageError("Cannot create a capture device (%d).", device_id);
        return( false );
    }

    const IplImage* frame = m_device.queryFrame();
    if ( !frame )
    {
        kvsMessageError("Cannot query a new frame from the capture device.");
        return( false );
    }

    m_width  = static_cast<size_t>( frame->width );
    m_height = static_cast<size_t>( frame->height );
    m_nchannels = static_cast<size_t>( frame->nChannels );

    const int depth = frame->depth;
    if ( depth != IPL_DEPTH_8U )
    {
        kvsMessageError("The depth of the grabbed image isn't 'IPL_DEPTH_8U'.");
        return( false );
    }

    m_type = m_nchannels == 1 ? Gray8 : Color24;

    return( true );
}

} // end of namespace opencv

} // end of namespace kvs
