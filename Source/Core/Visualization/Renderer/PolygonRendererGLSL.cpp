/*****************************************************************************/
/**
 *  @file   PolygonRendererGLSL.cpp
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#include "PolygonRenderer.h"
#include <kvs/OpenGL>
#include <kvs/ProgramObject>
#include <kvs/ShaderSource>
#include <kvs/VertexShader>
#include <kvs/FragmentShader>


namespace
{

/*===========================================================================*/
/**
 *  @brief  Returns true if the polygon object has the connectivity.
 *  @param  polygon [in] pointer to the polygon object
 *  @return true if the polygon object has the connectivity
 */
/*===========================================================================*/
bool HasConnections( const kvs::PolygonObject* polygon )
{
    bool has_connection = polygon->numberOfConnections() > 0;

    // In the following cases, the connection stored in the polygon object will be ignored.
    if ( polygon->normalType() == kvs::PolygonObject::PolygonNormal ) { has_connection = false; }
    if ( polygon->colorType() == kvs::PolygonObject::PolygonColor ) { has_connection = false; }

    return has_connection;
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

/*===========================================================================*/
/**
 *  @brief  Returns coordinate array.
 *  @param  polygon [in] pointer to the polygon object
 */
/*===========================================================================*/
kvs::ValueArray<kvs::Real32> VertexCoords( const kvs::PolygonObject* polygon )
{
    if ( polygon->connections().size() > 0 &&
         ( polygon->normalType() == kvs::PolygonObject::PolygonNormal ||
           polygon->colorType() == kvs::PolygonObject::PolygonColor ) )
    {
        const size_t nfaces = polygon->numberOfConnections();
        const kvs::Real32* polygon_coords = polygon->coords().data();
        const kvs::UInt32* polygon_connections = polygon->connections().data();

        kvs::ValueArray<kvs::Real32> coords( nfaces * 9 );
        for ( size_t i = 0; i < nfaces; i++ )
        {
            const kvs::UInt32 id0 = polygon_connections[ i * 3 + 0 ];
            const kvs::UInt32 id1 = polygon_connections[ i * 3 + 1 ];
            const kvs::UInt32 id2 = polygon_connections[ i * 3 + 2 ];

            coords[ i * 9 + 0 ] = polygon_coords[ id0 * 3 + 0 ];
            coords[ i * 9 + 1 ] = polygon_coords[ id0 * 3 + 1 ];
            coords[ i * 9 + 2 ] = polygon_coords[ id0 * 3 + 2 ];

            coords[ i * 9 + 3 ] = polygon_coords[ id1 * 3 + 0 ];
            coords[ i * 9 + 4 ] = polygon_coords[ id1 * 3 + 1 ];
            coords[ i * 9 + 5 ] = polygon_coords[ id1 * 3 + 2 ];

            coords[ i * 9 + 6 ] = polygon_coords[ id2 * 3 + 0 ];
            coords[ i * 9 + 7 ] = polygon_coords[ id2 * 3 + 1 ];
            coords[ i * 9 + 8 ] = polygon_coords[ id2 * 3 + 2 ];
        }

        return coords;
    }

    return polygon->coords();
}

/*===========================================================================*/
/**
 *  @brief  Returns vertex-normal array.
 *  @param  polygon [in] pointer to the polygon object
 */
/*===========================================================================*/
kvs::ValueArray<kvs::Real32> VertexNormals( const kvs::PolygonObject* polygon )
{
    if ( polygon->normals().size() == 0 )
    {
        return kvs::ValueArray<kvs::Real32>();
    }

    kvs::ValueArray<kvs::Real32> normals;
    switch ( polygon->normalType() )
    {
    case kvs::PolygonObject::VertexNormal:
    {
        if ( polygon->connections().size() > 0 &&
             polygon->colorType() == kvs::PolygonObject::PolygonColor )
        {
            const size_t nfaces = polygon->numberOfConnections();
            const kvs::Real32* polygon_normals = polygon->normals().data();
            const kvs::UInt32* polygon_connections = polygon->connections().data();
            normals.allocate( nfaces * 9 );
            for ( size_t i = 0; i < nfaces; i++ )
            {
                const kvs::UInt32 id0 = polygon_connections[ i * 3 + 0 ];
                const kvs::UInt32 id1 = polygon_connections[ i * 3 + 1 ];
                const kvs::UInt32 id2 = polygon_connections[ i * 3 + 2 ];

                normals[ i * 9 + 0 ] = polygon_normals[ id0 * 3 + 0 ];
                normals[ i * 9 + 1 ] = polygon_normals[ id0 * 3 + 1 ];
                normals[ i * 9 + 2 ] = polygon_normals[ id0 * 3 + 2 ];

                normals[ i * 9 + 3 ] = polygon_normals[ id1 * 3 + 0 ];
                normals[ i * 9 + 4 ] = polygon_normals[ id1 * 3 + 1 ];
                normals[ i * 9 + 5 ] = polygon_normals[ id1 * 3 + 2 ];

                normals[ i * 9 + 6 ] = polygon_normals[ id2 * 3 + 0 ];
                normals[ i * 9 + 7 ] = polygon_normals[ id2 * 3 + 1 ];
                normals[ i * 9 + 8 ] = polygon_normals[ id2 * 3 + 2 ];
            }
        }
        else
        {
            normals = polygon->normals();
        }
        break;
    }
    case kvs::PolygonObject::PolygonNormal:
    {
        const size_t nfaces = NumberOfVertices( polygon ) / 3;
        const kvs::Real32* polygon_normals = polygon->normals().data();
        normals.allocate( nfaces * 9 );
        for ( size_t i = 0; i < nfaces; i++ )
        {
            const kvs::Real32 nx = polygon_normals[ i * 3 + 0 ];
            const kvs::Real32 ny = polygon_normals[ i * 3 + 1 ];
            const kvs::Real32 nz = polygon_normals[ i * 3 + 2 ];

            normals[ i * 9 + 0 ] = nx;
            normals[ i * 9 + 1 ] = ny;
            normals[ i * 9 + 2 ] = nz;

            normals[ i * 9 + 3 ] = nx;
            normals[ i * 9 + 4 ] = ny;
            normals[ i * 9 + 5 ] = nz;

            normals[ i * 9 + 6 ] = nx;
            normals[ i * 9 + 7 ] = ny;
            normals[ i * 9 + 8 ] = nz;
        }
        break;
    }
    default: break;
    }

    return normals;
}

/*===========================================================================*/
/**
 *  @brief  Returns vertex-color array.
 *  @param  polygon [in] pointer to the polygon object
 */
/*===========================================================================*/
kvs::ValueArray<kvs::UInt8> VertexColors( const kvs::PolygonObject* polygon )
{
    const bool is_single_color = polygon->colors().size() == 3;
    const bool is_single_alpha = polygon->opacities().size() == 1;

    if ( polygon->colors().size() == 0 )
    {
        return kvs::ValueArray<kvs::UInt8>();
    }

    kvs::ValueArray<kvs::UInt8> colors;
    switch ( polygon->colorType() )
    {
    case kvs::PolygonObject::VertexColor:
    {
        if ( polygon->connections().size() > 0 &&
             polygon->normalType() == kvs::PolygonObject::PolygonNormal )
        {
            const size_t nfaces = polygon->numberOfConnections();
            const kvs::UInt8* polygon_colors = polygon->colors().data();
            const kvs::UInt8* polygon_alphas = polygon->opacities().data();
            const kvs::UInt32* polygon_connections = polygon->connections().data();
            colors.allocate( nfaces * 12 );
            for ( size_t i = 0; i < nfaces; i++ )
            {
                const kvs::UInt32 id0 = polygon_connections[ i * 3 + 0 ];
                const kvs::UInt32 id1 = polygon_connections[ i * 3 + 1 ];
                const kvs::UInt32 id2 = polygon_connections[ i * 3 + 2 ];

                const size_t idr0 = ( is_single_color ) ? 0 : id0 * 3 + 0;
                const size_t idg0 = ( is_single_color ) ? 1 : id0 * 3 + 1;
                const size_t idb0 = ( is_single_color ) ? 2 : id0 * 3 + 2;
                const size_t ida0 = ( is_single_alpha ) ? 0 : id0;
                colors[ i * 12 +  0 ] = polygon_colors[ idr0 ];
                colors[ i * 12 +  1 ] = polygon_colors[ idg0 ];
                colors[ i * 12 +  2 ] = polygon_colors[ idb0 ];
                colors[ i * 12 +  3 ] = polygon_alphas[ ida0 ];

                const size_t idr1 = ( is_single_color ) ? 0 : id1 * 3 + 0;
                const size_t idg1 = ( is_single_color ) ? 1 : id1 * 3 + 1;
                const size_t idb1 = ( is_single_color ) ? 2 : id1 * 3 + 2;
                const size_t ida1 = ( is_single_alpha ) ? 0 : id1;
                colors[ i * 12 +  4 ] = polygon_colors[ idr1 ];
                colors[ i * 12 +  5 ] = polygon_colors[ idg1 ];
                colors[ i * 12 +  6 ] = polygon_colors[ idb1 ];
                colors[ i * 12 +  7 ] = polygon_alphas[ ida1 ];

                const size_t idr2 = ( is_single_color ) ? 0 : id2 * 3 + 0;
                const size_t idg2 = ( is_single_color ) ? 1 : id2 * 3 + 1;
                const size_t idb2 = ( is_single_color ) ? 2 : id2 * 3 + 2;
                const size_t ida2 = ( is_single_alpha ) ? 0 : id2;
                colors[ i * 12 +  8 ] = polygon_colors[ idr2 ];
                colors[ i * 12 +  9 ] = polygon_colors[ idg2 ];
                colors[ i * 12 + 10 ] = polygon_colors[ idb2 ];
                colors[ i * 12 + 11 ] = polygon_alphas[ ida2 ];
            }
        }
        else
        {
            const size_t nverts = polygon->numberOfVertices();
            colors.allocate( nverts * 4 );
            if ( is_single_color )
            {
                const kvs::RGBColor polygon_color = polygon->color();
                if ( is_single_alpha )
                {
                    const kvs::UInt8 polygon_alpha = polygon->opacity();
                    for ( size_t i = 0; i < nverts; i++ )
                    {
                        colors[ 4 * i + 0 ] = polygon_color.r();
                        colors[ 4 * i + 1 ] = polygon_color.g();
                        colors[ 4 * i + 2 ] = polygon_color.b();
                        colors[ 4 * i + 3 ] = polygon_alpha;
                    }
                }
                else
                {
                    const kvs::UInt8* polygon_alphas = polygon->opacities().data();
                    for ( size_t i = 0; i < nverts; i++ )
                    {
                        colors[ 4 * i + 0 ] = polygon_color.r();
                        colors[ 4 * i + 1 ] = polygon_color.g();
                        colors[ 4 * i + 2 ] = polygon_color.b();
                        colors[ 4 * i + 3 ] = polygon_alphas[ i ];
                    }
                }
            }
            else
            {
                const kvs::UInt8* polygon_colors = polygon->colors().data();
                if ( is_single_alpha )
                {
                    const kvs::UInt8 polygon_alpha = polygon->opacity();
                    for ( size_t i = 0; i < nverts; i++ )
                    {
                        colors[ 4 * i + 0 ] = polygon_colors[ 3 * i + 0 ];
                        colors[ 4 * i + 1 ] = polygon_colors[ 3 * i + 1 ];
                        colors[ 4 * i + 2 ] = polygon_colors[ 3 * i + 2 ];
                        colors[ 4 * i + 3 ] = polygon_alpha;
                    }
                }
                else
                {
                    const kvs::UInt8* polygon_alphas = polygon->opacities().data();
                    for ( size_t i = 0; i < nverts; i++ )
                    {
                        colors[ 4 * i + 0 ] = polygon_colors[ 3 * i + 0 ];
                        colors[ 4 * i + 1 ] = polygon_colors[ 3 * i + 1 ];
                        colors[ 4 * i + 2 ] = polygon_colors[ 3 * i + 2 ];
                        colors[ 4 * i + 3 ] = polygon_alphas[ i ];
                    }
                }
            }
        }
        break;
    }
    case kvs::PolygonObject::PolygonColor:
    {
        const size_t nfaces = NumberOfVertices( polygon ) / 3;
        const kvs::UInt8* polygon_colors = polygon->colors().data();
        const kvs::UInt8* polygon_alphas = polygon->opacities().data();
        colors.allocate( nfaces * 12 );
        for ( size_t i = 0; i < nfaces; i++ )
        {
            const kvs::UInt8 r = ( is_single_color ) ? polygon_colors[0] : polygon_colors[ i * 3 + 0 ];
            const kvs::UInt8 g = ( is_single_color ) ? polygon_colors[1] : polygon_colors[ i * 3 + 1 ];
            const kvs::UInt8 b = ( is_single_color ) ? polygon_colors[2] : polygon_colors[ i * 3 + 2 ];
            const kvs::UInt8 a = ( is_single_alpha ) ? polygon_alphas[0] : polygon_alphas[ i ];

            colors[ i * 12 +  0 ] = r;
            colors[ i * 12 +  1 ] = g;
            colors[ i * 12 +  2 ] = b;
            colors[ i * 12 +  3 ] = a;

            colors[ i * 12 +  4 ] = r;
            colors[ i * 12 +  5 ] = g;
            colors[ i * 12 +  6 ] = b;
            colors[ i * 12 +  7 ] = a;

            colors[ i * 12 +  8 ] = r;
            colors[ i * 12 +  9 ] = g;
            colors[ i * 12 + 10 ] = b;
            colors[ i * 12 + 11 ] = a;
        }
        break;
    }
    default: { break; }
    }

    return colors;
}

} // end of namespace


