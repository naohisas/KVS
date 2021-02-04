/*****************************************************************************/
/**
 *  @file   StochasticLineRenderer.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#pragma once
#include <kvs/Module>
#include <kvs/ProgramObject>
#include <kvs/VertexBufferObjectManager>
#include <kvs/LineRenderer>
#include "StochasticRenderingEngine.h"
#include "StochasticRendererBase.h"


namespace kvs
{

class LineObject;

/*===========================================================================*/
/**
 *  @brief  Stochastic line renderer class.
 */
/*===========================================================================*/
class StochasticLineRenderer : public kvs::StochasticRendererBase
{
    kvsModule( kvs::StochasticLineRenderer, Renderer );
    kvsModuleBaseClass( kvs::StochasticRendererBase );

public:
    class Engine;

public:
    StochasticLineRenderer();
    void setLineOffset( const float offset );

    // KVS_DEPRECATED
    void setOpacity( const kvs::UInt8 opacity );
};

/*===========================================================================*/
/**
 *  @brief  Engine class for stochastic line renderer.
 */
/*===========================================================================*/
class StochasticLineRenderer::Engine : public kvs::StochasticRenderingEngine
{
public:
    using BaseClass = kvs::StochasticRenderingEngine;
    using BufferObject = kvs::glsl::LineRenderer::BufferObject;

    class RenderPass : public kvs::glsl::LineRenderer::RenderPass
    {
    private:
        using BaseRenderPass = kvs::glsl::LineRenderer::RenderPass;
        using Parent = BaseClass;
        const Parent* m_parent; ///< reference to the engine
        kvs::UInt8 m_opacity = 255; ///< point opacity
        float m_offset = 0.0f; ///< line offset
    public:
        RenderPass( BufferObject& buffer_object, Parent* parent );
        void setOpacity( const kvs::UInt8 opacity ) { m_opacity = opacity; }
        void setOffset( const float offset ) { m_offset = offset; }
        void setup( const kvs::Shader::ShadingModel& model );
        void draw( const kvs::ObjectBase* object );
    };

private:
    RenderPass m_render_pass; ///< render pass
    BufferObject m_buffer_object; ///< buffer object

public:
    Engine(): m_render_pass( m_buffer_object, this ) {}
    void release() { m_render_pass.release(); m_buffer_object.release(); }
    void create( kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light );
    void update( kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light );
    void setup( kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light );
    void draw( kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light );

    void setLineOffset( const float offset ) { m_render_pass.setOffset( offset ); }

public:
    // KVS_DEPRECATED
    void setOpacity( const kvs::UInt8 opacity ){ m_render_pass.setOpacity( opacity ); }
};

} // end of namespace kvs
