/*****************************************************************************/
/**
 *  @file   LineRendererGLSL.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id$
 */
/*****************************************************************************/
#include "LineRendererGLSL.h"
#include <kvs/OpenGL>
#include <kvs/ProgramObject>
#include <kvs/ShaderSource>
#include <kvs/VertexShader>
#include <kvs/FragmentShader>


namespace
{

/*===========================================================================*/
/**
 *  @brief  Returns vertex-color array.
 *  @param  line [in] pointer to the line object
 */
/*===========================================================================*/
kvs::ValueArray<kvs::UInt8> VertexColors( const kvs::LineObject* line )
{
    if ( line->colorType() == kvs::LineObject::VertexColor ) return line->colors();

    const size_t nvertices = line->numberOfVertices();
    const kvs::RGBColor color = line->color();

    kvs::ValueArray<kvs::UInt8> colors( nvertices * 3 );
    for ( size_t i = 0; i < nvertices; i++ )
    {
        colors[ 3 * i + 0 ] = color.r();
        colors[ 3 * i + 1 ] = color.g();
        colors[ 3 * i + 2 ] = color.b();
    }

    return colors;
}

/*===========================================================================*/
/**
 *  @brief  Returns the normalized color in range of [0,1].
 *  @param  color [in] color
 *  @return normalized color
 */
/*===========================================================================*/
kvs::Vec3 NormalizedColor( const kvs::RGBColor color )
{
    const float scale = 1.0f / 255.0f;
    return kvs::Vec3( color.r() * scale, color.g() * scale, color.b() * scale );
}

}


