/*****************************************************************************/
/**
 *  @file   StochasticTetrahedraRenderer.cpp
 *  @author Naohisa Sakamoto
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
kvs::ValueArray<kvs::UInt16> RandomIndices(
    const kvs::UnstructuredVolumeObject* volume,
    const size_t size )
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
kvs::ValueArray<kvs::Real32> NormalizedValuesForType(
    const kvs::UnstructuredVolumeObject* volume )
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
kvs::ValueArray<kvs::Real32> NormalizedValues(
    const kvs::UnstructuredVolumeObject* volume )
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
kvs::ValueArray<kvs::Real32> VertexNormalsForType(
    const kvs::UnstructuredVolumeObject* volume )
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
kvs::ValueArray<kvs::Real32> VertexNormals(
    const kvs::UnstructuredVolumeObject* volume )
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
void StochasticTetrahedraRenderer::setTransferFunction(
    const kvs::TransferFunction& transfer_function )
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


void StochasticTetrahedraRenderer::Engine::TransferFunctionBuffer::create(
    const kvs::TransferFunction& tfunc )
{
    const size_t width = tfunc.resolution();
    const auto table = tfunc.table();
    m_texture.setWrapS( GL_CLAMP_TO_EDGE );
    m_texture.setMagFilter( GL_LINEAR );
    m_texture.setMinFilter( GL_LINEAR );
    m_texture.setPixelFormat( GL_RGBA32F_ARB, GL_RGBA, GL_FLOAT  );
    m_texture.create( width, table.data() );
}

void StochasticTetrahedraRenderer::Engine::TransferFunctionBuffer::update(
    const kvs::TransferFunction& tfunc )
{
    m_texture.release();
    this->create( tfunc );
}

void StochasticTetrahedraRenderer::Engine::TransferFunctionBuffer::release()
{
    m_texture.release();
}

int StochasticTetrahedraRenderer::Engine::PreIntegrationBuffer::inverseTextureSize() const
{
    return kvs::Math::Min( 16384, kvs::OpenGL::MaxTextureSize() );
}

void StochasticTetrahedraRenderer::Engine::PreIntegrationBuffer::create(
    const kvs::TransferFunction& tfunc )
{
    kvs::PreIntegrationTable2D table;
    table.setTransferFunction( tfunc );
    table.create();

    auto T = table.T();
    auto T_inv = table.inverseT( this->inverseTextureSize() );
    const auto resolution = T.size();

    m_T_max = T.back();

    m_T_texture.setWrapS( GL_CLAMP_TO_EDGE );
    m_T_texture.setMagFilter( GL_LINEAR );
    m_T_texture.setMinFilter( GL_LINEAR );
    m_T_texture.setPixelFormat( GL_R32F, GL_RED, GL_FLOAT  );
    m_T_texture.create( T.size(), T.data() );

    m_T_inv_texture.setWrapS( GL_CLAMP_TO_EDGE );
    m_T_inv_texture.setMagFilter( GL_LINEAR );
    m_T_inv_texture.setMinFilter( GL_LINEAR );
    m_T_inv_texture.setPixelFormat( GL_R32F, GL_RED, GL_FLOAT  );
    m_T_inv_texture.create( T_inv.size(), T_inv.data() );

    m_texture.setWrapS( GL_CLAMP_TO_EDGE );
    m_texture.setWrapT( GL_CLAMP_TO_EDGE );
    m_texture.setMagFilter( GL_LINEAR );
    m_texture.setMinFilter( GL_LINEAR );
    m_texture.setPixelFormat( GL_R32F, GL_RED, GL_FLOAT );
    m_texture.create( resolution, resolution, table.table().data() );
}

void StochasticTetrahedraRenderer::Engine::PreIntegrationBuffer::update(
    const kvs::TransferFunction& tfunc )
{
    m_texture.release();
    m_T_texture.release();
    m_T_inv_texture.release();
    this->create( tfunc );
}

void StochasticTetrahedraRenderer::Engine::PreIntegrationBuffer::release()
{
    m_texture.release();
    m_T_texture.release();
    m_T_inv_texture.release();
}

void StochasticTetrahedraRenderer::Engine::DecompositionBuffer::create()
{
    const size_t size = 81;
    kvs::ValueArray<GLubyte> table( size * 4 );
    for ( size_t i = 0; i < size; i++ )
    {
        table[ i * 4 + 0 ] = kvs::ProjectedTetrahedraTable::PatternInfo[i][1] * 32;
        table[ i * 4 + 1 ] = kvs::ProjectedTetrahedraTable::PatternInfo[i][2] * 32;
        table[ i * 4 + 2 ] = kvs::ProjectedTetrahedraTable::PatternInfo[i][3] * 32;
        table[ i * 4 + 3 ] = kvs::ProjectedTetrahedraTable::PatternInfo[i][0] * 32;
    }

    m_texture.setWrapS( GL_CLAMP_TO_EDGE );
    m_texture.setWrapT( GL_CLAMP_TO_EDGE );
    m_texture.setMagFilter( GL_NEAREST );
    m_texture.setMinFilter( GL_NEAREST );
    m_texture.setPixelFormat( GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE );
    m_texture.create( size, 1, table.data() );
}

void StochasticTetrahedraRenderer::Engine::BufferObject::create(
    const kvs::UnstructuredVolumeObject* volume,
    const kvs::ProgramObject& shader_program )
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

    const auto indices = ::RandomIndices( volume, m_engine->randomTextureSize() );
    const auto values = ::NormalizedValues( volume );
    const auto coords = volume->coords();
    const auto normals = ::VertexNormals( volume );
    m_manager.setVertexAttribArray( indices, shader_program.attributeLocation("random_index"), 2 );
    m_manager.setVertexAttribArray( values, shader_program.attributeLocation("value"), 1 );
    m_manager.setVertexArray( coords, 3 );
    m_manager.setNormalArray( normals );
    m_manager.setIndexArray( volume->connections() );
    m_manager.create();
}

void StochasticTetrahedraRenderer::Engine::BufferObject::draw(
    const kvs::UnstructuredVolumeObject* volume )
{
    const size_t ncells = volume->numberOfCells();
    kvs::VertexBufferObjectManager::Binder bind( m_manager );
    m_manager.drawElements( GL_LINES_ADJACENCY_EXT, 4 * ncells );
}

void StochasticTetrahedraRenderer::Engine::RenderPass::setShaderFiles(
    const std::string& vert_file,
    const std::string& geom_file,
    const std::string& frag_file )
{
    this->setVertexShaderFile( vert_file );
    this->setGeometryShaderFile( geom_file );
    this->setFragmentShaderFile( frag_file );
}

void StochasticTetrahedraRenderer::Engine::RenderPass::create(
    const kvs::Shader::ShadingModel& model,
    const bool shading,
    const bool exact )
{
    kvs::ShaderSource vert( m_vert_shader_file );
    kvs::ShaderSource geom( m_geom_shader_file );
    kvs::ShaderSource frag( m_frag_shader_file );
    if ( shading )
    {
        switch ( model.type() )
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

    if ( exact )
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
}

void StochasticTetrahedraRenderer::Engine::RenderPass::update(
    const kvs::Shader::ShadingModel& model,
    const bool shading,
    const bool exact )
{
    m_shader_program.release();
    this->create( model, shading, exact );
}

void StochasticTetrahedraRenderer::Engine::RenderPass::setup(
    const kvs::Shader::ShadingModel& model )
{
    kvs::ProgramObject::Binder bind( m_shader_program );
    m_shader_program.setUniform( "shading.Ka", model.Ka );
    m_shader_program.setUniform( "shading.Kd", model.Kd );
    m_shader_program.setUniform( "shading.Ks", model.Ks );
    m_shader_program.setUniform( "shading.S",  model.S );

    const kvs::Mat4 M = kvs::OpenGL::ModelViewMatrix();
    const kvs::Mat4 PM = kvs::OpenGL::ProjectionMatrix() * M;
    const kvs::Mat4 PM_inverse = PM.inverted();
    const kvs::Mat3 N = kvs::Mat3( M[0].xyz(), M[1].xyz(), M[2].xyz() );
    m_shader_program.setUniform( "ModelViewMatrix", M );
    m_shader_program.setUniform( "ModelViewProjectionMatrix", PM );
    m_shader_program.setUniform( "ModelViewProjectionMatrixInverse", PM_inverse );
    m_shader_program.setUniform( "NormalMatrix", N );
    m_shader_program.setUniform( "sampling_step_inv", 1.0f / m_sampling_step );
}

void StochasticTetrahedraRenderer::Engine::RenderPass::draw(
    const kvs::UnstructuredVolumeObject* volume )
{
    kvs::ProgramObject::Binder bind( m_shader_program );
    m_buffer_object.draw( volume );
}

/*===========================================================================*/
/**
 *  @brief  Releases the GPU resources.
 */
