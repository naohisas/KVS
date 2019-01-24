/*****************************************************************************/
/**
 *  @file   StochasticMultivariateTetrahedraRenderer.cpp
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
#include "StochasticMultivariateTetrahedraRenderer.h"
#include <cmath>
#include <kvs/OpenGL>
#include <kvs/UnstructuredVolumeObject>
#include <kvs/Camera>
#include <kvs/Light>
#include <kvs/Assert>
#include <kvs/Message>
#include <kvs/Type>
#include <kvs/Math>
#include <kvs/TetrahedralCell>
#include <kvs/ProjectedTetrahedraTable>
#include <kvs/PreIntegrationTable3D>


namespace
{

/*===========================================================================*/
/**
 *  @brief  Returns random index array.
 *  @param  volume [in] pointer to the volume object
 *  @param  size [in] random texture size
 */
/*===========================================================================*/
kvs::ValueArray<kvs::UInt16> RandomIndices( const kvs::UnstructuredVolumeObject* volume, const size_t size )
{
    const size_t nnodes = volume->numberOfNodes();
    kvs::ValueArray<kvs::UInt16> indices( nnodes * 2 );
    for ( size_t i = 0; i < nnodes; i++ )
    {
        const unsigned int count = i * 12347;
        indices[ 2 * i + 0 ] = static_cast<kvs::UInt16>( ( count ) % size );
        indices[ 2 * i + 1 ] = static_cast<kvs::UInt16>( ( count / size ) % size );
    }

    return indices;
}

/*===========================================================================*/
/**
 *  @brief  Returns normalized value array for type of value array of the input volume.
 *  @param  volume [in] pointer to the volume object
 */
/*===========================================================================*/
template <typename T>
kvs::ValueArray<kvs::Real32> NormalizedValuesForType( const kvs::UnstructuredVolumeObject* volume )
{
    const T* src = static_cast<const T*>( volume->values().data() );

    // Calculate min/max values for each component.
    const size_t veclen = volume->veclen();
    kvs::ValueArray<kvs::Real64> min_values( veclen );
    kvs::ValueArray<kvs::Real64> max_values( veclen );
    for ( size_t i = 0; i < veclen; i++ )
    {
        min_values[i] = src[i];
        max_values[i] = src[i];
    }

    const size_t nnodes = volume->numberOfNodes();
    for ( size_t i = 0; i < nnodes; i++ )
    {
        const T* values = src + i * veclen;
        for ( size_t j = 0; j < veclen; j++ )
        {
            min_values[j] = kvs::Math::Min( min_values[j], kvs::Real64( values[j] ) );
            max_values[j] = kvs::Math::Max( max_values[j], kvs::Real64( values[j] ) );
        }
    }

    // Calculate normalized factors for each component.
    kvs::ValueArray<kvs::Real64> normalized_factors( veclen );
    for ( size_t i = 0; i < veclen; i++ )
    {
        if ( max_values[i] == min_values[i] ) normalized_factors[i] = 1.0;
        else normalized_factors[i] = 1.0 / ( max_values[i] - min_values[i] );
    }

    // Calculate normalized values.
    kvs::ValueArray<kvs::Real32> dst( nnodes * veclen );
    for ( size_t i = 0, index = 0; i < nnodes; i++ )
    {
        const T* values = src + i * veclen;
        for ( size_t j = 0; j < veclen; j++, index++ )
        {
            dst[index] = static_cast<kvs::Real32>( ( values[j] - min_values[j] ) * normalized_factors[j] );
        }
    }

    return dst;
}

/*===========================================================================*/
/**
 *  @brief  Returns normalized value array for the input volume.
 *  @param  volume [in] pointer to the volume object
 */
/*===========================================================================*/
kvs::ValueArray<kvs::Real32> NormalizedValues( const kvs::UnstructuredVolumeObject* volume )
{
    switch ( volume->values().typeID() )
    {
    case kvs::Type::TypeReal32: return NormalizedValuesForType<kvs::Real32>( volume );
    case kvs::Type::TypeReal64: return NormalizedValuesForType<kvs::Real64>( volume );
    default: break;
    }

    kvsMessageError("Not supported data type.");
    return kvs::ValueArray<kvs::Real32>();
}

/*===========================================================================*/
/**
 *  @brief  Returns normal vector array for type of value array of the input volume.
 *  @param  volume [in] pointer to the volume object
 */
