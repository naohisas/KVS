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

inline bool HasConnections( const kvs::LineObject* line )
{
    bool has_connection = line->numberOfConnections() > 0;

    // In the following cases, the connection stored in the line object will be ignored.
    if ( line->colorType() == kvs::LineObject::LineColor )
    {
        const auto ncolors = line->numberOfColors();
        if ( ncolors > 1 ) { has_connection = false; }
    }

    return has_connection;
}

inline kvs::ValueArray<kvs::Real32> VertexCoords( const kvs::LineObject* line )
{
    if ( line->colorType() == kvs::LineObject::VertexColor )
    {
        return line->coords();
    }

    // In case of LineColor type
    const auto ncolors = line->numberOfColors();
    switch ( line->lineType() )
    {
    case kvs::LineObject::Strip:
    {
        if ( ncolors > 1 )
        {
            std::vector<kvs::Real32> coords;
            const auto nsegments = line->numberOfVertices() - 1;
            for ( size_t i = 0; i < nsegments; ++i )
            {
                const auto c0 = line->coord( i + 0 );
                const auto c1 = line->coord( i + 1 );
                coords.push_back( c0.x() );
                coords.push_back( c0.y() );
                coords.push_back( c0.z() );
                coords.push_back( c1.x() );
                coords.push_back( c1.y() );
                coords.push_back( c1.z() );
            }
            return kvs::ValueArray<kvs::Real32>( coords );
        }
        break;
    }
    case kvs::LineObject::Uniline:
    {
        if ( ncolors > 1 )
        {
            std::vector<kvs::Real32> coords;
            const auto nsegments = line->numberOfConnections() - 1;
            for ( size_t i = 0; i < nsegments; ++i )
            {
                const auto id0 = line->connections().at( i + 0 );
                const auto id1 = line->connections().at( i + 1 );
                const auto c0 = line->coord( id0 );
                const auto c1 = line->coord( id1 );
                coords.push_back( c0.x() );
                coords.push_back( c0.y() );
                coords.push_back( c0.z() );
                coords.push_back( c1.x() );
                coords.push_back( c1.y() );
                coords.push_back( c1.z() );
            }
            return kvs::ValueArray<kvs::Real32>( coords );
        }
        break;
    }
    case kvs::LineObject::Polyline:
    {
        if ( ncolors > 1 )
        {
            std::vector<kvs::Real32> coords;
            const size_t nlines = line->numberOfConnections();
            for ( size_t i = 0; i < nlines; ++i )
            {
                const auto id = 2 * i;
                const auto id0 = line->connections().at( id + 0 );
                const auto id1 = line->connections().at( id + 1 );
                for ( size_t j = id0; j < id1; ++j )
                {
                    const auto c0 = line->coord( j + 0 );
                    const auto c1 = line->coord( j + 1 );
                    coords.push_back( c0.x() );
                    coords.push_back( c0.y() );
                    coords.push_back( c0.z() );
                    coords.push_back( c1.x() );
                    coords.push_back( c1.y() );
                    coords.push_back( c1.z() );
                }
            }
            return kvs::ValueArray<kvs::Real32>( coords );
        }
        break;
    }
    case kvs::LineObject::Segment:
    {
        if ( ncolors > 1 )
        {
            std::vector<kvs::Real32> coords;
            const auto nsegments = line->numberOfConnections();
            for ( size_t i = 0; i < nsegments; ++i )
            {
                const auto id0 = line->connections().at( 2 * i + 0 );
                const auto id1 = line->connections().at( 2 * i + 1 );
                const auto c0 = line->coord( id0 );
                const auto c1 = line->coord( id1 );
                coords.push_back( c0.x() );
                coords.push_back( c0.y() );
                coords.push_back( c0.z() );
                coords.push_back( c1.x() );
                coords.push_back( c1.y() );
                coords.push_back( c1.z() );
            }
            return kvs::ValueArray<kvs::Real32>( coords );
        }
        break;
    }
    default:
        break;
    }

    return line->coords();
}

/*===========================================================================*/
/**
 *  @brief  Returns vertex-color array.
 *  @param  line [in] pointer to the line object
 */
