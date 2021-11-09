/*****************************************************************************/
/**
 *  @file   MovieRenderer.cpp
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#include "MovieRenderer.h"
#include <kvs/IgnoreUnusedVariable>
#include <kvs/OpenGL>


namespace kvs
{

namespace ffmpeg
{

/*===========================================================================*/
/**
 *  @brief  Executes rendering process.
 *  @param  object [in] input movie object
 *  @param  camera [in] camera
 *  @param  light [in] light (not used)
 */
/*===========================================================================*/
void MovieRenderer::exec( kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light )
{
    kvs::IgnoreUnusedVariable( light );
    BaseClass::startTimer();

    auto* movie = kvs::ffmpeg::MovieObject::DownCast( object );
    if ( !movie ) { return; }

    kvs::OpenGL::Clear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    kvs::OpenGL::WithPushedAttrib p( GL_ALL_ATTRIB_BITS );
    p.disable( GL_DEPTH_TEST );
    p.disable( GL_TEXTURE_1D );
    p.disable( GL_TEXTURE_3D );
    p.enable( GL_TEXTURE_2D );

    if ( !this->object() ) { this->setObject( object ); }
    else { this->updateObject( object ); }

    if ( !this->texture().isValid() ) { this->createTexture( movie ); }
    if ( this->isCenteringEnabled() ) { this->alignCenter( camera ); }

    this->beginFrame( movie );
    {
        const auto width = movie->width();
        const auto height = movie->height();
        const auto buffer = movie->currentBuffer(); // RGBRGB...
        this->texture().bind();
        this->texture().load( width, height, buffer.data() );
        this->drawTexture();
        this->texture().unbind();
    }
    this->endFrame( movie );

    kvs::OpenGL::SetClearDepth( 1000 );
    BaseClass::stopTimer();
}

/*===========================================================================*/
/**
 *  @brief  Updates the object.
 *  @param  object [in] movie object
 */
/*===========================================================================*/
void MovieRenderer::updateObject( const kvs::ObjectBase* object )
{
    if ( m_object != object )
    {
        this->setObject( object );
        this->texture().release();
    }
}

/*===========================================================================*/
/**
 *  @brief  Creates texture of current frame
 *  @param  object [in] movie object
 */
/*===========================================================================*/
void MovieRenderer::createTexture( const kvs::ffmpeg::MovieObject* object )
{
    const double width  = object->width();
    const double height = object->height();
    m_initial_aspect_ratio = width / height;
    m_left = 0.0;
    m_right = 1.0;
    m_bottom = 0.0;
    m_top = 1.0;

    m_texture.release();
    m_texture.setWrapS( GL_REPEAT );
    m_texture.setWrapT( GL_REPEAT );
    m_texture.setMagFilter( GL_LINEAR );
    m_texture.setMinFilter( GL_LINEAR );
    m_texture.setPixelFormat( GL_RGB8, GL_RGB, GL_UNSIGNED_BYTE );
    m_texture.create( width, height );
}

/*===========================================================================*/
/**
 *  @brief  Aligns texture on center of screen.
 *  @param  camera [in] camera
 */
/*===========================================================================*/
void MovieRenderer::alignCenter( const kvs::Camera* camera )
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
 *  @brief  Draws current frame texture.
 */
/*===========================================================================*/
void MovieRenderer::drawTexture()
{
    kvs::OpenGL::WithPushedMatrix p1( GL_MODELVIEW );
    p1.loadIdentity();
    {
        kvs::OpenGL::WithPushedMatrix p2( GL_PROJECTION );
        p2.loadIdentity();
        {
            kvs::OpenGL::SetOrtho( m_left, m_right, m_bottom, m_top, -1, 1 );
            kvs::OpenGL::Begin( GL_QUADS );
            if ( m_is_mirroring_enabled )
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

/*===========================================================================*/
/**
 *  @brief  Setups rendering frame of movie object
 *  @param  object [in] movie object
 */
/*===========================================================================*/
void MovieRenderer::beginFrame( kvs::ffmpeg::MovieObject* object )
{
    if ( object->isLastFrame() )
    {
        if ( this->isPlaying() && !m_reset_flag )
        {
            this->stop();
        }
    }

    if ( m_reset_flag )
    {
        object->jumpToFrame(0);
        m_reset_flag = false;
    }
}

/*===========================================================================*/
/**
 *  @brief  Updates rendering frame of movie object
 *  @param  object [in] movie object
 */
/*===========================================================================*/
void MovieRenderer::endFrame( kvs::ffmpeg::MovieObject* object )
{
    if ( this->isPlaying() )
    {
        object->jumpToNextFrame();
    }
}

} // end of namespace ffmpeg

} // end of namespace kvs
