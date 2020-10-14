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

void PointRenderer::BufferObject::create( const kvs::PointObject* point )
{
    const bool has_normal = point->normals().size() > 0;
    auto coords = point->coords();
    auto colors = ::VertexColors( point );
    auto normals = point->normals();
    m_manager.setVertexArray( coords, 3 );
    m_manager.setColorArray( colors, 3 );
    if ( has_normal ) { m_manager.setNormalArray( normals ); }

    m_manager.create();
}

void PointRenderer::BufferObject::draw( const kvs::PointObject* point )
{
    kvs::VertexBufferObjectManager::Binder bind( m_manager );
    {
        const size_t nvertices = point->numberOfVertices();
        m_manager.drawArrays( GL_POINTS, 0, nvertices );
    }
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new PointRenderer class.
 */
/*===========================================================================*/
PointRenderer::PointRenderer():
    m_vert_file( "shader.vert" ),
    m_frag_file( "shader.frag" ),
    m_width( 0 ),
    m_height( 0 ),
    m_object( NULL ),
    m_shading_model( new kvs::Shader::Lambert() )
{
}

/*===========================================================================*/
/**
 *  @brief  Destructs the PointRenderer class.
 */
/*===========================================================================*/
PointRenderer::~PointRenderer()
{
    if ( m_shading_model ) { delete m_shading_model; }
}

/*===========================================================================*/
/**
 *  @brief  Executes the rendering process.
 *  @param  object [in] pointer to the object
 *  @param  camera [in] pointer to the camera
 *  @param  light [in] pointer to the light
 */
/*===========================================================================*/
void PointRenderer::exec( kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light )
{
    BaseClass::startTimer();
    kvs::OpenGL::WithPushedAttrib p( GL_ALL_ATTRIB_BITS );

    const size_t width = camera->windowWidth();
    const size_t height = camera->windowHeight();

    if ( this->isWindowCreated() )
    {
        this->setWindowSize( width, height );
        this->createBufferObject( object );
        this->createShaderProgram();
    }

    if ( this->isWindowResized( width, height ) )
    {
        this->setWindowSize( width, height );
    }

    if ( this->isObjectChanged( object ) )
    {
        this->updateBufferObject( object );
        this->updateShaderProgram();
    }

    this->setupShaderProgram();

    m_shader_program.bind();
    this->drawBufferObject( camera );
    m_shader_program.unbind();

    BaseClass::stopTimer();
}

/*===========================================================================*/
/**
 *  @brief  Creates shader program.
 */
/*===========================================================================*/
void PointRenderer::createShaderProgram()
{
    kvs::ShaderSource vert( this->vertexShaderFile() );
    kvs::ShaderSource frag( this->fragmentShaderFile() );
    if ( isEnabledShading() )
    {
        switch ( m_shading_model->type() )
        {
        case kvs::Shader::LambertShading: frag.define("ENABLE_LAMBERT_SHADING"); break;
        case kvs::Shader::PhongShading: frag.define("ENABLE_PHONG_SHADING"); break;
        case kvs::Shader::BlinnPhongShading: frag.define("ENABLE_BLINN_PHONG_SHADING"); break;
        default: break; /* NO SHADING */
        }

        if ( kvs::OpenGL::Boolean( GL_LIGHT_MODEL_TWO_SIDE ) == GL_TRUE )
        {
            frag.define("ENABLE_TWO_SIDE_LIGHTING");
        }
    }

    m_shader_program.build( vert, frag );
}

void PointRenderer::updateShaderProgram()
{
    m_shader_program.release();
    this->createShaderProgram();
}

void PointRenderer::setupShaderProgram()
{
    kvs::ProgramObject::Binder bind( m_shader_program );
    m_shader_program.setUniform( "shading.Ka", m_shading_model->Ka );
    m_shader_program.setUniform( "shading.Kd", m_shading_model->Kd );
    m_shader_program.setUniform( "shading.Ks", m_shading_model->Ks );
    m_shader_program.setUniform( "shading.S",  m_shading_model->S );
    m_shader_program.setUniform( "offset", 0 );

    const kvs::Mat4 M = kvs::OpenGL::ModelViewMatrix();
    const kvs::Mat4 PM = kvs::OpenGL::ProjectionMatrix() * M;
    const kvs::Mat3 N = kvs::Mat3( M[0].xyz(), M[1].xyz(), M[2].xyz() );
    m_shader_program.setUniform( "ModelViewMatrix", M );
    m_shader_program.setUniform( "ModelViewProjectionMatrix", PM );
    m_shader_program.setUniform( "NormalMatrix", N );
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
    m_buffer_object.create( kvs::PointObject::DownCast( object ) );
}

void PointRenderer::updateBufferObject( const kvs::ObjectBase* object )
{
    m_buffer_object.release();
    this->createBufferObject( object );
}

void PointRenderer::drawBufferObject( const kvs::Camera* camera )
{
    kvs::OpenGL::Enable( GL_DEPTH_TEST );
    kvs::OpenGL::Enable( GL_POINT_SMOOTH ); // Rounded shape.

    const auto* point = kvs::PointObject::DownCast( m_object );
    const float dpr = camera->devicePixelRatio();
    kvs::OpenGL::SetPointSize( point->size() * dpr );

    m_buffer_object.draw( point );
}

} // end of namespace glsl

} // end of namespace kvs
