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

inline void Draw()
{
    kvs::OpenGL::WithPushedMatrix p1( GL_MODELVIEW );
    p1.loadIdentity();
    {
        kvs::OpenGL::WithPushedMatrix p2( GL_PROJECTION );
        p2.loadIdentity();
        {
            kvs::OpenGL::SetOrtho( 0, 1, 0, 1, -1, 1 );
            kvs::OpenGL::WithDisabled d1( GL_DEPTH_TEST );
            kvs::OpenGL::WithEnabled e1( GL_TEXTURE_2D );
            {
                kvs::OpenGL::Begin( GL_QUADS );
                kvs::OpenGL::TexCoordVertex( kvs::Vec2( 0, 0 ), kvs::Vec2( 0, 0 ) );
                kvs::OpenGL::TexCoordVertex( kvs::Vec2( 1, 0 ), kvs::Vec2( 1, 0 ) );
                kvs::OpenGL::TexCoordVertex( kvs::Vec2( 1, 1 ), kvs::Vec2( 1, 1 ) );
                kvs::OpenGL::TexCoordVertex( kvs::Vec2( 0, 1 ), kvs::Vec2( 0, 1 ) );
                kvs::OpenGL::End();
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
    m_accum_texture.setMagFilter( GL_LINEAR );
    m_accum_texture.setMinFilter( GL_LINEAR );
    m_accum_texture.setPixelFormat( GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE );
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

    const std::string vert(
        "void main()"
        "{"
        "    gl_TexCoord[0] = gl_Vertex;"
        "    gl_Position = vec4( gl_Vertex.xy * 2.0 - 1.0, 0.0, 1.0 );"
        "}"
        );

    const std::string frag_average(
        "uniform sampler2D current_color_buffer;"
        "uniform sampler2D current_depth_buffer;"
        "uniform sampler2D accum_buffer;"
        "uniform float alpha;"
        "void main()"
        "{"
        "    vec2 p = gl_TexCoord[0].xy;"
        "    vec3 Csrc = texture2D( current_color_buffer, p ).rgb;"
        "    vec3 Cdst = texture2D( accum_buffer, p ).rgb;"
        "    float Dsrc = texture2D( current_depth_buffer, p ).z;"
        "    float Ddst = texture2D( accum_buffer, p ).a;"
        "    gl_FragColor = vec4( alpha * Csrc + ( 1.0 - alpha ) * Cdst, min( Dsrc, Ddst ) );"
        "}"
        );

    const std::string frag_drawing(
        "uniform sampler2D accum_buffer;"
        "void main()"
        "{"
        "    vec2 p = gl_TexCoord[0].xy;"
        "    vec4 c = texture2D( accum_buffer, p );"
        "    gl_FragColor = vec4( c.rgb, 1.0 );"
        "    gl_FragDepth = c.a;"
        "}"
        );

    m_average_shader.build( vert, frag_average );
    m_drawing_shader.build( vert, frag_drawing );
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
    m_average_shader.release();
    m_drawing_shader.release();
    m_count = 0;
}

/*===========================================================================*/
/**
 *  @brief  Clear the accumulation buffer.
 */
/*===========================================================================*/
void EnsembleAverageBuffer::clear()
{
    m_count = 0;

    kvs::FrameBufferObject::Binder binder( m_accum_framebuffer );
    kvs::OpenGL::Clear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
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

    kvs::FrameBufferObject::Binder fbo( m_accum_framebuffer );
    kvs::Texture::Binder tex0( m_current_color_texture, 0 );
    kvs::Texture::Binder tex1( m_current_depth_texture, 1 );
    kvs::Texture::Binder tex2( m_accum_texture, 2 );
    {
        kvs::OpenGL::WithDisabled d( GL_DEPTH_TEST );
        m_average_shader.bind();
        m_average_shader.setUniform( "current_color_buffer", 0 );
        m_average_shader.setUniform( "current_depth_buffer", 1 );
        m_average_shader.setUniform( "accum_buffer", 2 );
        m_average_shader.setUniform( "alpha", 1.0f / m_count );
        ::Draw();
        m_average_shader.unbind();
    }
}

/*===========================================================================*/
/**
 *  @brief  Draw the accumulation buffer.
 */
/*===========================================================================*/
void EnsembleAverageBuffer::draw()
{
    kvs::Texture::Binder tex( m_accum_texture, 0 );
    {
        m_drawing_shader.bind();
        m_drawing_shader.setUniform( "accum_texture", 0 );
        ::Draw();
        m_drawing_shader.unbind();
    }
}

} // end of namespace kvs
