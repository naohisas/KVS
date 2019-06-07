/*****************************************************************************/
/**
 *  @file   StochasticTetrahedraRenderer.cpp
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
/*----------------------------------------------------------------------------
 *
 * References:
 * [1] Naohisa Sakamoto, Koji Koyamada, "A Stochastic Approach for Rendering
 *     Multiple Irregular Volumes", In Proc. of IEEE Pacific Visualization
 *     2014 (VisNotes), pp.272-276, 2014.3.
 */
/*****************************************************************************/
#include "StochasticTetrahedraRenderer.h"
#include <cmath>
#include <kvs/OpenGL>
#include <kvs/UnstructuredVolumeObject>
#include <kvs/Camera>
#include <kvs/Light>
#include <kvs/Assert>
#include <kvs/Message>
#include <kvs/Type>
#include <kvs/Xorshift128>
#include <kvs/TetrahedralCell>
#include <kvs/ProjectedTetrahedraTable>
#include <kvs/PreIntegrationTable2D>


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
        const unsigned int count = i * RandomNumber();
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
kvs::ValueArray<kvs::Real32> VertexNormalsForType( const kvs::UnstructuredVolumeObject* volume )
{
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

/*===========================================================================*/
/**
 *  @brief  Returns normal vector array for type of value array of the input volume.
 *  @param  volume [in] pointer to the volume object
 */
/*===========================================================================*/
kvs::ValueArray<kvs::Real32> VertexNormals( const kvs::UnstructuredVolumeObject* volume )
{
    switch ( volume->values().typeID() )
    {
    case kvs::Type::TypeReal32:
    case kvs::Type::TypeReal64:
        return VertexNormalsForType( volume );
    default: break;
    }

    kvsMessageError("Not supported data type.");
    return kvs::ValueArray<kvs::Real32>();
}

}


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new StochasticTetrahedraRenderer class.
 */
/*===========================================================================*/
StochasticTetrahedraRenderer::StochasticTetrahedraRenderer():
    StochasticRendererBase( new Engine() )
{
}

/*===========================================================================*/
/**
 *  @brief  Sets a transfer function.
 *  @param  transfer_function [in] transfer function
 */
/*===========================================================================*/
void StochasticTetrahedraRenderer::setTransferFunction( const kvs::TransferFunction& transfer_function )
{
    static_cast<Engine&>( engine() ).setTransferFunction( transfer_function );
}

/*===========================================================================*/
/**
 *  @brief  Sets a sampling step.
 *  @param  sampling_step [in] sampling step
 */
/*===========================================================================*/
void StochasticTetrahedraRenderer::setSamplingStep( const float sampling_step )
{
    static_cast<Engine&>( engine() ).setSamplingStep( sampling_step );
}

const kvs::TransferFunction& StochasticTetrahedraRenderer::transferFunction() const
{
    return static_cast<const Engine&>( engine() ).transferFunction();
}

