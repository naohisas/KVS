/****************************************************************************/
/**
 *  @file   ImageRenderer.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: ImageRenderer.cpp 1822 2014-10-24 09:01:51Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#include "ImageRenderer.h"
#include <kvs/Camera>
#include <kvs/ImageObject>
#include <kvs/Message>
#include <kvs/IgnoreUnusedVariable>
#include <kvs/OpenGL>


namespace kvs
{

/*==========================================================================*/
/**
 *  @brief  Constructs a new ImageRenderer class.
 */
/*==========================================================================*/
ImageRenderer::ImageRenderer():
    m_enable_centering( true )
{
}

/*==========================================================================*/
/**
 *  @brief  Executes the rendering process.
 *  @param  object [in] pointer to the object
 *  @param  camera [in] pointer to the camera
 *  @param  light [in] pointer to the light
 */
/*==========================================================================*/
void ImageRenderer::exec( kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light )
{
    kvs::IgnoreUnusedVariable( light );

    kvs::ImageObject* image = kvs::ImageObject::DownCast( object );
    if ( !image ) { return; }

    BaseClass::startTimer();

    kvs::OpenGL::Clear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    kvs::OpenGL::WithPushedAttrib p( GL_ALL_ATTRIB_BITS );
    p.disable( GL_DEPTH_TEST );
    p.disable( GL_TEXTURE_1D );
    p.disable( GL_TEXTURE_3D );
    p.enable( GL_TEXTURE_2D );

    if ( !this->texture().isValid() ) { this->createTexture( image ); }
    if ( this->isEnabledCentering() ) { this->alignCenter( camera ); }

    this->texture().bind();
    this->textureMapping();
    this->texture().unbind();

    BaseClass::stopTimer();
}

/*===========================================================================*/
/**
 *  @brief  Create texture.
 *  @param  image [in] pointer to the image object
 */
/*===========================================================================*/
void ImageRenderer::createTexture( const kvs::ImageObject* image )
{
    const double width  = image->width();
    const double height = image->height();
    m_initial_aspect_ratio = width / height;
    m_left = 0.0;
    m_right = 1.0;
    m_bottom = 0.0;
    m_top = 1.0;

    switch ( image->pixelType() )
    {
    case kvs::ImageObject::Gray8:
    {
        const size_t nchannels = 1;
        const size_t bytes_per_channel = 1;
        m_texture.setPixelFormat( nchannels, bytes_per_channel );
        break;
    }
    case kvs::ImageObject::Gray16:
    {
        const size_t nchannels = 1;
        const size_t bytes_per_channel = 2;
        m_texture.setPixelFormat( nchannels, bytes_per_channel );
        break;
    }
    case kvs::ImageObject::Color24:
    {
        const size_t nchannels = 3;
        const size_t bytes_per_channel = 1;
        m_texture.setPixelFormat( nchannels, bytes_per_channel );
        break;
    }
    case kvs::ImageObject::Color32:
    {
        const size_t nchannels = 4;
        const size_t bytes_per_channel = 1;
        m_texture.setPixelFormat( nchannels, bytes_per_channel );
        break;
    }
    default:
    {
        kvsMessageError("Unknown pixel color type.");
        break;
    }
    }

    kvs::Texture::SetEnv( GL_TEXTURE_ENV_MODE, GL_REPLACE );
    m_texture.setWrapS( GL_REPEAT );
    m_texture.setWrapT( GL_REPEAT );
    m_texture.setMagFilter( GL_LINEAR );
    m_texture.setMinFilter( GL_LINEAR );
    m_texture.create( image->width(), image->height(), image->pixels().data() );
}

/*===========================================================================*/
/**
 *  @brief  Renders the grabbed frame in the center of the window.
 *  @param  camera [in] pointer to the camera
 */
/*===========================================================================*/
void ImageRenderer::alignCenter( const kvs::Camera* camera )
{
    const double width = camera->windowWidth();
    const double height = camera->windowHeight();
    const double current_aspect_ratio = width / height;
    const double aspect_ratio = current_aspect_ratio / m_initial_aspect_ratio;
    if( aspect_ratio >= 1.0 )
    {
        m_left = ( 1.0 - aspect_ratio ) * 0.5;
        m_right = ( 1.0 + aspect_ratio ) * 0.5;
        m_bottom = 0.0;
        m_top = 1.0;
    }
    else
    {
        m_left = 0.0;
        m_right = 1.0;
        m_bottom = ( 1.0 - 1.0 / aspect_ratio ) * 0.5;
        m_top = ( 1.0 + 1.0 / aspect_ratio ) * 0.5;
    }
}

/*===========================================================================*/
/**
 *  @brief  Texture mapping.
 */
/*===========================================================================*/
void ImageRenderer::textureMapping()
{
    kvs::OpenGL::WithPushedMatrix p1( GL_MODELVIEW );
    p1.loadIdentity();
    {
        kvs::OpenGL::WithPushedMatrix p2( GL_PROJECTION );
        p2.loadIdentity();
        {
            kvs::OpenGL::SetOrtho( m_left, m_right, m_bottom, m_top, -1, 1 );
            kvs::OpenGL::Begin( GL_QUADS );
            kvs::OpenGL::TexCoordVertex( kvs::Vec2( 0.0f, 0.0f ), kvs::Vec2( 0.0f, 1.0f ) );
            kvs::OpenGL::TexCoordVertex( kvs::Vec2( 0.0f, 1.0f ), kvs::Vec2( 0.0f, 0.0f ) );
            kvs::OpenGL::TexCoordVertex( kvs::Vec2( 1.0f, 1.0f ), kvs::Vec2( 1.0f, 0.0f ) );
            kvs::OpenGL::TexCoordVertex( kvs::Vec2( 1.0f, 0.0f ), kvs::Vec2( 1.0f, 1.0f ) );
            kvs::OpenGL::End();
        }
    }
}

} // end of namespace kvs
