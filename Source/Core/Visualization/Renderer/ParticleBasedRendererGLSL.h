/*****************************************************************************/
/**
 *  @file   ParticleBasedRendererGLSL.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#pragma once
#include <kvs/RendererBase>
#include <kvs/Module>
#include <kvs/ProgramObject>
#include <kvs/VertexBufferObjectManager>
#include <kvs/Vector4>
#include <kvs/Matrix44>
#include <kvs/Deprecated>
#include "StochasticRendererBase.h"
#include "StochasticRenderingEngine.h"


namespace kvs
{

class PointObject;

namespace glsl
{

/*===========================================================================*/
/**
 *  @brief  Particle-based renderer class.
 */
/*===========================================================================*/
class ParticleBasedRenderer : public kvs::StochasticRendererBase
{
    kvsModule( kvs::glsl::ParticleBasedRenderer, Renderer );
    kvsModuleBaseClass( kvs::StochasticRendererBase );

public:
    class Engine;

public:
    ParticleBasedRenderer();
    ParticleBasedRenderer( const kvs::Mat4& m, const kvs::Mat4& p, const kvs::Vec4& v );
    bool isShuffleEnabled() const;
    bool isZoomingEnabled() const;
    void setShuffleEnabled( const bool enable = true );
    void setZoomingEnabled( const bool enable = true );
    void enableShuffle();
    void enableZooming();
    void disableShuffle();
    void disableZooming();
    const kvs::Mat4& initialModelViewMatrix() const;
    const kvs::Mat4& initialProjectionMatrix() const;
    const kvs::Vec4& initialViewport() const;

public:
    // Deprecated method
    KVS_DEPRECATED( bool isEnabledShuffle() const ) { return this->isShuffleEnabled(); }
    KVS_DEPRECATED( bool isEnabledZooming() const ) { return this->isZoomingEnabled(); }
    KVS_DEPRECATED( void setEnabledShuffle( const bool enable ) ) { this->setShuffleEnabled( enable ); }
    KVS_DEPRECATED( void setEnabledZooming( const bool enable ) ) { this->setZoomingEnabled( enable ); }

private:
    // Not supported progressive refinement rendering.
    bool isRefinementEnabled() const;
    void enableRefinement();
    void disableRefinement();
};

/*===========================================================================*/
/**
 *  @brief  Engine class for particle-based renderer.
 */
/*===========================================================================*/
class ParticleBasedRenderer::Engine : public kvs::StochasticRenderingEngine
{
    using BaseClass = kvs::StochasticRenderingEngine;

public:
    class BufferObject
    {
    private:
        using Manager = kvs::VertexBufferObjectManager;
        size_t m_nmanagers = 0; ///< number of VBO managers (repeat level)
        Manager* m_managers = nullptr; ///< a set of VBO managers for each repetition
        bool m_enable_shuffle = false; ///< flag for shuffling particles
    public:
        BufferObject() = default;
        virtual ~BufferObject() { if ( m_managers ) { delete [] m_managers; m_nmanagers = 0; }  }
        Manager& manager( const size_t index ) { return m_managers[index]; }
        void release() { for ( size_t i = 0; i < m_nmanagers; ++i ) { m_managers[i].release(); } }
        void create( const kvs::ObjectBase* object, const size_t nmanagers );
        void draw( const kvs::ObjectBase* object, const size_t index );

        bool isShuffleEnabled() const { return m_enable_shuffle; }
        void setShuffleEnabled( const bool enable = true ) { m_enable_shuffle = enable; }
        void enableShuffle() { this->setShuffleEnabled( true ); }
        void disableShuffle() { this->setShuffleEnabled( false ); }
    };

