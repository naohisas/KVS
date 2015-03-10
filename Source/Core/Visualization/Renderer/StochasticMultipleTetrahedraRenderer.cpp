/*****************************************************************************/
/**
 *  @file   StochasticMultipleTetrahedraRenderer.cpp
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
#include "StochasticMultipleTetrahedraRenderer.h"
#include <cmath>
#include <kvs/OpenGL>
#include <kvs/UnstructuredVolumeObject>
#include <kvs/Camera>
#include <kvs/Light>
#include <kvs/Assert>
#include <kvs/Message>
#include <kvs/Type>
#include <kvs/IgnoreUnusedVariable>
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
    if ( !volume->hasMinMaxValues() ) volume->updateMinMaxValues();
    const kvs::Real64 min_value = volume->minValue();
    const kvs::Real64 max_value = volume->maxValue();
    const kvs::Real64 normalized_factor = 1.0 / ( max_value - min_value );

    const T* src = static_cast<const T*>( volume->values().data() );
    const size_t nvalues = volume->values().size();
    kvs::ValueArray<kvs::Real32> dst( nvalues );
    for ( size_t i = 0; i < nvalues; i++ )
    {
        dst[i] = static_cast<kvs::Real32>( ( src[i] - min_value ) * normalized_factor );
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
 *  @brief  Constructs a new StochasticMultipleTetrahedraRenderer class.
 */
/*===========================================================================*/
StochasticMultipleTetrahedraRenderer::StochasticMultipleTetrahedraRenderer():
    StochasticRendererBase( new Engine() )
{
}

/*===========================================================================*/
/**
 *  @brief  Rendering process.
 */
/*===========================================================================*/
void StochasticMultipleTetrahedraRenderer::exec( kvs::ObjectBase*, kvs::Camera*, kvs::Light* )
{
    /* Rendering method has not been implemented in this renderer class.
     * Multiple tetrahedra rendering can be done with the engine via the
     * StochasticMultipleTetrahedraCompositor class.
     *
     * For example,
     *
     *     typedef kvs::StochasticMultipleTetrahedraRenderer Renderer;
     *     typedef kvs::UnstructuredVolumeObject Object;
     *     typedef kvs::UnstructuredVolumeImporter Importer;
     *     typedef kvs::StochasticMultipleTetrahedraCompositor Compositor;
     *
     *     Object* object1 = new Importer( filename1 );
     *     Object* object2 = new Importer( filename2 );
     *     Renderer* renderer = new Renderer();
     *
     *     kvs::glut::Screen screen( &app );
     *     screen.registerObject( objec1, renderer ); // Same renderer must be
     *     screen.registerObject( objec2, renderer ); // assigned for each object.
     *
     *     Compositor compositor;
     *     compositor.setRepetitionLevel( 30 );
     *     compositor.enbleLODControl();
     *     screen.setEvent( &compositor );
     *     screen.show();
     */
}

/*===========================================================================*/
/**
 *  @brief  Returns the pointer to the volume specified by given index.
 *  @param  index [in] index of volume (0 or 1)
 *  @return pointer to the volume
 */
/*===========================================================================*/
const kvs::UnstructuredVolumeObject* StochasticMultipleTetrahedraRenderer::volume( const size_t index )
{
    return static_cast<Engine&>( engine() ).volume( index );
}

/*===========================================================================*/
/**
 *  @brief  Attaches a volume objct.
 *  @param  volume [in] pointer to the volume object
 *  @param  index [in] index of volume (0 or 1)
 */
/*===========================================================================*/
void StochasticMultipleTetrahedraRenderer::attachVolume(
    const kvs::UnstructuredVolumeObject* volume,
    const size_t index )
{
    static_cast<Engine&>( engine() ).attachVolume( volume, index );
}

/*===========================================================================*/
/**
 *  @brief  Sets a extra texture.
 *  @param  extra_texture [in] extra texture
 */
/*===========================================================================*/
void StochasticMultipleTetrahedraRenderer::setExtraTexture( const kvs::Texture2D& extra_texture )
{
    static_cast<Engine&>( engine() ).setExtraTexture( extra_texture );
}

