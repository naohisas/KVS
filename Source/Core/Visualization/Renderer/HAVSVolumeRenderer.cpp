/*****************************************************************************/
/**
 *  @file   HAVSVolumeRenderer.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: HAVSVolumeRenderer.cpp 1457 2013-03-24 06:32:17Z naohisa.sakamoto@gmail.com $
 */
/*----------------------------------------------------------------------------
 *
 * References:
 * [1] S.Callahan, M.Ikits, J.Comba, and C.Silva, "Hardware-Assisted Visibility
 *     Sorting for Unstructured Volume Rendering," IEEE Trans. on Visualization
 *     and Computer Graphics, Vol.11, No.3, pp.285-295, 2005.
 * [2] S.Callahan, L.Bavoil, V.Pascucci and C.Silva, "Progressive Volume Rendering
 *     of Large Unstructured Grids," IEEE Trans. on Visualization and Computer
 *     Graphics, Vol.12, No.5, pp.1307-1314, 2006.
 *
 * Acknowledgements:
 * Original source code was developped by University of Utah and can be
 * downloaded at http://havs.sourceforge.net/
 */
/*****************************************************************************/
#include "HAVSVolumeRenderer.h"
#include <set>
#include <kvs/Coordinate>
#include <kvs/OpenGL>
#include <kvs/VertexShader>
#include <kvs/FragmentShader>
#include <kvs/PreIntegrationTable3D>


namespace
{

void PartialSort(
    int byte,
    int length,
    kvs::HAVSVolumeRenderer::SortedFace* src,
    kvs::HAVSVolumeRenderer::SortedFace* dst,
    int* count )
{
    unsigned int i;
    kvs::HAVSVolumeRenderer::SortedFace* k;

    static int index[256];
    index[0] = 0;
    for ( i = 1; i < 256; i++ ) index[i] = index[i-1] + count[i-1];

    for ( i = 0; i < static_cast<unsigned int>( length ); i++ )
    {
        k = &src[i];
        dst[ index[ ( k->distance() >> (byte*8))&0xff ]++ ] = *k;
    }
}

struct LTFace
{
    bool operator() (
        const kvs::HAVSVolumeRenderer::Face& f1,
        const kvs::HAVSVolumeRenderer::Face& f2 ) const
    {
        unsigned int min1, mid1, max1, min2, mid2, max2;

        min1 = (f1.index(0) < f1.index(1) && f1.index(0) < f1.index(2)) ? f1.index(0) :
            ((f1.index(1) < f1.index(2)) ? f1.index(1) : f1.index(2));
        max1 = (f1.index(0) > f1.index(1) && f1.index(0) > f1.index(2)) ? f1.index(0) :
            ((f1.index(1) > f1.index(2)) ? f1.index(1) : f1.index(2));
        mid1 = (f1.index(0) != min1 && f1.index(0) != max1) ? f1.index(0) :
            ((f1.index(1) != min1 && f1.index(1) != max1) ? f1.index(1) : f1.index(2));

        min2 = (f2.index(0) < f2.index(1) && f2.index(0) < f2.index(2)) ? f2.index(0) :
            ((f2.index(1) < f2.index(2)) ? f2.index(1) : f2.index(2));
        max2 = (f2.index(0) > f2.index(1) && f2.index(0) > f2.index(2)) ? f2.index(0) :
            ((f2.index(1) > f2.index(2)) ? f2.index(1) : f2.index(2));
        mid2 = (f2.index(0) != min2 && f2.index(0) != max2) ? f2.index(0) :
            ((f2.index(1) != min2 && f2.index(1) != max2) ? f2.index(1) : f2.index(2));

        if ( min1 == min2 )
        {
            if ( mid1 == mid2 )
            {
                return max1 < max2;
            }
            else
            {
                return mid1 < mid2;
            }
        }
        return min1 < min2;
    }
};

// Use a union to convert floats to unsigned ints and avoid aliasing problems
union FloatOrInt
{
  float f;
  unsigned int i;
};

} // end of namespace