/*===========================================================================*/
inline kvs::ValueArray<kvs::UInt8> VertexColors( const kvs::LineObject* line )
{
    if ( line->colorType() == kvs::LineObject::VertexColor )
    {
        return line->colors();
    }

    // In case of LineColor type
    const auto ncolors = line->numberOfColors();
    switch ( line->lineType() )
    {
    case kvs::LineObject::Strip:
    {
        if ( ncolors > 1 )
        {
            std::vector<kvs::UInt8> colors;
            const auto nsegments = line->numberOfVertices() - 1;
            for ( size_t i = 0; i < nsegments; ++i )
            {
                const auto c = line->color( i );
                colors.push_back( c.r() );
                colors.push_back( c.g() );
                colors.push_back( c.b() );
                colors.push_back( c.r() );
                colors.push_back( c.g() );
                colors.push_back( c.b() );
            }
            return kvs::ValueArray<kvs::UInt8>( colors );
        }
        break;
    }
    case kvs::LineObject::Uniline:
    {
        if ( ncolors > 1 )
        {
            std::vector<kvs::UInt8> colors;
            const auto nsegments = line->numberOfConnections() - 1;
            for ( size_t i = 0; i < nsegments; ++i )
            {
                const auto c = line->color( i );
                colors.push_back( c.r() );
                colors.push_back( c.g() );
                colors.push_back( c.b() );
                colors.push_back( c.r() );
                colors.push_back( c.g() );
                colors.push_back( c.b() );
            }
            return kvs::ValueArray<kvs::UInt8>( colors );
        }
        break;
    }
    case kvs::LineObject::Polyline:
    {
        if ( ncolors > 1 )
        {
            std::vector<kvs::UInt8> colors;
            const auto nlines = line->numberOfConnections();
            for ( size_t i = 0, index = 0; i < nlines; ++i )
            {
                const auto id = 2 * i;
                const auto id0 = line->connections().at( id + 0 );
                const auto id1 = line->connections().at( id + 1 );
                for ( size_t j = id0; j < id1; ++j, ++index )
                {
                    const auto c = line->color( index );
                    colors.push_back( c.r() );
                    colors.push_back( c.g() );
                    colors.push_back( c.b() );
                    colors.push_back( c.r() );
                    colors.push_back( c.g() );
                    colors.push_back( c.b() );
                }
            }
            return kvs::ValueArray<kvs::UInt8>( colors );
        }
        break;
    }
    case kvs::LineObject::Segment:
    {
        if ( ncolors > 1 )
        {
            std::vector<kvs::UInt8> colors;
            const auto nsegments = line->numberOfConnections();
            for ( size_t i = 0; i < nsegments; ++i )
            {
                const auto c = line->color( i );
                colors.push_back( c.r() );
                colors.push_back( c.g() );
                colors.push_back( c.b() );
                colors.push_back( c.r() );
                colors.push_back( c.g() );
                colors.push_back( c.b() );
            }
            return kvs::ValueArray<kvs::UInt8>( colors );
        }
        break;
    }
    default:
        break;
    }

    const auto nvertices = line->numberOfVertices();
    kvs::ValueArray<kvs::UInt8> colors( nvertices * 3 );
    colors.fill(0);
    if ( ncolors == 1 )
    {
        const auto c = line->color();
        for ( size_t i = 0; i < nvertices; ++i )
        {
            colors[ 3 * i + 0 ] = c.r();
            colors[ 3 * i + 1 ] = c.g();
            colors[ 3 * i + 2 ] = c.b();
        }
    }

    return colors;
}

}