/*===========================================================================*/
void StochasticTetrahedraRenderer::Engine::release()
{
    m_render_pass.release();
    m_buffer_object.release();
    m_transfer_function_buffer.release();
    m_preintegration_buffer.release();
    m_decomposition_buffer.release();
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
void StochasticTetrahedraRenderer::Engine::create(
    kvs::ObjectBase* object,
    kvs::Camera* camera,
    kvs::Light* light )
{
    auto* volume = kvs::UnstructuredVolumeObject::DownCast( object );

    BaseClass::attachObject( object );
    BaseClass::createRandomTexture();
    this->create_shader_program();
    this->create_buffer_object( volume );
    this->create_transfer_function_texture();
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
void StochasticTetrahedraRenderer::Engine::update(
    kvs::ObjectBase* object,
    kvs::Camera* camera,
    kvs::Light* light )
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
void StochasticTetrahedraRenderer::Engine::setup(
    kvs::ObjectBase* object,
    kvs::Camera* camera,
    kvs::Light* light )
{
    if ( m_transfer_function_changed ) { this->update_transfer_function_texture(); }

    m_render_pass.setup( BaseClass::shader() );

    auto& shader_program = m_render_pass.shaderProgram();
    shader_program.bind();
    shader_program.setUniform( "maxT", m_preintegration_buffer.Tmax() );
    shader_program.setUniform( "delta", 0.5f / m_transfer_function.resolution() );
    shader_program.unbind();
}

/*===========================================================================*/
/**
 *  @brief  Draw an ensemble.
 *  @param  object [in] pointer to the unstructured volume object
 *  @param  camera [in] pointer to the camera
 *  @param  light [in] pointer to the light
 */
/*===========================================================================*/
void StochasticTetrahedraRenderer::Engine::draw(
    kvs::ObjectBase* object,
    kvs::Camera* camera,
    kvs::Light* light )
{
    kvs::OpenGL::WithEnabled d( GL_DEPTH_TEST );

    const size_t size = randomTextureSize();
    const int count = repetitionCount() * ::RandomNumber();
    const float offset_x = static_cast<float>( ( count ) % size );
    const float offset_y = static_cast<float>( ( count / size ) % size );
    const kvs::Vec2 random_offset( offset_x, offset_y );

    auto& shader_program = m_render_pass.shaderProgram();
    kvs::ProgramObject::Binder bind( shader_program );
    shader_program.setUniform( "random_offset", random_offset );

    kvs::Texture::Binder unit0( BaseClass::randomTexture(), 0 );
    kvs::Texture::Binder unit1( m_preintegration_buffer.texture(), 1 );
    kvs::Texture::Binder unit2( m_decomposition_buffer.texture(), 2 );
    kvs::Texture::Binder unit3( m_transfer_function_buffer.texture(), 3 );
    kvs::Texture::Binder unit4( m_preintegration_buffer.T(), 4 );
    kvs::Texture::Binder unit5( m_preintegration_buffer.Tinverse(), 5 );
    auto* volume = kvs::UnstructuredVolumeObject::DownCast( object );
    m_buffer_object.draw( volume );
}

/*===========================================================================*/
/**
 *  @brief  Creates shader program.
 */
/*===========================================================================*/
void StochasticTetrahedraRenderer::Engine::create_shader_program()
{
    m_render_pass.create(
        BaseClass::shader(),
        BaseClass::isShadingEnabled(),
        BaseClass::depthTexture().isCreated() );

    auto& shader_program = m_render_pass.shaderProgram();
    kvs::ProgramObject::Binder bind( shader_program );
    shader_program.setUniform( "random_texture_size_inv", 1.0f / randomTextureSize() );
    shader_program.setUniform( "random_texture", 0 );
    shader_program.setUniform( "preintegration_texture", 1 );
    shader_program.setUniform( "decomposition_texture", 2 );
    shader_program.setUniform( "transfer_function_texture", 3 );
    shader_program.setUniform( "T_texture", 4 );
    shader_program.setUniform( "invT_texture", 5 );
}

/*===========================================================================*/
/**
 *  @brief  Create buffer objects.
 *  @param  volume [in] pointer to the volume object
 */
/*===========================================================================*/
void StochasticTetrahedraRenderer::Engine::create_buffer_object(
    const kvs::UnstructuredVolumeObject* volume )
{
    m_buffer_object.create( volume, m_render_pass.shaderProgram() );
}

/*===========================================================================*/
/**
 *  @brief  Creates pre-integration texture and transfer function texture.
 */
/*===========================================================================*/
void StochasticTetrahedraRenderer::Engine::create_transfer_function_texture()
{
    m_preintegration_buffer.create( m_transfer_function );
    m_transfer_function_buffer.create( m_transfer_function );
    m_transfer_function_changed = false;

    const auto inv_size = m_preintegration_buffer.inverseTextureSize();
    auto& shader_program = m_render_pass.shaderProgram();
    shader_program.bind();
    shader_program.setUniform( "delta2", 0.5f / inv_size );
    shader_program.unbind();
}

void StochasticTetrahedraRenderer::Engine::update_transfer_function_texture()
{
    m_transfer_function_buffer.release();
    m_preintegration_buffer.release();
    this->create_transfer_function_texture();
}

/*===========================================================================*/
/**
 *  @brief  Creates decomposition texture.
 */
/*===========================================================================*/
void StochasticTetrahedraRenderer::Engine::create_decomposition_texture()
{
    m_decomposition_buffer.create();
}

} // end of namespace kvs
