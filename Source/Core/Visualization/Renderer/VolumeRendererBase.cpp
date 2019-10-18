/****************************************************************************/
/**
 *  @file   VolumeRendererBase.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: VolumeRendererBase.cpp 1642 2013-09-15 12:41:47Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#include "VolumeRendererBase.h"
#include <kvs/Camera>
#include <kvs/Math>
#include <kvs/OpenGL>


namespace kvs
{

/*==========================================================================*/
/**
 *  Constructor.
 */
/*==========================================================================*/
VolumeRendererBase::VolumeRendererBase():
    m_window_width( 0 ),
    m_window_height( 0 ),
    m_device_pixel_ratio( 1.0f ),
    m_shader( NULL )
{
    m_depth_buffer.setFormat( GL_DEPTH_COMPONENT );
    m_depth_buffer.setType( GL_FLOAT );

    m_color_buffer.setFormat( GL_RGBA );
    m_color_buffer.setType( GL_UNSIGNED_BYTE );
}

/*==========================================================================*/
/**
 *  Destructor.
 */
/*==========================================================================*/
VolumeRendererBase::~VolumeRendererBase()
{
    if ( m_shader ) { delete m_shader; }
}

/*===========================================================================*/
/**
 *  @brief  Allocates a memory for the depth data.
 *  @param  size [in] number of element of the data
 */
/*===========================================================================*/
void VolumeRendererBase::allocateDepthData( const size_t size )
{
    m_depth_data.allocate( size );
}

/*===========================================================================*/
/**
 *  @brief  Allocates a memory for the color data.
 *  @param  size [in] number of element of the data
 */
/*===========================================================================*/
void VolumeRendererBase::allocateColorData( const size_t size )
{
    m_color_data.allocate( size );
}

/*===========================================================================*/
/**
 *  @brief  Fills a value into the depth data.
 *  @param  value [in] depth value
 */
/*===========================================================================*/
void VolumeRendererBase::fillDepthData( const kvs::Real32 value )
{
    m_depth_data.fill( value );
}

/*===========================================================================*/
/**
 *  @brief  Fills a value into the color data.
 *  @param  value [in] color value (single component)
 */
/*===========================================================================*/
void VolumeRendererBase::fillColorData( const kvs::UInt8 value )
{
    m_color_data.fill( value );
}

/*===========================================================================*/
/**
 *  @brief  Reads color and depth buffers.
 */
/*===========================================================================*/
void VolumeRendererBase::readImage()
{
    const size_t width = this->framebufferWidth();
    const size_t height = this->framebufferHeight();
    m_depth_buffer.readPixels( 0, 0, width, height, m_depth_data.data() );
    m_color_buffer.readPixels( 0, 0, width, height, m_color_data.data() );
}

/*==========================================================================*/
/**
 *  Draw the rendering image.
 */
/*==========================================================================*/
void VolumeRendererBase::drawImage()
{
    const size_t width = this->framebufferWidth();
    const size_t height = this->framebufferHeight();

    kvs::OpenGL::SetDepthFunc( GL_LEQUAL );
    kvs::OpenGL::SetDepthMask( GL_TRUE );
    kvs::OpenGL::SetColorMask( GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE );
    {
        kvs::OpenGL::WithEnabled e( GL_DEPTH_TEST );
        m_depth_buffer.drawPixels( 0, 0, width, height, m_depth_data.data() );
    }

    kvs::OpenGL::SetBlendFunc( GL_ONE, GL_ONE_MINUS_SRC_ALPHA );
    kvs::OpenGL::SetColorMask( GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE );
    {
        kvs::OpenGL::WithEnabled e( GL_BLEND );
        kvs::OpenGL::WithDisabled d( GL_DEPTH_TEST );
        m_color_buffer.drawPixels( 0, 0, width, height, m_color_data.data() );
    }
}

} // end of namespace kvs
