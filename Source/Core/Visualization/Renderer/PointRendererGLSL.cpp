/*****************************************************************************/
/**
 *  @file   PointRendererGLSL.cpp
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#include "PointRenderer.h"
#include <kvs/DebugNew>
#include <kvs/OpenGL>
#include <kvs/ProgramObject>
#include <kvs/ShaderSource>
#include <kvs/VertexShader>
#include <kvs/FragmentShader>


namespace
{

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

} // end of namespace


namespace kvs
{

namespace glsl
{

/*===========================================================================*/
/**
 *  @brief  Creates a buffer object.
 *  @param  object [in] pointer to point object  
 */
/*===========================================================================*/
void PointRenderer::BufferObject::create( const kvs::ObjectBase* object )
{
    const auto* point = kvs::PointObject::DownCast( object );
    const bool has_normal = point->normals().size() > 0;
    auto coords = point->coords();
    auto colors = ::VertexColors( point );
    auto normals = point->normals();

    m_manager.setVertexArray( coords, 3 );
    m_manager.setColorArray( colors, 3 );
    if ( has_normal ) { m_manager.setNormalArray( normals ); }

    m_manager.create();
}

/*===========================================================================*/
/**
 *  @brief  Draws buffer object.
 *  @param  object [in] pointer to point object
 */
/*===========================================================================*/
void PointRenderer::BufferObject::draw( const kvs::ObjectBase* object )
{
    const auto* point = kvs::PointObject::DownCast( object );
    const size_t nvertices = point->numberOfVertices();

    kvs::VertexBufferObjectManager::Binder bind( m_manager );
    m_manager.drawArrays( GL_POINTS, 0, nvertices );
}

/*===========================================================================*/
/**
 *  @brief  Sets vertex and fragment shader files.
 *  @param  vert_file [in] vertex shader file
 *  @param  frag_file [in] fragment shader file
 */
/*===========================================================================*/
void PointRenderer::RenderPass::setShaderFiles(
    const std::string& vert_file, const std::string& frag_file )
{
    this->setVertexShaderFile( vert_file );
    this->setFragmentShaderFile( frag_file );
}

/*===========================================================================*/
/**
 *  @brief  Builds shader program.
 *  @param  model [in] shading model
 *  @param  enable [in] if true, shading is enabled
 */
/*===========================================================================*/
void PointRenderer::RenderPass::create(
    const kvs::Shader::ShadingModel& model, const bool enable )
{
    kvs::ShaderSource vert( m_vert_shader_file );
    kvs::ShaderSource frag( m_frag_shader_file );
    if ( enable )
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

    m_shader_program.build( vert, frag );
}

/*===========================================================================*/
/**
 *  @brief  Updates render pass.
 *  @param  shading_model [in] shading model
 *  @param  shading_enabled [in] if true, shading is enabled
 */
/*===========================================================================*/
void PointRenderer::RenderPass::update(
    const kvs::Shader::ShadingModel& shading_model,
    const bool shading_enabled )
{
    m_shader_program.release();
    this->create( shading_model, shading_enabled );
}

/*===========================================================================*/
/**
 *  @brief  Setups render pass.
 *  @param  shading_model [in] shading model
 */
/*===========================================================================*/
void PointRenderer::RenderPass::setup(
    const kvs::Shader::ShadingModel& shading_model )
{
    kvs::ProgramObject::Binder bind( m_shader_program );
    m_shader_program.setUniform( "shading.Ka", shading_model.Ka );
    m_shader_program.setUniform( "shading.Kd", shading_model.Kd );
    m_shader_program.setUniform( "shading.Ks", shading_model.Ks );
    m_shader_program.setUniform( "shading.S",  shading_model.S );

    const kvs::Mat4 M = kvs::OpenGL::ModelViewMatrix();
    const kvs::Mat4 PM = kvs::OpenGL::ProjectionMatrix() * M;
    const kvs::Mat3 N = kvs::Mat3( M[0].xyz(), M[1].xyz(), M[2].xyz() );
    m_shader_program.setUniform( "ModelViewMatrix", M );
    m_shader_program.setUniform( "ModelViewProjectionMatrix", PM );
    m_shader_program.setUniform( "NormalMatrix", N );
}

/*===========================================================================*/
/**
 *  @brief  Draws buffer object.
 *  @param  object [in] pointer to point object
 */
/*===========================================================================*/
void PointRenderer::RenderPass::draw( const kvs::ObjectBase* object )
{
    kvs::ProgramObject::Binder bind( m_shader_program );
    m_buffer_object.draw( object );
}

/*===========================================================================*/
/**
 *  @brief  Executes the rendering process.
 *  @param  object [in] pointer to the object
 *  @param  camera [in] pointer to the camera
 *  @param  light [in] pointer to the light
 */
/*===========================================================================*/
void PointRenderer::exec(
    kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light )
{
    BaseClass::startTimer();
    kvs::OpenGL::WithPushedAttrib p( GL_ALL_ATTRIB_BITS );

    const size_t width = camera->windowWidth();
    const size_t height = camera->windowHeight();
    const auto shading_enabled = BaseClass::isShadingEnabled();
    const auto& shading_model = *m_shading_model;

    if ( this->isWindowCreated() )
    {
        this->setWindowSize( width, height );
        this->createBufferObject( object );
        m_render_pass.create( shading_model, shading_enabled );
    }

    if ( this->isWindowResized( width, height ) )
    {
        this->setWindowSize( width, height );
    }

    if ( this->isObjectChanged( object ) )
    {
        this->updateBufferObject( object );
        m_render_pass.update( shading_model, shading_enabled );
    }

    m_render_pass.setup( shading_model );
    this->drawBufferObject( camera );

    BaseClass::stopTimer();
}

/*===========================================================================*/
/**
 *  @brief  Creates buffer object.
 *  @param  point [in] pointer to the point object
 */
/*===========================================================================*/
void PointRenderer::createBufferObject( const kvs::ObjectBase* object )
{
    m_object = object;
    m_buffer_object.create( object );
}

/*===========================================================================*/
/**
 *  @brief  Updates buffer object.
 *  @param  object [in] pointer to object
 */
/*===========================================================================*/
void PointRenderer::updateBufferObject( const kvs::ObjectBase* object )
{
    m_buffer_object.release();
    this->createBufferObject( object );
}

/*===========================================================================*/
/**
 *  @brief  Draws buffer object.
 *  @param  object [in] pointer to  object
 */
/*===========================================================================*/
void PointRenderer::drawBufferObject( const kvs::Camera* camera )
{
    auto* point = kvs::PointObject::DownCast( m_object );
    auto dpr = camera->devicePixelRatio();

    // Depth offset
    const auto depth_offset = BaseClass::depthOffset();
    if ( !kvs::Math::IsZero( depth_offset[0] ) )
    {
        kvs::OpenGL::SetPolygonOffset( depth_offset[0], depth_offset[1] );
        kvs::OpenGL::Enable( GL_POLYGON_OFFSET_FILL );
    }

    kvs::OpenGL::Enable( GL_DEPTH_TEST );
    kvs::OpenGL::Enable( GL_POINT_SMOOTH ); // Rounded shape.
    kvs::OpenGL::SetPointSize( point->size() * dpr );
    m_render_pass.draw( point );
}

} // end of namespace glsl

} // end of namespace kvs