namespace kvs
{

namespace glsl
{

void LineRenderer::BufferObject::create( const kvs::ObjectBase* object )
{
    const auto* line = kvs::LineObject::DownCast( object);
    const auto coords = ::VertexCoords( line );
    const auto colors = ::VertexColors( line );
    m_manager.setVertexArray( coords, 3 );
    m_manager.setColorArray( colors, 3 );
    if ( ::HasConnections( line ) )
    {
        if ( line->lineType() == kvs::LineObject::Polyline )
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
        else
        {
            m_manager.setIndexArray( line->connections() );
        }
    }
    m_manager.create();
}

void LineRenderer::BufferObject::draw( const kvs::ObjectBase* object )
{
    const auto* line = kvs::LineObject::DownCast( object);
    kvs::VertexBufferObjectManager::Binder bind( m_manager );

    // Draw lines.
    if ( m_first_array.size() > 0 )
    {
        // kvs::LineObject::Polyline
        m_manager.drawArrays( GL_LINE_STRIP, m_first_array, m_count_array );
    }
    else
    {
        if ( ::HasConnections( line ) )
        {
            const size_t nlines = line->numberOfConnections();
            switch ( line->lineType() )
            {
            case kvs::LineObject::Uniline:
            {
                m_manager.drawElements( GL_LINE_STRIP, nlines );
                break;
            }
            case kvs::LineObject::Segment:
            {
                m_manager.drawElements( GL_LINES, 2 * nlines );
                break;
            }
            default:
                break;
            }
        }
        else
        {
            if ( line->lineType() == kvs::LineObject::Strip &&
                 line->coords().byteSize() == m_manager.vertexArray().size )
            {
                const size_t nvertices = line->numberOfVertices();
                m_manager.drawArrays( GL_LINE_STRIP, 0, nvertices );
            }
            else
            {
                const size_t nelements = m_manager.vertexArray().size / sizeof( kvs::Real32 );
                const size_t nvertices = nelements / 3;
                m_manager.drawArrays( GL_LINES, 0, nvertices );
            }
        }
    }
}

/*===========================================================================*/
/**
 *  @brief  Sets vertex and fragment shader files.
 *  @param  vert_file [in] vertex shader file
 *  @param  frag_file [in] fragment shader file
 */
/*===========================================================================*/
void LineRenderer::RenderPass::setShaderFiles(
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
void LineRenderer::RenderPass::create(
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
void LineRenderer::RenderPass::update(
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
void LineRenderer::RenderPass::setup(
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
 *  @param  object [in] pointer to line object
 */
/*===========================================================================*/
void LineRenderer::RenderPass::draw( const kvs::ObjectBase* object )
{
    kvs::ProgramObject::Binder bind( m_shader_program );
    m_buffer_object.draw( object );
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new LineRenderer class.
 */
/*===========================================================================*/
LineRenderer::LineRenderer():
    m_shading_model( new kvs::Shader::Lambert() ),
    m_render_pass( m_buffer_object )
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
 *  @param  line [in] pointer to the line object
 */
/*===========================================================================*/
void LineRenderer::createBufferObject( const kvs::ObjectBase* object )
{
    m_object = object;
    m_buffer_object.create( object );
}

void LineRenderer::updateBufferObject( const kvs::ObjectBase* object )
{
    m_buffer_object.release();
    this->createBufferObject( object );
}

void LineRenderer::drawBufferObject( const kvs::Camera* camera )
{
    auto* line = kvs::LineObject::DownCast( m_object );
    auto dpr = camera->devicePixelRatio();
    auto line_width = line->size();

    // Seting for drawing outline.
    // {
    auto outline_color = m_render_pass.outlineColor();
    auto outline_width = m_render_pass.outlineWidth();

    kvs::Vec2 line_width_range( 0.0f, 0.0f );
    kvs::OpenGL::GetFloatv( GL_LINE_WIDTH_RANGE, &line_width_range[0] );

    outline_width = kvs::Math::Min( outline_width, line_width_range[1] * 0.5f );
    line_width = kvs::Math::Min( line->size() + outline_width * 2.0f, line_width_range[1] );

    auto& shader_program = m_render_pass.shaderProgram();
    shader_program.bind();
    shader_program.setUniform( "screen_width", float( m_width ) * dpr );
    shader_program.setUniform( "screen_height",  float( m_height ) * dpr );
    shader_program.setUniform( "line_width_range", line_width_range * dpr );
    shader_program.setUniform( "line_width", line_width * dpr );
    shader_program.setUniform( "outline_width", outline_width * dpr );
    shader_program.setUniform( "outline_color", outline_color.toVec3() );
    shader_program.unbind();
    // }

    kvs::OpenGL::Enable( GL_DEPTH_TEST );
    kvs::OpenGL::Enable( GL_BLEND );
    kvs::OpenGL::SetBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    kvs::OpenGL::SetLineWidth( line_width * dpr );
    m_render_pass.draw( line );
}

} // end of namespace glsl

} // end of namespace kvs
