/*****************************************************************************/
/**
 *  @file   LineRendererGLSL.cpp
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#include "LineRendererGLSL.h"
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
 *  @param  line [in] pointer to the line object
 */
/*===========================================================================*/
kvs::ValueArray<kvs::UInt8> VertexColors( const kvs::LineObject* line )
{
    if ( line->colorType() == kvs::LineObject::VertexColor ) return line->colors();

    const size_t nvertices = line->numberOfVertices();
    const kvs::RGBColor color = line->color();

    kvs::ValueArray<kvs::UInt8> colors( nvertices * 3 );
    for ( size_t i = 0; i < nvertices; i++ )
    {
        colors[ 3 * i + 0 ] = color.r();
        colors[ 3 * i + 1 ] = color.g();
        colors[ 3 * i + 2 ] = color.b();
    }

    return colors;
}

}

namespace kvs
{

namespace glsl
{

void LineRenderer::BufferObject::set( const kvs::LineObject* line )
{
    if ( line->numberOfColors() != 1 &&
         line->colorType() == kvs::LineObject::LineColor )
    {
        kvsMessageError("Not supported line color type.");
        return;
    }

    const auto type = line->lineType();
    m_has_connection = line->numberOfConnections() > 0 &&
        ( type == kvs::LineObject::Segment || type == kvs::LineObject::Uniline );

    auto coords = line->coords();
    auto colors = ::VertexColors( line );

    m_manager.setVertexArray( coords, 3 );
    m_manager.setColorArray( colors, 3 );
    if ( m_has_connection ) { m_manager.setIndexArray( line->connections() ); }

    if ( ( !m_has_connection ) && ( type == kvs::LineObject::Polyline ) )
    {
        const kvs::UInt32* pconnections = line->connections().data();
        m_first_array.allocate( line->numberOfConnections() );
        m_count_array.allocate( m_first_array.size() );
        for ( size_t i = 0; i < m_first_array.size(); ++i )
        {
            m_first_array[i] = pconnections[ 2 * i ];
            m_count_array[i] = pconnections[ 2 * i + 1 ] - pconnections[ 2 * i ] + 1;
        }
    }
}

void LineRenderer::BufferObject::draw( const kvs::LineObject* line )
{
    kvs::VertexBufferObjectManager::Binder bind( m_manager );

    // Draw lines.
    const auto type = line->lineType();
    if ( m_has_connection )
    {
        const size_t nlines = line->numberOfConnections();
        if ( type == kvs::LineObject::Uniline )
        {
            m_manager.drawElements( GL_LINE_STRIP, nlines );
        }
        else if ( type ==  kvs::LineObject::Segment )
        {
            m_manager.drawElements( GL_LINES, 2 * nlines );
        }
    }
    else
    {
        if ( type == kvs::LineObject::Polyline )
        {
            m_manager.drawArrays( GL_LINE_STRIP, m_first_array, m_count_array );
        }
        else if ( type == kvs::LineObject::Strip )
        {
            const size_t nvertices = line->numberOfVertices();
            m_manager.drawArrays( GL_LINE_STRIP, 0, nvertices );
        }
    }

}

/*===========================================================================*/
/**
 *  @brief  Constructs a new LineRenderer class.
 */
/*===========================================================================*/
LineRenderer::LineRenderer():
    m_vert_file( "line.vert" ),
    m_frag_file( "line.frag" ),
    m_width( 0 ),
    m_height( 0 ),
    m_object( NULL ),
    m_shading_model( new kvs::Shader::Lambert() ),
    m_line_width( 0.0f ),
    m_line_width_range( 0.0f, 0.0f ),
    m_outline_width( 0.0f ),
    m_outline_color( kvs::RGBColor::Black() )
{
}

/*===========================================================================*/
/**
 *  @brief  Destroys the LineRenderer class.
 */
/*===========================================================================*/
LineRenderer::~LineRenderer()
{
    if ( m_shading_model ) { delete m_shading_model; }
}

/*===========================================================================*/
/**
 *  @brief  Executes rendering process.
 *  @param  object [in] pointer to the object
 *  @param  camera [in] pointer to the camera
 *  @param  light [in] pointer to the light
 */
/*===========================================================================*/
void LineRenderer::exec( kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light )
{
    BaseClass::startTimer();
    kvs::OpenGL::WithPushedAttrib p( GL_ALL_ATTRIB_BITS );

    const size_t width = camera->windowWidth();
    const size_t height = camera->windowHeight();

    if ( this->isWindowCreated() )
    {
        this->setWindowSize( width, height );
        this->createShaderProgram();
        this->createBufferObject( object );
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
void LineRenderer::createShaderProgram()
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
        default: break; // NO SHADING
        }

        if ( kvs::OpenGL::Boolean( GL_LIGHT_MODEL_TWO_SIDE ) == GL_TRUE )
        {
            frag.define("ENABLE_TWO_SIDE_LIGHTING");
        }
    }

    m_shader_program.build( vert, frag );
}

void LineRenderer::updateShaderProgram()
{
    m_shader_program.release();
    this->createShaderProgram();
}

void LineRenderer::setupShaderProgram()
{
    kvs::ProgramObject::Binder bind( m_shader_program );
    m_shader_program.setUniform( "shading.Ka", m_shading_model->Ka );
    m_shader_program.setUniform( "shading.Kd", m_shading_model->Kd );
    m_shader_program.setUniform( "shading.Ks", m_shading_model->Ks );
    m_shader_program.setUniform( "shading.S",  m_shading_model->S );

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
 *  @param  line [in] pointer to the line object
 */
/*===========================================================================*/
void LineRenderer::createBufferObject( const kvs::ObjectBase* object )
{
    const auto* line = kvs::LineObject::DownCast( object );
    m_line_width = line->size();
    kvs::OpenGL::GetFloatv( GL_LINE_WIDTH_RANGE, &m_line_width_range[0] );

    m_object = object;
    m_buffer_object.set( line );
    m_buffer_object.create();
}

void LineRenderer::updateBufferObject( const kvs::ObjectBase* object )
{
    m_buffer_object.release();
    this->createBufferObject( object );
}

void LineRenderer::drawBufferObject( const kvs::Camera* camera )
{
    const auto* line = kvs::LineObject::DownCast( m_object );
    const float dpr = camera->devicePixelRatio();
    const float line_width = kvs::Math::Min( line->size() + m_outline_width * 2.0f, m_line_width_range[1] );
    const float outline_width = kvs::Math::Min( m_outline_width, m_line_width_range[1] * 0.5f );
    kvs::ProgramObject::Binder bind( m_shader_program );
    m_shader_program.setUniform( "screen_width", float( m_width ) * dpr );
    m_shader_program.setUniform( "screen_height",  float( m_height ) * dpr );
    m_shader_program.setUniform( "line_width_range", m_line_width_range * dpr );
    m_shader_program.setUniform( "line_width", line_width * dpr );
    m_shader_program.setUniform( "outline_width", outline_width * dpr );
    m_shader_program.setUniform( "outline_color", m_outline_color.toVec3() );

    kvs::OpenGL::Enable( GL_DEPTH_TEST );
    kvs::OpenGL::Enable( GL_BLEND );
    kvs::OpenGL::SetBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    kvs::OpenGL::SetLineWidth( line_width * dpr );
    m_buffer_object.draw( line );
}

} // end of namespace glsl

} // end of namespace kvs