namespace kvs
{

HAVSVolumeRenderer::HAVSVolumeRenderer():
    m_ref_volume( NULL )
{
    this->initialize();
}

HAVSVolumeRenderer::HAVSVolumeRenderer( kvs::UnstructuredVolumeObject* volume, const size_t k_size )
{
    BaseClass::setShader( kvs::Shader::Lambert() );
    this->initialize();
    this->attachVolumeObject( volume );
    this->setKBufferSize( k_size );
}

HAVSVolumeRenderer::~HAVSVolumeRenderer()
{
    if ( m_meshes ) delete m_meshes;
    if ( !this->isEnabledVBO() )
    {
        if ( m_pindices ) delete [] m_pindices;
    }
}

void HAVSVolumeRenderer::exec( kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light )
{
    kvs::UnstructuredVolumeObject* volume = kvs::UnstructuredVolumeObject::DownCast( object );
    if ( !m_ref_volume ) this->attachVolumeObject( volume );

    BaseClass::startTimer();

    kvs::OpenGL::PushAttrib( GL_CURRENT_BIT | GL_ENABLE_BIT | GL_LIGHTING_BIT );
    kvs::OpenGL::SetShadeModel( GL_SMOOTH );
    kvs::OpenGL::GetModelViewMatrix( m_modelview );

    kvs::OpenGL::Disable( GL_DEPTH_TEST );
    kvs::OpenGL::Disable( GL_LIGHTING );
    kvs::OpenGL::Disable( GL_NORMALIZE );
    kvs::OpenGL::Disable( GL_BLEND );

    // Following processes are executed once.
    if ( BaseClass::windowWidth() == 0 && BaseClass::windowHeight() == 0 )
    {
        BaseClass::setWindowSize( camera->windowWidth(), camera->windowHeight() );
        BaseClass::setDevicePixelRatio( camera->devicePixelRatio() );
        this->initialize_geometry();
        this->initialize_shader();
        this->initialize_table();
        this->initialize_framebuffer();
    }

    // Following processes are executed when the window size is changed.
    if ( ( BaseClass::windowWidth() != camera->windowWidth() ) ||
         ( BaseClass::windowHeight() != camera->windowHeight() ) )
    {
        BaseClass::setWindowSize( camera->windowWidth(), camera->windowHeight() );
        this->update_framebuffer();
   }

    this->sort_geometry( camera, object );
    this->enable_MRT_rendering();
    this->draw_initialization_pass();
    this->draw_geometry_pass();
    this->draw_flush_pass();
    this->disable_MRT_rendering();
    this->draw_texture();

    kvs::OpenGL::PopAttrib();
    kvs::OpenGL::Finish();

    BaseClass::stopTimer();
}

void HAVSVolumeRenderer::initialize()
{
    BaseClass::setWindowSize( 0, 0 );
    m_k_size = 2;
    m_meshes = NULL;
    m_enable_vbo = true;
    m_pindices = NULL;
}

void HAVSVolumeRenderer::attachVolumeObject( const kvs::UnstructuredVolumeObject* volume )
{
    m_ref_volume = volume;

    if ( !m_meshes )
    {
        m_meshes = new HAVSVolumeRenderer::Meshes();
        m_meshes->setVolume( volume );
        m_meshes->build();
    }
}

void HAVSVolumeRenderer::initialize_geometry()
{
    if ( this->isEnabledVBO() )
    {
        const size_t coords_size = m_meshes->coords().byteSize();
        const kvs::Real32* coords_pointer = m_meshes->coords().data();
        m_vertex_coords.setUsage( GL_STATIC_DRAW_ARB );
        m_vertex_coords.create( coords_size, coords_pointer );

        const size_t values_size = m_meshes->values().byteSize();
        const kvs::Real32* values_pointer = m_meshes->values().data();
        m_vertex_values.setUsage( GL_STATIC_DRAW_ARB );
        m_vertex_values.create( values_size, values_pointer );

        const size_t faces_size = m_meshes->nfaces() * 3 * sizeof(GLuint);
        m_vertex_indices.setUsage( GL_STREAM_DRAW_ARB );
        m_vertex_indices.create( faces_size );
    }
    else
    {
        m_pindices = new GLuint [ m_meshes->nfaces() * 3 ];
    }
}

void HAVSVolumeRenderer::initialize_shader()
{
    kvs::ShaderSource vert_begin( "HAVS_begin.vert" );
    kvs::ShaderSource vert_kbuffer( "HAVS_kbuffer.vert" );
    if ( this->kBufferSize() == 2 )
    {
        kvs::ShaderSource frag_begin( "HAVS_k2_begin.frag" );
        kvs::ShaderSource frag_end( "HAVS_k2_end.frag" );
        kvs::ShaderSource frag_kbuffer( "HAVS_k2.frag" );
        m_shader_begin.build( vert_begin, frag_begin );
        m_shader_kbuffer.build( vert_kbuffer, frag_kbuffer );
        m_shader_end.build( vert_kbuffer, frag_end );
    }
    else
    {
        kvs::ShaderSource frag_begin( "HAVS_k6_begin.frag" );
        kvs::ShaderSource frag_end( "HAVS_k6_end.frag" );
        kvs::ShaderSource frag_kbuffer( "HAVS_k6.frag" );
        m_shader_begin.build( vert_begin, frag_begin );
        m_shader_kbuffer.build( vert_kbuffer, frag_kbuffer );
        m_shader_end.build( vert_kbuffer, frag_end );
    }
}

void HAVSVolumeRenderer::initialize_table()
{
    if ( !m_ref_volume->hasMinMaxValues() ) m_ref_volume->updateMinMaxValues();

    const float min_value = static_cast<float>( m_ref_volume->minValue() );
    const float max_value = static_cast<float>( m_ref_volume->maxValue() );
    const float max_size_of_cell = m_meshes->depthScale() * 2.0f;
    const size_t dim_scalar = 128;
    const size_t dim_depth = 128;
    kvs::PreIntegrationTable3D table;
    table.setScalarResolution( dim_scalar );
    table.setDepthResolution( dim_depth );
    table.setTransferFunction( BaseClass::transferFunction(), min_value, max_value );
    table.create( max_size_of_cell );

    m_preintegration_texture.setWrapS( GL_CLAMP_TO_EDGE );
    m_preintegration_texture.setWrapT( GL_CLAMP_TO_EDGE );
    m_preintegration_texture.setWrapR( GL_CLAMP_TO_EDGE );
    m_preintegration_texture.setMagFilter( GL_LINEAR );
    m_preintegration_texture.setMinFilter( GL_LINEAR );
    m_preintegration_texture.setPixelFormat( GL_RGBA8, GL_RGBA, GL_FLOAT );
    m_preintegration_texture.create( dim_scalar, dim_scalar, dim_depth, table.table().data() );
}

void HAVSVolumeRenderer::initialize_framebuffer()
{
    if ( m_k_size == 2 ) { m_ntargets = 2; }
    else { m_ntargets = 4; }

    // Create FBO textures
    for ( size_t i = 0; i < m_ntargets; i++ )
    {
        m_mrt_texture[i].setWrapS( GL_CLAMP );
        m_mrt_texture[i].setWrapT( GL_CLAMP );
        m_mrt_texture[i].setMinFilter( GL_NEAREST );
        m_mrt_texture[i].setMagFilter( GL_NEAREST );
        m_mrt_texture[i].setPixelFormat( GL_RGBA32F_ARB, GL_RGBA, GL_FLOAT );
        m_mrt_texture[i].create( BaseClass::windowWidth(), BaseClass::windowHeight() );
    }

    // Create FBO
    m_mrt_framebuffer.create();

    // Attach texture to framebuffer color buffer
    m_mrt_framebuffer.attachColorTexture( m_mrt_texture[0], 0 );
    m_mrt_framebuffer.attachColorTexture( m_mrt_texture[1], 1 );
    if ( m_ntargets == 4 )
    {
        m_mrt_framebuffer.attachColorTexture( m_mrt_texture[2], 2 );
        m_mrt_framebuffer.attachColorTexture( m_mrt_texture[3], 3 );
    }

    // Setup OpenGL state in FBO
    kvs::OpenGL::SetShadeModel( GL_SMOOTH );
    kvs::OpenGL::Disable( GL_DEPTH_TEST );
    kvs::OpenGL::Disable( GL_CULL_FACE );
    kvs::OpenGL::Disable( GL_LIGHTING );
    kvs::OpenGL::Disable( GL_NORMALIZE );
}

void HAVSVolumeRenderer::update_framebuffer()
{
    // Reallocate textures
    for ( size_t i = 0; i < m_ntargets; i++ )
    {
        m_mrt_texture[i].release();
        m_mrt_texture[i].create( BaseClass::windowWidth(), BaseClass::windowHeight() );
    }

    // Reset attached textures.
    m_mrt_framebuffer.detachColorTexture( 0 );
    m_mrt_framebuffer.detachColorTexture( 1 );
    m_mrt_framebuffer.attachColorTexture( m_mrt_texture[0], 0 );
    m_mrt_framebuffer.attachColorTexture( m_mrt_texture[1], 1 );
    if ( m_ntargets == 4 )
    {
        m_mrt_framebuffer.detachColorTexture( 2 );
        m_mrt_framebuffer.detachColorTexture( 3 );
        m_mrt_framebuffer.attachColorTexture( m_mrt_texture[2], 2 );
        m_mrt_framebuffer.attachColorTexture( m_mrt_texture[3], 3 );
    }
}

void HAVSVolumeRenderer::enable_MRT_rendering()
{
    // Enable FBO.
    m_mrt_framebuffer.bind();

    // Enable MRTs.
    const GLenum buffers[4] = {
        GL_COLOR_ATTACHMENT0_EXT,
        GL_COLOR_ATTACHMENT1_EXT,
        GL_COLOR_ATTACHMENT2_EXT,
        GL_COLOR_ATTACHMENT3_EXT };
    kvs::OpenGL::SetDrawBuffers( m_ntargets, buffers );

    // Bind textures for reading.
    kvs::OpenGL::Enable( GL_TEXTURE_2D );
    kvs::Texture::Bind( m_mrt_texture[0], 0 );
    kvs::Texture::Bind( m_mrt_texture[1], 1 );
    if ( m_ntargets == 4 )
    {
        kvs::Texture::Bind( m_mrt_texture[2], 2 );
        kvs::Texture::Bind( m_mrt_texture[3], 3 );
    }

    // Bind pre-integration table.
    kvs::OpenGL::Enable( GL_TEXTURE_3D );
    kvs::Texture::SelectActiveUnit( m_ntargets == 2 ? 2 : 4 );
    m_preintegration_texture.bind();
}

void HAVSVolumeRenderer::disable_MRT_rendering()
{
    // Disable pre-integration table.
    kvs::Texture::SelectActiveUnit( m_ntargets == 2 ? 2 : 4 );
    kvs::OpenGL::Disable( GL_TEXTURE_3D );
    m_preintegration_texture.unbind();

    // Disable FBO rendering
    m_mrt_framebuffer.unbind();
}

void HAVSVolumeRenderer::sort_geometry( kvs::Camera* camera, kvs::ObjectBase* object )
{
    // Visibility sorting in the object coordinate system.
    const kvs::Vec3 position = kvs::WorldCoordinate( camera->position() ).toObjectCoordinate( object ).position();
    const HAVSVolumeRenderer::Vertex eye( position );
    m_meshes->sort( eye );

    if ( this->isEnabledVBO() )
    {
        m_vertex_indices.bind();
        m_pindices = static_cast<GLuint*>( m_vertex_indices.map( kvs::IndexBufferObject::WriteOnly ) );
    }

    for ( size_t i = 0, index = 0; i < m_meshes->nrenderfaces(); i++ )
    {
        for ( size_t j = 0; j < 3; j++, index++ )
        {
            const kvs::UInt32 face_index = m_meshes->sortedFace( i );
            const GLuint vertex_index = static_cast<GLuint>( m_meshes->face( face_index ).index( j ) );
            m_pindices[ index ] = vertex_index;
        }
    }

    if ( this->isEnabledVBO() )
    {
        m_vertex_indices.unmap();
        m_vertex_indices.unbind();
    }
}

void HAVSVolumeRenderer::draw_initialization_pass()
{
    kvs::ProgramObject::Binder binder( m_shader_begin );

    const size_t width = BaseClass::windowWidth();
    const size_t height = BaseClass::windowHeight();
    kvs::OpenGL::WithPushedMatrix p1( GL_MODELVIEW );
    p1.loadIdentity();
    {
        kvs::OpenGL::WithPushedMatrix p2( GL_PROJECTION );
        p2.loadIdentity();
        {
            kvs::OpenGL::SetOrtho( 0, width, 0, height );
            kvs::OpenGL::Begin( GL_QUADS );
            kvs::OpenGL::Vertex( GLint(0), GLint(0) );
            kvs::OpenGL::Vertex( GLint(width), GLint(0) );
            kvs::OpenGL::Vertex( GLint(width), GLint(height) );
            kvs::OpenGL::Vertex( GLint(0), GLint(height) );
            kvs::OpenGL::End();
        }
    }
}

void HAVSVolumeRenderer::draw_geometry_pass()
{
    kvs::ProgramObject::Binder binder( m_shader_kbuffer );

    const float* mat = m_modelview;
    const float table_size = 128.0f;
    const float edge_length = m_meshes->depthScale();
    const float bb_scale = std::sqrt( mat[0]*mat[0] + mat[1]*mat[1] + mat[2]*mat[2] );
    const kvs::Vector4f scale(
        1.0f / static_cast<float>( BaseClass::windowWidth() ),
        1.0f / static_cast<float>( BaseClass::windowHeight() ),
        ( 1.0f - 1.0f / table_size ) / ( edge_length * bb_scale ),
        1.0f / ( 2.0f * table_size ) );

    m_shader_kbuffer.setUniform( "scale", scale );
    m_shader_kbuffer.setUniform( "lut", GLint(m_ntargets) );
    m_shader_kbuffer.setUniform( "framebuffer", 0 );
    m_shader_kbuffer.setUniform( "kbuffer1", 1 );
    if ( m_ntargets == 4 )
    {
        m_shader_kbuffer.setUniform( "kbuffer2", 2 );
        m_shader_kbuffer.setUniform( "kbuffer3", 3 );
    }

    if ( this->isEnabledVBO() )
    {
        kvs::OpenGL::EnableClientState( GL_VERTEX_ARRAY );
        m_vertex_coords.bind();
        kvs::OpenGL::VertexPointer( 3, GL_FLOAT, 0, (char*)NULL );

        kvs::OpenGL::EnableClientState( GL_TEXTURE_COORD_ARRAY );
        m_vertex_values.bind();
        kvs::OpenGL::TexCoordPointer( 1, GL_FLOAT, 0, (char*)NULL );

        m_vertex_indices.bind();
        kvs::OpenGL::DrawElements( GL_TRIANGLES, m_meshes->nrenderfaces() * 3, GL_UNSIGNED_INT, (char*)NULL );

        kvs::OpenGL::DisableClientState( GL_VERTEX_ARRAY );
        kvs::OpenGL::DisableClientState( GL_TEXTURE_COORD_ARRAY );
    }
    else
    {
        kvs::OpenGL::EnableClientState( GL_VERTEX_ARRAY );
        kvs::OpenGL::VertexPointer( 3, GL_FLOAT, 0, m_meshes->coords().data() );

        kvs::OpenGL::EnableClientState( GL_TEXTURE_COORD_ARRAY );
        kvs::OpenGL::TexCoordPointer( 1, GL_FLOAT, 0, m_meshes->values().data() );

        kvs::OpenGL::DrawElements( GL_TRIANGLES, m_meshes->nrenderfaces() * 3, GL_UNSIGNED_INT, m_pindices );

        kvs::OpenGL::DisableClientState( GL_VERTEX_ARRAY );
        kvs::OpenGL::DisableClientState( GL_TEXTURE_COORD_ARRAY );
    }
}

void HAVSVolumeRenderer::draw_flush_pass()
{
    kvs::ProgramObject::Binder binder( m_shader_end );

    const float* mat = m_modelview;
    const float table_size = 128.0f;
    const float edge_length = m_meshes->depthScale();
    const float bb_scale = std::sqrt( mat[0]*mat[0] + mat[1]*mat[1] + mat[2]*mat[2] );
    const kvs::Vector4f scale(
        1.0f / static_cast<float>( BaseClass::windowWidth() ),
        1.0f / static_cast<float>( BaseClass::windowHeight() ),
        ( 1.0f - 1.0f / table_size ) / ( edge_length * bb_scale ),
        1.0f / ( 2.0f * table_size ) );

    m_shader_end.setUniform( "scale", scale );
    m_shader_end.setUniform( "lut", GLint(m_ntargets) );
    m_shader_end.setUniform( "framebuffer", 0 );
    m_shader_end.setUniform( "kbuffer1", 1 );
    if ( m_ntargets == 4 )
    {
        m_shader_end.setUniform( "kbuffer2", 2 );
        m_shader_end.setUniform( "kbuffer3", 3 );
    }

    // Draw k-1 quads to flush A-buffer
    const size_t width = BaseClass::windowWidth();
    const size_t height = BaseClass::windowHeight();
    kvs::OpenGL::WithPushedMatrix p1( GL_MODELVIEW );
    p1.loadIdentity();
    {
        kvs::OpenGL::WithPushedMatrix p2( GL_PROJECTION );
        p2.loadIdentity();
        {
            kvs::OpenGL::SetOrtho( 0, width, 0, height );
            for ( size_t i = 0; i < this->kBufferSize() - 1; i++ )
            {
                kvs::OpenGL::Begin( GL_QUADS );
                kvs::OpenGL::Vertex( GLint(0), GLint(0) );
                kvs::OpenGL::Vertex( GLint(0), GLint(height) );
                kvs::OpenGL::Vertex( GLint(width), GLint(height) );
                kvs::OpenGL::Vertex( GLint(width), GLint(0) );
                kvs::OpenGL::End();
            }
        }
    }

    kvs::OpenGL::Flush();
}

void HAVSVolumeRenderer::draw_texture()
{
    // Setup 2D view
    const size_t width = BaseClass::windowWidth();
    const size_t height = BaseClass::windowHeight();
    kvs::OpenGL::WithPushedMatrix p1( GL_MODELVIEW );
    p1.loadIdentity();
    {
        kvs::OpenGL::WithPushedMatrix p2( GL_PROJECTION );
        p2.loadIdentity();
        {
            kvs::OpenGL::SetOrtho( 0, width, 0, height );
            kvs::OpenGL::WithEnabled texture( GL_TEXTURE_2D );
            kvs::OpenGL::WithEnabled blend( GL_BLEND );
            {
                kvs::OpenGL::SetDrawBuffer( GL_BACK );
                kvs::OpenGL::SetBlendFunc( GL_ONE, GL_ONE_MINUS_SRC_ALPHA );

                // Bind the last MRT texture.
                kvs::Texture::Bind( m_mrt_texture[0], 0 );
                kvs::Texture::SetEnv( GL_TEXTURE_ENV_MODE, GL_REPLACE );

                // Draw texture using screen-aligned quad
                kvs::OpenGL::Begin( GL_QUADS );
                kvs::OpenGL::TexCoord( GLint(0), GLint(0) ); kvs::OpenGL::Vertex( GLint(0), GLint(0) );
                kvs::OpenGL::TexCoord( GLint(1), GLint(0) ); kvs::OpenGL::Vertex( GLint(width), GLint(0) );
                kvs::OpenGL::TexCoord( GLint(1), GLint(1) ); kvs::OpenGL::Vertex( GLint(width), GLint(height) );
                kvs::OpenGL::TexCoord( GLint(0), GLint(1) ); kvs::OpenGL::Vertex( GLint(0), GLint(height) );
                kvs::OpenGL::End();
            }
        }
    }
}

HAVSVolumeRenderer::Meshes::Meshes():
    m_faces( NULL ),
    m_centers( NULL ),
    m_radix_temp( NULL ),
    m_nvertices( 0 ),
    m_ntetrahedra( 0 ),
    m_nfaces( 0 ),
    m_nboundaryfaces( 0 ),
    m_ninternalfaces( 0 ),
    m_nrenderfaces( 0 ),
    m_diagonal( 0.0f ),
    m_depth_scale( 0.0f )
{
    m_bb_min = kvs::Vector3f( 0.0f, 0.0f, 0.0f );
    m_bb_max = kvs::Vector3f( 0.0f, 0.0f, 0.0f );
    m_sorted_faces = NULL;
}

HAVSVolumeRenderer::Meshes::~Meshes()
{
}

void HAVSVolumeRenderer::Meshes::setVolume( const kvs::UnstructuredVolumeObject* volume )
{
    if ( volume->cellType() != kvs::UnstructuredVolumeObject::Tetrahedra )
    {
        kvsMessageError("Volume data is not tetrahdra cells.");
        return;
    }

    if ( volume->veclen() != 1 )
    {
        kvsMessageError("Volume data is not scalar volume.");
        return;
    }

    if ( volume->values().typeInfo()->type() != typeid( kvs::Real32 ) )
    {
        kvsMessageError("Data type of the node value is not floating-point.");
        return;
    }

    this->clean();

    m_nvertices = volume->numberOfNodes();
    m_ntetrahedra = volume->numberOfCells();
    m_coords = volume->coords();
    m_connections = volume->connections();

    // Normalize scalars
    if ( !volume->hasMinMaxValues() ) const_cast<kvs::UnstructuredVolumeObject*>(volume)->updateMinMaxValues();
    const size_t nvalues = volume->values().size();
    const float* values = static_cast<const float*>(volume->values().data());
    const float min_value = static_cast<float>( volume->minValue() );
    const float max_value = static_cast<float>( volume->maxValue() );
    const float range = max_value - min_value;
    m_values.allocate( nvalues );
    for ( size_t i = 0; i < nvalues; i++ )
    {
        m_values[i] = ( values[i] - min_value ) / range;
    }

    // Find bounding box.
    if ( !volume->hasMinMaxObjectCoords() ) const_cast<kvs::UnstructuredVolumeObject*>(volume)->updateMinMaxCoords();
    m_bb_min = volume->minObjectCoord();
    m_bb_max = volume->maxObjectCoord();
    m_diagonal = static_cast<float>( ( m_bb_max - m_bb_min ).length() );
}

void HAVSVolumeRenderer::Meshes::build()
{
    typedef std::set<HAVSVolumeRenderer::Face, ::LTFace> FaceSet;

    FaceSet face_set;
    FaceSet::const_iterator face_it;

    std::pair<FaceSet::iterator,bool> result1;
    std::pair<FaceSet::iterator,bool> result2;
    std::pair<FaceSet::iterator,bool> result3;
    std::pair<FaceSet::iterator,bool> result4;

    const kvs::UInt32* pconnections = m_connections.data();
    for ( size_t i = 0; i < m_ntetrahedra; i++ )
    {
        const kvs::UInt32 id0 = pconnections[4*i+0];
        const kvs::UInt32 id1 = pconnections[4*i+1];
        const kvs::UInt32 id2 = pconnections[4*i+2];
        const kvs::UInt32 id3 = pconnections[4*i+3];

        HAVSVolumeRenderer::Face f1( id0, id1, id2 );
        HAVSVolumeRenderer::Face f2( id0, id1, id3 );
        HAVSVolumeRenderer::Face f3( id0, id2, id3 );
        HAVSVolumeRenderer::Face f4( id1, id2, id3 );

        result1 = face_set.insert( f1 );
        result2 = face_set.insert( f2 );
        result3 = face_set.insert( f3 );
        result4 = face_set.insert( f4 );

        if ( !result1.second ) (*result1.first).setBoundary( false );
        if ( !result2.second ) (*result2.first).setBoundary( false );
        if ( !result3.second ) (*result3.first).setBoundary( false );
        if ( !result4.second ) (*result4.first).setBoundary( false );
    }

    int boundary_count = 0;
    face_it = face_set.begin();
    while ( face_it != face_set.end() )
    {
        if ( (*face_it++).isBoundary() ) boundary_count++;
    }

    m_nfaces = face_set.size();
    m_nboundaryfaces = boundary_count;
    m_ninternalfaces = m_nfaces - m_nboundaryfaces;
    m_nrenderfaces = m_nfaces;

    m_faces = new HAVSVolumeRenderer::Face [ m_nfaces ];
    m_boundary_faces.allocate( m_nboundaryfaces );
    m_internal_faces.allocate( m_ninternalfaces );
    m_sorted_faces = new HAVSVolumeRenderer::SortedFace [ m_nfaces ];
    m_centers = new HAVSVolumeRenderer::Vertex [ m_nfaces ];
    m_radix_temp = new HAVSVolumeRenderer::SortedFace [ m_nfaces ];

    face_it = face_set.begin();
    size_t boundary_face_index = 0;
    size_t internal_face_index = 0;
    size_t face_index = 0;
    while ( face_it != face_set.end() )
    {
        HAVSVolumeRenderer::Face f = *face_it++;
        if ( f.isBoundary() )
        {
            m_boundary_faces[boundary_face_index++] = face_index;
        }
        else
        {
            m_internal_faces[internal_face_index++] = face_index;
        }

        m_faces[face_index++] = f;
    }

    // Build centers.
    float max_edge_length = 0.0f;
    for ( size_t i = 0; i < m_nfaces; i++ )
    {
        const HAVSVolumeRenderer::Face f = m_faces[i];
        const HAVSVolumeRenderer::Vertex v1(
            m_coords[ f.index(0) * 3 + 0 ],
            m_coords[ f.index(0) * 3 + 1 ],
            m_coords[ f.index(0) * 3 + 2 ], 0.0f );
        const HAVSVolumeRenderer::Vertex v2(
            m_coords[ f.index(1) * 3 + 0 ],
            m_coords[ f.index(1) * 3 + 1 ],
            m_coords[ f.index(1) * 3 + 2 ], 0.0f );
        const HAVSVolumeRenderer::Vertex v3(
            m_coords[ f.index(2) * 3 + 0 ],
            m_coords[ f.index(2) * 3 + 1 ],
            m_coords[ f.index(2) * 3 + 2 ], 0.0f );

        // Calculate max edge length.
        const float d1 = ( v1 - v2 ).norm2();
        const float d2 = ( v1 - v3 ).norm2();
        const float d3 = ( v2 - v3 ).norm2();
        max_edge_length = kvs::Math::Max( max_edge_length, d1, d2, d3 );

        // Calculate center.
        const HAVSVolumeRenderer::Vertex center(
            ( v1.x() + v2.x() + v3.x() ) / 3.0f,
            ( v1.y() + v2.y() + v3.y() ) / 3.0f,
            ( v1.z() + v2.z() + v3.z() ) / 3.0f, 0.0f );
        m_centers[i] = center;
    }

    m_depth_scale = std::sqrt( max_edge_length );
    m_diagonal = static_cast<float>( ( m_bb_max - m_bb_min ).length() );
}

void HAVSVolumeRenderer::Meshes::sort( HAVSVolumeRenderer::Vertex eye )
{
    HAVSVolumeRenderer::SortedFace sorted_face;
    ::FloatOrInt dist2;
    size_t sorted_face_count = 0;
    size_t i = 0;

    // Add boundary faces first.
    for ( i = 0; i < m_nboundaryfaces; i++ )
    {
        const size_t f = m_boundary_faces[i];
        const HAVSVolumeRenderer::Vertex fc = m_centers[f];
        dist2.f = static_cast<float>(( eye - fc ).norm2());
        sorted_face = HAVSVolumeRenderer::SortedFace( f, dist2.i );
        m_sorted_faces[sorted_face_count++] = sorted_face;
    }

    // Add internal faces as determined by LOD budget
    const size_t internal_count = m_nrenderfaces - m_nboundaryfaces;
    for ( i = 0; i < internal_count; i++ )
    {
        const size_t f = m_internal_faces[i];
        const HAVSVolumeRenderer::Vertex fc = m_centers[f];
        dist2.f = static_cast<float>(( eye - fc ).norm2());
        sorted_face = HAVSVolumeRenderer::SortedFace( f, dist2.i );
        m_sorted_faces[sorted_face_count++] = sorted_face;
    }

    this->radix_sort( m_sorted_faces, m_radix_temp, 0, m_nrenderfaces );
}

void HAVSVolumeRenderer::Meshes::clean()
{
    m_coords.release();
    m_connections.release();
    m_values.release();
    m_boundary_faces.release();
    m_internal_faces.release();

    if ( m_faces ) { delete m_faces; m_faces = NULL; }
    if ( m_sorted_faces ) { delete m_sorted_faces; m_sorted_faces = NULL; }
    if ( m_centers ) { delete m_centers; m_centers = NULL; }
    if ( m_radix_temp ) { delete m_radix_temp; m_radix_temp = NULL; }
}

void HAVSVolumeRenderer::Meshes::radix_sort(
    HAVSVolumeRenderer::SortedFace* array,
    HAVSVolumeRenderer::SortedFace* temp,
    int lo,
    int up )
{
    int length = up -lo;
    unsigned int i;
    unsigned int u;

    SortedFace* uints = array + lo;
    int count[4][256] = {{0}};

    // Generate count arrays.
    for ( i = 0; i < static_cast<unsigned int>( length ); i++ )
    {
        u = uints[i].distance();
        count[0][ u & 0xff ]++;
        count[1][ (u >> 8) & 0xff ]++;
        count[2][ (u >> 16) & 0xff ]++;
        count[3][ (u >> 24) & 0xff ]++;
    }

    // Start sorting.
    ::PartialSort( 0, length, uints, temp,  count[0] );
    ::PartialSort( 1, length, temp,  uints, count[1] );
    ::PartialSort( 2, length, uints, temp,  count[2] );
    ::PartialSort( 3, length, temp,  uints, count[3] );
}

} // end of namespace kvs
