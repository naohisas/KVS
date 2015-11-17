/*****************************************************************************/
/**
 *  @file   ParticleBasedRendererRITS.cpp
 *  @author Makoto Uemura, Satoshi Tanaka
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id$
 */
/*****************************************************************************/
#include "ParticleBasedRendererRITS.h" // SAKAMOTO-TANAKA
#include <cstring>
#include <kvs/DebugNew>
#include <kvs/PointObject>
#include <kvs/Camera>
#include <kvs/Assert>
#include <kvs/Math>
#include <kvs/MersenneTwister>
#include <kvs/CellByCellParticleGenerator>
#include <algorithm>

// #define DEBUG_RL_BASED_ZOOMING  // TANAKA

namespace
{
    //ADD_UEMURA
template<typename T>
static void Swap(T* a, size_t ai, size_t bi) {
    T t[3];
    for(size_t i=0; i < 3; i++) t[i] = a[ai*3+i];
    for(size_t i=0; i < 3; i++)
    { a[ai*3+i] = a[bi*3+i]; a[bi*3+i] = t[i]; }
}

const size_t VBOInterleave = 2;

}
    /*ADD_UEMURA(begin)*/
namespace rendering_process{
double default_camera_distance = 12.0;
double used_point_ratio = 1.0;
size_t used_points = 0;
size_t start_pos = 0;
bool division_mode = 0;
bool enable_hybrid_zooming;
kvs::Vector3f object_center;
float object_magnification;
float screen_magnification;
}
    /*ADD_UEMURA(end)*/

