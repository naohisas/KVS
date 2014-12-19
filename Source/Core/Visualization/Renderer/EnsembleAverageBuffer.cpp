/*****************************************************************************/
/**
 *  @file   EnsembleAverageBuffer.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: EnsembleAverageBuffer.cpp 1457 2013-03-24 06:32:17Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#include "EnsembleAverageBuffer.h"
#include <kvs/OpenGL>

namespace
{

void Draw( const float r, const float g, const float b, const float a )
{
    kvs::OpenGL::WithPushedMatrix p1( GL_MODELVIEW );
    p1.loadIdentity();
    {
        kvs::OpenGL::WithPushedMatrix p2( GL_PROJECTION );
        p2.loadIdentity();
        {
            kvs::OpenGL::SetOrtho( 0, 1, 0, 1, -1, 1 );
            kvs::OpenGL::WithDisabled d1( GL_DEPTH_TEST );
            kvs::OpenGL::WithDisabled d2( GL_LIGHTING );
            kvs::OpenGL::WithEnabled e1( GL_TEXTURE_2D );
            {
                glBegin( GL_QUADS );
                glColor4f( r, g, b, a );
                glTexCoord2f( 0, 0 ); glVertex2f( 0, 0 );
                glTexCoord2f( 1, 0 ); glVertex2f( 1, 0 );
                glTexCoord2f( 1, 1 ); glVertex2f( 1, 1 );
                glTexCoord2f( 0, 1 ); glVertex2f( 0, 1 );
                glEnd();
            }
        }
    }
}

}

namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Creates the buffers.
 *  @param  width [in] buffer width
 *  @param  height [in] buffer height
 */
/*===========================================================================*/
void EnsembleAverageBuffer::create( const size_t width, const size_t height )
{
    m_count = 0;
    m_accum_framebuffer.create();
    m_current_framebuffer.create();

    m_accum_texture.setWrapS( GL_CLAMP_TO_EDGE );
    m_accum_texture.setWrapT( GL_CLAMP_TO_EDGE );
    m_accum_texture.setMagFilter( GL_NEAREST );
    m_accum_texture.setMinFilter( GL_NEAREST );
    m_accum_texture.setPixelFormat( GL_RGB32F_ARB, GL_RGB, GL_FLOAT );
    m_accum_texture.create( width, height );
    m_accum_framebuffer.attachColorTexture( m_accum_texture );

    m_current_color_texture.setWrapS( GL_CLAMP_TO_EDGE );
    m_current_color_texture.setWrapT( GL_CLAMP_TO_EDGE );
    m_current_color_texture.setMagFilter( GL_LINEAR );
    m_current_color_texture.setMinFilter( GL_LINEAR );
    m_current_color_texture.setPixelFormat( GL_RGB8, GL_RGB, GL_UNSIGNED_BYTE );
    m_current_color_texture.create( width, height );
    m_current_framebuffer.attachColorTexture( m_current_color_texture );

    m_current_depth_texture.setWrapS( GL_CLAMP_TO_EDGE );
    m_current_depth_texture.setWrapT( GL_CLAMP_TO_EDGE );
    m_current_depth_texture.setMagFilter( GL_LINEAR );
    m_current_depth_texture.setMinFilter( GL_LINEAR );
    m_current_depth_texture.setPixelFormat( GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT, GL_FLOAT );
    m_current_depth_texture.create( width, height );
    m_current_framebuffer.attachDepthTexture( m_current_depth_texture );
}

/*===========================================================================*/
/**
 *  @brief  Release the buffer resources.
 */
/*===========================================================================*/
void EnsembleAverageBuffer::release()
{
    m_current_color_texture.release();
    m_current_depth_texture.release();
    m_current_framebuffer.release();
    m_accum_texture.release();
    m_accum_framebuffer.release();
    m_count = 0;
}

/*===========================================================================*/
/**
 *  @brief  Clear the accumulation buffer.
 */
/*===========================================================================*/
void EnsembleAverageBuffer::clear()
{
    kvs::FrameBufferObject::Binder binder( m_accum_framebuffer );
    ::Draw( 0.0f, 0.0f, 0.0f, 0.0f );
    m_count = 0;
}

/*===========================================================================*/
/**
 *  @brief  Bind the current buffer.
 */
/*===========================================================================*/
void EnsembleAverageBuffer::bind()
{
    m_current_framebuffer.bind();
    kvs::OpenGL::Clear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
}

/*===========================================================================*/
/**
 *  @brief  Unbind the current buffer.
 */
/*===========================================================================*/
void EnsembleAverageBuffer::unbind()
{
    m_current_framebuffer.unbind();
}

/*===========================================================================*/
/**
 *  @brief  Accumulate the current buffer to the accumulation buffer.
 */
/*===========================================================================*/
void EnsembleAverageBuffer::add()
{
    m_count++;

    kvs::FrameBufferObject::Binder binder1( m_accum_framebuffer );
    {
        kvs::OpenGL::WithDisabled d1( GL_DEPTH_TEST );
        kvs::OpenGL::WithEnabled e1( GL_BLEND );
        {
            kvs::OpenGL::SetBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
            kvs::Texture::Binder binder2( m_current_color_texture );
            ::Draw( 1.0f, 1.0f, 1.0f, 1.0f / m_count );
        }
    }
}

/*===========================================================================*/
/**
 *  @brief  Draw the accumulation buffer.
 */
/*===========================================================================*/
void EnsembleAverageBuffer::draw()
{
    kvs::Texture::Binder binder( m_accum_texture );
    ::Draw( 1.0f, 1.0f, 1.0f, 1.0f );
}

} // end of namespace kvs
