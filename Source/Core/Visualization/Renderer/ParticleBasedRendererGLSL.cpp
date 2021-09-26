/*****************************************************************************/
/**
 *  @file   ParticleBasedRenderer.cpp
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#include "ParticleBasedRendererGLSL.h"
#include <cmath>
#include <kvs/OpenGL>
#include <kvs/PointObject>
#include <kvs/Camera>
#include <kvs/Light>
#include <kvs/Assert>
#include <kvs/Math>
#include <kvs/MersenneTwister>
#include <kvs/Xorshift128>


namespace
{

/*===========================================================================*/
/**
 *  @brief  Returns shuffled array.
 *  @param  values [in] value array
 *  @param  seed   [in] seed value for random number generator
 */
/*===========================================================================*/
template <int Dim, typename T>
kvs::ValueArray<T> ShuffleArray( const kvs::ValueArray<T>& values, kvs::UInt32 seed )
{
    KVS_ASSERT( Dim > 0 );
    KVS_ASSERT( values.size() % Dim == 0 );

    kvs::Xorshift128 rng; rng.setSeed( seed );
    kvs::ValueArray<T> ret;
    if ( values.unique() ) { ret = values; }
    else { ret = values.clone(); }

    T* p = ret.data();
    size_t size = ret.size() / Dim;

    for ( size_t i = 0; i < size; ++i )
    {
        size_t j = rng.randInteger() % ( i + 1 );
        for ( int k = 0; k < Dim; ++k )
        {
            std::swap( p[ i * Dim + k ], p[ j * Dim + k ] );
        }
    }
    return ret;
}

}


