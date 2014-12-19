/*****************************************************************************/
/**
 *  @file   StochasticPointRenderer.cpp
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
#include "StochasticPointRenderer.h"
#include <cmath>
#include <kvs/OpenGL>
#include <kvs/PointObject>
#include <kvs/Camera>
#include <kvs/Light>
#include <kvs/Assert>
#include <kvs/Message>
#include <kvs/Xorshift128>


namespace
{

/*===========================================================================*/
/**
 *  @brief  Returns a random number as integer value.
 *  @return random number
 */
/*===========================================================================*/
int RandomNumber()
{
    const int C = 12347;
    static kvs::Xorshift128 R;
    return C * R.randInteger();
}

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

}


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new StochasticPointRenderer class.
 */
/*===========================================================================*/
StochasticPointRenderer::StochasticPointRenderer():
    StochasticRendererBase( new Engine() )
{
}

/*===========================================================================*/
/**
 *  @brief  Sets an opacity value.
 *  @param  opacity [in] opacity value
 */
/*===========================================================================*/
void StochasticPointRenderer::setOpacity( const kvs::UInt8 opacity )
{
    static_cast<Engine&>( engine() ).setOpacity( opacity );
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new Engine class.
 */
/*===========================================================================*/
StochasticPointRenderer::Engine::Engine():
    m_point_opacity( 255 ),
    m_has_normal( false ),
    m_random_index( 0 )
{
}

/*===========================================================================*/
/**
 *  @brief  Releases the GPU resources.
 */
/*===========================================================================*/
void StochasticPointRenderer::Engine::release()
{
    m_shader_program.release();
    m_vbo.release();
    m_ibo.release();
}

/*===========================================================================*/
/**
 *  @brief  Create.
 *  @param  object [in] pointer to the point object
 *  @param  camera [in] pointer to the camera
 *  @param  light [in] pointer to the light
 */
/*===========================================================================*/
void StochasticPointRenderer::Engine::create( kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light )
{
    kvs::PointObject* point = kvs::PointObject::DownCast( object );
    m_has_normal = point->numberOfNormals() > 0;
    if ( !m_has_normal ) setEnabledShading( false );

    attachObject( object );
    createRandomTexture();
    this->create_shader_program();
    this->create_buffer_object( point );
}

/*===========================================================================*/
/**
 *  @brief  Update.
 *  @param  object [in] pointer to the object
 *  @param  camera [in] pointer to the camera
 *  @param  light [in] pointer to the light
 */
/*===========================================================================*/
void StochasticPointRenderer::Engine::update( kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light )
{
}

/*===========================================================================*/
/**
 *  @brief  Set up.
 *  @param  object [in] pointer to the object
 *  @param  camera [in] pointer to the camera
 *  @param  light [in] pointer to the light
 */
/*===========================================================================*/
void StochasticPointRenderer::Engine::setup( kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light )
{
    m_random_index = m_shader_program.attributeLocation("random_index");

    const kvs::Mat4 M = kvs::OpenGL::ModelViewMatrix();
    const kvs::Mat4 PM = kvs::OpenGL::ProjectionMatrix() * M;
    const kvs::Mat3 N = kvs::Mat3( M[0].xyz(), M[1].xyz(), M[2].xyz() );
    m_shader_program.bind();
    m_shader_program.setUniform( "ModelViewMatrix", M );
    m_shader_program.setUniform( "ModelViewProjectionMatrix", PM );
    m_shader_program.setUniform( "NormalMatrix", N );
    m_shader_program.setUniform( "random_texture_size_inv", 1.0f / randomTextureSize() );
    m_shader_program.setUniform( "random_texture", 0 );
    m_shader_program.setUniform( "opacity", m_point_opacity / 255.0f );
    m_shader_program.unbind();
}

/*===========================================================================*/
/**
 *  @brief  Draw an ensemble.
 *  @param  object [in] pointer to the line object
 *  @param  camera [in] pointer to the camera
 *  @param  light [in] pointer to the light
 */
/*===========================================================================*/
void StochasticPointRenderer::Engine::draw( kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light )
{
    kvs::PointObject* point = kvs::PointObject::DownCast( object );

    kvs::VertexBufferObject::Binder bind1( m_vbo );
    kvs::ProgramObject::Binder bind2( m_shader_program );
    kvs::Texture::Binder bind3( randomTexture() );
    {
        const size_t size = randomTextureSize();
        const int count = repetitionCount() * ::RandomNumber();
        const float offset_x = static_cast<float>( ( count ) % size );
        const float offset_y = static_cast<float>( ( count / size ) % size );
        const kvs::Vec2 random_offset( offset_x, offset_y );
        m_shader_program.setUniform( "random_offset", random_offset );

        const size_t nvertices = point->numberOfVertices();
        const size_t index_size = nvertices * 2 * sizeof( kvs::UInt16 );
        const size_t coord_size = nvertices * 3 * sizeof( kvs::Real32 );
        const size_t color_size = nvertices * 3 * sizeof( kvs::UInt8 );

        KVS_GL_CALL( glPointSize( point->size() ) );

        // Enable coords.
        KVS_GL_CALL( glEnableClientState( GL_VERTEX_ARRAY ) );
        KVS_GL_CALL( glVertexPointer( 3, GL_FLOAT, 0, (GLbyte*)NULL + index_size ) );

        // Enable colors.
        KVS_GL_CALL( glEnableClientState( GL_COLOR_ARRAY ) );
        KVS_GL_CALL( glColorPointer( 3, GL_UNSIGNED_BYTE, 0, (GLbyte*)NULL + index_size + coord_size ) );

        // Enable normals.
        if ( m_has_normal )
        {
            KVS_GL_CALL( glEnableClientState( GL_NORMAL_ARRAY ) );
            KVS_GL_CALL( glNormalPointer( GL_FLOAT, 0, (GLbyte*)NULL + index_size + coord_size + color_size ) );
        }

        // Enable random index.
        KVS_GL_CALL( glEnableVertexAttribArray( m_random_index ) );
        KVS_GL_CALL( glVertexAttribPointer( m_random_index, 2, GL_UNSIGNED_SHORT, GL_FALSE, 0, (GLubyte*)NULL + 0 ) );

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

        // Disable random index.
        KVS_GL_CALL( glDisableVertexAttribArray( m_random_index ) );
    }
}

/*===========================================================================*/
/**
 *  @brief  Creates shader program.
 */
/*===========================================================================*/
void StochasticPointRenderer::Engine::create_shader_program()
{
    kvs::ShaderSource vert( "SR_point.vert" );
    kvs::ShaderSource frag( "SR_point.frag" );
    if ( isEnabledShading() )
    {
        switch ( shader().type() )
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
    m_shader_program.setUniform( "shading.Ka", shader().Ka );
    m_shader_program.setUniform( "shading.Kd", shader().Kd );
    m_shader_program.setUniform( "shading.Ks", shader().Ks );
    m_shader_program.setUniform( "shading.S",  shader().S );
    m_shader_program.unbind();
}

/*===========================================================================*/
/**
 *  @brief  Create buffer objects.
 *  @param  point [in] pointer to the point object
 */
/*===========================================================================*/
void StochasticPointRenderer::Engine::create_buffer_object( const kvs::PointObject* point )
{
    const size_t nvertices = point->numberOfVertices();
    kvs::ValueArray<kvs::UInt16> indices( nvertices * 2 );
    for ( size_t i = 0; i < nvertices; i++ )
    {
        const unsigned int count = i * 12347;
        indices[ 2 * i + 0 ] = static_cast<kvs::UInt16>( ( count ) % randomTextureSize() );
        indices[ 2 * i + 1 ] = static_cast<kvs::UInt16>( ( count / randomTextureSize() ) % randomTextureSize() );
    }
    kvs::ValueArray<kvs::Real32> coords = point->coords();
    kvs::ValueArray<kvs::UInt8> colors = ::VertexColors( point );
    kvs::ValueArray<kvs::Real32> normals = point->normals();

    const size_t index_size = indices.byteSize();
    const size_t coord_size = coords.byteSize();
    const size_t color_size = colors.byteSize();
    const size_t normal_size = normals.byteSize();
    const size_t byte_size = index_size + coord_size + color_size + normal_size;

    m_vbo.create( byte_size );
    m_vbo.bind();
    m_vbo.load( index_size, indices.data(), 0 );
    m_vbo.load( coord_size, coords.data(), index_size );
    m_vbo.load( color_size, colors.data(), index_size + coord_size );
    if ( normal_size > 0 )
    {
        m_vbo.load( normal_size, normals.data(), index_size + coord_size + color_size );
    }
    m_vbo.unbind();
}

} // end of namespace kvs
