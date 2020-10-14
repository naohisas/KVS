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
 *  @brief  Sets a line offset value.
 *  @param  offset [in] offset value
 */
/*===========================================================================*/
void StochasticLineRenderer::setLineOffset( const float offset )
{
    static_cast<Engine&>( engine() ).setLineOffset( offset );
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
 *  @brief  Constructs a new Engine class.
 */
/*===========================================================================*/
StochasticLineRenderer::Engine::Engine():
    m_line_opacity( 255 ),
    m_line_offset( 0.0f )
{
}

/*===========================================================================*/
/**
 *  @brief  Releases the GPU resources.
 */
/*===========================================================================*/
void StochasticLineRenderer::Engine::release()
{
    m_shader_program.release();
    m_buffer_object.release();
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
    attachObject( object );
    createRandomTexture();
    this->create_shader_program();
    this->create_buffer_object( line );
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
    const kvs::Mat4 M = kvs::OpenGL::ModelViewMatrix();
    const kvs::Mat4 PM = kvs::OpenGL::ProjectionMatrix() * M;
    kvs::ProgramObject::Binder bind2( m_shader_program );
    m_shader_program.setUniform( "ModelViewProjectionMatrix", PM );
    m_shader_program.setUniform( "random_texture_size_inv", 1.0f / randomTextureSize() );
    m_shader_program.setUniform( "random_texture", 0 );
    m_shader_program.setUniform( "opacity", m_line_opacity / 255.0f );
    m_shader_program.setUniform( "line_offset", m_line_offset );
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
    const size_t size = randomTextureSize();
    const int count = repetitionCount() * ::RandomNumber();
    const float offset_x = static_cast<float>( ( count ) % size );
    const float offset_y = static_cast<float>( ( count / size ) % size );
    const kvs::Vec2 random_offset( offset_x, offset_y );
    kvs::ProgramObject::Binder bind2( m_shader_program );
    m_shader_program.setUniform( "random_offset", random_offset );

    kvs::Texture::Binder bind3( randomTexture() );
    auto* line = kvs::LineObject::DownCast( object );
    kvs::OpenGL::SetLineWidth( line->size() * camera->devicePixelRatio() );
    m_buffer_object.draw( line );
}

/*===========================================================================*/
/**
 *  @brief  Creates shader program.
 */
/*===========================================================================*/
void StochasticLineRenderer::Engine::create_shader_program()
{
    kvs::ShaderSource vert( "SR_line.vert" );
    kvs::ShaderSource frag( "SR_line.frag" );
    m_shader_program.build( vert, frag );
}

/*===========================================================================*/
/**
 *  @brief  Create buffer objects.
 *  @param  line [in] pointer to the line object
 */
/*===========================================================================*/
void StochasticLineRenderer::Engine::create_buffer_object( const kvs::LineObject* line )
{
    const size_t nvertices = line->numberOfVertices();
    const auto tex_size = randomTextureSize();
    kvs::ValueArray<kvs::UInt16> indices( nvertices * 2 );
    for ( size_t i = 0; i < nvertices; i++ )
    {
        const unsigned int count = i * 12347;
        indices[ 2 * i + 0 ] = static_cast<kvs::UInt16>( ( count ) % tex_size );
        indices[ 2 * i + 1 ] = static_cast<kvs::UInt16>( ( count / tex_size ) % tex_size );
    }

    auto location = m_shader_program.attributeLocation( "random_index" );
    m_buffer_object.manager().setVertexAttribArray( indices, location, 2 );
    m_buffer_object.create( line );
}

} // end of namespace kvs