namespace kvs
{

namespace glsl
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new LineRenderer class.
 */
/*===========================================================================*/
LineRenderer::LineRenderer():
    m_width( 0 ),
    m_height( 0 ),
    m_object( NULL ),
    m_has_connection( false ),
    m_shader( NULL ),
    m_line_width_range( 0.0f, 0.0f ),
    m_outline_width( 0.0f ),
    m_outline_color( kvs::RGBColor::Black() )
{
    this->setShader( kvs::Shader::Lambert() );
}

/*===========================================================================*/
/**
 *  @brief  Destroys the LineRenderer class.
 */
/*===========================================================================*/
LineRenderer::~LineRenderer()
{
    if ( m_shader ) { delete m_shader; }
}

/*===========================================================================*/
/**
 *  @brief  Executes rendering process.
 *  @param  object [in] pointer to the object
 *  @param  camera [in] pointer to the camera
 *  @param  light [in] pointer to the light
 */
/*===========================================================================*/
void LineRenderer::exec( kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light )
{
    kvs::LineObject* line = kvs::LineObject::DownCast( object );
    kvs::LineObject::LineType type = line->lineType();
    m_has_connection = line->numberOfConnections() > 0 &&
        ( type == kvs::LineObject::Segment || type == kvs::LineObject::Uniline );

    BaseClass::startTimer();

    kvs::OpenGL::WithPushedAttrib p( GL_ALL_ATTRIB_BITS );
    kvs::OpenGL::Enable( GL_DEPTH_TEST );
    kvs::OpenGL::Enable( GL_BLEND );
    kvs::OpenGL::SetBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

    const size_t width = camera->windowWidth();
    const size_t height = camera->windowHeight();
    const bool window_created = m_width == 0 && m_height == 0;
    if ( window_created )
    {
        m_width = width;
        m_height = height;
        m_object = object;
        this->create_shader_program();
        this->create_buffer_object( line );

        kvs::OpenGL::GetFloatv( GL_LINE_WIDTH_RANGE, &m_line_width_range[0] );
    }

    const bool window_resized = m_width != width || m_height != height;
    if ( window_resized )
    {
        m_width = width;
        m_height = height;
    }

    const bool object_changed = m_object != object;
    if ( object_changed )
    {
        m_object = object;
        m_shader_program.release();
        m_vbo_manager.release();
        this->create_shader_program();
        this->create_buffer_object( line );
    }

    kvs::VertexBufferObjectManager::Binder bind1( m_vbo_manager );
    kvs::ProgramObject::Binder bind2( m_shader_program );
    {
        const kvs::Mat4 M = kvs::OpenGL::ModelViewMatrix();
        const kvs::Mat4 PM = kvs::OpenGL::ProjectionMatrix() * M;
        const kvs::Mat3 N = kvs::Mat3( M[0].xyz(), M[1].xyz(), M[2].xyz() );
        m_shader_program.setUniform( "ModelViewMatrix", M );
        m_shader_program.setUniform( "ModelViewProjectionMatrix", PM );
        m_shader_program.setUniform( "NormalMatrix", N );

        const float dpr = camera->devicePixelRatio();
        const float line_width = kvs::Math::Min( line->size() + m_outline_width * 2.0f, m_line_width_range[1] );
        const float outline_width = kvs::Math::Min( m_outline_width, m_line_width_range[1] * 0.5f );
        const kvs::Vec3 outline_color = ::NormalizedColor( m_outline_color );
        m_shader_program.setUniform( "screen_width", float( m_width ) * dpr );
        m_shader_program.setUniform( "screen_height",  float( m_height ) * dpr );
        m_shader_program.setUniform( "line_width_range", m_line_width_range * dpr );
        m_shader_program.setUniform( "line_width", line_width * dpr );
        m_shader_program.setUniform( "outline_width", outline_width * dpr );
        m_shader_program.setUniform( "outline_color", outline_color );

        kvs::OpenGL::SetLineWidth( line_width * dpr );

        // Draw lines.
        if ( m_has_connection )
        {
            const size_t nlines = line->numberOfConnections();
            if ( line->lineType() == kvs::LineObject::Uniline )
            {
                m_vbo_manager.drawElements( GL_LINE_STRIP, nlines );
            }
            else if ( line->lineType() ==  kvs::LineObject::Segment )
            {
                m_vbo_manager.drawElements( GL_LINES, 2 * nlines );
            }
        }
        else
        {
            if ( line->lineType() == kvs::LineObject::Polyline )
            {
                m_vbo_manager.drawArrays( GL_LINE_STRIP, m_first_array, m_count_array );
            }
            else if ( line->lineType() == kvs::LineObject::Strip )
            {
                const size_t nvertices = line->numberOfVertices();
                m_vbo_manager.drawArrays( GL_LINE_STRIP, 0, nvertices );
            }
        }
    }

    BaseClass::stopTimer();
}

/*===========================================================================*/
/**
 *  @brief  Creates shader program.
 */
/*===========================================================================*/
void LineRenderer::create_shader_program()
{
    kvs::ShaderSource vert( "line.vert" );
    kvs::ShaderSource frag( "line.frag" );
    if ( isEnabledShading() )
    {
        switch ( m_shader->type() )
        {
        case kvs::Shader::LambertShading: frag.define("ENABLE_LAMBERT_SHADING"); break;
        case kvs::Shader::PhongShading: frag.define("ENABLE_PHONG_SHADING"); break;
        case kvs::Shader::BlinnPhongShading: frag.define("ENABLE_BLINN_PHONG_SHADING"); break;
        default: break; // NO SHADING
        }

        if ( kvs::OpenGL::Boolean( GL_LIGHT_MODEL_TWO_SIDE ) == GL_TRUE )
        {
            frag.define("ENABLE_TWO_SIDE_LIGHTING");
        }
    }

    m_shader_program.build( vert, frag );
    m_shader_program.bind();
    m_shader_program.setUniform( "shading.Ka", m_shader->Ka );
    m_shader_program.setUniform( "shading.Kd", m_shader->Kd );
    m_shader_program.setUniform( "shading.Ks", m_shader->Ks );
    m_shader_program.setUniform( "shading.S",  m_shader->S );
    m_shader_program.unbind();
}

/*===========================================================================*/
/**
 *  @brief  Creates buffer object.
 *  @param  line [in] pointer to the line object
 */
/*===========================================================================*/
void LineRenderer::create_buffer_object( const kvs::LineObject* line )
{
    if ( line->numberOfColors() != 1 && line->colorType() == kvs::LineObject::LineColor )
    {
        kvsMessageError("Not supported line color type.");
        return;
    }

    kvs::ValueArray<kvs::Real32> coords = line->coords();
    kvs::ValueArray<kvs::UInt8> colors = ::VertexColors( line );

    m_vbo_manager.setVertexArray( coords, 3 );
    m_vbo_manager.setColorArray( colors, 3 );
    if ( m_has_connection ) { m_vbo_manager.setIndexArray( line->connections() ); }
    m_vbo_manager.create();

    if ( ( !m_has_connection ) && ( line->lineType() == kvs::LineObject::Polyline ) )
    {
        const kvs::UInt32* pconnections = line->connections().data();
        m_first_array.allocate( line->numberOfConnections() );
        m_count_array.allocate( m_first_array.size() );
        for ( size_t i = 0; i < m_first_array.size(); ++i )
        {
            m_first_array[i] = pconnections[ 2 * i ];
            m_count_array[i] = pconnections[ 2 * i + 1 ] - pconnections[ 2 * i ] + 1;
        }
    }
}

} // end of namespace glsl

} // end of namespace kvs
