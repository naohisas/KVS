/*****************************************************************************/
/**
 *  @file   StochasticPolygonRenderer.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#pragma once
#include <kvs/Module>
#include <kvs/ProgramObject>
#include <kvs/PolygonRenderer>
#include <kvs/Deprecated>
#include "StochasticRenderingEngine.h"
#include "StochasticRendererBase.h"


namespace kvs
{

class PolygonObject;

/*===========================================================================*/
/**
 *  @brief  Stochastic polygon renderer class.
 */
/*===========================================================================*/
class StochasticPolygonRenderer : public kvs::StochasticRendererBase
{
    kvsModule( kvs::StochasticPolygonRenderer, Renderer );
    kvsModuleBaseClass( kvs::StochasticRendererBase );

public:
    class Engine;

public:
    StochasticPolygonRenderer();

    void setEdgeFactor( const float factor );
    void setDepthOffset( const kvs::Vec2& offset );
    void setDepthOffset( const float factor, const float units = 0.0f );
    void setVertexShaderFile( const std::string& file );
    void setFragmentShaderFile( const std::string& file );
    void setShaderFiles( const std::string& vert_file, const std::string& frag_file );

public:
    KVS_DEPRECATED( void setPolygonOffset( const float offset ) ) { this->setDepthOffset( offset ); }
};

/*===========================================================================*/
/**
 *  @brief  Engine class for stochastic polygon renderer.
 */
/*===========================================================================*/
class StochasticPolygonRenderer::Engine : public kvs::StochasticRenderingEngine
{
public:
    using BaseClass = kvs::StochasticRenderingEngine;
    using BufferObject = kvs::glsl::PolygonRenderer::BufferObject;

    class RenderPass : public kvs::glsl::PolygonRenderer::RenderPass
    {
    private:
        using BaseRenderPass = kvs::glsl::PolygonRenderer::RenderPass;
        using Parent = BaseClass;
        const Parent* m_parent; ///< reference to the engine
    public:
        RenderPass( BufferObject& buffer_object, Parent* parent );
        void setup( const kvs::Shader::ShadingModel& shading_model );
        void draw( const kvs::ObjectBase* object );
    };

private:
    float m_edge_factor = 0.0f; ///< edge enhancement factor
    kvs::Vec2 m_depth_offset{ 0.0f, 0.0f }; ///< depth offset {factor, units}
    BufferObject m_buffer_object{}; ///< buffer object
    RenderPass m_render_pass{ m_buffer_object, this }; ///< render pass

public:
    Engine() = default;
    virtual ~Engine() { this->release(); }
    void release() { m_render_pass.release(); m_buffer_object.release(); }
    void create( kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light );
    void update( kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light );
    void setup( kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light );
    void draw( kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light );

    void setEdgeFactor( const float factor ) { m_edge_factor = factor; }
    void setDepthOffset( const kvs::Vec2& offset ) { m_depth_offset = offset; }
    void setDepthOffset( const float factor, const float units = 0.0f ) { m_depth_offset = kvs::Vec2( factor, units ); }

    const std::string& vertexShaderFile() const { return m_render_pass.vertexShaderFile(); }
    const std::string& fragmentShaderFile() const { return m_render_pass.fragmentShaderFile(); }
    void setVertexShaderFile( const std::string& file ) { m_render_pass.setVertexShaderFile( file ); }
    void setFragmentShaderFile( const std::string& file ) { m_render_pass.setFragmentShaderFile( file ); }
    void setShaderFiles( const std::string& vert_file, const std::string& frag_file )
    {
        m_render_pass.setShaderFiles( vert_file, frag_file );
    }

public:
    KVS_DEPRECATED( void setPolygonOffset( const float offset ) ) { this->setDepthOffset( offset ); }
};

} // end of namespace kvs
