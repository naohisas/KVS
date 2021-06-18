/*****************************************************************************/
/**
 *  @file   StochasticLineRenderer.cpp
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#include "StochasticLineRenderer.h"
#include <cmath>
#include <kvs/OpenGL>
#include <kvs/LineObject>
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

}

namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new StochasticLineRenderer class.
 */
/*===========================================================================*/
StochasticLineRenderer::StochasticLineRenderer():
    StochasticRendererBase( new Engine() )
{
}

/*===========================================================================*/
/**
 *  @brief  Sets depth offset.
 *  @param  offset [in] depth offset
 */
/*===========================================================================*/
void StochasticLineRenderer::setDepthOffset( const kvs::Vec2& offset )
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
void StochasticLineRenderer::setDepthOffset( const float factor, const float units )
{
    static_cast<Engine&>( engine() ).setDepthOffset( factor, units );
}

/*===========================================================================*/
/**
 *  @brief  Sets an opacity value.
 *  @param  opacity [in] opacity value
 */
/*===========================================================================*/
void StochasticLineRenderer::setOpacity( const kvs::UInt8 opacity )
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
StochasticLineRenderer::Engine::RenderPass::RenderPass(
    Engine::BufferObject& buffer_object,
    RenderPass::Parent* parent ):
    BaseRenderPass( buffer_object ),
    m_parent( parent )
{
    this->setVertexShaderFile( "SR_line.vert" );
    this->setFragmentShaderFile( "SR_line.frag" );
}

/*===========================================================================*/
/**
 *  @brief  Setups render pass.
 *  @param  shading_model [in] shading model 
 */
/*===========================================================================*/
void StochasticLineRenderer::Engine::RenderPass::setup(
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
 *  @param  object [in] pointer to line object
 */
/*===========================================================================*/
void StochasticLineRenderer::Engine::RenderPass::draw(
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
 *  @param  object [in] pointer to the line object
 *  @param  camera [in] pointer to the camera
 *  @param  light [in] pointer to the light
 */
/*===========================================================================*/
void StochasticLineRenderer::Engine::create(
    kvs::ObjectBase* object,
    kvs::Camera* camera,
    kvs::Light* light )
{
    auto* line = kvs::LineObject::DownCast( object );
    BaseClass::attachObject( object );
    BaseClass::createRandomTexture();

    m_render_pass.create( BaseClass::shader(), BaseClass::isShadingEnabled() );

    const size_t nvertices = line->numberOfVertices();
    const auto indices = BaseClass::randomIndices( nvertices );
    auto location = m_render_pass.shaderProgram().attributeLocation( "random_index" );
    m_buffer_object.manager().setVertexAttribArray( indices, location, 2 );
    m_buffer_object.create( line );
}

/*===========================================================================*/
/**
 *  @brief  Update.
 *  @param  object [in] pointer to the object
 *  @param  camera [in] pointer to the camera
 *  @param  light [in] pointer to the light
 */
/*===========================================================================*/
void StochasticLineRenderer::Engine::update(
    kvs::ObjectBase* object,
    kvs::Camera* camera,
    kvs::Light* light )
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
void StochasticLineRenderer::Engine::setup(
    kvs::ObjectBase* object,
    kvs::Camera* camera,
    kvs::Light* light )
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
void StochasticLineRenderer::Engine::draw(
    kvs::ObjectBase* object,
    kvs::Camera* camera,
    kvs::Light* light )
{
    auto* line = kvs::LineObject::DownCast( object );
    auto dpr = camera->devicePixelRatio();
    auto line_width = line->size();

    // Depth offset
    if ( !kvs::Math::IsZero( m_depth_offset[0] ) )
    {
        kvs::OpenGL::SetPolygonOffset( m_depth_offset[0], m_depth_offset[1] );
        kvs::OpenGL::Enable( GL_POLYGON_OFFSET_FILL );
    }

    kvs::OpenGL::Enable( GL_DEPTH_TEST );
    kvs::OpenGL::SetLineWidth( line_width * dpr );
    m_render_pass.draw( line );
}

} // end of namespace kvs