/*===========================================================================*/
kvs::ValueArray<kvs::Real32> VertexNormals( const kvs::UnstructuredVolumeObject* volume )
{
    KVS_ASSERT( volume->values().typeID() == kvs::Type::TypeReal32 ||
                volume->values().typeID() == kvs::Type::TypeReal64 );

    const size_t nnodes = volume->numberOfNodes();
    const size_t ncells = volume->numberOfCells();
    kvs::TetrahedralCell cell( volume );
    kvs::ValueArray<kvs::Int32> counter( nnodes );
    kvs::ValueArray<kvs::Real32> normals( nnodes * 3 );
    counter.fill(0);
    normals.fill(0);
    for ( size_t i = 0; i < ncells; i++ )
    {
        cell.bindCell( i );
        const kvs::Vec3 g = -cell.gradientVector();
        const kvs::UInt32 index0 = volume->connections()[ 4 * i + 0 ];
        const kvs::UInt32 index1 = volume->connections()[ 4 * i + 1 ];
        const kvs::UInt32 index2 = volume->connections()[ 4 * i + 2 ];
        const kvs::UInt32 index3 = volume->connections()[ 4 * i + 3 ];

        counter[ index0 ]++;
        counter[ index1 ]++;
        counter[ index2 ]++;
        counter[ index3 ]++;

        normals[ 3 * index0 + 0 ] += g.x();
        normals[ 3 * index0 + 1 ] += g.y();
        normals[ 3 * index0 + 2 ] += g.z();

        normals[ 3 * index1 + 0 ] += g.x();
        normals[ 3 * index1 + 1 ] += g.y();
        normals[ 3 * index1 + 2 ] += g.z();

        normals[ 3 * index2 + 0 ] += g.x();
        normals[ 3 * index2 + 1 ] += g.y();
        normals[ 3 * index2 + 2 ] += g.z();

        normals[ 3 * index3 + 0 ] += g.x();
        normals[ 3 * index3 + 1 ] += g.y();
        normals[ 3 * index3 + 2 ] += g.z();
    }

    for ( size_t i = 0; i < nnodes; i++ )
    {
        const kvs::Real32 c = static_cast<kvs::Real32>( counter[i] );
        const kvs::Vec3 v( normals.data() + 3 * i );
        const kvs::Vec3 n = ( v / c ).normalized();
        normals[ 3 * i + 0 ] = n.x();
        normals[ 3 * i + 1 ] = n.y();
        normals[ 3 * i + 2 ] = n.z();
    }

    return normals;
}

}


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new StochasticMultivariateTetrahedraRenderer class.
 */
/*===========================================================================*/
StochasticMultivariateTetrahedraRenderer::StochasticMultivariateTetrahedraRenderer():
    StochasticRendererBase( new Engine() )
{
}

/*===========================================================================*/
/**
 *  @brief  Shows the component specified by the given index.
 *  @param  index [in] index of component (0 or 1)
 */
/*===========================================================================*/
void StochasticMultivariateTetrahedraRenderer::showComponent( const size_t index )
{
    static_cast<Engine&>( engine() ).showComponent( index );
}

/*===========================================================================*/
/**
 *  @brief  Hides the component specified by the given index.
 *  @param  index [in] index of component (0 or 1)
 */
/*===========================================================================*/
void StochasticMultivariateTetrahedraRenderer::hideComponent( const size_t index )
{
    static_cast<Engine&>( engine() ).hideComponent( index );
}

/*===========================================================================*/
/**
 *  @brief  Sets a transfer function for the component specified by the given index.
 *  @param  transfer_function [in] transfer function
 *  @param  index [in] index of component (0 or 1)
 */