/*===========================================================================*/
/**
 *  @brief  Sets a transfer functio.
 *  @param  transfer_function [in] transfer function
 *  @param  index [in] index of volume (0 or 1)
 */
/*===========================================================================*/
void StochasticMultipleTetrahedraRenderer::setTransferFunction(
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
StochasticMultipleTetrahedraRenderer::Engine::Engine()
{
    m_volume[0] = NULL;
    m_volume[1] = NULL;
    m_random_index[0] = 0;
    m_random_index[1] = 0;
    m_value[0] = 0;
    m_value[1] = 0;
    m_transfer_function_changed[0] = true;
    m_transfer_function_changed[1] = true;
}

/*===========================================================================*/
/**
 *  @brief  Releases the GPU resources.
 */
/*===========================================================================*/
void StochasticMultipleTetrahedraRenderer::Engine::release()
{
    m_shader_program[0].release();
    m_shader_program[1].release();
    m_vbo[0].release();
    m_vbo[1].release();
    m_ibo[0].release();
    m_ibo[1].release();
    m_preintegration_texture[0].release();
    m_preintegration_texture[1].release();
    m_decomposition_texture.release();
    m_extra_texture.release();
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
void StochasticMultipleTetrahedraRenderer::Engine::create( kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light )
{
    kvs::IgnoreUnusedVariable( object );

    createRandomTexture();
    this->create_shader_program();
    this->create_buffer_object();
    this->create_preintegration_texture( 0 );
    this->create_preintegration_texture( 1 );
    this->create_decomposition_texture();
}

/*===========================================================================*/
/**
 *  @brief  Update.
 *  @param  object [in] pointer to the object
 *  @param  camera [in] pointer to the camera
 *  @param  light [in] pointer to the light
 */
/*===========================================================================*/
void StochasticMultipleTetrahedraRenderer::Engine::update( kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light )
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
void StochasticMultipleTetrahedraRenderer::Engine::setup( kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light )
{
    for ( size_t i = 0; i < 2; i++ )
    {
        m_random_index[i] = m_shader_program[i].attributeLocation("random_index");
        m_value[i] = m_shader_program[i].attributeLocation("value");

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
void StochasticMultipleTetrahedraRenderer::Engine::draw( kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light )
{
    if ( m_extra_texture.isCreated() )
    {
        const GLenum buffers[] = { GL_COLOR_ATTACHMENT0_EXT, GL_COLOR_ATTACHMENT1_EXT, };
        KVS_GL_CALL( glDrawBuffers( 2, buffers ) );
    }

    kvs::Texture::Binder bind0( randomTexture(), 0 );
    kvs::Texture::Binder bind1( m_preintegration_texture[0], 1 );
    kvs::Texture::Binder bind2( m_preintegration_texture[1], 2 );
    kvs::Texture::Binder bind3( m_decomposition_texture, 3 );
    if ( depthTexture().isCreated() ) { kvs::Texture::Bind( depthTexture(), 4 ); }
    if ( m_extra_texture.isCreated() ) { kvs::Texture::Bind( m_extra_texture, 5 ); }

    for ( size_t i = 0; i < 2; i++ )
    {
        kvs::VertexBufferObject::Binder bind6( m_vbo[i] );
        kvs::IndexBufferObject::Binder bind7( m_ibo[i] );
        kvs::ProgramObject::Binder bind8( m_shader_program[i] );

        // Set shader parameters.
        {
            const size_t size = randomTextureSize();
            const int count = repetitionCount() * 12347;
            const float offset_x = static_cast<float>( ( count ) % size );
            const float offset_y = static_cast<float>( ( count / size ) % size );
            const kvs::Vec2 random_offset( offset_x, offset_y );
            m_shader_program[i].setUniform( "random_texture_size_inv", 1.0f / randomTextureSize() );
            m_shader_program[i].setUniform( "random_offset", random_offset );
            m_shader_program[i].setUniform( "random_texture", 0 );

            const kvs::Vec2 screen_scale( camera->windowWidth() * 0.5f, camera->windowHeight() * 0.5f );
            const kvs::Vec2 screen_scale_inv( 1.0f / camera->windowWidth(), 1.0f / camera->windowHeight() );
            m_shader_program[i].setUniform( "screen_scale", screen_scale );
            m_shader_program[i].setUniform( "screen_scale_inv", screen_scale_inv );

            const float max_size_of_cell = 1.0f;
            const size_t depth_resolution = 128;
            const kvs::Vec2 preintegration_scale_offset(
                1.0f - 1.0f / depth_resolution / max_size_of_cell,
                1.0f / ( 2.0f * depth_resolution ) );
            m_shader_program[i].setUniform( "preintegration_scale_offset", preintegration_scale_offset );
            m_shader_program[i].setUniform( "preintegration_texture0", 1 );
            m_shader_program[i].setUniform( "preintegration_texture1", 2 );
            m_shader_program[i].setUniform( "decomposition_texture", 3 );
            m_shader_program[i].setUniform( "depth_texture", 4 );
            m_shader_program[i].setUniform( "extra_texture", 5 );
        }

        // Draw object.
        {
            const size_t nnodes = m_volume[i]->numberOfNodes();
            const size_t ncells = m_volume[i]->numberOfCells();
            const size_t index_size = nnodes * 2 * sizeof( kvs::UInt16 );
            const size_t value_size = nnodes * sizeof( kvs::Real32 );
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
            KVS_GL_CALL( glEnableVertexAttribArray( m_value[i] ) );
            KVS_GL_CALL( glVertexAttribPointer( m_value[i], 1, GL_FLOAT, GL_FALSE, 0, (GLubyte*)NULL + value_offset ) );

            // Enable random index.
            KVS_GL_CALL( glEnableVertexAttribArray( m_random_index[i] ) );
            KVS_GL_CALL( glVertexAttribPointer( m_random_index[i], 2, GL_UNSIGNED_SHORT, GL_FALSE, 0, (GLubyte*)NULL + index_offset ) );

            // Draw
            KVS_GL_CALL( glDrawElements( GL_LINES_ADJACENCY_EXT, 4 * ncells, GL_UNSIGNED_INT, 0 ) );

            // Disable normals.
            KVS_GL_CALL( glDisableClientState( GL_NORMAL_ARRAY ) );

            // Disable coords.
            KVS_GL_CALL( glDisableClientState( GL_VERTEX_ARRAY ) );

            // Disable value index.
            KVS_GL_CALL( glDisableVertexAttribArray( m_value[i] ) );

            // Disable random index.
            KVS_GL_CALL( glDisableVertexAttribArray( m_random_index[i] ) );
        }
    }
    if ( depthTexture().isCreated() ) { kvs::Texture::Unbind( depthTexture(), 4 ); }
    if ( m_extra_texture.isCreated() ) { kvs::Texture::Unbind( m_extra_texture, 5 ); }
}

/*===========================================================================*/
/**
 *  @brief  Creates shader program.
 */
/*===========================================================================*/
void StochasticMultipleTetrahedraRenderer::Engine::create_shader_program()
{
    kvs::ShaderSource vert[2] = {
        kvs::ShaderSource( "SR_multiple_tetrahedra1.vert" ),
        kvs::ShaderSource( "SR_multiple_tetrahedra2.vert" ) };

    kvs::ShaderSource geom[2] = {
        kvs::ShaderSource( "SR_multiple_tetrahedra1.geom" ),
        kvs::ShaderSource( "SR_multiple_tetrahedra2.geom" ) };

    kvs::ShaderSource frag[2] = {
        kvs::ShaderSource( "SR_multiple_tetrahedra1.frag" ),
        kvs::ShaderSource( "SR_multiple_tetrahedra2.frag" ) };

    for ( size_t i = 0; i < 2; i++ )
    {
        if ( isEnabledShading() )
        {
            switch ( shader().type() )
            {
            case kvs::Shader::LambertShading: frag[i].define("ENABLE_LAMBERT_SHADING"); break;
            case kvs::Shader::PhongShading: frag[i].define("ENABLE_PHONG_SHADING"); break;
            case kvs::Shader::BlinnPhongShading: frag[i].define("ENABLE_BLINN_PHONG_SHADING"); break;
            default: break; // NO SHADING
            }

            if ( kvs::OpenGL::Boolean( GL_LIGHT_MODEL_TWO_SIDE ) == GL_TRUE )
            {
                frag[i].define("ENABLE_TWO_SIDE_LIGHTING");
            }
        }

        // Parameters for geometry shader.
        m_shader_program[i].setGeometryInputType( GL_LINES_ADJACENCY_EXT );
        m_shader_program[i].setGeometryOutputType( GL_TRIANGLE_STRIP );
        m_shader_program[i].setGeometryOutputVertices( 4 * 3 );

        // Build shaders and set default shading parameters.
        m_shader_program[i].build( vert[i], geom[i], frag[i] );
        m_shader_program[i].bind();
        m_shader_program[i].setUniform( "shading.Ka", shader().Ka );
        m_shader_program[i].setUniform( "shading.Kd", shader().Kd );
        m_shader_program[i].setUniform( "shading.Ks", shader().Ks );
        m_shader_program[i].setUniform( "shading.S",  shader().S );
        m_shader_program[i].unbind();
    }
}

/*===========================================================================*/
/**
 *  @brief  Create buffer objects.
 *  @param  volume [in] pointer to the volume object
 */
/*===========================================================================*/
void StochasticMultipleTetrahedraRenderer::Engine::create_buffer_object()
{
    KVS_ASSERT( m_volume[0] );
    KVS_ASSERT( m_volume[1] );

    for ( size_t i = 0; i < 2; i++ )
    {
        if ( m_volume[i]->cellType() != kvs::UnstructuredVolumeObject::Tetrahedra )
        {
            kvsMessageError("Not supported cell type.");
            return;
        }

        if ( m_volume[i]->veclen() != 1 )
        {
            kvsMessageError("Not scalar volume.");
            return;
        }

        // Create VBO and IBO.
        {
            kvs::ValueArray<kvs::UInt16> indices = ::RandomIndices( m_volume[i], randomTextureSize() );
            kvs::ValueArray<kvs::Real32> values = ::NormalizedValues( m_volume[i] );
            kvs::ValueArray<kvs::Real32> coords = m_volume[i]->coords();
            kvs::ValueArray<kvs::Real32> normals = ::VertexNormals( m_volume[i] );

            size_t index_size = indices.byteSize();
            size_t value_size = values.byteSize();
            size_t coord_size = coords.byteSize();
            size_t normal_size = normals.byteSize();
            size_t byte_size = index_size + value_size + coord_size + normal_size;

            size_t index_offset = 0;
            size_t value_offset = index_offset + index_size;
            size_t coord_offset = value_offset + value_size;
            size_t normal_offset = coord_offset + coord_size;

            m_vbo[i].create( byte_size );
            m_vbo[i].bind();
            m_vbo[i].load( index_size, indices.data(), index_offset );
            m_vbo[i].load( value_size, values.data(), value_offset );
            m_vbo[i].load( coord_size, coords.data(), coord_offset );
            m_vbo[i].load( normal_size, normals.data(), normal_offset );
            m_vbo[i].unbind();

            kvs::ValueArray<kvs::UInt32> connections = m_volume[i]->connections();
            size_t connection_offset = 0;
            size_t connection_size = connections.byteSize();

            m_ibo[i].create( connection_size );
            m_ibo[i].bind();
            m_ibo[i].load( connection_size, connections.data(), connection_offset );
            m_ibo[i].unbind();
        }
    }
}

/*===========================================================================*/
/**
 *  @brief  Creates a pre-integration table.
 *  @param  index [in] component index (0 or 1)
 */
/*===========================================================================*/
void StochasticMultipleTetrahedraRenderer::Engine::create_preintegration_texture( const size_t index )
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
void StochasticMultipleTetrahedraRenderer::Engine::create_decomposition_texture()
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
