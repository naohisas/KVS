/*****************************************************************************/
/**
 *  @file   PointRendererGLSL.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: PointRenderer.cpp 1414 2013-01-21 02:16:48Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#include "PointRenderer.h"
#include <kvs/DebugNew>
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
 *  @param  point [in] pointer to the point object
 */
/*===========================================================================*/
kvs::ValueArray<kvs::UInt8> VertexColors( const kvs::PointObject* point )
{
    if ( point->numberOfVertices() == point->numberOfColors() ) return point->colors();

    const size_t nvertices = point->numberOfVertices();
    const kvs::RGBColor color = point->color();

    kvs::ValueArray<kvs::UInt8> colors( nvertices * 3 );
    for ( size_t i = 0; i < nvertices; i++ )
    {
        colors[ 3 * i + 0 ] = color.r();
        colors[ 3 * i + 1 ] = color.g();
        colors[ 3 * i + 2 ] = color.b();
    }

    return colors;
}

} // end of namespace


namespace kvs
{

namespace glsl
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new PointRenderer class.
 */
/*===========================================================================*/
PointRenderer::PointRenderer():
    m_width( 0 ),
    m_height( 0 ),
    m_object( NULL ),
    m_has_normal( false ),
    m_shader( NULL )
{
    this->setShader( kvs::Shader::Lambert() );
}

/*===========================================================================*/
/**
 *  @brief  Destructs the PointRenderer class.
 */
/*===========================================================================*/
PointRenderer::~PointRenderer()
{
    if ( m_shader ) { delete m_shader; }
}

/*===========================================================================*/
/**
 *  @brief  Executes the rendering process.
 *  @param  object [in] pointer to the object
 *  @param  camera [in] pointer to the camera
 *  @param  light [in] pointer to the light
 */
/*===========================================================================*/
void PointRenderer::exec( kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light )
{
    kvs::PointObject* point = kvs::PointObject::DownCast( object );
    m_has_normal = point->numberOfNormals() > 0;
    if ( !m_has_normal ) setEnabledShading( false );

    BaseClass::startTimer();
    kvs::OpenGL::WithPushedAttrib p( GL_ALL_ATTRIB_BITS );
    kvs::OpenGL::Enable( GL_DEPTH_TEST );

    const size_t width = camera->windowWidth();
    const size_t height = camera->windowHeight();
    const bool window_created = m_width == 0 && m_height == 0;
    if ( window_created )
    {
        m_width = width;
        m_height = height;
        m_object = object;
        this->create_shader_program();
        this->create_buffer_object( point );
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
        m_vbo.release();
        this->create_shader_program();
        this->create_buffer_object( point );
    }

    kvs::VertexBufferObject::Binder bind1( m_vbo );
    kvs::ProgramObject::Binder bind2( m_shader_program );
    {
        const kvs::Mat4 M = kvs::OpenGL::ModelViewMatrix();
        const kvs::Mat4 PM = kvs::OpenGL::ProjectionMatrix() * M;
        const kvs::Mat3 N = kvs::Mat3( M[0].xyz(), M[1].xyz(), M[2].xyz() );
        m_shader_program.setUniform( "ModelViewMatrix", M );
        m_shader_program.setUniform( "ModelViewProjectionMatrix", PM );
        m_shader_program.setUniform( "NormalMatrix", N );

        const size_t nvertices = point->numberOfVertices();
        const size_t coord_size = nvertices * 3 * sizeof( kvs::Real32 );
        const size_t color_size = nvertices * 3 * sizeof( kvs::UInt8 );

        KVS_GL_CALL( glPointSize( point->size() ) );

        // Enable coords.
        KVS_GL_CALL( glEnableClientState( GL_VERTEX_ARRAY ) );
        KVS_GL_CALL( glVertexPointer( 3, GL_FLOAT, 0, (GLbyte*)NULL + 0 ) );

        // Enable colors.
        KVS_GL_CALL( glEnableClientState( GL_COLOR_ARRAY ) );
        KVS_GL_CALL( glColorPointer( 3, GL_UNSIGNED_BYTE, 0, (GLbyte*)NULL + coord_size ) );

        // Enable normals.
        if ( m_has_normal )
        {
            KVS_GL_CALL( glEnableClientState( GL_NORMAL_ARRAY ) );
            KVS_GL_CALL( glNormalPointer( GL_FLOAT, 0, (GLbyte*)NULL + coord_size + color_size ) );
        }

        // Draw points.
        KVS_GL_CALL( glDrawArrays( GL_POINTS, 0, nvertices ) );

        // Disable coords.
        KVS_GL_CALL( glDisableClientState( GL_VERTEX_ARRAY ) );

        // Disable colors.
        KVS_GL_CALL( glDisableClientState( GL_COLOR_ARRAY ) );

        // Disable normals.
        if ( m_has_normal )
        {
            KVS_GL_CALL( glDisableClientState( GL_NORMAL_ARRAY ) );
        }
    }

    BaseClass::stopTimer();
}

/*===========================================================================*/
/**
 *  @brief  Creates shader program.
 */
/*===========================================================================*/
void PointRenderer::create_shader_program()
{
    kvs::ShaderSource vert( "shader.vert" );
    kvs::ShaderSource frag( "shader.frag" );
    if ( isEnabledShading() )
    {
        switch ( m_shader->type() )
        {
        case kvs::Shader::LambertShading: frag.define("ENABLE_LAMBERT_SHADING"); break;
        case kvs::Shader::PhongShading: frag.define("ENABLE_PHONG_SHADING"); break;
        case kvs::Shader::BlinnPhongShading: frag.define("ENABLE_BLINN_PHONG_SHADING"); break;
        default: break; /* NO SHADING */
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
 *  @param  point [in] pointer to the point object
 */
/*===========================================================================*/
void PointRenderer::create_buffer_object( const kvs::PointObject* point )
{
    kvs::ValueArray<kvs::Real32> coords = point->coords();
    kvs::ValueArray<kvs::UInt8> colors = ::VertexColors( point );
    kvs::ValueArray<kvs::Real32> normals = point->normals();

    const size_t coord_size = coords.byteSize();
    const size_t color_size = colors.byteSize();
    const size_t normal_size = normals.byteSize();
    const size_t byte_size = coord_size + color_size + normal_size;

    m_vbo.create( byte_size );
    m_vbo.bind();
    m_vbo.load( coord_size, coords.data(), 0 );
    m_vbo.load( color_size, colors.data(), coord_size );
    if ( normal_size > 0 )
    {
        m_vbo.load( normal_size, normals.data(), coord_size + color_size );
    }
    m_vbo.unbind();
}

} // end of namespace glsl

} // end of namespace kvs