/*===========================================================================*/
void StochasticMultivariateTetrahedraRenderer::setTransferFunction(
    const kvs::TransferFunction& transfer_function,
    const size_t index )
{
    static_cast<Engine&>( engine() ).setTransferFunction( transfer_function, index );
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new Engine class.
 */
/*===========================================================================*/
StochasticMultivariateTetrahedraRenderer::Engine::Engine():
    m_random_index( 0 ),
    m_value( 0 )
{
    m_show_component[0] = true;
    m_show_component[1] = true;
    m_transfer_function_changed[0] = true;
    m_transfer_function_changed[1] = true;
}

/*===========================================================================*/
/**
 *  @brief  Releases the GPU resources.
 */
/*===========================================================================*/
void StochasticMultivariateTetrahedraRenderer::Engine::release()
{
    m_shader_program.release();
    m_vbo.release();
    m_ibo.release();
    m_decomposition_texture.release();
    m_preintegration_texture[0].release();
    m_preintegration_texture[1].release();
    m_transfer_function_changed[0] = true;
    m_transfer_function_changed[1] = true;
}

/*===========================================================================*/
/**
 *  @brief  Create shaders and buffer objects.
 *  @param  object [in] pointer to the polygon object
 *  @param  camera [in] pointer to the camera
 *  @param  light [in] pointer to the light
 */
/*===========================================================================*/
void StochasticMultivariateTetrahedraRenderer::Engine::create( kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light )
{
    kvs::UnstructuredVolumeObject* volume = kvs::UnstructuredVolumeObject::DownCast( object );

    attachObject( object );
    createRandomTexture();
    this->create_shader_program();
    this->create_buffer_object( volume );
    this->create_preintegration_texture( 0 );
    this->create_preintegration_texture( 1 );
    this->create_decomposition_texture();
}

/*===========================================================================*/
/**
 *  @brief  Update.
 *  @param  object [in] pointer to the polygon object
 *  @param  camera [in] pointer to the camera
 *  @param  light [in] pointer to the light
 */
/*===========================================================================*/
void StochasticMultivariateTetrahedraRenderer::Engine::update( kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light )
{
}

/*===========================================================================*/
/**
 *  @brief  Set up.
 *  @param  object [in] pointer to the polygon object
 *  @param  camera [in] pointer to the camera
 *  @param  light [in] pointer to the light
 */
/*===========================================================================*/
void StochasticMultivariateTetrahedraRenderer::Engine::setup( kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light )
{
    m_random_index = m_shader_program.attributeLocation("random_index");
    m_value = m_shader_program.attributeLocation("value");

    for ( size_t i = 0; i < 2; i++ )
    {
        if ( m_transfer_function_changed[i] )
        {
            m_preintegration_texture[i].release();
            this->create_preintegration_texture(i);
        }
    }
}

/*===========================================================================*/
/**
 *  @brief  Draw an ensemble.
 *  @param  object [in] pointer to the unstructured volume object
 *  @param  camera [in] pointer to the camera
 *  @param  light [in] pointer to the light
 */
/*===========================================================================*/
void StochasticMultivariateTetrahedraRenderer::Engine::draw( kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light )
{
    kvs::UnstructuredVolumeObject* volume = kvs::UnstructuredVolumeObject::DownCast( object );

    kvs::VertexBufferObject::Binder bind1( m_vbo );
    kvs::IndexBufferObject::Binder bind2( m_ibo );
    kvs::ProgramObject::Binder bind3( m_shader_program );
    kvs::Texture::Binder bind4( randomTexture(), 0 );
    kvs::Texture::Binder bind5( m_preintegration_texture[0], 1 );
    kvs::Texture::Binder bind6( m_preintegration_texture[1], 2 );
    kvs::Texture::Binder bind7( m_decomposition_texture, 3 );

    // Set shader parameters.
    {
        kvs::OpenGL::WithEnabled d( GL_DEPTH_TEST );
        const size_t size = randomTextureSize();
        const int count = repetitionCount() * 12347;
        const float offset_x = static_cast<float>( ( count ) % size );
        const float offset_y = static_cast<float>( ( count / size ) % size );
        const kvs::Vec2 random_offset( offset_x, offset_y );
        m_shader_program.setUniform( "random_texture_size_inv", 1.0f / randomTextureSize() );
        m_shader_program.setUniform( "random_offset", random_offset );
        m_shader_program.setUniform( "random_texture", 0 );

        const kvs::Vec2 screen_scale( camera->windowWidth() * 0.5f, camera->windowHeight() * 0.5f );
        const kvs::Vec2 screen_scale_inv( 1.0f / camera->windowWidth(), 1.0f / camera->windowHeight() );
        m_shader_program.setUniform( "screen_scale", screen_scale );
        m_shader_program.setUniform( "screen_scale_inv", screen_scale_inv );

        const float max_size_of_cell = 1.0f;
        const size_t depth_resolution = 128;
        const kvs::Vec2 preintegration_scale_offset(
            1.0f - 1.0f / depth_resolution / max_size_of_cell,
            1.0f / ( 2.0f * depth_resolution ) );
        m_shader_program.setUniform( "preintegration_scale_offset", preintegration_scale_offset );
        m_shader_program.setUniform( "preintegration_texture0", 1 );
        m_shader_program.setUniform( "preintegration_texture1", 2 );
        m_shader_program.setUniform( "decomposition_texture", 3 );

        const int start_volume = m_show_component[0] ? 0 : 1;
        const int end_volume = m_show_component[1] ? 2 : 1;
        m_shader_program.setUniform( "start_volume", start_volume );
        m_shader_program.setUniform( "end_volume", end_volume );
    }

    // Draw object.
    {
        const size_t nnodes = volume->numberOfNodes();
        const size_t ncells = volume->numberOfCells();
        const size_t veclen = volume->veclen();
        const size_t index_size = nnodes * 2 * sizeof( kvs::UInt16 );
        const size_t value_size = nnodes * veclen * sizeof( kvs::Real32 );
        const size_t coord_size = nnodes * 3 * sizeof( kvs::Real32 );

        const size_t index_offset = 0;
        const size_t value_offset = index_offset + index_size;
        const size_t coord_offset = value_offset + value_size;
        const size_t normal_offset = coord_offset + coord_size;

        // Enable coords.
        KVS_GL_CALL( glEnableClientState( GL_VERTEX_ARRAY ) );
        KVS_GL_CALL( glVertexPointer( 3, GL_FLOAT, 0, (GLbyte*)NULL + coord_offset ) );

        // Enable normals.
        KVS_GL_CALL( glEnableClientState( GL_NORMAL_ARRAY ) );
        KVS_GL_CALL( glNormalPointer( GL_FLOAT, 0, (GLbyte*)NULL + normal_offset ) );

        // Enable value index.
        KVS_GL_CALL( glEnableVertexAttribArray( m_value ) );
        KVS_GL_CALL( glVertexAttribPointer( m_value, veclen, GL_FLOAT, GL_FALSE, 0, (GLubyte*)NULL + value_offset ) );

        // Enable random index.
        KVS_GL_CALL( glEnableVertexAttribArray( m_random_index ) );
        KVS_GL_CALL( glVertexAttribPointer( m_random_index, 2, GL_UNSIGNED_SHORT, GL_FALSE, 0, (GLubyte*)NULL + index_offset ) );

        // Draw
        KVS_GL_CALL( glDrawElements( GL_LINES_ADJACENCY_EXT, 4 * ncells, GL_UNSIGNED_INT, 0 ) );

        // Disable normals.
        KVS_GL_CALL( glDisableClientState( GL_NORMAL_ARRAY ) );

        // Disable coords.
        KVS_GL_CALL( glDisableClientState( GL_VERTEX_ARRAY ) );

        // Disable value index.
        KVS_GL_CALL( glDisableVertexAttribArray( m_value ) );

        // Disable random index.
        KVS_GL_CALL( glDisableVertexAttribArray( m_random_index ) );
    }
}

/*===========================================================================*/
/**
 *  @brief  Creates shader program.
 */
/*===========================================================================*/
void StochasticMultivariateTetrahedraRenderer::Engine::create_shader_program()
{
    kvs::ShaderSource vert( "SR_multivariate_tetrahedra.vert" );
    kvs::ShaderSource geom( "SR_multivariate_tetrahedra.geom" );
    kvs::ShaderSource frag( "SR_multivariate_tetrahedra.frag" );

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

    if ( depthTexture().isCreated() )
    {
        vert.define("ENABLE_EXACT_DEPTH_TESTING");
        geom.define("ENABLE_EXACT_DEPTH_TESTING");
        frag.define("ENABLE_EXACT_DEPTH_TESTING");
    }

    // Parameters for geometry shader.
    m_shader_program.setGeometryInputType( GL_LINES_ADJACENCY_EXT );
    m_shader_program.setGeometryOutputType( GL_TRIANGLE_STRIP );
    m_shader_program.setGeometryOutputVertices( 4 * 3 );

    // Build shaders and set default shading parameters.
    m_shader_program.build( vert, geom, frag );
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
 *  @param  volume [in] pointer to the volume object
 */
/*===========================================================================*/
void StochasticMultivariateTetrahedraRenderer::Engine::create_buffer_object( const kvs::UnstructuredVolumeObject* volume )
{
    if ( volume->cellType() != kvs::UnstructuredVolumeObject::Tetrahedra )
    {
        kvsMessageError("Not supported cell type.");
        return;
    }

    if ( volume->veclen() != 2 )
    {
        // In the current implementation of this class, the input volume object
        // must have only two variables (components). And, these variables must
        // be stored in the value array of the volume as a vector volume.
        kvsMessageError("Not vector volume (Veclen is not equal to 2).");
        return;
    }

    const kvs::ValueArray<kvs::UInt16> indices = ::RandomIndices( volume, randomTextureSize() );
    const kvs::ValueArray<kvs::Real32> values = ::NormalizedValues( volume );
    const kvs::ValueArray<kvs::Real32> coords = volume->coords();
    const kvs::ValueArray<kvs::Real32> normals = ::VertexNormals( volume );

    const size_t index_size = indices.byteSize();
    const size_t value_size = values.byteSize();
    const size_t coord_size = coords.byteSize();
    const size_t normal_size = normals.byteSize();
    const size_t byte_size = index_size + value_size + coord_size + normal_size;

    const size_t index_offset = 0;
    const size_t value_offset = index_offset + index_size;
    const size_t coord_offset = value_offset + value_size;
    const size_t normal_offset = coord_offset + coord_size;

    m_vbo.create( byte_size );
    m_vbo.bind();
    m_vbo.load( index_size, indices.data(), index_offset );
    m_vbo.load( value_size, values.data(), value_offset );
    m_vbo.load( coord_size, coords.data(), coord_offset );
    m_vbo.load( normal_size, normals.data(), normal_offset );
    m_vbo.unbind();

    const kvs::ValueArray<kvs::UInt32> connections = volume->connections();
    const size_t connection_offset = 0;
    const size_t connection_size = connections.byteSize();

    m_ibo.create( connection_size );
    m_ibo.bind();
    m_ibo.load( connection_size, connections.data(), connection_offset );
    m_ibo.unbind();
}

/*===========================================================================*/
/**
 *  @brief  Creates a pre-integration table.
 *  @param  index [in] component index (0 or 1)
 */
/*===========================================================================*/
void StochasticMultivariateTetrahedraRenderer::Engine::create_preintegration_texture( const size_t index )
{
    KVS_ASSERT( index == 0 || index == 1 );

    const float max_size_of_cell = 1.0f;
    const size_t dim_scalar = 128;
    const size_t dim_depth = 128;
    kvs::PreIntegrationTable3D table;
    table.setScalarResolution( dim_scalar );
    table.setDepthResolution( dim_depth );
    table.setTransferFunction( m_transfer_function[index], 0.0f, 1.0f );
    table.create( max_size_of_cell );

    m_preintegration_texture[index].setWrapS( GL_CLAMP_TO_EDGE );
    m_preintegration_texture[index].setWrapT( GL_CLAMP_TO_EDGE );
    m_preintegration_texture[index].setWrapR( GL_CLAMP_TO_EDGE );
    m_preintegration_texture[index].setMagFilter( GL_LINEAR );
    m_preintegration_texture[index].setMinFilter( GL_LINEAR );
    m_preintegration_texture[index].setPixelFormat( GL_RGBA8, GL_RGBA, GL_FLOAT );
    m_preintegration_texture[index].create( dim_scalar, dim_scalar, dim_depth, table.table().data() );

    m_transfer_function_changed[index] = false;
}

/*===========================================================================*/
/**
 *  @brief  Creates a decomposition texture.
 */
/*===========================================================================*/
void StochasticMultivariateTetrahedraRenderer::Engine::create_decomposition_texture()
{
    kvs::ValueArray<GLubyte> table( 81 * 4 );
    for ( size_t i = 0; i < 81; i++ )
    {
        table[ i * 4 + 0 ] = kvs::ProjectedTetrahedraTable::PatternInfo[i][1] * 32;
        table[ i * 4 + 1 ] = kvs::ProjectedTetrahedraTable::PatternInfo[i][2] * 32;
        table[ i * 4 + 2 ] = kvs::ProjectedTetrahedraTable::PatternInfo[i][3] * 32;
        table[ i * 4 + 3 ] = kvs::ProjectedTetrahedraTable::PatternInfo[i][0] * 32;
    }

    m_decomposition_texture.setWrapS( GL_CLAMP_TO_EDGE );
    m_decomposition_texture.setWrapT( GL_CLAMP_TO_EDGE );
    m_decomposition_texture.setMagFilter( GL_NEAREST );
    m_decomposition_texture.setMinFilter( GL_NEAREST );
    m_decomposition_texture.setPixelFormat( GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE );
    m_decomposition_texture.create( 81, 1, table.data() );
}

} // end of namespace kvs