float StochasticTetrahedraRenderer::samplingStep() const
{
    return static_cast<const Engine&>( engine() ).samplingStep();
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new Engine class.
 */
/*===========================================================================*/
StochasticTetrahedraRenderer::Engine::Engine():
    m_transfer_function_changed( true ),
    m_sampling_step( 1.0f ),
    m_maxT( 0.0f )
{
}

/*===========================================================================*/
/**
 *  @brief  Releases the GPU resources.
 */
/*===========================================================================*/
void StochasticTetrahedraRenderer::Engine::release()
{
    m_shader_program.release();
    m_vbo_manager.release();
    m_preintegration_texture.release();
    m_decomposition_texture.release();
    m_transfer_function_texture.release();
    m_T_texture.release();
    m_inv_T_texture.release();
    m_transfer_function_changed = true;
}

/*===========================================================================*/
/**
 *  @brief  Create shaders and buffer objects.
 *  @param  object [in] pointer to the polygon object
 *  @param  camera [in] pointer to the camera
 *  @param  light [in] pointer to the light
 */
/*===========================================================================*/
void StochasticTetrahedraRenderer::Engine::create( kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light )
{
    kvs::UnstructuredVolumeObject* volume = kvs::UnstructuredVolumeObject::DownCast( object );

    attachObject( object );
    createRandomTexture();
    this->create_shader_program();
    this->create_buffer_object( volume );
    this->create_preintegration_texture();
    this->create_decomposition_texture();
}

/*===========================================================================*/
/**
 *  @brief  Update.
 *  @param  polygon [in] pointer to the polygon object
 *  @param  camera [in] pointer to the camera
 *  @param  light [in] pointer to the light
 */
/*===========================================================================*/
void StochasticTetrahedraRenderer::Engine::update( kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light )
{
}

/*===========================================================================*/
/**
 *  @brief  Set up.
 *  @param  polygon [in] pointer to the polygon object
 *  @param  camera [in] pointer to the camera
 *  @param  light [in] pointer to the light
 */
/*===========================================================================*/
void StochasticTetrahedraRenderer::Engine::setup( kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light )
{
    if ( m_transfer_function_changed )
    {
        // Re-create pre-integration table.
        m_preintegration_texture.release();
        m_transfer_function_texture.release();
        m_T_texture.release();
        m_inv_T_texture.release();
        this->create_preintegration_texture();
    }

    const kvs::Mat4 M = kvs::OpenGL::ModelViewMatrix();
    const kvs::Mat4 PM = kvs::OpenGL::ProjectionMatrix() * M;
    const kvs::Mat4 PM_inverse = PM.inverted();
    const kvs::Mat3 N = kvs::Mat3( M[0].xyz(), M[1].xyz(), M[2].xyz() );
    m_shader_program.bind();
    m_shader_program.setUniform( "ModelViewMatrix", M );
    m_shader_program.setUniform( "ModelViewProjectionMatrix", PM );
    m_shader_program.setUniform( "ModelViewProjectionMatrixInverse", PM_inverse );
    m_shader_program.setUniform( "NormalMatrix", N );
    m_shader_program.setUniform( "maxT", m_maxT );
    m_shader_program.setUniform( "sampling_step_inv", 1.0f / m_sampling_step );
    m_shader_program.setUniform( "delta", 0.5f / m_transfer_function.resolution() );
    m_shader_program.unbind();
}

/*===========================================================================*/
/**
 *  @brief  Draw an ensemble.
 *  @param  object [in] pointer to the unstructured volume object
 *  @param  camera [in] pointer to the camera
 *  @param  light [in] pointer to the light
 */
/*===========================================================================*/
void StochasticTetrahedraRenderer::Engine::draw( kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light )
{
    kvs::UnstructuredVolumeObject* volume = kvs::UnstructuredVolumeObject::DownCast( object );

    kvs::VertexBufferObjectManager::Binder bind1( m_vbo_manager );
    kvs::ProgramObject::Binder bind2( m_shader_program );
    kvs::Texture::Binder bind3( randomTexture(), 0 );
    kvs::Texture::Binder bind4( m_preintegration_texture, 1 );
    kvs::Texture::Binder bind5( m_decomposition_texture, 2 );
    kvs::Texture::Binder bind6( m_transfer_function_texture, 3 );
    kvs::Texture::Binder bind7( m_T_texture, 4 );
    kvs::Texture::Binder bind8( m_inv_T_texture, 5 );
    {
        kvs::OpenGL::WithEnabled d( GL_DEPTH_TEST );

        const size_t size = randomTextureSize();
        const int count = repetitionCount() * ::RandomNumber();
        const float offset_x = static_cast<float>( ( count ) % size );
        const float offset_y = static_cast<float>( ( count / size ) % size );
        const kvs::Vec2 random_offset( offset_x, offset_y );
        m_shader_program.setUniform( "random_offset", random_offset );

        const size_t ncells = volume->numberOfCells();
        m_vbo_manager.drawElements( GL_LINES_ADJACENCY_EXT, 4 * ncells );
    }
}

/*===========================================================================*/
/**
 *  @brief  Creates shader program.
 */
/*===========================================================================*/
void StochasticTetrahedraRenderer::Engine::create_shader_program()
{
    kvs::ShaderSource vert( "SR_tetrahedra.vert" );
    kvs::ShaderSource geom( "SR_tetrahedra.geom" );
    kvs::ShaderSource frag( "SR_tetrahedra.frag" );

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

    // Build shaders.
    m_shader_program.build( vert, geom, frag );

    // Set default parameters.
    kvs::ProgramObject::Binder bind( m_shader_program );
    {
        // Shading parameters.
        m_shader_program.setUniform( "shading.Ka", shader().Ka );
        m_shader_program.setUniform( "shading.Kd", shader().Kd );
        m_shader_program.setUniform( "shading.Ks", shader().Ks );
        m_shader_program.setUniform( "shading.S",  shader().S );

        m_shader_program.setUniform( "random_texture_size_inv", 1.0f / randomTextureSize() );
        m_shader_program.setUniform( "random_texture", 0 );
        m_shader_program.setUniform( "preintegration_texture", 1 );
        m_shader_program.setUniform( "decomposition_texture", 2 );
        m_shader_program.setUniform( "transfer_function_texture", 3 );
        m_shader_program.setUniform( "T_texture", 4 );
        m_shader_program.setUniform( "invT_texture", 5 );
    }
}

/*===========================================================================*/
/**
 *  @brief  Create buffer objects.
 *  @param  volume [in] pointer to the volume object
 */
/*===========================================================================*/
void StochasticTetrahedraRenderer::Engine::create_buffer_object( const kvs::UnstructuredVolumeObject* volume )
{
    if ( volume->cellType() != kvs::UnstructuredVolumeObject::Tetrahedra )
    {
        kvsMessageError("Not supported cell type.");
        return;
    }

    if ( volume->veclen() != 1 )
    {
        kvsMessageError("Not scalar volume.");
        return;
    }

    const kvs::ValueArray<kvs::UInt16> indices = ::RandomIndices( volume, randomTextureSize() );
    const kvs::ValueArray<kvs::Real32> values = ::NormalizedValues( volume );
    const kvs::ValueArray<kvs::Real32> coords = volume->coords();
    const kvs::ValueArray<kvs::Real32> normals = ::VertexNormals( volume );
    m_vbo_manager.setVertexAttribArray( indices, m_shader_program.attributeLocation("random_index"), 2 );
    m_vbo_manager.setVertexAttribArray( values, m_shader_program.attributeLocation("value"), 1 );
    m_vbo_manager.setVertexArray( coords, 3 );
    m_vbo_manager.setNormalArray( normals );
    m_vbo_manager.setIndexArray( volume->connections() );
    m_vbo_manager.create();
}

/*===========================================================================*/
/**
 *  @brief  Creates pre-integration texture.
 */
/*===========================================================================*/
void StochasticTetrahedraRenderer::Engine::create_preintegration_texture()
{
    kvs::PreIntegrationTable2D table;
    table.setTransferFunction( m_transfer_function );
    table.create();

    int resolution_inverse_texture_size = kvs::Math::Min( 16384, kvs::OpenGL::MaxTextureSize() );
    m_shader_program.bind();
    m_shader_program.setUniform( "delta2", 0.5f / resolution_inverse_texture_size );
    m_shader_program.unbind();

    kvs::ValueArray<kvs::Real32> T = table.T();
    kvs::ValueArray<kvs::Real32> invT = table.inverseT( resolution_inverse_texture_size );
    size_t resolution = T.size();

    m_maxT = T.back();

    m_T_texture.setWrapS( GL_CLAMP_TO_EDGE );
    m_T_texture.setMagFilter( GL_LINEAR );
    m_T_texture.setMinFilter( GL_LINEAR );
    m_T_texture.setPixelFormat( GL_R32F, GL_RED, GL_FLOAT  );
    m_T_texture.create( T.size(), T.data() );

    m_inv_T_texture.setWrapS( GL_CLAMP_TO_EDGE );
    m_inv_T_texture.setMagFilter( GL_LINEAR );
    m_inv_T_texture.setMinFilter( GL_LINEAR );
    m_inv_T_texture.setPixelFormat( GL_R32F, GL_RED, GL_FLOAT  );
    m_inv_T_texture.create( invT.size(), invT.data() );

    m_transfer_function_texture.setWrapS( GL_CLAMP_TO_EDGE );
    m_transfer_function_texture.setMagFilter( GL_LINEAR );
    m_transfer_function_texture.setMinFilter( GL_LINEAR );
    m_transfer_function_texture.setPixelFormat( GL_RGBA32F_ARB, GL_RGBA, GL_FLOAT );
    m_transfer_function_texture.create( m_transfer_function.resolution(), m_transfer_function.table().data() );

    m_preintegration_texture.setWrapS( GL_CLAMP_TO_EDGE );
    m_preintegration_texture.setWrapT( GL_CLAMP_TO_EDGE );
    m_preintegration_texture.setMagFilter( GL_LINEAR );
    m_preintegration_texture.setMinFilter( GL_LINEAR );
    m_preintegration_texture.setPixelFormat( GL_R32F, GL_RED, GL_FLOAT );
    m_preintegration_texture.create( resolution, resolution, table.table().data() );

    m_transfer_function_changed = false;
}

/*===========================================================================*/
/**
 *  @brief  Creates decomposition texture.
 */
/*===========================================================================*/
void StochasticTetrahedraRenderer::Engine::create_decomposition_texture()
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
