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
    m_point_opacity( 255 )
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
    m_buffer_object.release();
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
    auto* point = kvs::PointObject::DownCast( object );

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
    kvs::ProgramObject::Binder bind2( m_shader_program );
    m_shader_program.setUniform( "shading.Ka", shader().Ka );
    m_shader_program.setUniform( "shading.Kd", shader().Kd );
    m_shader_program.setUniform( "shading.Ks", shader().Ks );
    m_shader_program.setUniform( "shading.S",  shader().S );
    m_shader_program.setUniform( "opacity", m_point_opacity / 255.0f );

    const kvs::Mat4 M = kvs::OpenGL::ModelViewMatrix();
    const kvs::Mat4 PM = kvs::OpenGL::ProjectionMatrix() * M;
    const kvs::Mat3 N = kvs::Mat3( M[0].xyz(), M[1].xyz(), M[2].xyz() );
    m_shader_program.setUniform( "ModelViewMatrix", M );
    m_shader_program.setUniform( "ModelViewProjectionMatrix", PM );
    m_shader_program.setUniform( "NormalMatrix", N );
    m_shader_program.setUniform( "random_texture_size_inv", 1.0f / randomTextureSize() );
    m_shader_program.setUniform( "random_texture", 0 );
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
    kvs::OpenGL::Enable( GL_POINT_SMOOTH ); // Rounded shape.

    const size_t size = randomTextureSize();
    const int count = repetitionCount() * ::RandomNumber();
    const float offset_x = static_cast<float>( ( count ) % size );
    const float offset_y = static_cast<float>( ( count / size ) % size );
    const kvs::Vec2 random_offset( offset_x, offset_y );
    kvs::ProgramObject::Binder bind2( m_shader_program );
    m_shader_program.setUniform( "random_offset", random_offset );

    kvs::Texture::Binder bind3( randomTexture() );
    auto* point = kvs::PointObject::DownCast( object );
    auto dpr = camera->devicePixelRatio();
    kvs::OpenGL::SetPointSize( point->size() * dpr );
    m_buffer_object.draw( point );
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
    if ( BaseClass::isShadingEnabled() )
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
    m_buffer_object.create( point );
}

} // end of namespace kvs