namespace kvs
{

namespace glsl
{

namespace rits
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new EnsembleAverageBuffer class.
 *  @param  enable_accum [in] check flag whether the accumulation buffer is used or not
 */
/*===========================================================================*/
EnsembleAverageBuffer::EnsembleAverageBuffer( const bool enable_accum ):
    m_enable_accum( enable_accum ),
    m_width(0),
    m_height(0),
    m_count(0),
    m_accum_scale(1.0f)
{
}

/*==========================================================================*/
/**
 *  @brief  Initializes the rendering framebuffer.
 *  @param  width  [in] screen width
 *  @param  height [in] screen height
 */
/*==========================================================================*/
void EnsembleAverageBuffer::create( const size_t width, const size_t height )
{
    m_width = width;
    m_height = height;

    if ( m_enable_accum )
    {
        KVS_GL_CALL( glClearAccum( 0.0, 0.0, 0.0, 0.0 ) );
        GLint accum_r = 0;
        kvs::OpenGL::GetIntegerv( GL_ACCUM_RED_BITS, &accum_r );
        const float pixel_scale = 255.0;
        const float accum_scale = float( 1 << ( accum_r ) );
        m_accum_scale = pixel_scale / accum_scale;
    }
    else
    {
        m_framebuffer.release();
        m_framebuffer.create();
        m_framebuffer.bind();
        {
            m_texture.release();
            m_texture.setWrapS( GL_CLAMP_TO_EDGE );
            m_texture.setWrapT( GL_CLAMP_TO_EDGE );
            m_texture.setMagFilter( GL_NEAREST );
            m_texture.setMinFilter( GL_NEAREST );
            m_texture.setPixelFormat( GL_RGB32F_ARB, GL_RGB, GL_FLOAT );
            m_texture.create( m_width, m_height );
            m_framebuffer.attachColorTexture( m_texture );
        }
        m_framebuffer.unbind();
        m_framebuffer.checkStatus();
    }
}

/*==========================================================================*/
/**
 *  @brief  Clear the average count.
 */
/*==========================================================================*/
void EnsembleAverageBuffer::clear()
{
    if ( m_enable_accum )
    {
        kvs::OpenGL::Clear( GL_ACCUM_BUFFER_BIT );
    }
    else
    {
        m_framebuffer.bind();
        this->draw_quad( 0.0f, 0.0f, 0.0f, 0.0f );
        m_framebuffer.unbind();
    }

    m_count = 0;
}

/*==========================================================================*/
/**
 *  @brief  Start rendering to the ensemble average buffer.
 */
/*==========================================================================*/
void EnsembleAverageBuffer::bind()
{
    if ( m_enable_accum )
    {
        kvs::OpenGL::Clear( GL_COLOR_BUFFER_BIT );
    }
    else
    {
        m_count++;
        m_framebuffer.bind();

        kvs::OpenGL::SetViewport( 0, 0, m_width, m_height );
        kvs::OpenGL::Disable( GL_DEPTH_TEST );
        kvs::OpenGL::Disable( GL_CULL_FACE );
        kvs::OpenGL::Disable( GL_LIGHTING );
        kvs::OpenGL::Enable( GL_TEXTURE_2D );

        kvs::OpenGL::Enable( GL_BLEND );
        kvs::OpenGL::SetBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
        KVS_GL_CALL( glColor4f( 1.0f, 1.0f, 1.0f, this->opacity() ) );
    }
}

/*==========================================================================*/
/**
 *  @brief  Stop rendering to the ensemble average buffer.
 */
/*==========================================================================*/
void EnsembleAverageBuffer::unbind()
{
    if ( m_enable_accum ) {}
    else
    {
        m_framebuffer.unbind();
        kvs::OpenGL::Disable( GL_BLEND );
        kvs::OpenGL::SetBlendFunc( GL_ONE, GL_ZERO );
    }
}

/*==========================================================================*/
/**
 *  @brief  Add rendering result to the buffer.
 */
/*==========================================================================*/
void EnsembleAverageBuffer::add()
{
    if ( m_enable_accum )
    {
        KVS_GL_CALL( glAccum( GL_ACCUM, m_accum_scale ) );
        m_count++;
    }
    else
    {
        this->unbind();
    }
}

/*==========================================================================*/
/**
 *  @brief  Draw the accumulation result.
 */
/*==========================================================================*/
void EnsembleAverageBuffer::draw()
{
    if ( m_enable_accum )
    {
        KVS_GL_CALL( glAccum( GL_RETURN, static_cast<GLfloat>(1.0) / m_accum_scale / m_count ) );
    }
    else
    {
        kvs::OpenGL::Disable( GL_DEPTH_TEST );
        kvs::OpenGL::Disable( GL_CULL_FACE );
        kvs::OpenGL::Disable( GL_LIGHTING );
        kvs::OpenGL::Disable( GL_NORMALIZE );
        kvs::OpenGL::Enable( GL_TEXTURE_2D );
        kvs::Texture::SelectActiveUnit( 0 );
        m_texture.bind();
        this->draw_quad( 1.0f, 1.0f, 1.0f, 1.0f );
        m_texture.unbind();
    }
}

/*===========================================================================*/
/**
 *  @brief  Enables the accumulation buffer.
 */
/*===========================================================================*/
void EnsembleAverageBuffer::enableAccumulationBuffer()
{
    GLint accum_r = 0; kvs::OpenGL::GetIntegerv( GL_ACCUM_RED_BITS,   &accum_r );
    GLint accum_g = 0; kvs::OpenGL::GetIntegerv( GL_ACCUM_GREEN_BITS, &accum_g );
    GLint accum_b = 0; kvs::OpenGL::GetIntegerv( GL_ACCUM_BLUE_BITS,  &accum_b );
    if ( accum_r == 0 || accum_g == 0 || accum_b == 0 )
    {
        kvsMessageError("Accumulation buffer cannot be enabled.");
        m_enable_accum = false;
    }
    else
    {
        m_enable_accum = true;
    }
}

/*===========================================================================*/
/**
 *  @brief  Disables the accumulation buffer.
 */
/*===========================================================================*/
void EnsembleAverageBuffer::disableAccumulationBuffer()
{
    m_enable_accum = false;
}

/*===========================================================================*/
/**
 *  @brief  Returns the check flag whether the accumulation buffer is enabled
 *  @return true, if the accumulation buffer is enabled
 */
/*===========================================================================*/
bool EnsembleAverageBuffer::isEnabledAccumulationBuffer()
{
    return m_enable_accum;
}

/*===========================================================================*/
/**
 *  @brief  Returns the number of accumulations.
 *  @return number of accumulations
 */
/*===========================================================================*/
size_t EnsembleAverageBuffer::count() const
{
    return m_count;
}

/*==========================================================================*/
/**
 *  @brief   Returns an opacity value for rendering to the buffer.
 *  @return  opacity value
 */
/*==========================================================================*/
float EnsembleAverageBuffer::opacity() const
{
    if ( m_enable_accum ) { return 1.0f; }
    else { return m_count > 0 ? 1.0f / m_count : 1.0f; }
}

/*==========================================================================*/
/**
 *  @brief  Draws a quad.
 *  @param  r [in] red value
 *  @param  g [in] green value
 *  @param  b [in] blue value
 *  @param  a [in] opacity value
 */
/*==========================================================================*/
void EnsembleAverageBuffer::draw_quad(
    const float r,
    const float g,
    const float b,
    const float a )
{
    kvs::OpenGL::WithPushedMatrix p1( GL_MODELVIEW );
    p1.loadIdentity();
    {
        kvs::OpenGL::WithPushedMatrix p2( GL_PROJECTION );
        p2.loadIdentity();
        {
            kvs::OpenGL::SetOrtho( 0, 1, 0, 1, -1, 1 );
            kvs::OpenGL::WithDisabled d1( GL_DEPTH_TEST );
            kvs::OpenGL::WithDisabled d2( GL_LIGHTING );
            kvs::OpenGL::WithEnabled e1( GL_TEXTURE_2D );
            {
                KVS_GL_CALL_BEG( glBegin( GL_QUADS ) );
                KVS_GL_CALL_VER( glColor4f( r, g, b, a ) );
                KVS_GL_CALL_VER( glTexCoord2f( 1, 1 ) ); KVS_GL_CALL_VER( glVertex2f( 1, 1 ) );
                KVS_GL_CALL_VER( glTexCoord2f( 0, 1 ) ); KVS_GL_CALL_VER( glVertex2f( 0, 1 ) );
                KVS_GL_CALL_VER( glTexCoord2f( 0, 0 ) ); KVS_GL_CALL_VER( glVertex2f( 0, 0 ) );
                KVS_GL_CALL_VER( glTexCoord2f( 1, 0 ) ); KVS_GL_CALL_VER( glVertex2f( 1, 0 ) );
                KVS_GL_CALL_END( glEnd() );
            }
        }
    }
}


/*===========================================================================*/
/**
 *  @brief  Constructs a Particle class.
 */
/*===========================================================================*/
ParticleBasedRenderer::Particles::Particles():
    m_nvertices( 0 ),
    m_indices( NULL ),
    m_coords( NULL ),
    m_normals( NULL ),
    m_colors( NULL )
{
}

/*===========================================================================*/
/**
 *  @brief  Destructs the Particles class.
 */
/*===========================================================================*/
ParticleBasedRenderer::Particles::~Particles()
{
    this->release();
}

/*===========================================================================*/
/**
 *  @brief  Releases the particle resources.
 */
/*===========================================================================*/
void ParticleBasedRenderer::Particles::release()
{
    if ( m_indices ) { delete[] m_indices; m_indices = NULL; }
    if ( m_coords )  { delete[] m_coords;  m_coords  = NULL; }
    if ( m_normals ) { delete[] m_normals; m_normals = NULL; }
    if ( m_colors )  { delete[] m_colors;  m_colors  = NULL; }
}

/*===========================================================================*/
/**
 *  @brief  Creates the particle resources.
 *  @param  nvertices [in] number of vertices
 *  @param  has_normal [in] check flag whether the particle has the index
 *  @param  has_index [in] check flag whether the particle has the normal vector
 */
/*===========================================================================*/
void ParticleBasedRenderer::Particles::create(
    const size_t nvertices,
    const bool has_normal,
    const bool has_index )
{
    this->release();

    m_nvertices = nvertices;
    m_coords = new ParticleBasedRenderer::CoordType [ nvertices * 3 ];
    m_colors = new ParticleBasedRenderer::ColorType [ nvertices * 3 ];
    if ( has_index ) m_indices = new ParticleBasedRenderer::IndexType [ nvertices * 2 ];
    if ( has_normal ) m_normals = new ParticleBasedRenderer::NormalType [ nvertices * 3 ];
}

/*===========================================================================*/
/**
 *  @brief  Check whether the particle has the index or not.
 *  @return true, if the particle has the index
 */
/*===========================================================================*/
bool ParticleBasedRenderer::Particles::hasIndex() const
{
    return( m_indices != NULL );
}

/*===========================================================================*/
/**
 *  @brief  Check whether the particle has the normal vector or not.
 *  @return true, if the particle has the normal vector
 */
/*===========================================================================*/
bool ParticleBasedRenderer::Particles::hasNormal() const
{
    return( m_normals != NULL );
}

/*===========================================================================*/
/**
 *  @brief  Returns the number of vertices.
 *  @return number of vertices
 */
/*===========================================================================*/
size_t ParticleBasedRenderer::Particles::nvertices() const
{
    return( m_nvertices );
}

/*===========================================================================*/
/**
 *  @brief  Returns the byte size per vertex.
 *  @return byte size per vertex
 */
/*===========================================================================*/
size_t ParticleBasedRenderer::Particles::byteSizePerVertex() const
{
    const size_t index_size  = this->hasIndex() ? sizeof( ParticleBasedRenderer::IndexType ) * 2 : 0;
    const size_t coord_size  = sizeof( ParticleBasedRenderer::CoordType ) * 3;
    const size_t normal_size = this->hasNormal() ? sizeof( ParticleBasedRenderer::NormalType ) * 3 : 0;
    const size_t color_size  = sizeof( ParticleBasedRenderer::ColorType ) * 3;
    return( index_size + coord_size + normal_size + color_size );
}

/*===========================================================================*/
/**
 *  @brief  Returns the total byte size of the all particles.
 *  @return byte size
 */
/*===========================================================================*/
size_t ParticleBasedRenderer::Particles::byteSize() const
{
    return( this->byteSizePerVertex() * m_nvertices );
}

/*===========================================================================*/
/**
 *  @brief  Returns the pointer to the index array.
 *  @return pointer to the index array
 */
/*===========================================================================*/
const ParticleBasedRenderer::IndexType* ParticleBasedRenderer::Particles::indices() const
{
    return( m_indices );
}

/*===========================================================================*/
/**
 *  @brief  Returns the pointer to the index array without const.
 *  @return pointer to the index array
 */
/*===========================================================================*/
ParticleBasedRenderer::IndexType* ParticleBasedRenderer::Particles::indices()
{
    return( m_indices );
}

/*===========================================================================*/
/**
 *  @brief  Returns the pointer to the coordinate value array.
 *  @return pointer to the coordinate value array
 */
/*===========================================================================*/
const ParticleBasedRenderer::CoordType* ParticleBasedRenderer::Particles::coords() const
{
    return( m_coords );
}

/*===========================================================================*/
/**
 *  @brief  Returns the pointer to the coordinate value array without const.
 *  @return pointer to the coordinate value array
 */
/*===========================================================================*/
ParticleBasedRenderer::CoordType* ParticleBasedRenderer::Particles::coords()
{
    return( m_coords );
}

/*===========================================================================*/
/**
 *  @brief  Returns the pointer to the normal vector array.
 *  @return pointer to the normal vector array
 */
/*===========================================================================*/
const ParticleBasedRenderer::NormalType* ParticleBasedRenderer::Particles::normals() const
{
    return( m_normals );
}

/*===========================================================================*/
/**
 *  @brief  Returns the pointer to the normal vector array without const.
 *  @return pointer to normal vector array
 */
/*===========================================================================*/
ParticleBasedRenderer::NormalType* ParticleBasedRenderer::Particles::normals()
{
    return( m_normals );
}

/*===========================================================================*/
/**
 *  @brief  Returns the pointer to the color value array.
 *  @return pointer to the color value array
 */
/*===========================================================================*/
const ParticleBasedRenderer::ColorType* ParticleBasedRenderer::Particles::colors() const
{
    return( m_colors );
}

/*===========================================================================*/
/**
 *  @brief  Returns the pointer to the color value array without const.
 *  @return pointer to the color value array
 */
/*===========================================================================*/
ParticleBasedRenderer::ColorType* ParticleBasedRenderer::Particles::colors()
{
    return( m_colors );
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new Renderer class.
 */
/*===========================================================================*/
ParticleBasedRenderer::Renderer::Renderer():
    m_particles(NULL),
    m_start(0),
    m_count(0),
    m_off_index(0),
    m_off_coord(0),
    m_off_normal(0),
    m_off_color(0),
    m_loc_identifier(0)
{
}

/*===========================================================================*/
/**
 *  @brief  Sets particles for rendering on GPU.
 *  @param  particles [in] pointer to the particles.
 *  @param  start [in] start number of the particles
 *  @param  count [in]
 *  @param  loc_identifier [in]
 */
/*===========================================================================*/
void ParticleBasedRenderer::Renderer::set(
    const ParticleBasedRenderer::Particles* particles,
    const size_t start,
    const size_t count,
    const size_t loc_identifier )
{
    m_particles = particles;
    m_start = start; // start vertex number

    size_t end = start + count;
    if ( end > m_particles->nvertices() )
    {
        end = m_particles->nvertices(); // end vertex number + 1
    }

    m_count = end - start;  // number of vertices

    m_loc_identifier = loc_identifier;
}

/*===========================================================================*/
/**
 *  @brief  Downloads the Vertex Buffer Object.
 *  @param  vbo [in] Vertex Buffer Object
 *  @return number of vertices
 */
/*===========================================================================*/
size_t ParticleBasedRenderer::Renderer::download( kvs::VertexBufferObject& vbo )
{
    if ( m_particles == NULL ) return( false );

    const bool has_index = m_particles->hasIndex();
    const bool has_normal = m_particles->hasNormal();

    const size_t start = m_start;
    size_t end = m_start + m_count;
    if ( end > m_particles->nvertices() ) end = m_particles->nvertices(); // end vertex number + 1

    const size_t size = end - start;  // number of vertices
    const size_t size_i = has_index ? sizeof(ParticleBasedRenderer::IndexType) * 2 * size : 0;
    const size_t size_v = sizeof(ParticleBasedRenderer::CoordType)  * 3 * size;
    const size_t size_n = has_normal ? sizeof(ParticleBasedRenderer::NormalType) * 3 * size : 0;
    const size_t size_c = sizeof(ParticleBasedRenderer::ColorType)  * 3 * size;
    const size_t off_i = 0;
    const size_t off_v = off_i + size_i;
    const size_t off_n = off_v + size_v;
    const size_t off_c = off_n + size_n;

    const ParticleBasedRenderer::IndexType*  ptr_i = m_particles->indices()  + start * 2;
    const ParticleBasedRenderer::CoordType*  ptr_v = m_particles->coords()  + start * 3;
    const ParticleBasedRenderer::NormalType* ptr_n = has_normal ? (m_particles->normals() + start * 3) : NULL;
    const ParticleBasedRenderer::ColorType*  ptr_c = m_particles->colors()  + start * 3;

    if ( has_index ) vbo.load( size_i, ptr_i, off_i );
    vbo.load( size_v, ptr_v, off_v );
    if ( has_normal ) vbo.load( size_n, ptr_n, off_n );
    vbo.load( size_c, ptr_c, off_c );

    m_off_index  = off_i;
    m_off_coord  = off_v;
    m_off_normal = off_n;
    m_off_color  = off_c;

    return( size );
}

/*===========================================================================*/
/**
 *  @brief  Draws the particles.
 */
/*===========================================================================*/
void ParticleBasedRenderer::Renderer::draw() const
{
#if defined( KVS_GLSL_RITS_PARTICLE_BASED_RENDERER__NORMAL_TYPE_IS_FLOAT )
    GLenum normal_type = GL_FLOAT;
#elif defined( KVS_GLSL_RITS_PARTICLE_BASED_RENDERER__NORMAL_TYPE_IS_BYTE )
    GLenum normal_type = GL_BYTE;
#else
#error "KVS_GLSL_RITS_PARTICLE_BASED_RENDERER__NORMAL_TYPE_IS_* is not defined."
#endif
    KVS_GL_CALL( glEnableClientState(GL_VERTEX_ARRAY) );
    KVS_GL_CALL( glVertexPointer(3, GL_FLOAT, 0, (char*)(m_off_coord)) );

    KVS_GL_CALL( glEnableClientState(GL_COLOR_ARRAY) );
    KVS_GL_CALL( glColorPointer(3, GL_UNSIGNED_BYTE, 0, (char*)(m_off_color)) );

    if ( m_particles->hasNormal() )
    {
        KVS_GL_CALL( glEnableClientState(GL_NORMAL_ARRAY) );
        KVS_GL_CALL( glNormalPointer( normal_type, 0, (char*)(m_off_normal)) );
    }

    if ( m_particles->hasIndex() )
    {
        KVS_GL_CALL( glEnableVertexAttribArray( m_loc_identifier ) );
        KVS_GL_CALL( glVertexAttribPointer( m_loc_identifier, 2, GL_UNSIGNED_SHORT, GL_FALSE, 0, (void*)(m_off_index)) );
    }

    if(rendering_process::division_mode){
        KVS_GL_CALL( glDrawArrays(GL_POINTS, rendering_process::start_pos, rendering_process::used_points) );
    }
    else {
        KVS_GL_CALL( glDrawArrays(GL_POINTS, 0, static_cast<size_t>(static_cast<double>(m_count)*rendering_process::used_point_ratio)) );
    }

    KVS_GL_CALL( glDisableClientState(GL_VERTEX_ARRAY) );
    KVS_GL_CALL( glDisableClientState(GL_NORMAL_ARRAY) );
    KVS_GL_CALL( glDisableClientState(GL_COLOR_ARRAY) );


    if ( m_particles->hasIndex() )
    {
        KVS_GL_CALL( glDisableVertexAttribArray( m_loc_identifier ) );
    }
}

/*===========================================================================*/
/**
 *  @brief  Returns the number of vertices.
 *  @return number of vertices
 */
/*===========================================================================*/
size_t ParticleBasedRenderer::Renderer::nvertices() const
{
    return( m_count );
}

/*===========================================================================*/
/**
 *  @brief  Returns the byte size per vertex.
 *  @return byte size per vertex
 */
/*===========================================================================*/
size_t ParticleBasedRenderer::Renderer::byteSizePerVertex() const
{
    return( m_particles->byteSizePerVertex() );
}

/*===========================================================================*/
/**
 *  @brief  Returns the total byte size of the vertices.
 *  @return total byte size of the vertices
 */
/*===========================================================================*/
size_t ParticleBasedRenderer::Renderer::byteSize() const
{
    return( this->byteSizePerVertex() * m_count );
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new ParticleBasedRenderer class.
 */
/*===========================================================================*/
ParticleBasedRenderer::ParticleBasedRenderer():
    m_ref_point( NULL )
{
    BaseClass::setShader( kvs::Shader::Lambert() );
    this->initialize();
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new ParticleBasedRenderer class.
 *  @param  point [in] pointer to the point object
 *  @param  subpixel_level [in] subpixel level
 *  @param  repeat_level [i] repetition level
 */
/*===========================================================================*/
ParticleBasedRenderer::ParticleBasedRenderer(
    kvs::PointObject* point,
    const size_t subpixel_level,
    const size_t repeat_level ):
    m_ref_point( NULL )
{
    BaseClass::setShader( kvs::Shader::Lambert() );

    this->initialize();
    this->attachPointObject( point );
    this->setSubpixelLevel( subpixel_level );
    this->setRepetitionLevel( repeat_level );
}

/*===========================================================================*/
/**
 *  @brief  Destructs the ParticleBasedRenderer class.
 */
/*===========================================================================*/
ParticleBasedRenderer::~ParticleBasedRenderer()
{
    delete [] m_vbo;
    delete [] m_particles;
    delete [] m_renderer;
}

/*==========================================================================*/
/**
 *  @brief  Main rendering routine.
 *  @param  object [i] pointer to the render object (not used. using ctor argument "point")
 *  @param  camera [i] pointer to the camera information (not used. using OpenGL setting)
 *  @param  light [i] pointer to the light information (not used. using OpenGL setting)
 */
/*==========================================================================*/
void ParticleBasedRenderer::exec(
    kvs::ObjectBase* object,
    kvs::Camera*     camera,
    kvs::Light*      light )
{
    kvs::IgnoreUnusedVariable( light );

    kvs::PointObject* point = kvs::PointObject::DownCast( object );
    if ( !m_ref_point ) this->attachPointObject( point );

    if ( point->normals().size() == 0 ) BaseClass::disableShading();

    BaseClass::startTimer();
    this->create_image( point, camera, light );
    BaseClass::stopTimer();
}

/*===========================================================================*/
/**
 *  @brief  Initializes the member parameters.
 */
/*===========================================================================*/
void ParticleBasedRenderer::initialize()
{
    BaseClass::setWindowSize( 0, 0 );

    m_subpixel_level = 1;
    m_repetition_level = 1;
    m_coarse_level = 1;
    m_zooming_factor = 1.0f;
    m_random_texture_size = 1024;
    m_circle_threshold = 3;
    m_lower_limit_level = 10;//ADD_UEMURA

    m_render_width = 0;
    m_render_height = 0;

    m_enable_pre_downloading = true;
    m_enable_accumulation_buffer = false;
    m_enable_random_texture = true;
    m_enable_repetition_level_zooming = false;//ADD_UEMURA

    m_vbo = NULL;
    m_particles = NULL;
    m_renderer = NULL;

    m_enable_lod = true;
    m_enable_zooming = true;//ADD_UEMURA
    m_enable_shuffle = true;//ADD_UEMURA

    memset( m_modelview_matrix, 0, sizeof( m_modelview_matrix ) );
}

/*===========================================================================*/
/**
 *  @brief  Attaches the point object.
 *  @param  point [in] pointer to the point object
 */
/*===========================================================================*/
void ParticleBasedRenderer::attachPointObject( const kvs::PointObject* point )
{
    m_ref_point = point;

    if ( m_ref_point->normals().data() == NULL ||
         m_ref_point->numberOfNormals() == 0 )
    {
        BaseClass::disableShading();
    }
}

/*===========================================================================*/
/**
 *  @brief  Sets a subpixel level.
 *  @param  subpixel_level [in] subpixel level
 */
/*===========================================================================*/
void ParticleBasedRenderer::setSubpixelLevel( const size_t subpixel_level )
{
    m_subpixel_level = subpixel_level;
}

/*===========================================================================*/
/**
 *  @brief  Sets a repetition level
 *  @param  repetition_level [in] repetition level
 */
/*===========================================================================*/
void ParticleBasedRenderer::setRepetitionLevel( const size_t repetition_level )
{
    m_repetition_level = repetition_level;
    m_coarse_level = repetition_level;
}

/*===========================================================================*/
/**
 *  @brief  Sets a random texture size.
 *  @param  random_texture_size [in] random texture size
 */
/*===========================================================================*/
void ParticleBasedRenderer::setRandomTextureSize( const size_t random_texture_size )
{
    /* For the random number texture, random_texture_size * random_texture_size
     * bytes is allocated on GPU.
     */
    m_random_texture_size = random_texture_size;
}


/*===========================================================================*/
/**
 *  @brief  Sets a circle threshold.
 *  @param  circle_threshold [in] threshold to draw a primitive as circle or square
 */
/*===========================================================================*/
void ParticleBasedRenderer::setCircleThreshold( const size_t circle_threshold )
{
    /* If the given threshold is larger than the calculated particle size,
     * the particle is drawn as circle. Otherwse, the particle is drawn as
     * square. In case that the threshold is zero, all of the particles are
     * drawn as square.
     */
    m_circle_threshold = circle_threshold;
}

/*===========================================================================*/
/**
 *  @brief  Sets a lower limit of repetition level for repeat level zooming.
 *  @param  lower_limit_level [in] lower limit of repetition level for repeat level zooming
 */
/*===========================================================================*/
//ADD_UEMURA
void ParticleBasedRenderer::setMinRepetitionLevelInZooming(const size_t lower_limit_level )
{
    // Set a minimum value of the decreasing lepeat level in zooming up
    m_lower_limit_level = lower_limit_level;

}

void ParticleBasedRenderer::enableLODControl( const size_t coarse_level )
{
    m_enable_lod = true;
    this->enableCoarseRendering( coarse_level );
}

void ParticleBasedRenderer::disableLODControl()
{
    m_enable_lod = false;
    this->disableCoarseRendering();
}

void ParticleBasedRenderer::enableZooming()
{
    m_enable_zooming = true;
}
//ADD_UEMURA
void ParticleBasedRenderer::enableRepetitionLevelZooming()
{
    m_enable_repetition_level_zooming = true;
}
//ADD_UEMURA
void ParticleBasedRenderer::enableShuffle()
{
    m_enable_shuffle = true;
}

void ParticleBasedRenderer::disableZooming()
{
    m_enable_zooming = false;
}
//ADD_UEMURA
void ParticleBasedRenderer::disableRepetitionLevelZooming()
{
    m_enable_repetition_level_zooming = false;
}
//ADD_UEMURA
void ParticleBasedRenderer::disableShuffle()
{
    m_enable_shuffle = false;
}
/*===========================================================================*/
/**
 *  @brief  Coarse rendering.
 *  @param  coarse_level [in] coarse repetition level (default: 1)
 */
/*===========================================================================*/
void ParticleBasedRenderer::enableCoarseRendering( const size_t coarse_level )
{
    m_coarse_level = coarse_level;
}

/*===========================================================================*/
/**
 *  @brief  Fine rendering.
 */
/*===========================================================================*/
void ParticleBasedRenderer::disableCoarseRendering()
{
    m_coarse_level = m_repetition_level;
}

/*===========================================================================*/
/**
 *  @brief  Enables accumulation buffer.
 */
/*===========================================================================*/
void ParticleBasedRenderer::enableAccumulationBuffer()
{
    m_enable_accumulation_buffer = true;
}

/*===========================================================================*/
/**
 *  @brief  Disables accumulation buffer.
 */
/*===========================================================================*/
void ParticleBasedRenderer::disableAccumulationBuffer()
{
    m_enable_accumulation_buffer = false;
}

/*===========================================================================*/
/**
 *  @brief  Enables random number texture.
 */
/*===========================================================================*/
void ParticleBasedRenderer::enableRandomTexture()
{
    m_enable_random_texture = true;
}

/*===========================================================================*/
/**
 *  @brief  Disables random number texture.
 */
/*===========================================================================*/
void ParticleBasedRenderer::disableRandomTexture()
{
    m_enable_random_texture = false;
}

/*===========================================================================*/
/**
 *  @brief  Returns the subpixel level.
 *  @return subpixel level
 */
/*===========================================================================*/
size_t ParticleBasedRenderer::subpixelLevel() const
{
    return( m_subpixel_level );
}

/*===========================================================================*/
/**
 *  @brief  Returns the repetition level.
 *  @return repetition level
 */
/*===========================================================================*/
size_t ParticleBasedRenderer::repetitionLevel() const
{
    return( m_repetition_level );
}

/*===========================================================================*/
/**
 *  @brief  Returns the random texture size.
 *  @return random texture size
 */
/*===========================================================================*/
size_t ParticleBasedRenderer::randomTextureSize() const
{
    return( m_random_texture_size );
}

/*===========================================================================*/
/**
 *  @brief  Returns the circle threshold.
 *  @return circle threshold
 */
/*===========================================================================*/
size_t ParticleBasedRenderer::circleThreshold() const
{
    return( m_circle_threshold );
}

/*===========================================================================*/
/**
 *  @brief  Tests whether the accumulation buffer is enabled or disabled.
 *  @return true, if the accumulation buffer is enabled
 */
/*===========================================================================*/
bool ParticleBasedRenderer::isEnabledAccumulationBuffer() const
{
    return( m_enable_accumulation_buffer );
}

/*===========================================================================*/
/**
 *  @brief  Tests whether the random number texture is enabled or disabled.
 *  @return true, if the random number texture is enabled
 */
/*===========================================================================*/
bool ParticleBasedRenderer::isEnabledRandomTexture() const
{
    return( m_enable_random_texture );
}

/*===========================================================================*/
/**
 *  @brief  Creates an image.
 *  @param  point [in] pointer to the point object
 *  @param  camera [in] pointer to the camera
 *  @param  light [in] pointer to the light
 */
/*===========================================================================*/

void ParticleBasedRenderer::create_image(
    const kvs::PointObject* point,
    const kvs::Camera*      camera,
    const kvs::Light*       light )
{
    kvs::IgnoreUnusedVariable( light );

    rendering_process::enable_hybrid_zooming = m_enable_repetition_level_zooming && m_enable_zooming;

    // Set shader initial parameters.
    //BaseClass::shader().set( camera, light );

    kvs::OpenGL::WithPushedAttrib attrib( GL_CURRENT_BIT | GL_ENABLE_BIT | GL_LIGHTING_BIT );

    /*ADD_UEMURA(begin)*/
    static const float default_window_size = static_cast<float>(camera->windowHeight());
    static float scaled_window_size = static_cast<float>(camera->windowHeight());
    /*ADD_UEMURA(end)*/

    // Following processes are executed once.
    if ( BaseClass::windowWidth() == 0 && BaseClass::windowHeight() == 0 )
    {
        if ( m_enable_accumulation_buffer )
        {
            m_ensemble_buffer.enableAccumulationBuffer();
        }
        else
        {
            m_ensemble_buffer.disableAccumulationBuffer();
        }

        this->initialize_opengl();
        this->align_particles();
        this->create_vertexbuffer();
        this->calculate_zooming_factor( point, camera );

        kvs::OpenGL::GetFloatv( GL_MODELVIEW_MATRIX, m_modelview_matrix );
    }

    // Get the modelview matrix.
    float modelview_matrix[16];
    kvs::OpenGL::GetFloatv( GL_MODELVIEW_MATRIX, modelview_matrix );

    // LOD control.
    size_t coarse_level=0;
    if ( m_enable_lod )
    {
        float modelview_matrix[16];
        kvs::OpenGL::GetFloatv( GL_MODELVIEW_MATRIX, modelview_matrix );

        for ( size_t i = 0; i < 16; i++ )
        {
            if ( m_modelview_matrix[i] != modelview_matrix[i] )
            {
                coarse_level = m_coarse_level;
                break;
            }
        }
        memcpy( m_modelview_matrix, modelview_matrix, sizeof( modelview_matrix ) );
    }

    // Following processes are executed when the window size is changed.
    if ( ( BaseClass::windowWidth()  != camera->windowWidth() ) ||
         ( BaseClass::windowHeight() != camera->windowHeight() ) )

    {
        scaled_window_size = static_cast<float>( camera->windowHeight() );//ADD_UEMURA
        BaseClass::setWindowSize( camera->windowWidth(), camera->windowHeight() );

        m_render_width  = BaseClass::windowWidth()  * m_subpixel_level;
        m_render_height = BaseClass::windowHeight() * m_subpixel_level;

        if ( coarse_level != 1 )
        {
            m_ensemble_buffer.create( BaseClass::windowWidth(), BaseClass::windowHeight() );
        }

        this->initialize_resize_texture();
    }

    /*ADD_UEMURA(begin)*/
    // Distance parameter: (d_0/d)^2
    //   d_0: Initial camera distance, d: current camera distance
    rendering_process::object_center        = point->objectCenter();
    rendering_process::object_magnification = static_cast<float>( point->xform().scaling().x() * rendering_process::default_camera_distance /camera->position().length() );
    rendering_process::screen_magnification =  scaled_window_size / default_window_size;
    double distance_param                   = (rendering_process::enable_hybrid_zooming)?  rendering_process::object_magnification * rendering_process::screen_magnification: 
                                              rendering_process::object_magnification * rendering_process::object_magnification *
                                              rendering_process::screen_magnification * rendering_process::screen_magnification;
    double used_point_ratio                 = distance_param;
    size_t Lr                               = static_cast<size_t>( 1.0 / distance_param*m_repetition_level );

    if(used_point_ratio > 1 || !m_enable_repetition_level_zooming) used_point_ratio = 1.0;
    rendering_process::used_point_ratio = used_point_ratio;

    if(Lr <= m_lower_limit_level) Lr = m_lower_limit_level;
    if(Lr > m_repetition_level || !m_enable_repetition_level_zooming) Lr = m_repetition_level;
    /*ADD_UEMURA(end)*/
    
    if ( coarse_level > 1 ) kvs::OpenGL::Clear( GL_ACCUM_BUFFER_BIT );

    if ( !m_enable_pre_downloading )
    {
        for ( size_t rp = 0; rp < ::VBOInterleave && rp < m_repetition_level; rp++ )
        {
            // download next vertex buffer object.
            // for example, if vbo_share_interlave is 2,
            // first  vertices list(rp=0) is downloaded to GPU m_vbo[0].
            // second vertices list(rp=1) is downloaded to GPU m_vbo[1].
            this->download_vertexbuffer( m_renderer[rp], m_vbo[rp] );
        }
    }

    // Enable or disable OpenGL capabilities.
    if ( BaseClass::isEnabledShading() ) kvs::OpenGL::Enable( GL_LIGHTING );
    else kvs::OpenGL::Disable( GL_LIGHTING );
    kvs::OpenGL::Enable( GL_DEPTH_TEST );
    kvs::OpenGL::Enable( GL_VERTEX_PROGRAM_POINT_SIZE ); // enable zooming.

    // Project particles.
    const size_t repeat_count = (m_coarse_level == coarse_level)? coarse_level : Lr;//ADD_UEMURA
    const bool enable_averaging = repeat_count > 1;
    const bool enable_resizing = m_subpixel_level > 1 || ( enable_averaging && !m_enable_accumulation_buffer );
    m_ensemble_buffer.clear();

    /*ADD_UEMURA(begin)*/
    // Size of a remainder array
    // r_count                     : (L_R (org) - L_R (cur))/L_R(cur)
    // current_r_pos               : Current position in the (set of) remainder arrays during their projection
    // flag_enable_next_buffer_use : Flag that becomes true if the next remainder array is also used for rendering
    // flag_not_move_and_zoom_on   : Flag that becomes true if the repetition-zomm is on and in non-moving state
    // num_remainder_points        : Number of particles used in the remainder-array projection
    const float r_count              = static_cast<float>(m_repetition_level - Lr)/static_cast<float>(Lr);
    float current_r_pos              = 0;
    rendering_process::start_pos     = 0;
    rendering_process::used_points   = 0;
    bool flag_enable_next_buffer_use = 0;
    bool flag_not_move_and_zoom_on   = repeat_count > 1 && r_count > 0; 
    size_t num_remainder_points      = static_cast<size_t>(static_cast<float>(m_ref_point->numberOfVertices() / m_repetition_level) * r_count);
    /*ADD_UEMURA(end)*/

    // Create image ensembles
    for ( size_t rp = 0; rp < repeat_count; rp++ )
    {
        if ( enable_resizing )
        {
            // Render to the texture.
            m_resize_framebuffer.bind();
            kvs::OpenGL::SetViewport( 0, 0, m_render_width, m_render_height );
        }

        kvs::OpenGL::Clear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
        kvs::OpenGL::Enable( GL_DEPTH_TEST );

        if ( m_enable_pre_downloading )
        {
            rendering_process::division_mode = 0;
            this->draw_vertexbuffer( m_renderer[rp], m_vbo[rp], modelview_matrix );

            /*ADD_UEMURA(begin)*/
            // Start the remainder-array projection
            if( flag_not_move_and_zoom_on && r_count < 1.0 )
            {// Case that the number of remainder arrays are less than half 
             //  of the total number of arrays

                // Mode in which one array is devided and multiple renderings are executed.
                rendering_process::division_mode = 1; 
                if( !flag_enable_next_buffer_use )
                {//Normal case
                    rendering_process::used_points = num_remainder_points;

                    if(static_cast<int>(current_r_pos) + repeat_count < m_repetition_level + 1 )
                    {
                        // Remainder-array projection
                        this->draw_vertexbuffer( m_renderer[repeat_count + static_cast<size_t>(current_r_pos)], 
                                                 m_vbo[repeat_count + static_cast<size_t>(current_r_pos)], 
                                                 modelview_matrix );
                    }

                    current_r_pos += r_count;

                }
                else if( flag_enable_next_buffer_use )
                {// Just after the previous remainder-array is rendered
                    flag_enable_next_buffer_use = 0;

                    // Particle projection to supplement the previous remainder-array projection
                    rendering_process::used_points = num_remainder_points - rendering_process::used_points;

                    if(rp != repeat_count - 1  )
                    {
                        this->draw_vertexbuffer( m_renderer[repeat_count + static_cast<size_t>(current_r_pos)], 
                                                 m_vbo[repeat_count + static_cast<size_t>(current_r_pos)], 
                                                 modelview_matrix );
                    }

                } 

                rendering_process::start_pos += rendering_process::used_points;

                // Case that the remaining number of paticles in the current remainder array
                //  is insufficient
                if( static_cast<size_t>(current_r_pos + r_count) > static_cast<size_t>(current_r_pos) )
                {
                    flag_enable_next_buffer_use = 1;

                    // Project all particles in the current remainder array, and 
                    //  supplement particles later in the next loop (next remainder array) 
                    rendering_process::used_points = m_ref_point->numberOfVertices() / m_repetition_level - rendering_process::start_pos;
                    this->draw_vertexbuffer( m_renderer[repeat_count + static_cast<size_t>(current_r_pos)], 
                                             m_vbo[repeat_count + static_cast<size_t>(current_r_pos)], 
                                             modelview_matrix                                               );
                    rendering_process::start_pos = 0;
                    current_r_pos += r_count;
                }
            }
            else
            {// Case that the number of remainder arrays are more than half 
             //  of the total number of arrays


                if( flag_not_move_and_zoom_on )
                {
                    float tmp_pos = current_r_pos;
                    current_r_pos += r_count;

                    for(size_t i = static_cast<size_t>(tmp_pos) ; i < static_cast<size_t>(current_r_pos) ; i++ )
                    {
                        this->draw_vertexbuffer( m_renderer[repeat_count + i ],
                                                 m_vbo[repeat_count + i ], 
                                                 modelview_matrix               );

                    }

                    if(rp + 1 == repeat_count && (rp+1) * r_count < repeat_count)
                    { //After the final array is projected, remaining particles are projected at the end.
                        current_r_pos += 1;
                        this->draw_vertexbuffer( m_renderer[repeat_count + static_cast<size_t>(current_r_pos)], 
                                                 m_vbo[repeat_count + static_cast<size_t>(current_r_pos)], 
                                                 modelview_matrix );
                    }

                } 

            }
            /*ADD_UEMURA(end)*/
        }
        else
        {
            const size_t no_vbo = rp % ::VBOInterleave;
            this->draw_vertexbuffer( m_renderer[rp], m_vbo[no_vbo], modelview_matrix );
            const size_t nextrp = rp + ::VBOInterleave;
            if ( nextrp < repeat_count )
            {
                // Download the next vertex buffer object.
                this->download_vertexbuffer( m_renderer[nextrp], m_vbo[no_vbo] );
            }

        } // else



        if ( enable_resizing )
        {
            m_resize_framebuffer.unbind(); // render to the screen
            kvs::OpenGL::SetViewport( 0, 0, BaseClass::windowWidth(), BaseClass::windowHeight() );

            if ( enable_averaging )
            {
                m_ensemble_buffer.bind();
            }

            kvs::OpenGL::SetMatrixMode( GL_MODELVIEW );  kvs::OpenGL::PushMatrix(); kvs::OpenGL::LoadIdentity();
            kvs::OpenGL::SetMatrixMode( GL_PROJECTION ); kvs::OpenGL::PushMatrix(); kvs::OpenGL::LoadIdentity();
            kvs::OpenGL::SetOrtho( 0, 1, 0, 1, -1, 1 );

            m_resize_texture.bind();
            if ( m_subpixel_level > 1 )
            {
                m_resize_shader.bind();
                this->setup_resize_shader();
            }

            kvs::OpenGL::Disable( GL_DEPTH_TEST );
            kvs::OpenGL::Disable( GL_LIGHTING );
            KVS_GL_CALL_BEG( glBegin( GL_QUADS ) );
            KVS_GL_CALL_VER( glColor4f( 1.0, 1.0, 1.0, m_ensemble_buffer.opacity() ) );
            KVS_GL_CALL_VER( glTexCoord2f( 1, 1 ) ); KVS_GL_CALL_VER( glVertex2f( 1, 1 ) );
            KVS_GL_CALL_VER( glTexCoord2f( 0, 1 ) ); KVS_GL_CALL_VER( glVertex2f( 0, 1 ) );
            KVS_GL_CALL_VER( glTexCoord2f( 0, 0 ) ); KVS_GL_CALL_VER( glVertex2f( 0, 0 ) );
            KVS_GL_CALL_VER( glTexCoord2f( 1, 0 ) ); KVS_GL_CALL_VER( glVertex2f( 1, 0 ) );
            KVS_GL_CALL_END( glEnd() );

            if ( BaseClass::isEnabledShading() ) kvs::OpenGL::Enable( GL_LIGHTING );
            kvs::OpenGL::Enable( GL_DEPTH_TEST );

            if ( m_subpixel_level > 1 )
            {
                m_resize_shader.unbind();
                m_resize_texture.unbind();
            }

            kvs::OpenGL::PopMatrix(); // Pop PROJECTION matrix
            kvs::OpenGL::SetMatrixMode( GL_MODELVIEW );
            kvs::OpenGL::PopMatrix(); // Pop MODELVIEW matrix

        }
        if ( enable_averaging ) m_ensemble_buffer.add();

    }

    if ( enable_averaging ) m_ensemble_buffer.draw();

    kvs::OpenGL::Finish();
} 

/*==========================================================================*/
/**
 *  @brief  Initialize OpenGL.
 */
/*==========================================================================*/
void ParticleBasedRenderer::initialize_opengl()
{
/*
  GLuint mode = GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH;
  if ( m_repetition_level != 1 && m_enable_accumulation_buffer ) mode |= GLUT_ACCUM;
  glutInitDisplayMode( mode );
*/
    KVS_GL_CALL( glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST ) );

    if ( m_repetition_level > 1 ) KVS_GL_CALL( glClearAccum( 0.0, 0.0, 0.0, 0.0 ) );

    // Initialize the shader for zooming.
    {
        const std::string vert_code = "PBR_zooming_rits.vert";
        const std::string frag_code = "PBR_zooming.frag";

        kvs::ShaderSource vert( vert_code );
        kvs::ShaderSource frag( frag_code );

        if ( m_enable_random_texture ) vert.define("ENABLE_RANDOM_TEXTURE");
        if ( m_enable_zooming ) vert.define("ENABLE_ZOOMING");

        if ( BaseClass::isEnabledShading() )
        {
            switch ( BaseClass::shader().type() )
            {
            case kvs::Shader::LambertShading: frag.define("ENABLE_LAMBERT_SHADING"); break;
            case kvs::Shader::PhongShading: frag.define("ENABLE_PHONG_SHADING"); break;
            case kvs::Shader::BlinnPhongShading: frag.define("ENABLE_BLINN_PHONG_SHADING"); break;
            default: /* NO SHADING */ break;
            }
        }

        m_zoom_shader.create( vert, frag );

        m_loc_point_identifier = m_zoom_shader.attributeLocation("identifier");

        if ( BaseClass::isEnabledShading() )
        {
            m_zoom_shader.bind();
            switch ( BaseClass::shader().type() )
            {
            case kvs::Shader::LambertShading:
            {
                m_zoom_shader.setUniform( "shading.Ka", BaseClass::shader().Ka );
                m_zoom_shader.setUniform( "shading.Kd", BaseClass::shader().Kd );
                break;
            }
            case kvs::Shader::PhongShading:
            {
                m_zoom_shader.setUniform( "shading.Ka", BaseClass::shader().Ka );
                m_zoom_shader.setUniform( "shading.Kd", BaseClass::shader().Kd );
                m_zoom_shader.setUniform( "shading.Ks", BaseClass::shader().Ks );
                m_zoom_shader.setUniform( "shading.S",  BaseClass::shader().S );
                break;
            }
            case kvs::Shader::BlinnPhongShading:
            {
                m_zoom_shader.setUniform( "shading.Ka", BaseClass::shader().Ka );
                m_zoom_shader.setUniform( "shading.Kd", BaseClass::shader().Kd );
                m_zoom_shader.setUniform( "shading.Ks", BaseClass::shader().Ks );
                m_zoom_shader.setUniform( "shading.S",  BaseClass::shader().S );
                break;
            }
            default: /* NO SHADING */ break;
            }
            m_zoom_shader.unbind();
        }
    }


    // Inititalize the shader for resizing.
    {
        const std::string vert_code = "PBR_resize.vert";
        const std::string frag_code = "PBR_resize.frag";

        kvs::ShaderSource vert( vert_code );
        kvs::ShaderSource frag( frag_code );
        m_resize_shader.create( vert, frag );
    }

    // Inititlize the random number texture.
    if ( m_enable_random_texture )
    {
        this->create_random_texture();
    }
}

/*==========================================================================*/
/**
 *  @brief  Initialize subpixel rendering framebuffer.
 */
/*==========================================================================*/
void ParticleBasedRenderer::initialize_resize_texture()
{
    if ( m_subpixel_level <= 1 && ( m_repetition_level == 1 || m_enable_accumulation_buffer ) )
    {
        return;
    }

    // Release GPU resources for the resize texture and the resize depthbuffer.
    m_resize_texture.release();
    m_resize_depthbuffer.release();

    // Create framebuffer.
    m_resize_framebuffer.create();
    m_resize_framebuffer.bind();
    {
        // Set up the resize texture.
        m_resize_texture.setWrapS( GL_CLAMP_TO_EDGE );
        m_resize_texture.setWrapT( GL_CLAMP_TO_EDGE );
        m_resize_texture.setMagFilter( GL_LINEAR );
        m_resize_texture.setMinFilter( GL_LINEAR );
        m_resize_texture.setPixelFormat( GL_RGB8, GL_RGB, GL_UNSIGNED_BYTE );
        m_resize_texture.create( m_render_width, m_render_height );
        m_resize_framebuffer.attachColorTexture( m_resize_texture );

        // Set up the resize depthbuffer.
        m_resize_depthbuffer.setInternalFormat( GL_DEPTH_COMPONENT );
        m_resize_depthbuffer.create( m_render_width, m_render_height );
        m_resize_framebuffer.attachDepthRenderBuffer( m_resize_depthbuffer );
    }
    m_resize_framebuffer.unbind();
}

/*===========================================================================*/
/**
 *  @brief  Creates a random number texture.
 */
/*===========================================================================*/
void ParticleBasedRenderer::create_random_texture()
{
    const size_t npixels = m_random_texture_size * m_random_texture_size;
    kvs::MersenneTwister random;
    GLubyte* pixels = new GLubyte[ npixels ];
    for ( size_t i = 0; i < npixels; i++ )
    {
        pixels[i] = static_cast<GLubyte>( random.randInteger() );
    }

    m_random_texture.release();
    m_random_texture.setWrapS( GL_REPEAT );
    m_random_texture.setWrapT( GL_REPEAT );
    m_random_texture.setMagFilter( GL_NEAREST );
    m_random_texture.setMinFilter( GL_NEAREST );
    m_random_texture.setPixelFormat( GL_INTENSITY8,  GL_LUMINANCE, GL_UNSIGNED_BYTE  );
    m_random_texture.create( m_random_texture_size, m_random_texture_size, pixels );

    delete [] pixels;
}

/*==========================================================================*/
/**
 *  @brief  Align particles for each m_repetition_level.
 */
/*==========================================================================*/
void ParticleBasedRenderer::align_particles()
{
    if ( m_particles ) delete [] m_particles;
    m_particles = new Particles[ m_repetition_level ];

    const int total_vertices = m_ref_point->numberOfVertices();

    // Source pointers.
    /*ADD_UEMURA(begin)*/
    kvs::Real32* src_coord  = const_cast<kvs::Real32*>(m_ref_point->coords().data());
    kvs::Real32* src_normal = const_cast<kvs::Real32*>(m_ref_point->normals().data());
    kvs::UInt8*  src_color  = const_cast<kvs::UInt8*>(m_ref_point->colors().data());

    const bool has_color_array = m_ref_point->numberOfColors() == m_ref_point->numberOfVertices();
    const bool has_normal_array = m_ref_point->numberOfNormals() == m_ref_point->numberOfVertices();

    //Shuffle source coord array
    if(m_enable_shuffle){
        int* index_f = new int[total_vertices];
        for(int i = 0; i< total_vertices; i++){
            index_f[i] = i;
        }
        std::random_shuffle(index_f, index_f+total_vertices);

        for(int i = 0; i< total_vertices; i++){
            ::Swap(src_coord, i, index_f[i]);
            if(has_normal_array) ::Swap(src_normal, i, index_f[i]);
            if(has_color_array) ::Swap(src_color, i, index_f[i]);
        }

        delete[] index_f;
    }
    /*ADD_UEMURA(end)*/

    // Distination pointers.
    static const size_t max_repeat = 1024;
    IndexType*  dst_indices[max_repeat];
    ColorType*  dst_colors[max_repeat];
    CoordType*  dst_coords[max_repeat];
    NormalType* dst_normals[max_repeat];
    if ( m_repetition_level >= max_repeat )
    {
        kvsMessageError( "Repetition level is too large.");
    }

    // Allocate memory for each vertex array.
    size_t memorysize = 0;
    const size_t each_vertices = ( total_vertices - 1 ) / m_repetition_level + 1;
    const size_t overflow = each_vertices * m_repetition_level - total_vertices;
    for ( size_t rp = 0; rp < m_repetition_level; rp++ )
    {
        // first  [m_repetition_level - overflow] arrays have [each_vertices    ] vertices.
        // latter [                 overflow] arrays have [each_vertices - 1] vertices.
        size_t size = (rp < m_repetition_level - overflow) ?  each_vertices : each_vertices - 1;
        try
        {
            m_particles[rp].create( size, BaseClass::isEnabledShading(), m_enable_random_texture );
        }
        catch (...)
        {
            kvsMessageError( "Cannot allocate vertex memory.");
        }
        dst_coords [rp] = m_particles[rp].coords();
        dst_normals[rp] = m_particles[rp].normals();
        dst_colors [rp] = m_particles[rp].colors();
        dst_indices[rp] = m_particles[rp].indices();
        memorysize += m_particles[rp].byteSize();
    }

    // Distribute vertex into [m_repetition_level] arrays
    //    for ( size_t i = 0, rp = 0; i < total_vertices; i++ )
    size_t rp = 0 ; // TANAKA (This is to avoid warning.) 
    for ( int i = 0; i < total_vertices; i++ ) // TANAKA
    {
        *(dst_coords [rp])++ = *src_coord++;
        *(dst_coords [rp])++ = *src_coord++;
        *(dst_coords [rp])++ = *src_coord++;
        if ( BaseClass::isEnabledShading() )
        {
#if defined( KVS_GLSL_RITS_PARTICLE_BASED_RENDERER__NORMAL_TYPE_IS_FLOAT )
            // In case that the normal type of the particle is 'GLfloat'.
            *(dst_normals[rp])++ = (has_normal_array)? static_cast<NormalType>(*src_normal++) : static_cast<NormalType>(*src_normal);//ADD_UEMURA
            *(dst_normals[rp])++ = (has_normal_array)? static_cast<NormalType>(*src_normal++) : static_cast<NormalType>(*src_normal);//ADD_UEMURA
            *(dst_normals[rp])++ = (has_normal_array)? static_cast<NormalType>(*src_normal++) : static_cast<NormalType>(*src_normal);//ADD_UEMURA
#elif defined( KVS_GLSL_RITS_PARTICLE_BASED_RENDERER__NORMAL_TYPE_IS_BYTE )
            // In case that the normal type of the particle is 'GLbyte'.
            kvs::Vector3f v( src_normal );
            src_normal += 3;
            kvs::Vector3f n = v.normalized() * 127.0f;
            *(dst_normals[rp])++ = static_cast<NormalType>(n[0]);
            *(dst_normals[rp])++ = static_cast<NormalType>(n[1]);
            *(dst_normals[rp])++ = static_cast<NormalType>(n[2]);
#else
#error "KVS_GLSL_RITS_PARTICLE_BASED_RENDERER__NORMAL_TYPE_IS_* is not defined."
#endif
        }
        *(dst_colors [rp])++ = (has_color_array)? static_cast<ColorType>(*src_color++) : static_cast<ColorType>(*src_color);//ADD_UEMURA
        *(dst_colors [rp])++ = (has_color_array)? static_cast<ColorType>(*src_color++) : static_cast<ColorType>(*src_color);//ADD_UEMURA
        *(dst_colors [rp])++ = (has_color_array)? static_cast<ColorType>(*src_color++) : static_cast<ColorType>(*src_color);//ADD_UEMURA
        if ( dst_indices[rp] )
        {
#if 1
            unsigned int index = i * 12347;
            *(dst_indices[rp])++ = static_cast<IndexType>( index % m_random_texture_size );
            *(dst_indices[rp])++ = static_cast<IndexType>(( index / m_random_texture_size ) % m_random_texture_size);
#else
            *(dst_indices[rp])++ = static_cast<IndexType>(i);
#endif
        }
        if ( ++rp >= m_repetition_level ) rp = 0;
    }
}

/*==========================================================================*/
/**
 *  @brief  Creates Vertex Buffer Object for each repeatation.
 */
/*==========================================================================*/
void ParticleBasedRenderer::create_vertexbuffer()
{
    size_t memorysize = 0;
    if ( m_enable_pre_downloading )
    {
        for ( size_t rp = 0; rp < m_repetition_level; rp++ )
        {
            memorysize += m_particles[rp].byteSize();
        }
    }
    else
    {
        size_t count = ::VBOInterleave;
        if ( count > m_repetition_level ) count = m_repetition_level;
        memorysize  = count * m_particles[0].byteSize();
    }

    delete [] m_renderer; m_renderer = new Renderer[m_repetition_level];
    delete [] m_vbo;

    if ( m_enable_pre_downloading )
    {
        m_vbo = new kvs::VertexBufferObject[ m_repetition_level ];
    }
    else
    {
        m_vbo = new kvs::VertexBufferObject[ ::VBOInterleave ];
    }

    size_t memorymax = 0;
    for ( size_t rp = 0; rp < m_repetition_level; rp++)
    {
        const size_t nvertices  = m_particles[rp].nvertices();
        const size_t copysize = nvertices;
        m_renderer[rp].set( &m_particles[rp], 0, copysize, m_loc_point_identifier );
        const size_t memorysize = m_renderer[rp].byteSize();

        if ( m_enable_pre_downloading )
        {
            // if m_share_vbo is false, vertex buffer data is downloaded beforehand.
            m_vbo[rp].create( memorysize );
            m_vbo[rp].bind();
            m_renderer[rp].download( m_vbo[rp] );
            m_vbo[rp].unbind();
        }
        if ( memorymax < memorysize ) memorymax = memorysize;
    }

    if ( !m_enable_pre_downloading )
    {
        // if m_share_vbo is false, vertex buffer data is downloaded beforehand.
        for ( size_t i = 0; i < ::VBOInterleave && i < m_repetition_level; i++)
        {
            m_vbo[i].create(memorymax);
        }
    }
}

/*==========================================================================*/
/**
 *  @brief  Downloads the vertices into GPU.
 *  @param  renderer [in/out] rendering information
 *  @param  vbo [in] Vertex Buffer Object
 */
/*==========================================================================*/
void ParticleBasedRenderer::download_vertexbuffer(
    Renderer& renderer,
    kvs::VertexBufferObject& vbo )
{
    vbo.bind();
    renderer.download( vbo );
    vbo.unbind();
}

/*==========================================================================*/
/**
 *  @brief  Renders the vertices in VBO.
 *  @param  renderer [in] rendering information
 *  @param  vbo [in] Vertex Buffer Object
 *  @param  modelview_matrix [in] modelview matrix
 */
/*==========================================================================*/
void ParticleBasedRenderer::draw_vertexbuffer(
    const Renderer& renderer,
    kvs::VertexBufferObject& vbo,
    const float modelview_matrix[16] )
{
    vbo.bind();
    m_random_texture.bind();
    m_zoom_shader.bind();
    this->setup_zoom_shader( modelview_matrix );
    renderer.draw();
    m_zoom_shader.unbind();
    m_random_texture.unbind();
    vbo.unbind();
}

/*===========================================================================*/
/**
 *  @brief  Calculates the zooming factor.
 *  @param  point [in] pointer to the point object
 *  @param  camera [in] pointer to the camera
 */
/*===========================================================================*/
void ParticleBasedRenderer::calculate_zooming_factor( const kvs::PointObject* point, const kvs::Camera* camera )
{
    const float sqrt_repeat_level = sqrtf( static_cast<float>(m_repetition_level) );
    const float subpixel_level = m_subpixel_level * sqrt_repeat_level;
    const float subpixel_length =
        kvs::CellByCellParticleGenerator::CalculateSubpixelLength(
            subpixel_level, *camera, *point );

    m_zooming_factor = subpixel_length * sqrt_repeat_level;
}

/*==========================================================================*/
/**
 *  @brief  Setup several variables for the zoom shader.
 */
/*==========================================================================*/
void ParticleBasedRenderer::setup_zoom_shader( const float modelview_matrix[16] )
{
    float projection_matrix[16]; kvs::OpenGL::GetFloatv( GL_PROJECTION_MATRIX, projection_matrix );
    const float tan_inv = projection_matrix[5]; // (1,1)

    const float* mat = modelview_matrix;
    const float width = static_cast<float>( m_render_width );
    const float scale = sqrtf( mat[0] * mat[0] + mat[1] * mat[1] + mat[2] * mat[2] );
    const GLfloat densityFactor = m_zooming_factor * 0.5f * tan_inv * width * scale;
    const GLint   random_texture = 0;
    const GLfloat random_texture_size_inv = 1.0f / m_random_texture_size;
    const GLint   circle_threshold = static_cast<GLint>( m_circle_threshold );
//    const GLfloat screen_scale_x = m_render_width * 0.5f;
//    const GLfloat screen_scale_y = m_render_height * 0.5f;
    const kvs::Vector2f screen_scale( m_render_width * 0.5f, m_render_height * 0.5f );
    const int zoom_mode = static_cast<int>(m_enable_zooming) + static_cast<int>(m_enable_repetition_level_zooming)*2;//ADD_UEMURA

    m_zoom_shader.setUniform( "densityFactor", densityFactor );
    m_zoom_shader.setUniform( "random_texture", random_texture );
    m_zoom_shader.setUniform( "random_texture_size_inv", random_texture_size_inv );
    m_zoom_shader.setUniform( "circle_threshold", circle_threshold );
    /*ADD_UEMURA(begin)*/
    m_zoom_shader.setUniform( "zoom_mode", zoom_mode );
//    m_zoom_shader.setUniformValuef( "screen_scale", screen_scale_x, screen_scale_y );
    m_zoom_shader.setUniform( "screen_scale", screen_scale );
    m_zoom_shader.setUniform("object_magnification", rendering_process::object_magnification);
    m_zoom_shader.setUniform("object_center", rendering_process::object_center);
    m_zoom_shader.setUniform("screen_magnification", rendering_process::screen_magnification);
    /*ADD_UEMURA(end)*/
}

/*==========================================================================*/
/**
 *  @brief  Setup several variables for the resize shader.
 */
/*==========================================================================*/
void ParticleBasedRenderer::setup_resize_shader()
{
//    const GLfloat step_x = 1.0f / m_render_width;
//    const GLfloat step_y = 1.0f / m_render_height;
//    const GLfloat start_x = -step_x * ( ( m_subpixel_level - 1 ) * 0.5f );
//    const GLfloat start_y = -step_y * ( ( m_subpixel_level - 1 ) * 0.5f );

    const kvs::Vector2f step(
        1.0f / m_render_width,
        1.0f / m_render_height );
    const kvs::Vector2f start(
        -step.x() * ( ( m_subpixel_level - 1 ) * 0.5f ),
        -step.y() * ( ( m_subpixel_level - 1 ) * 0.5f ) );
    const kvs::Vector2i count(
        m_subpixel_level,
        m_subpixel_level );
    const GLfloat scale = 1.0f / ( m_subpixel_level * m_subpixel_level );
//    m_resize_shader.setUniformValuei( "texture", 0 );
//    m_resize_shader.setUniformValuef( "start", start_x, start_y );
//    m_resize_shader.setUniformValuef( "step", step_x,  step_y );
//    m_resize_shader.setUniformValuei( "count", m_subpixel_level, m_subpixel_level );
//    m_resize_shader.setUniformValuef( "scale", 1.0f / ( m_subpixel_level * m_subpixel_level ) );
    m_resize_shader.setUniform( "texture", 0 );
    m_resize_shader.setUniform( "start", start );
    m_resize_shader.setUniform( "step", step );
    m_resize_shader.setUniform( "count", count );
    m_resize_shader.setUniform( "scale", scale );
}

} // end of rits

} // end of glsl

} // end of kvs