    class RenderPass
    {
        using Parent = BaseClass;
    private:
        const Parent* m_parent; ///< reference to the engine
        BufferObject& m_buffer_object; ///< buffer object (reference)
        std::string m_vert_shader_file = "PBR_zooming.vert"; ///< vertex shader file
        std::string m_frag_shader_file = "PBR_zooming.frag"; ///< fragment shader file
        kvs::ProgramObject m_shader_program{}; ///< shader program
        bool m_enable_zooming = true; ///< flag for zooming particles
    public:
        RenderPass( BufferObject& buffer_object, Parent* parent ):
            m_parent( parent ), m_buffer_object( buffer_object ) {}
        virtual ~RenderPass() { this->release(); }
        BufferObject& bufferObject() { return m_buffer_object; }
        const std::string& vertexShaderFile() const { return m_vert_shader_file; }
        const std::string& fragmentShaderFile() const { return m_frag_shader_file; }
        kvs::ProgramObject& shaderProgram() { return m_shader_program; }
        void setVertexShaderFile( const std::string& file ) { m_vert_shader_file = file; }
        void setFragmentShaderFile( const std::string& file ) { m_frag_shader_file = file; }
        void setShaderFiles( const std::string& vert_file, const std::string& frag_file );
        virtual void release() { m_shader_program.release(); }
        virtual void create( const kvs::Shader::ShadingModel& model, const bool enable );
        virtual void update( const kvs::Shader::ShadingModel& model, const bool enable );
        virtual void setup( const kvs::Shader::ShadingModel& model );
        virtual void draw( const kvs::ObjectBase* object, const size_t index );

        bool isZoomingEnabled() const { return m_enable_zooming; }
        void setZoomingEnabled( const bool enable = true ) { m_enable_zooming = enable; }
        void enableZooming() { this->setZoomingEnabled( true ); }
        void disableZooming() { this->setZoomingEnabled( false ); }
    };

private:
    bool m_has_normal; ///< check flag for the normal array
    bool m_enable_shuffle; ///< flag for shuffling particles
    bool m_enable_zooming; ///< flag for zooming particles
    size_t m_random_index; ///< index used for refering the random texture
    kvs::Mat4 m_initial_modelview; ///< initial modelview matrix
    kvs::Mat4 m_initial_projection; ///< initial projection matrix
    kvs::Vec4 m_initial_viewport; ///< initial viewport
    float m_initial_object_depth; ///< initial object depth
    kvs::ProgramObject m_shader_program; ///< zooming shader program
    kvs::VertexBufferObjectManager* m_vbo_manager; ///< vertex buffer object managers for each repetition

public:
    Engine();
    Engine( const kvs::Mat4& m, const kvs::Mat4& p, const kvs::Vec4& v );
    virtual ~Engine();
    void release();
    void create( kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light );
    void update( kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light );
    void setup( kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light );
    void draw( kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light );

    bool isShuffleEnabled() const { return m_enable_shuffle; }
    bool isZoomingEnabled() const { return m_enable_zooming; }
    void setShuffleEnabled( const bool enable = true ) { m_enable_shuffle = enable; }
    void setZoomingEnabled( const bool enable = true ) { m_enable_zooming = enable; }
    void enableShuffle() { this->setShuffleEnabled( true ); }
    void enableZooming() { this->setZoomingEnabled( true ); }
    void disableShuffle() { this->setShuffleEnabled( false ); }
    void disableZooming() { this->setZoomingEnabled( false ); }
    const kvs::Mat4& initialModelViewMatrix() const { return m_initial_modelview; }
    const kvs::Mat4& initialProjectionMatrix() const { return m_initial_projection; }
    const kvs::Vec4& initialViewport() const { return m_initial_viewport; }

private:
    void create_shader_program();
    void create_buffer_object( const kvs::PointObject* point );

public:
    KVS_DEPRECATED( bool isEnabledShuffle() const ) { return this->isShuffleEnabled(); }
    KVS_DEPRECATED( bool isEnabledZooming() const ) { return this->isZoomingEnabled(); }
    KVS_DEPRECATED( void setEnabledShuffle( const bool enable ) ) { this->setShuffleEnabled( enable ); }
    KVS_DEPRECATED( void setEnabledZooming( const bool enable ) ) { this->setZoomingEnabled( enable ); }
};

} // end of namespace glsl

} // end of namespace kvs
