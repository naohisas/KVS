/*****************************************************************************/
/**
 *  @file   StylizedLineRenderer.cpp
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#include "StylizedLineRenderer.h"
#include <kvs/OpenGL>
#include <kvs/ProgramObject>
#include <kvs/ShaderSource>
#include <kvs/VertexShader>
#include <kvs/FragmentShader>


namespace
{

kvs::ValueArray<kvs::Real32> QuadVertexCoords( const kvs::LineObject* line )
{
    const size_t nvertices = line->numberOfVertices();
    kvs::ValueArray<kvs::Real32> coords( nvertices * 6 );
    for ( size_t i = 0; i < nvertices; i++ )
    {
        coords[ 6 * i + 0 ] = line->coords()[ 3 * i + 0 ];
        coords[ 6 * i + 1 ] = line->coords()[ 3 * i + 1 ];
        coords[ 6 * i + 2 ] = line->coords()[ 3 * i + 2 ];
        coords[ 6 * i + 3 ] = line->coords()[ 3 * i + 0 ];
        coords[ 6 * i + 4 ] = line->coords()[ 3 * i + 1 ];
        coords[ 6 * i + 5 ] = line->coords()[ 3 * i + 2 ];
    }
    return coords;
}

kvs::ValueArray<kvs::UInt8> QuadVertexColors( const kvs::LineObject* line )
{
    const size_t nvertices = line->numberOfVertices();
    kvs::ValueArray<kvs::UInt8> colors( nvertices * 6 );
    if ( line->colorType() == kvs::LineObject::VertexColor )
    {
        for ( size_t i = 0; i < nvertices; i++ )
        {
            colors[ 6 * i + 0 ] = line->colors()[ 3 * i + 0 ];
            colors[ 6 * i + 1 ] = line->colors()[ 3 * i + 1 ];
            colors[ 6 * i + 2 ] = line->colors()[ 3 * i + 2 ];
            colors[ 6 * i + 3 ] = line->colors()[ 3 * i + 0 ];
            colors[ 6 * i + 4 ] = line->colors()[ 3 * i + 1 ];
            colors[ 6 * i + 5 ] = line->colors()[ 3 * i + 2 ];
        }
    }
    else
    {
        const kvs::RGBColor color = line->color();
        for ( size_t i = 0; i < nvertices; i++ )
        {
            colors[ 6 * i + 0 ] = color.r();
            colors[ 6 * i + 1 ] = color.g();
            colors[ 6 * i + 2 ] = color.b();
            colors[ 6 * i + 3 ] = color.r();
            colors[ 6 * i + 4 ] = color.g();
            colors[ 6 * i + 5 ] = color.b();
        }
    }
    return colors;
}

kvs::ValueArray<kvs::Real32> QuadVertexNormals( const kvs::LineObject* line )
{
    const size_t nvertices = line->numberOfVertices();
    kvs::ValueArray<kvs::Real32> normals( nvertices * 6 );

    if ( line->coords().size() == line->normals().size() )
    {
        for ( size_t i = 0; i < nvertices; i++ )
        {
            normals[ 6 * i + 0 ] = line->normals()[ 3 * i + 0 ];
            normals[ 6 * i + 1 ] = line->normals()[ 3 * i + 1 ];
            normals[ 6 * i + 2 ] = line->normals()[ 3 * i + 2 ];
            normals[ 6 * i + 3 ] = line->normals()[ 3 * i + 0 ];
            normals[ 6 * i + 4 ] = line->normals()[ 3 * i + 1 ];
            normals[ 6 * i + 5 ] = line->normals()[ 3 * i + 2 ];
        }
    }
    else
    {
        switch ( line->lineType() )
        {
        case kvs::LineObject::Uniline:
        {
            const size_t nconnections = line->numberOfConnections();
            for ( size_t i = 0; i < nconnections - 1; i++ )
            {
                const size_t id0 = line->connections().at( i );
                const size_t id1 = line->connections().at( i + 1 );
                const kvs::Vec3 p0 = line->coord( id0 );
                const kvs::Vec3 p1 = line->coord( id1 );
                const kvs::Vec3 n = ( p1 - p0 ).normalized();
                normals[ 6 * id0 + 0 ] = n.x();
                normals[ 6 * id0 + 1 ] = n.y();
                normals[ 6 * id0 + 2 ] = n.z();
                normals[ 6 * id0 + 3 ] = n.x();
                normals[ 6 * id0 + 4 ] = n.y();
                normals[ 6 * id0 + 5 ] = n.z();
                if ( i == nconnections - 2 )
                {
                    normals[ 6 * id1 + 0 ] = n.x();
                    normals[ 6 * id1 + 1 ] = n.y();
                    normals[ 6 * id1 + 2 ] = n.z();
                    normals[ 6 * id1 + 3 ] = n.x();
                    normals[ 6 * id1 + 4 ] = n.y();
                    normals[ 6 * id1 + 5 ] = n.z();
                }
            }
            break;
        }
        case kvs::LineObject::Segment:
        {
            const size_t nconnections = line->numberOfConnections();
            for ( size_t i = 0; i < nconnections; i++ )
            {
                const size_t id0 = line->connections().at( 2 * i );
                const size_t id1 = line->connections().at( 2 * i + 1 );
                const kvs::Vec3 p0 = line->coord( id0 );
                const kvs::Vec3 p1 = line->coord( id1 );
                const kvs::Vec3 n = ( p1 - p0 ).normalized();
                normals[ 6 * id0 + 0 ] = n.x();
                normals[ 6 * id0 + 1 ] = n.y();
                normals[ 6 * id0 + 2 ] = n.z();
                normals[ 6 * id0 + 3 ] = n.x();
                normals[ 6 * id0 + 4 ] = n.y();
                normals[ 6 * id0 + 5 ] = n.z();
                normals[ 6 * id1 + 0 ] = n.x();
                normals[ 6 * id1 + 1 ] = n.y();
                normals[ 6 * id1 + 2 ] = n.z();
                normals[ 6 * id1 + 3 ] = n.x();
                normals[ 6 * id1 + 4 ] = n.y();
                normals[ 6 * id1 + 5 ] = n.z();
            }
            break;
        }
        case kvs::LineObject::Polyline:
        {
            const size_t nconnections = line->numberOfConnections();
            for ( size_t i = 0; i < nconnections; i++ )
            {
                const size_t id0 = line->connections().at( 2 * i );
                const size_t id1 = line->connections().at( 2 * i + 1 );
                for ( size_t j = id0; j < id1; j++ )
                {
                    const kvs::Vec3 p0 = line->coord( j );
                    const kvs::Vec3 p1 = line->coord( j + 1 );
                    const kvs::Vec3 n = ( p1 - p0 ).normalized();
                    normals[ 6 * j + 0 ] = n.x();
                    normals[ 6 * j + 1 ] = n.y();
                    normals[ 6 * j + 2 ] = n.z();
                    normals[ 6 * j + 3 ] = n.x();
                    normals[ 6 * j + 4 ] = n.y();
                    normals[ 6 * j + 5 ] = n.z();
                    if ( j == id1 - 1 )
                    {
                        normals[ 6 * ( j + 1 ) + 0 ] = n.x();
                        normals[ 6 * ( j + 1 ) + 1 ] = n.y();
                        normals[ 6 * ( j + 1 ) + 2 ] = n.z();
                        normals[ 6 * ( j + 1 ) + 3 ] = n.x();
                        normals[ 6 * ( j + 1 ) + 4 ] = n.y();
                        normals[ 6 * ( j + 1 ) + 5 ] = n.z();
                    }
                }
            }
            break;
        }
        case kvs::LineObject::Strip:
        {
            const size_t nvertices = line->numberOfVertices();
            for ( size_t i = 0; i < nvertices - 1; i++ )
            {
                const kvs::Vec3 p0 = line->coord( i );
                const kvs::Vec3 p1 = line->coord( i + 1 );
                const kvs::Vec3 n = ( p1 - p0 ).normalized();
                normals[ 6 * i + 0 ] = n.x();
                normals[ 6 * i + 1 ] = n.y();
                normals[ 6 * i + 2 ] = n.z();
                normals[ 6 * i + 3 ] = n.x();
                normals[ 6 * i + 4 ] = n.y();
                normals[ 6 * i + 5 ] = n.z();
                if ( i == nvertices - 2 )
                {
                    normals[ 6 * ( i + 1 ) + 0 ] = n.x();
                    normals[ 6 * ( i + 1 ) + 1 ] = n.y();
                    normals[ 6 * ( i + 1 ) + 2 ] = n.z();
                    normals[ 6 * ( i + 1 ) + 3 ] = n.x();
                    normals[ 6 * ( i + 1 ) + 4 ] = n.y();
                    normals[ 6 * ( i + 1 ) + 5 ] = n.z();
                }
            }
            break;
        }
        default: break;
        }
    }

    return normals;
}

kvs::ValueArray<kvs::Real32> QuadVertexTexCoords(
    const kvs::LineObject* line,
    const float halo_size,
    const float radius_size )
{
    const size_t nvertices = line->numberOfVertices();
    kvs::ValueArray<kvs::Real32> texcoords( nvertices * 4 * 2 );

    const float halo_factor = 1.0f + 2.0f * halo_size;
    const float rot = 0.0f;
    const float zdiff = 0.0f;
    for ( size_t i = 0; i < nvertices; i++ )
    {
        texcoords[ 8 * i + 0 ] = -radius_size * halo_factor;
        texcoords[ 8 * i + 1 ] =  radius_size;
        texcoords[ 8 * i + 2 ] =  rot;
        texcoords[ 8 * i + 3 ] =  zdiff;
        texcoords[ 8 * i + 4 ] =  radius_size * halo_factor;
        texcoords[ 8 * i + 5 ] =  radius_size;
        texcoords[ 8 * i + 6 ] =  rot;
        texcoords[ 8 * i + 7 ] =  zdiff;
    }
    return texcoords;
}

}


namespace kvs
{

void StylizedLineRenderer::BufferObject::set(
    const kvs::LineObject* line,
    const kvs::Real32 halo,
    const kvs::Real32 radius )
{
    if ( line->numberOfColors() != 1 &&
         line->colorType() == kvs::LineObject::LineColor )
    {
        kvsMessageError("Not supported line color type.");
        return;
    }

    const auto type = line->lineType();
    m_has_connection = line->numberOfConnections() > 0;

    auto coords = ::QuadVertexCoords( line );
    auto colors = ::QuadVertexColors( line );
    auto normals = ::QuadVertexNormals( line );
    auto texcoords = ::QuadVertexTexCoords( line, halo, radius );
    m_manager.setVertexArray( coords, 3 );
    m_manager.setColorArray( colors, 3 );
    m_manager.setNormalArray( normals );
    m_manager.setTexCoordArray( texcoords, 4 );

    if ( type == kvs::LineObject::Polyline )
    {
        const kvs::UInt32* pconnections = line->connections().data();
        m_first_array.allocate( line->numberOfConnections() );
        m_count_array.allocate( m_first_array.size() );
        for ( size_t i = 0; i < m_first_array.size(); ++i )
        {
            m_first_array[i] = 2 * ( pconnections[ 2 * i ] );
            m_count_array[i] = 2 * ( pconnections[ 2 * i + 1 ] - pconnections[ 2 * i ] + 1 );
        }
    }

    if ( !m_shape_texture.isCreated() ) { this->create_shape_texture(); }
    if ( !m_diffuse_texture.isCreated() ) { this->create_diffuse_texture(); }
}

void StylizedLineRenderer::BufferObject::draw( const kvs::LineObject* line )
{
    kvs::VertexBufferObjectManager::Binder bind( m_manager );
    kvs::Texture::Binder unit0( m_shape_texture, 0 );
    kvs::Texture::Binder unit1( m_diffuse_texture, 1 );

    // Draw lines.
    const auto type = line->lineType();
    switch ( type )
    {
    case kvs::LineObject::Polyline:
        m_manager.drawArrays( GL_QUAD_STRIP, m_first_array, m_count_array );
        break;
    case kvs::LineObject::Strip:
        m_manager.drawArrays( GL_QUAD_STRIP, 0, line->numberOfVertices() * 2 );
        break;
    default: break;
    }
}

void StylizedLineRenderer::BufferObject::create_shape_texture()
{
    const size_t resolution = 256;
    kvs::ValueArray<kvs::Real32> shape( resolution * resolution * 4 );
    for ( size_t j = 0, index = 0; j < resolution; j++ )
    {
        for ( size_t i = 0; i < resolution; i++, index++ )
        {
            const size_t index4 = index * 4;
            const kvs::Real32 x = ( i * 2.0f ) / kvs::Real32( resolution ) - 1.0f;

            // Cylinder shape.
            shape[ index4 + 0 ] = x * 0.5f + 0.5f;
            shape[ index4 + 1 ] = std::sqrt( 1.0f - x * x );
            shape[ index4 + 2 ] = std::sqrt( 1.0f - x * x );
        }
    }

    m_shape_texture.setWrapS( GL_REPEAT );
    m_shape_texture.setWrapT( GL_REPEAT );
    m_shape_texture.setMagFilter( GL_NEAREST );
    m_shape_texture.setMinFilter( GL_NEAREST );
    m_shape_texture.setPixelFormat( GL_RGBA32F_ARB, GL_RGBA, GL_FLOAT );
    m_shape_texture.create( resolution, resolution, shape.data() );
}

void StylizedLineRenderer::BufferObject::create_diffuse_texture()
{
/*
    const int resolution = 256;
    kvs::ValueArray<kvs::Real32> diffuse( resolution * resolution * 4 );
    for ( int j = 0, index = 0; j < resolution; j++ )
    {
        for ( int i = 0; i < resolution; i++, index++ )
        {
            const size_t index4 = index * 4;
//            const size_t index4 = ( j * resolution + i ) * 4;
//            const float x = kvs::Real32(i) / kvs::Real32( resolution );
            const float y = kvs::Real32(j) / kvs::Real32( resolution );

            if ( abs( ( resolution / 2 ) - i ) > ( j % ( resolution / 4 ) ) )
            {
                diffuse[ index4 + 0 ] = 1.0f;
                diffuse[ index4 + 1 ] = 1.0f;
                diffuse[ index4 + 2 ] = 1.0f;
            }
            else
            {
                diffuse[ index4 + 0 ] = 0.5f;
                diffuse[ index4 + 1 ] = 0.5f;
                diffuse[ index4 + 2 ] = 0.5f;
            }
            diffuse[ index4 + 3 ] = 1.0f;

//            diffuse[ index4 + 0 ] = (y * 4.0f) - (float)(int)(y*4.0f);
//            diffuse[ index4 + 1 ] = (y * 4.0f) - (float)(int)(y*4.0f);
//            diffuse[ index4 + 2 ] = (y * 4.0f) - (float)(int)(y*4.0f);
//            diffuse[ index4 + 3 ] = 1.0f;
        }
    }
    m_diffuse_texture.setWrapS( GL_REPEAT );
    m_diffuse_texture.setWrapT( GL_REPEAT );
    m_diffuse_texture.setMagFilter( GL_LINEAR );
    m_diffuse_texture.setMinFilter( GL_LINEAR );
    m_diffuse_texture.setPixelFormat( GL_RGBA32F, GL_RGBA, GL_FLOAT );
    m_diffuse_texture.create( resolution, resolution, diffuse.data() );
*/

    kvs::ValueArray<kvs::UInt8> diffuse( 3 );
    diffuse[0] = 255;
    diffuse[1] = 255;
    diffuse[2] = 255;

    m_diffuse_texture.setWrapS( GL_REPEAT );
    m_diffuse_texture.setWrapT( GL_REPEAT );
    m_diffuse_texture.setMagFilter( GL_LINEAR );
    m_diffuse_texture.setMinFilter( GL_LINEAR );
    m_diffuse_texture.setPixelFormat( GL_RGB, GL_RGB, GL_UNSIGNED_BYTE );
    m_diffuse_texture.create( 1, 1, diffuse.data() );
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new StylizedLineRenderer class.
 */
