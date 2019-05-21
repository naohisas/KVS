/*****************************************************************************/
/**
 *  @file   VideoRenderer.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: VideoRenderer.cpp 1506 2013-04-06 07:58:23Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#include "VideoRenderer.h"
#include <kvs/Camera>
#include <kvs/Message>
#include <kvs/IgnoreUnusedVariable>
#include <kvs/Vector2>
#include <kvs/OpenGL>


namespace kvs
{

namespace opencv
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new VideoRenderer class.
 */
/*===========================================================================*/
VideoRenderer::VideoRenderer():
    m_enable_centering( true ),
    m_enable_mirroring( true )
{
}

/*===========================================================================*/
/**
 *  @brief  Renders the grabbed frame.
 *  @param  object [in] pointer to the video object
 *  @param  camera [in] pointer to the camera in KVS
 *  @param  light [in] pointer to the light
 */
/*===========================================================================*/
void VideoRenderer::exec( kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light )
{
    kvs::IgnoreUnusedVariable( light );

    kvs::opencv::VideoObject* video = kvs::opencv::VideoObject::DownCast( object );
    const IplImage* frame = video->device().queryFrame();
    if ( !frame ) { return; }

    BaseClass::startTimer();

    kvs::OpenGL::WithPushedAttrib p( GL_ALL_ATTRIB_BITS );
    p.disable( GL_DEPTH_TEST );
    p.enable( GL_TEXTURE_2D );

    if ( !this->texture().isValid() ) { this->createTexture( video ); }
    if ( this->isEnabledCentering() ) { this->alignCenter( camera ); }

    const int width = frame->width;
    const int height = frame->height;
    const char* data = frame->imageData; // BGRBGRBGR...
    this->texture().bind();
    this->texture().load( width, height, data );
    this->textureMapping();
    this->texture().unbind();

    kvs::OpenGL::SetClearDepth( 1000 );
    BaseClass::stopTimer();
}

/*===========================================================================*/
/**
 *  @brief  Create texture.
 *  @param  video [in] pointer to the video object
 */
/*===========================================================================*/
void VideoRenderer::createTexture( const kvs::opencv::VideoObject* video )
{
    const double width  = video->width();
    const double height = video->height();
    m_initial_aspect_ratio = width / height;
    m_left = 0.0;
    m_right = 1.0;
    m_bottom = 0.0;
    m_top = 1.0;

    if ( video->type() == kvs::opencv::VideoObject::Gray8 )
    {
        m_texture.setPixelFormat( GL_INTENSITY8, GL_LUMINANCE, GL_UNSIGNED_BYTE );
    }
    else if ( video->type() == kvs::opencv::VideoObject::Color24 )
    {
#ifdef GL_BGR_EXT
        m_texture.setPixelFormat( GL_RGB8, GL_BGR_EXT, GL_UNSIGNED_BYTE );
#else
        m_texture.setPixelFormat( GL_RGB8, GL_RGB, GL_UNSIGNED_BYTE );
#endif
    }
    else
    {
        kvsMessageError("Unknown pixel color type.");
    }

    const IplImage* frame = video->device().queryFrame();
    m_texture.create( frame->width, frame->height );
}

/*===========================================================================*/
/**
 *  @brief  Renders the grabbed frame in the center of the window.
 *  @param  camera [in] pointer to the camera
 */
/*===========================================================================*/
void VideoRenderer::alignCenter( const kvs::Camera* camera )
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
void VideoRenderer::textureMapping()
{
    kvs::OpenGL::WithPushedMatrix p1( GL_MODELVIEW );
    p1.loadIdentity();
    {
        kvs::OpenGL::WithPushedMatrix p2( GL_PROJECTION );
        p2.loadIdentity();
        {
            kvs::OpenGL::SetOrtho( m_left, m_right, m_bottom, m_top, -1, 1 );
            kvs::OpenGL::Begin( GL_QUADS );
            if ( m_enable_mirroring )
            {
                kvs::OpenGL::TexCoordVertex( kvs::Vec2( 0.0f, 0.0f ), kvs::Vec2( 1.0f, 1.0f ) );
                kvs::OpenGL::TexCoordVertex( kvs::Vec2( 0.0f, 1.0f ), kvs::Vec2( 1.0f, 0.0f ) );
                kvs::OpenGL::TexCoordVertex( kvs::Vec2( 1.0f, 1.0f ), kvs::Vec2( 0.0f, 0.0f ) );
                kvs::OpenGL::TexCoordVertex( kvs::Vec2( 1.0f, 0.0f ), kvs::Vec2( 0.0f, 1.0f ) );
            }
            else
            {
                kvs::OpenGL::TexCoordVertex( kvs::Vec2( 0.0f, 0.0f ), kvs::Vec2( 0.0f, 1.0f ) );
                kvs::OpenGL::TexCoordVertex( kvs::Vec2( 0.0f, 1.0f ), kvs::Vec2( 0.0f, 0.0f ) );
                kvs::OpenGL::TexCoordVertex( kvs::Vec2( 1.0f, 1.0f ), kvs::Vec2( 1.0f, 0.0f ) );
                kvs::OpenGL::TexCoordVertex( kvs::Vec2( 1.0f, 0.0f ), kvs::Vec2( 1.0f, 1.0f ) );
            }
            kvs::OpenGL::End();
        }
    }
}

} // end of namespace opencv

} // end of namespace kvs