namespace kvs
{

namespace glsl
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new ParticleBasedRenderer class.
 */
/*===========================================================================*/
ParticleBasedRenderer::ParticleBasedRenderer():
    StochasticRendererBase( new Engine() )
{
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new ParticleBasedRenderer class.
 *  @param  m [in] initial modelview matrix
 *  @param  p [in] initial projection matrix
 *  @param  v [in] initial viewport
 */
/*===========================================================================*/
ParticleBasedRenderer::ParticleBasedRenderer( const kvs::Mat4& m, const kvs::Mat4& p, const kvs::Vec4& v ):
    StochasticRendererBase( new Engine( m, p, v ) )
{
}

/*===========================================================================*/
/**
 *  @brief  Returns true if the particle shuffling is enabled
 *  @return true, if the shuffling is enabled
 */
/*===========================================================================*/
bool ParticleBasedRenderer::isShuffleEnabled() const
{
    return static_cast<const Engine&>( engine() ).isShuffleEnabled();
}

/*===========================================================================*/
/**
 *  @brief  Returns true if the particle zooming is enabled
 *  @return true, if the zooming is enabled
 */
/*===========================================================================*/
bool ParticleBasedRenderer::isZoomingEnabled() const
{
    return static_cast<const Engine&>( engine() ).isZoomingEnabled();
}

/*===========================================================================*/
/**
 *  @brief  Sets enable-flag for the particle shuffling.
 *  @param  enable [in] enable-flag
 */
/*===========================================================================*/
void ParticleBasedRenderer::setShuffleEnabled( const bool enable )
{
    static_cast<Engine&>( engine() ).setShuffleEnabled( enable );
}

/*===========================================================================*/
/**
 *  @brief  Sets enable-flag for the particle zooming.
 *  @param  enable [in] enable-flag
 */
/*===========================================================================*/
void ParticleBasedRenderer::setZoomingEnabled( const bool enable )
{
    static_cast<Engine&>( engine() ).setZoomingEnabled( enable );
}

/*===========================================================================*/
/**
 *  @brief  Enable the particle shuffling.
 */
/*===========================================================================*/
void ParticleBasedRenderer::enableShuffle()
{
    static_cast<Engine&>( engine() ).enableShuffle();
}

/*===========================================================================*/
/**
 *  @brief  Enable the particle zooming.
 */
/*===========================================================================*/
void ParticleBasedRenderer::enableZooming()
{
    static_cast<Engine&>( engine() ).enableZooming();
}

/*===========================================================================*/
/**
 *  @brief  Disable the particle shuffling.
 */
/*===========================================================================*/
void ParticleBasedRenderer::disableShuffle()
{
    static_cast<Engine&>( engine() ).disableShuffle();
}

/*===========================================================================*/
/**
 *  @brief  Disable the particle zooming.
 */
/*===========================================================================*/
void ParticleBasedRenderer::disableZooming()
{
    static_cast<Engine&>( engine() ).disableZooming();
}

/*===========================================================================*/
/**
 *  @brief  Returns the initial modelview matrix.
 *  @param  initial modelview matrix
 */
/*===========================================================================*/
const kvs::Mat4& ParticleBasedRenderer::initialModelViewMatrix() const
{
    return static_cast<const Engine&>( engine() ).initialModelViewMatrix();
}

/*===========================================================================*/
/**
 *  @brief  Returns the initial projection matrix.
 *  @param  initial projection matrix
 */
/*===========================================================================*/
const kvs::Mat4& ParticleBasedRenderer::initialProjectionMatrix() const
{
    return static_cast<const Engine&>( engine() ).initialProjectionMatrix();
}

/*===========================================================================*/
/**
 *  @brief  Returns the initial viewport.
 *  @param  initial viewport
 */
/*===========================================================================*/
const kvs::Vec4& ParticleBasedRenderer::initialViewport() const
{
    return static_cast<const Engine&>( engine() ).initialViewport();
}

void ParticleBasedRenderer::setVertexShaderFile( const std::string& file )
{
    static_cast<Engine&>( engine() ).setVertexShaderFile( file );
}

void ParticleBasedRenderer::setFragmentShaderFile( const std::string& file )
{
    static_cast<Engine&>( engine() ).setFragmentShaderFile( file );
}

void ParticleBasedRenderer::setShaderFiles( const std::string& vert_file, const std::string& frag_file )
{
    static_cast<Engine&>( engine() ).setShaderFiles( vert_file, frag_file );
}

void ParticleBasedRenderer::Engine::BufferObject::create(
    const kvs::ObjectBase* object,
    const size_t nmanagers )
{
    const auto* point = kvs::PointObject::DownCast( object );
    KVS_ASSERT( point->coords().size() == point->colors().size() );

    const bool has_normal = point->normals().size() > 0;
    auto coords = point->coords();
    auto colors = point->colors();
    auto normals = point->normals();
    if ( m_enable_shuffle )
    {
        kvs::UInt32 seed = 12345678;
        coords = ::ShuffleArray<3>( point->coords(), seed );
        colors = ::ShuffleArray<3>( point->colors(), seed );
        if ( has_normal ) { normals = ::ShuffleArray<3>( point->normals(), seed ); }
    }

    if ( !m_managers ) { delete [] m_managers; }
    m_nmanagers = nmanagers;
    m_managers = new Manager [ m_nmanagers ];

    const size_t nvertices = point->numberOfVertices();
    const size_t rem = nvertices % m_nmanagers;
    const size_t quo = nvertices / m_nmanagers;
    for ( size_t i = 0; i < m_nmanagers; i++ )
    {
        const size_t count = quo + ( i < rem ? 1 : 0 );
        const size_t first = quo * i + kvs::Math::Min( i, rem );

        Manager::VertexBuffer vertex_array;
        vertex_array.type = GL_FLOAT;
        vertex_array.size = count * sizeof( kvs::Real32 ) * 3;;
        vertex_array.dim = 3;
        vertex_array.pointer = coords.data() + first * 3;
        m_managers[i].setVertexArray( vertex_array );

        Manager::VertexBuffer color_array;
        color_array.type = GL_UNSIGNED_BYTE;
        color_array.size = count * sizeof( kvs::UInt8 ) * 3;
        color_array.dim = 3;
        color_array.pointer = colors.data() + first * 3;
        m_managers[i].setColorArray( color_array );

        if ( has_normal )
        {
            Manager::VertexBuffer normal_array;
            normal_array.type = GL_FLOAT;
            normal_array.size = count * sizeof( kvs::Real32 ) * 3;
            normal_array.dim = 3;
            normal_array.pointer = normals.data() + first * 3;
            m_managers[i].setNormalArray( normal_array );
        }

        m_managers[i].create();
    }
}

void ParticleBasedRenderer::Engine::BufferObject::draw(
    const kvs::ObjectBase* object,
    const size_t index )
{
    KVS_ASSERT( index < m_nmanagers );

    const auto* point = kvs::PointObject::DownCast( object );
    const size_t nvertices = point->numberOfVertices();
    const size_t rem = nvertices % m_nmanagers;
    const size_t quo = nvertices / m_nmanagers;
    const size_t count = quo + ( index < rem ? 1 : 0 );

    auto& manager = m_managers[index];
    kvs::VertexBufferObjectManager::Binder bind( manager );
    kvs::OpenGL::EnableVertexAttribArray( m_random_index );
    kvs::OpenGL::VertexAttribPointer( m_random_index, 2, GL_UNSIGNED_SHORT, GL_FALSE, 0, (GLubyte*)NULL + 0 );
    manager.drawArrays( GL_POINTS, 0, count );
    kvs::OpenGL::DisableVertexAttribArray( m_random_index );
}

void ParticleBasedRenderer::Engine::RenderPass::setShaderFiles(
    const std::string& vert_file,
    const std::string& frag_file )
{
    this->setVertexShaderFile( vert_file );
    this->setFragmentShaderFile( frag_file );
}

void ParticleBasedRenderer::Engine::RenderPass::create(
    const kvs::Shader::ShadingModel& model,
    const bool enable )
{
    kvs::ShaderSource vert( "PBR_zooming.vert" );
    kvs::ShaderSource frag( "PBR_zooming.frag" );

    if ( enable )
    {
        switch ( model.type() )
        {
        case kvs::Shader::LambertShading: frag.define("ENABLE_LAMBERT_SHADING"); break;
        case kvs::Shader::PhongShading: frag.define("ENABLE_PHONG_SHADING"); break;
        case kvs::Shader::BlinnPhongShading: frag.define("ENABLE_BLINN_PHONG_SHADING"); break;
        default: break; // NO SHADING
        }

        if ( model.two_side_lighting )
        {
            frag.define("ENABLE_TWO_SIDE_LIGHTING");
        }
    }

    if ( m_enable_zooming )
    {
        vert.define("ENABLE_PARTICLE_ZOOMING");
        frag.define("ENABLE_PARTICLE_ZOOMING");
    }

    m_shader_program.build( vert, frag );
}

void ParticleBasedRenderer::Engine::RenderPass::update(
    const kvs::Shader::ShadingModel& model,
    const bool enable )
{
    m_shader_program.release();
    this->create( model, enable );
}

void ParticleBasedRenderer::Engine::RenderPass::setup(
    const kvs::Shader::ShadingModel& model )
{
    kvs::ProgramObject::Binder bind( m_shader_program );
    m_shader_program.setUniform( "shading.Ka", model.Ka );
    m_shader_program.setUniform( "shading.Kd", model.Kd );
    m_shader_program.setUniform( "shading.Ks", model.Ks );
    m_shader_program.setUniform( "shading.S",  model.S );

    const auto M = kvs::OpenGL::ModelViewMatrix();
    const auto P = kvs::OpenGL::ProjectionMatrix();
    m_shader_program.setUniform( "ModelViewMatrix", M );
    m_shader_program.setUniform( "ProjectionMatrix", P );

    const auto size_inv = 1.0f / m_parent->randomTextureSize();
    m_shader_program.setUniform( "random_texture", 0 );
    m_shader_program.setUniform( "random_texture_size_inv", size_inv );

    auto random_index = m_shader_program.attributeLocation("random_index");
    m_buffer_object.setRandomIndex( random_index );
}

void ParticleBasedRenderer::Engine::RenderPass::draw(
    const kvs::ObjectBase* object,
    const size_t index )
{
    kvs::ProgramObject::Binder po( m_shader_program );
    kvs::Texture::Binder tex( m_parent->randomTexture() );
    m_buffer_object.draw( object, index );
}

/*===========================================================================*/
/**
 *  @brief  Create shaders, VBO, and framebuffers.
 *  @param  point [in] pointer to the point object
 *  @param  camera [in] pointer to the camera
 *  @param  light [in] pointer to the light
 */
/*===========================================================================*/
void ParticleBasedRenderer::Engine::create(
    kvs::ObjectBase* object,
    kvs::Camera* camera,
    kvs::Light* light )
{
    auto* point = kvs::PointObject::DownCast( object );
    BaseClass::attachObject( object );
    BaseClass::createRandomTexture();

    m_render_pass.create( BaseClass::shader(), BaseClass::isShadingEnabled() );
    m_buffer_object.create( object, BaseClass::repetitionLevel() );

    // Initial values for calculating the object depth.
    if ( kvs::Math::IsZero( m_initial_modelview[3][3] ) )
    {
        m_initial_modelview = kvs::OpenGL::ModelViewMatrix();
    }

    if ( kvs::Math::IsZero( m_initial_projection[3][3] ) )
    {
        m_initial_projection = kvs::OpenGL::ProjectionMatrix();
    }

    if ( kvs::Math::IsZero( m_initial_viewport[2] ) )
    {
        const float dpr = camera->devicePixelRatio();
        const float framebuffer_width = camera->windowWidth() * dpr;
        const float framebuffer_height = camera->windowHeight() * dpr;
        m_initial_viewport[2] = framebuffer_width;
        m_initial_viewport[3] = framebuffer_height;
    }

    const kvs::Vec4 I( point->objectCenter(), 1.0f );
    const kvs::Vec4 O = m_initial_projection * m_initial_modelview * I;
    m_initial_object_depth = O.z();
}

/*===========================================================================*/
/**
 *  @brief  Update.
 *  @param  point [in] pointer to the point object
 *  @param  camera [in] pointer to the camera
 *  @param  light [in] pointer to the light
 */
/*===========================================================================*/
void ParticleBasedRenderer::Engine::update(
    kvs::ObjectBase* object,
    kvs::Camera* camera,
    kvs::Light* light )
{
    m_render_pass.update( BaseClass::shader(), BaseClass::isShadingEnabled() );

    const float dpr = camera->devicePixelRatio();
    const float framebuffer_width = camera->windowWidth() * dpr;
    m_initial_viewport[2] = framebuffer_width;
}

/*===========================================================================*/
/**
 *  @brief  Setup.
 *  @param  point [in] pointer to the point object
 *  @param  camera [in] pointer to the camera
 *  @param  light [in] pointer to the light
 */
/*===========================================================================*/
void ParticleBasedRenderer::Engine::setup(
    kvs::ObjectBase* object,
    kvs::Camera* camera,
    kvs::Light* light )
{
    // The repetition counter must be reset here.
    BaseClass::resetRepetitions();

    m_render_pass.setup( BaseClass::shader() );

    const kvs::Mat4& m0 = m_initial_modelview;
    const float scale0 = kvs::Vec3( m0[0][0], m0[1][0], m0[2][0] ).length();
    const float width0 = m_initial_viewport[2];
    const float height0 = m_initial_viewport[3];

    const kvs::Mat4 m = kvs::OpenGL::ModelViewMatrix();
    const float scale = kvs::Vec3( m[0][0], m[1][0], m[2][0] ).length();
    const float dpr = camera->devicePixelRatio();
    const float width = camera->windowWidth() * dpr;
    const float height = camera->windowHeight() * dpr;

    const float Cr = ( width / width0 ) * ( height / height0 );
    const float Cs = scale / scale0;
    const float D0 = m_initial_object_depth;
    const float object_scale = Cr * Cs * dpr;
    const float object_depth = object_scale * D0;

    auto& shader_program = m_render_pass.shaderProgram();
    shader_program.bind();
    shader_program.setUniform( "object_scale", object_scale );
    shader_program.setUniform( "object_depth", object_depth );
    shader_program.setUniform( "screen_scale", kvs::Vec2( width * 0.5f, height * 0.5f ) );
    shader_program.unbind();
}

/*===========================================================================*/
/**
 *  @brief  Draw an ensemble.
 *  @param  point [in] pointer to the point object
 *  @param  camera [in] pointer to the camera
 *  @param  light [in] pointer to the light
 */
/*===========================================================================*/
void ParticleBasedRenderer::Engine::draw(
    kvs::ObjectBase* object,
    kvs::Camera* camera,
    kvs::Light* light )
{
    kvs::OpenGL::Enable( GL_DEPTH_TEST );
    kvs::OpenGL::Enable( GL_VERTEX_PROGRAM_POINT_SIZE );
    m_render_pass.draw( object, BaseClass::repetitionCount() );
}

} // end of glsl

} // end of kvs
