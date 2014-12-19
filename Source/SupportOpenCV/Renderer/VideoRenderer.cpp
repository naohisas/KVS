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
#ifdef KVS_SUPPORT_GLUT
#include <kvs/glut/GLUT>
#endif


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
VideoRenderer::~VideoRenderer( void )
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

    kvs::opencv::VideoObject* video = reinterpret_cast<kvs::opencv::VideoObject*>( object );

    BaseClass::startTimer();

    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    glPushAttrib( GL_CURRENT_BIT | GL_ENABLE_BIT );

    if ( !glIsTexture( m_texture.id() ) ) this->create_texture( video );

    glDisable( GL_DEPTH_TEST );
    glEnable( GL_TEXTURE_2D );

    switch( m_type )
    {
    case VideoRenderer::Centering:
        this->centering( camera->windowWidth(), camera->windowHeight() );
        break;
    default: break;
    }

    const IplImage* frame = video->device().queryFrame();
    const int width = frame->width;
    const int height = frame->height;
    const char* data = frame->imageData; // BGRBGRBGR...
    m_texture.bind();
    m_texture.download( width, height, data );

    glMatrixMode( GL_MODELVIEW );
    glPushMatrix();
    {
        glLoadIdentity();

        glMatrixMode( GL_PROJECTION );
        glPushMatrix();
        {
            glLoadIdentity();
            glOrtho( m_left, m_right, m_bottom, m_top, -1, 1 );

            glBegin( GL_QUADS );
/* mirror */
            glTexCoord2f( 0.0, 0.0 ); glVertex2f( 1.0, 1.0 );
            glTexCoord2f( 0.0, 1.0 ); glVertex2f( 1.0, 0.0 );
            glTexCoord2f( 1.0, 1.0 ); glVertex2f( 0.0, 0.0 );
            glTexCoord2f( 1.0, 0.0 ); glVertex2f( 0.0, 1.0 );
/* normal */
/*
            glTexCoord2f( 0.0, 0.0 ); glVertex2f( 0.0, 1.0 );
            glTexCoord2f( 0.0, 1.0 ); glVertex2f( 0.0, 0.0 );
            glTexCoord2f( 1.0, 1.0 ); glVertex2f( 1.0, 0.0 );
            glTexCoord2f( 1.0, 0.0 ); glVertex2f( 1.0, 1.0 );
*/
            glEnd();
        }
        glPopMatrix();
        glMatrixMode( GL_MODELVIEW );
    }
    glPopMatrix();

    m_texture.unbind();

    glClearDepth( 1000 );
    glEnable( GL_DEPTH_TEST );
    glDisable( GL_TEXTURE_2D );

    glPopAttrib();

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
    m_left   = 0.0;
    m_right  = 1.0;
    m_bottom = 0.0;
    m_top    = 1.0;

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

#ifdef KVS_SUPPORT_GLUT
    // Auto-play mode (auto-redraw).
    glutIdleFunc( glutPostRedisplay );
#endif
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
        m_left   = ( 1.0 - aspect_ratio ) * 0.5;
        m_right  = ( 1.0 + aspect_ratio ) * 0.5;
        m_bottom = 0.0;
        m_top    = 1.0;
    }
    else
    {
        m_left   = 0.0;
        m_right  = 1.0;
        m_bottom = ( 1.0 - 1.0 / aspect_ratio ) * 0.5;
        m_top    = ( 1.0 + 1.0 / aspect_ratio ) * 0.5;
    }
}

} // end of namespace opencv

} // end of namespace kvs
