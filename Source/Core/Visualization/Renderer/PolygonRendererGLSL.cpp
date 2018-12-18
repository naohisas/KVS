/*****************************************************************************/
/**
 *  @file   PolygonRendererGLSL.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: PolygonRenderer.cpp 1414 2013-01-21 02:16:48Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#include "PolygonRenderer.h"
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
 *  @param  polygon [in] pointer to the polygon object
 */
/*===========================================================================*/
kvs::ValueArray<kvs::UInt8> VertexColors( const kvs::PolygonObject* polygon )
{
    const size_t nvertices = polygon->numberOfVertices();
    const bool is_single_color = polygon->colors().size() == 3;
    const bool is_single_alpha = polygon->opacities().size() == 1;
    const kvs::UInt8* pcolors = polygon->colors().data();
    const kvs::UInt8* palphas = polygon->opacities().data();

    kvs::ValueArray<kvs::UInt8> colors( nvertices * 4 );
    for ( size_t i = 0; i < nvertices; i++ )
    {
        colors[ 4 * i + 0 ] = is_single_color ? pcolors[0] : pcolors[ 3 * i + 0 ];
        colors[ 4 * i + 1 ] = is_single_color ? pcolors[1] : pcolors[ 3 * i + 1 ];
        colors[ 4 * i + 2 ] = is_single_color ? pcolors[2] : pcolors[ 3 * i + 2 ];
        colors[ 4 * i + 3 ] = is_single_alpha ? palphas[0] : palphas[i];
    }

    return colors;
}

/*===========================================================================*/
/**
 *  @brief  Returns vertex-normal array.
 *  @param  polygon [in] pointer to the polygon object
 */
/*===========================================================================*/
kvs::ValueArray<kvs::Real32> VertexNormals( const kvs::PolygonObject* polygon )
{
    if ( polygon->normals().size() == 0 )
    {
        return kvs::ValueArray<kvs::Real32>();
    }

    kvs::ValueArray<kvs::Real32> normals;
    switch ( polygon->normalType() )
    {
    case kvs::PolygonObject::VertexNormal:
    {
        normals = polygon->normals();
        break;
    }
    case kvs::PolygonObject::PolygonNormal:
    {
        // Same normal vectors are assigned for each vertex of the polygon.
        const size_t npolygons = polygon->normals().size() / 3;
        const size_t nnormals = npolygons * 3;
        normals.allocate( nnormals * 3 );
        kvs::Real32* pnormals = normals.data();
        for ( size_t i = 0; i < npolygons; i++ )
        {
            const kvs::Vec3 n = polygon->normal(i);
            for ( size_t j = 0; j < 3; j++ )
            {
                *(pnormals++) = n.x();
                *(pnormals++) = n.y();
                *(pnormals++) = n.z();
            }
        }
        break;
    }
    default: break;
    }

    return normals;
}

} // end of namespace


namespace kvs
{

namespace glsl
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new PolygonRenderer class.
 */
/*===========================================================================*/
PolygonRenderer::PolygonRenderer():
    m_width( 0 ),
    m_height( 0 ),
    m_object( NULL ),
    m_has_normal( false ),
    m_has_connection( false ),
    m_shader( NULL )
{
    this->setShader( kvs::Shader::Lambert() );
}

/*===========================================================================*/
/**
 *  @brief  Destroys the PolygonRenderer class.
 */
/*===========================================================================*/
PolygonRenderer::~PolygonRenderer()
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
void PolygonRenderer::exec( kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light )
{
    kvs::PolygonObject* polygon = kvs::PolygonObject::DownCast( object );
    m_has_normal = polygon->normals().size() > 0;
    m_has_connection = polygon->numberOfConnections() > 0;
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
        this->create_buffer_object( polygon );
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
        this->create_buffer_object( polygon );
    }

    kvs::VertexBufferObjectManager::Binder bind1( m_vbo_manager );
    kvs::ProgramObject::Binder bind2( m_shader_program );
    {
        kvs::OpenGL::SetPolygonMode( GL_FRONT_AND_BACK, GL_FILL );

        const kvs::Mat4 M = kvs::OpenGL::ModelViewMatrix();
        const kvs::Mat4 PM = kvs::OpenGL::ProjectionMatrix() * M;
        const kvs::Mat3 N = kvs::Mat3( M[0].xyz(), M[1].xyz(), M[2].xyz() );
        m_shader_program.setUniform( "ModelViewMatrix", M );
        m_shader_program.setUniform( "ModelViewProjectionMatrix", PM );
        m_shader_program.setUniform( "NormalMatrix", N );

        const size_t nconnections = polygon->numberOfConnections();
        const size_t nvertices = polygon->numberOfVertices();
        const size_t npolygons = nconnections == 0 ? nvertices / 3 : nconnections;

        // Draw triangles.
        if ( m_has_connection )
        {
            m_vbo_manager.drawElements( GL_TRIANGLES, 3 * npolygons );
        }
        else
        {
            m_vbo_manager.drawArrays( GL_TRIANGLES, 0, 3 * npolygons );
        }
    }

    BaseClass::stopTimer();
}

/*===========================================================================*/
/**
 *  @brief  Creates shader program.
 */
/*===========================================================================*/
void PolygonRenderer::create_shader_program()
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
 *  @param  polygon [in] pointer to the polygon object
 */
/*===========================================================================*/
void PolygonRenderer::create_buffer_object( const kvs::PolygonObject* polygon )
{
    if ( polygon->polygonType() != kvs::PolygonObject::Triangle )
    {
        kvsMessageError("Not supported polygon type.");
        return;
    }

    if ( polygon->colors().size() != 3 && polygon->colorType() == kvs::PolygonObject::PolygonColor )
    {
        kvsMessageError("Not supported polygon color type.");
        return;
    }

    kvs::ValueArray<kvs::Real32> coords = polygon->coords();
    kvs::ValueArray<kvs::UInt8> colors = ::VertexColors( polygon );
    kvs::ValueArray<kvs::Real32> normals = ::VertexNormals( polygon );

    m_vbo_manager.setVertexArray( coords, 3 );
    m_vbo_manager.setColorArray( colors, 3 );
    if ( normals.size() > 0 ) { m_vbo_manager.setNormalArray( normals ); }
    if ( m_has_connection ) { m_vbo_manager.setIndexArray( polygon->connections() ); }
    m_vbo_manager.create();
}

} // end of namespace glsl

} // end of namespace kvs
