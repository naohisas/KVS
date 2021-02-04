/*****************************************************************************/
/**
 *  @file   StochasticPointRenderer.cpp
 *  @author Naohisa Sakamoto
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

} // end of namespace


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
 *  @brief  Constructs a new RenderPass class.
 *  @param  buffer_object [in] buffer object
 *  @param  parent [in] pointer to engin
 */
/*===========================================================================*/
StochasticPointRenderer::Engine::RenderPass::RenderPass(
    Engine::BufferObject& buffer_object,
    RenderPass::Parent* parent ):
    BaseRenderPass( buffer_object ),
    m_parent( parent )
{
    this->setVertexShaderFile( "SR_point.vert" );
    this->setFragmentShaderFile( "SR_point.frag" );
}

/*===========================================================================*/
/**
 *  @brief  Setups render pass.
 *  @param  shading_model [in] shading model 
 */
/*===========================================================================*/
void StochasticPointRenderer::Engine::RenderPass::setup(
    const kvs::Shader::ShadingModel& shading_model )
{
    BaseRenderPass::setup( shading_model );

    auto& shader_program = BaseRenderPass::shaderProgram();
    kvs::ProgramObject::Binder bind( shader_program );

    const auto size_inv = 1.0f / m_parent->randomTextureSize();
    shader_program.setUniform( "random_texture_size_inv", size_inv );
    shader_program.setUniform( "random_texture", 0 );
    shader_program.setUniform( "opacity", m_opacity / 255.0f );
}

/*===========================================================================*/
/**
 *  @brief  Draws buffer object.
 *  @param  object [in] pointer to point object
 */
/*===========================================================================*/
void StochasticPointRenderer::Engine::RenderPass::draw(
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
 *  @brief  Create.
 *  @param  object [in] pointer to the point object
 *  @param  camera [in] pointer to the camera
 *  @param  light [in] pointer to the light
 */
/*===========================================================================*/
void StochasticPointRenderer::Engine::create(
    kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light )
{
    auto* point = kvs::PointObject::DownCast( object );
    BaseClass::attachObject( object );
    BaseClass::createRandomTexture();

    m_render_pass.create( BaseClass::shader(), BaseClass::isShadingEnabled() );

    const size_t nvertices = point->numberOfVertices();
    const auto indices = BaseClass::randomIndices( nvertices );
    auto location = m_render_pass.shaderProgram().attributeLocation( "random_index" );
    m_buffer_object.manager().setVertexAttribArray( indices, location, 2 );
    m_buffer_object.create( object );
}

/*===========================================================================*/
/**
 *  @brief  Update.
 *  @param  object [in] pointer to the object
 *  @param  camera [in] pointer to the camera
 *  @param  light [in] pointer to the light
 */
/*===========================================================================*/
void StochasticPointRenderer::Engine::update(
    kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light )
{
    m_render_pass.update( BaseClass::shader(), BaseClass::isShadingEnabled() );
}

/*===========================================================================*/
/**
 *  @brief  Set up.
 *  @param  object [in] pointer to the object
 *  @param  camera [in] pointer to the camera
 *  @param  light [in] pointer to the light
 */
/*===========================================================================*/
void StochasticPointRenderer::Engine::setup(
    kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light )
{
    m_render_pass.setup( BaseClass::shader() );
}

/*===========================================================================*/
/**
 *  @brief  Draw an ensemble.
 *  @param  object [in] pointer to the line object
 *  @param  camera [in] pointer to the camera
 *  @param  light [in] pointer to the light
 */
/*===========================================================================*/
void StochasticPointRenderer::Engine::draw(
    kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light )
{
    auto* point = kvs::PointObject::DownCast( object );
    auto dpr = camera->devicePixelRatio();

    kvs::OpenGL::Enable( GL_DEPTH_TEST );
    kvs::OpenGL::Enable( GL_POINT_SMOOTH ); // Rounded shape.
    kvs::OpenGL::SetPointSize( point->size() * dpr );
    m_render_pass.draw( point );
}

} // end of namespace kvs
