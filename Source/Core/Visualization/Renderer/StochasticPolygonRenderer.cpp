/*****************************************************************************/
/**
 *  @file   StochasticPolygonRenderer.cpp
 *  @author Naohisa Sakamoto, Zhao Kun
 */
/*****************************************************************************/
#include "StochasticPolygonRenderer.h"
#include <cmath>
#include <kvs/OpenGL>
#include <kvs/PolygonObject>
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
 *  @brief  Returns number of vertices of the polygon object
 *  @param  polygon [in] pointer to the polygon object
 *  @return number of vertices
 */
/*===========================================================================*/
size_t NumberOfVertices( const kvs::PolygonObject* polygon )
{
    if ( polygon->connections().size() > 0 &&
         ( polygon->normalType() == kvs::PolygonObject::PolygonNormal ||
           polygon->colorType() == kvs::PolygonObject::PolygonColor ) )
    {
        const size_t nfaces = polygon->numberOfConnections();
        return nfaces * 3;
    }

    return polygon->numberOfVertices();
}

} // end of namespace


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new StochasticPolygonRenderer class.
 */
/*===========================================================================*/
StochasticPolygonRenderer::StochasticPolygonRenderer():
    StochasticRendererBase( new Engine() )
{
}

/*===========================================================================*/
/**
 *  @brief  Sets depth offset.
 *  @param  offset [in] depth offset
 */
/*===========================================================================*/
void StochasticPolygonRenderer::setDepthOffset( const kvs::Vec2& offset )
{
    static_cast<Engine&>( engine() ).setDepthOffset( offset );
}

/*===========================================================================*/
/**
 *  @brief  Sets depth offset.
 *  @param  factor [in] scale factor
 *  @param  units [in] constant depth offset
 */
/*===========================================================================*/
void StochasticPolygonRenderer::setDepthOffset( const float factor, const float units )
{
    static_cast<Engine&>( engine() ).setDepthOffset( factor, units );
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new RenderPass class.
 *  @param  buffer_object [in] buffer object
 *  @param  parent [in] pointer to engin
 */
/*===========================================================================*/
StochasticPolygonRenderer::Engine::RenderPass::RenderPass(
    Engine::BufferObject& buffer_object,
    RenderPass::Parent* parent ):
    BaseRenderPass( buffer_object ),
    m_parent( parent )
{
    this->setVertexShaderFile( "SR_polygon.vert" );
    this->setFragmentShaderFile( "SR_polygon.frag" );
}

/*===========================================================================*/
/**
 *  @brief  Setups render pass.
 *  @param  shading_model [in] shading model 
 */
/*===========================================================================*/
void StochasticPolygonRenderer::Engine::RenderPass::setup(
    const kvs::Shader::ShadingModel& shading_model )
{
    BaseRenderPass::setup( shading_model );

    const auto size_inv = 1.0f / m_parent->randomTextureSize();
    auto& shader_program = this->shaderProgram();
    kvs::ProgramObject::Binder bind( shader_program );
    shader_program.setUniform( "random_texture_size_inv", size_inv );
    shader_program.setUniform( "random_texture", 0 );
}

/*===========================================================================*/
/**
 *  @brief  Draws buffer object.
 *  @param  object [in] pointer to polygon object
 */
/*===========================================================================*/
void StochasticPolygonRenderer::Engine::RenderPass::draw(
    const kvs::ObjectBase* object )
{
    const size_t size = m_parent->randomTextureSize();
    const int count = m_parent->repetitionCount() * ::RandomNumber();
    const float offset_x = static_cast<float>( ( count ) % size );
    const float offset_y = static_cast<float>( ( count / size ) % size );
    const kvs::Vec2 random_offset( offset_x, offset_y );

    auto& shader_program = this->shaderProgram();
    kvs::ProgramObject::Binder bind2( shader_program );
    shader_program.setUniform( "random_offset", random_offset );

    auto& buffer_object = this->bufferObject();
    kvs::Texture::Binder bind3( m_parent->randomTexture() );
    buffer_object.draw( object );
}

/*===========================================================================*/
/**
 *  @brief  Create shaders, VBO, and framebuffers.
 *  @param  object [in] pointer to the object
 *  @param  camera [in] pointer to the camera
 *  @param  light [in] pointer to the light
 */
/*===========================================================================*/
void StochasticPolygonRenderer::Engine::create(
    kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light )
{
    auto* polygon = kvs::PolygonObject::DownCast( object );
    const bool has_normal = polygon->normals().size() > 0;
    BaseClass::setShadingEnabled( has_normal );

    BaseClass::attachObject( object );
    BaseClass::createRandomTexture();

    m_render_pass.create( BaseClass::shader(), BaseClass::isShadingEnabled() );

    const auto nvertices = ::NumberOfVertices( polygon );
    const auto indices = BaseClass::randomIndices( nvertices );
    auto location = m_render_pass.shaderProgram().attributeLocation( "random_index" );
    m_buffer_object.manager().setVertexAttribArray( indices, location, 2 );
    m_buffer_object.create( polygon );
}

/*===========================================================================*/
/**
 *  @brief  Updates render pass.
 *  @param  object [in] pointer to the object
 *  @param  camera [in] pointer to the camera
 *  @param  light [in] pointer to the light
 */
/*===========================================================================*/
void StochasticPolygonRenderer::Engine::update(
    kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light )
{
    m_render_pass.update( BaseClass::shader(), BaseClass::isShadingEnabled() );
}

/*===========================================================================*/
/**
 *  @brief  Setups render pass
 *  @param  polygon [in] pointer to the object
 *  @param  camera [in] pointer to the camera
 *  @param  light [in] pointer to the light
 */
/*===========================================================================*/
void StochasticPolygonRenderer::Engine::setup(
    kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light )
{
    m_render_pass.setup( BaseClass::shader() );
}

/*===========================================================================*/
/**
 *  @brief  Draw an ensemble.
 *  @param  object [in] pointer to the object
 *  @param  camera [in] pointer to the camera
 *  @param  light [in] pointer to the light
 */
/*===========================================================================*/
void StochasticPolygonRenderer::Engine::draw(
    kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light )
{
    // Depth offset
    if ( !kvs::Math::IsZero( m_depth_offset[0] ) )
    {
        kvs::OpenGL::SetPolygonOffset( m_depth_offset[0], m_depth_offset[1] );
        kvs::OpenGL::Enable( GL_POLYGON_OFFSET_FILL );
    }

    kvs::OpenGL::Enable( GL_DEPTH_TEST );
    kvs::OpenGL::SetPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
    m_render_pass.draw( object );
}

} // end of namespace kvs