namespace kvs
{

namespace glsl
{

void PolygonRenderer::BufferObject::set( const kvs::PolygonObject* polygon )
{
    if ( polygon->polygonType() != kvs::PolygonObject::Triangle )
    {
        kvsMessageError("Not supported polygon type.");
        return;
    }

    const bool has_normal = polygon->normals().size() > 0;
    const bool has_connection = ::HasConnections( polygon );

    auto coords = ::VertexCoords( polygon );
    auto colors = ::VertexColors( polygon );
    auto normals = ::VertexNormals( polygon );

    m_manager.setVertexArray( coords, 3 );
    m_manager.setColorArray( colors, 4 );
    if ( has_normal ) { m_manager.setNormalArray( normals ); }
    if ( has_connection ) { m_manager.setIndexArray( polygon->connections() ); }
}

void PolygonRenderer::BufferObject::draw( const kvs::PolygonObject* polygon )
{
    kvs::VertexBufferObjectManager::Binder bind( m_manager );
    {
        kvs::OpenGL::Enable( GL_DEPTH_TEST );
        kvs::OpenGL::SetPolygonMode( GL_FRONT_AND_BACK, GL_FILL );

        const size_t nconnections = polygon->numberOfConnections();
        const size_t nvertices = ::NumberOfVertices( polygon );
        const size_t npolygons = nconnections == 0 ? nvertices / 3 : nconnections;
        const bool has_connection = m_manager.indexBufferObject().size() > 0;

        // Draw triangles.
        if ( has_connection ) { m_manager.drawElements( GL_TRIANGLES, 3 * npolygons ); }
        else { m_manager.drawArrays( GL_TRIANGLES, 0, 3 * npolygons ); }
    }
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new PolygonRenderer class.
 */
/*===========================================================================*/
PolygonRenderer::PolygonRenderer():
    m_vert_file( "shader.vert" ),
    m_frag_file( "shader.frag" ),
    m_width( 0 ),
    m_height( 0 ),
    m_object( NULL ),
    m_shading_model( new kvs::Shader::Lambert() ),
    m_polygon_offset( 0.0f )
{
}

/*===========================================================================*/
/**
 *  @brief  Destroys the PolygonRenderer class.
 */
/*===========================================================================*/
PolygonRenderer::~PolygonRenderer()
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
void PolygonRenderer::exec( kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light )
{
    BaseClass::startTimer();
    kvs::OpenGL::WithPushedAttrib p( GL_ALL_ATTRIB_BITS );

    auto* polygon = kvs::PolygonObject::DownCast( object );
    const bool has_normal = polygon->normals().size() > 0;
    if ( !has_normal ) { setEnabledShading( false ); }

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
void PolygonRenderer::createShaderProgram()
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

void PolygonRenderer::updateShaderProgram()
{
    m_shader_program.release();
    this->createShaderProgram();
}

void PolygonRenderer::setupShaderProgram()
{
    kvs::ProgramObject::Binder bind( m_shader_program );
    m_shader_program.setUniform( "shading.Ka", m_shading_model->Ka );
    m_shader_program.setUniform( "shading.Kd", m_shading_model->Kd );
    m_shader_program.setUniform( "shading.Ks", m_shading_model->Ks );
    m_shader_program.setUniform( "shading.S",  m_shading_model->S );
    m_shader_program.setUniform( "offset", m_polygon_offset );

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
 *  @param  polygon [in] pointer to the polygon object
 */
/*===========================================================================*/
void PolygonRenderer::createBufferObject( const kvs::ObjectBase* object )
{
    m_object = object;
    m_buffer_object.set( kvs::PolygonObject::DownCast( object ) );
    m_buffer_object.create();
}

void PolygonRenderer::updateBufferObject( const kvs::ObjectBase* object )
{
    m_buffer_object.release();
    this->createBufferObject( object );
}

/*===========================================================================*/
/**
 *  @brief  Draws buffer object.
 *  @param  polygon [in] pointer to the polygon object
 */
/*===========================================================================*/
void PolygonRenderer::drawBufferObject( const kvs::Camera* camera )
{
    const auto* polygon = kvs::PolygonObject::DownCast( m_object );
    kvs::OpenGL::Enable( GL_DEPTH_TEST );
    m_buffer_object.draw( polygon );
}

} // end of namespace glsl

} // end of namespace kvs