/*===========================================================================*/
StylizedLineRenderer::StylizedLineRenderer():
    m_vert_file( "stylized_line.vert" ),
    m_frag_file( "stylized_line.frag" ),
    m_width( 0 ),
    m_height( 0 ),
    m_object( NULL ),
    m_shading_model( new kvs::Shader::Lambert() ),
    m_radius_size( 0.05f ),
    m_halo_size( 0.0f )
{
}

/*===========================================================================*/
/**
 *  @brief  Destroys the StylizedLineRenderer class.
 */
/*===========================================================================*/
StylizedLineRenderer::~StylizedLineRenderer()
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
void StylizedLineRenderer::exec( kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light )
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
void StylizedLineRenderer::createShaderProgram()
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

void StylizedLineRenderer::updateShaderProgram()
{
    m_shader_program.release();
    this->createShaderProgram();
}

void StylizedLineRenderer::setupShaderProgram()
{
    kvs::ProgramObject::Binder bind( m_shader_program );
    m_shader_program.setUniform( "shading.Ka", m_shading_model->Ka );
    m_shader_program.setUniform( "shading.Kd", m_shading_model->Kd );
    m_shader_program.setUniform( "shading.Ks", m_shading_model->Ks );
    m_shader_program.setUniform( "shading.S",  m_shading_model->S );

    const kvs::Mat4 M = kvs::OpenGL::ModelViewMatrix();
    const kvs::Mat4 P = kvs::OpenGL::ProjectionMatrix();
    const kvs::Mat3 N = kvs::Mat3( M[0].xyz(), M[1].xyz(), M[2].xyz() );
    m_shader_program.setUniform( "ModelViewMatrix", M );
    m_shader_program.setUniform( "ProjectionMatrix", P );
    m_shader_program.setUniform( "NormalMatrix", N );
    m_shader_program.setUniform( "shape_texture", 0 );
    m_shader_program.setUniform( "diffuse_texture", 1 );
}

/*===========================================================================*/
/**
 *  @brief  Creates buffer object.
 *  @param  line [in] pointer to the line object
 */
/*===========================================================================*/
void StylizedLineRenderer::createBufferObject( const kvs::ObjectBase* object )
{
    const auto* line = kvs::LineObject::DownCast( object );
    m_object = object;
    m_buffer_object.set( line, m_halo_size, m_radius_size );
    m_buffer_object.create();
}

void StylizedLineRenderer::updateBufferObject( const kvs::ObjectBase* object )
{
    m_buffer_object.release();
    this->createBufferObject( object );
}

void StylizedLineRenderer::drawBufferObject( const kvs::Camera* camera )
{
    const auto* line = kvs::LineObject::DownCast( m_object );
    kvs::OpenGL::Enable( GL_DEPTH_TEST );
    kvs::OpenGL::Enable( GL_TEXTURE_2D );
    kvs::OpenGL::Enable( GL_BLEND );
    kvs::OpenGL::SetBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    kvs::Texture::SetEnv( GL_TEXTURE_ENV_MODE, GL_REPLACE );
    m_buffer_object.draw( line );
}

} // end of namespace kvs
