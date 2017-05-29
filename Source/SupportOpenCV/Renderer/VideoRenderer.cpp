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
 *  @param  type [in] video type
 */
/*===========================================================================*/
VideoRenderer::VideoRenderer( const Type type ):
    m_type( type )
{
}

/*===========================================================================*/
/**
 *  @brief  Destructs the VideoRenderer class.
 */
/*===========================================================================*/
VideoRenderer::~VideoRenderer()
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

    BaseClass::startTimer();
    kvs::opencv::VideoObject* video = reinterpret_cast<kvs::opencv::VideoObject*>( object );
    kvs::OpenGL::WithPushedAttrib p( GL_ALL_ATTRIB_BITS );

    if ( !m_texture.isValid() )
    {
        this->create_texture( video );
    }

    kvs::OpenGL::Disable( GL_DEPTH_TEST );
    kvs::OpenGL::Enable( GL_TEXTURE_2D );

    switch ( m_type )
    {
    case VideoRenderer::Centering:
    {
        this->centering( camera->windowWidth(), camera->windowHeight() );
        break;
    }
    default: break;
    }

    const IplImage* frame = video->device().queryFrame();
    const int width = frame->width;
    const int height = frame->height;
    const char* data = frame->imageData; // BGRBGRBGR...
    m_texture.bind();
    m_texture.load( width, height, data );

    kvs::OpenGL::WithPushedMatrix p1( GL_MODELVIEW );
    p1.loadIdentity();
    {
        kvs::OpenGL::WithPushedMatrix p2( GL_PROJECTION );
        p2.loadIdentity();
        {
            kvs::OpenGL::SetOrtho( m_left, m_right, m_bottom, m_top, -1, 1 );
            kvs::OpenGL::Begin( GL_QUADS );
/* mirror */
            kvs::OpenGL::TexCoordVertex( kvs::Vec2( 0.0f, 0.0f ), kvs::Vec2( 1.0f, 1.0f ) );
            kvs::OpenGL::TexCoordVertex( kvs::Vec2( 0.0f, 1.0f ), kvs::Vec2( 1.0f, 0.0f ) );
            kvs::OpenGL::TexCoordVertex( kvs::Vec2( 1.0f, 1.0f ), kvs::Vec2( 0.0f, 0.0f ) );
            kvs::OpenGL::TexCoordVertex( kvs::Vec2( 1.0f, 0.0f ), kvs::Vec2( 0.0f, 1.0f ) );
/* normal */
/*
            kvs::OpenGL::TexCoordVertex( kvs::Vec2( 0.0f, 0.0f ), kvs::Vec2( 0.0f, 1.0f ) );
            kvs::OpenGL::TexCoordVertex( kvs::Vec2( 0.0f, 1.0f ), kvs::Vec2( 0.0f, 0.0f ) );
            kvs::OpenGL::TexCoordVertex( kvs::Vec2( 1.0f, 1.0f ), kvs::Vec2( 1.0f, 0.0f ) );
            kvs::OpenGL::TexCoordVertex( kvs::Vec2( 1.0f, 0.0f ), kvs::Vec2( 1.0f, 1.0f ) );
*/
            kvs::OpenGL::End();
        }
    }

    m_texture.unbind();

    kvs::OpenGL::SetClearDepth( 1000 );
    BaseClass::stopTimer();
}

/*===========================================================================*/
/**
 *  @brief  Create texture.
 *  @param  video [in] pointer to the video object
 */
/*===========================================================================*/
void VideoRenderer::create_texture( const kvs::opencv::VideoObject* video )
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
 *  @param  width [in] window width
 *  @param  height [in] window height
 */
/*===========================================================================*/
void VideoRenderer::centering( const double width, const double height )
{
    double current_aspect_ratio = width / height;
    double aspect_ratio = current_aspect_ratio / m_initial_aspect_ratio;
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

} // end of namespace opencv

} // end of namespace kvs
