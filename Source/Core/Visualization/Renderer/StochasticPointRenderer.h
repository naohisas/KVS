/*****************************************************************************/
/**
 *  @file   StochasticPointRenderer.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#pragma once
#include <kvs/Module>
#include <kvs/ProgramObject>
#include <kvs/VertexBufferObjectManager>
#include <kvs/PointRenderer>
#include "StochasticRenderingEngine.h"
#include "StochasticRendererBase.h"


namespace kvs
{

class PointObject;

/*===========================================================================*/
/**
 *  @brief  Stochastic point renderer class.
 */
/*===========================================================================*/
class StochasticPointRenderer : public kvs::StochasticRendererBase
{
    kvsModule( kvs::StochasticPointRenderer, Renderer );
    kvsModuleBaseClass( kvs::StochasticRendererBase );

public:
    class Engine;

public:
    StochasticPointRenderer();
    void setDepthOffset( const kvs::Vec2& offset );
    void setDepthOffset( const float factor, const float units = 0.0f );

public:
    // KVS_DEPRECATED
    void setOpacity( const kvs::UInt8 opacity );
};

/*===========================================================================*/
/**
 *  @brief  Engine class for stochastic point renderer.
 */
/*===========================================================================*/
class StochasticPointRenderer::Engine : public kvs::StochasticRenderingEngine
{
public:
    using BaseClass = kvs::StochasticRenderingEngine;
    using BufferObject = kvs::glsl::PointRenderer::BufferObject;

    class RenderPass : public kvs::glsl::PointRenderer::RenderPass
    {
        using BaseRenderPass = kvs::glsl::PointRenderer::RenderPass;
        using Parent = BaseClass;
    private:
        const Parent* m_parent; ///< reference to the engine
        kvs::UInt8 m_opacity = 255; ///< point opacity
    public:
        RenderPass( BufferObject& buffer_object, Parent* parent );
        void setOpacity( const kvs::UInt8 opacity ) { m_opacity = opacity; }
        void setup( const kvs::Shader::ShadingModel& model );
        void draw( const kvs::ObjectBase* object );
    };

private:
    kvs::Vec2 m_depth_offset{ 0.0f, 0.0f }; ///< depth offset {factor, units}
    RenderPass m_render_pass; ///< render pass
    BufferObject m_buffer_object; ///< buffer object

public:
    Engine(): m_render_pass( m_buffer_object, this ) {}
    void release() { m_render_pass.release(); m_buffer_object.release(); }
    void create( kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light );
    void update( kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light );
    void setup( kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light );
    void draw( kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light );

    void setDepthOffset( const kvs::Vec2& offset ) { m_depth_offset = offset; }
    void setDepthOffset( const float factor, const float units = 0.0f ) { m_depth_offset = kvs::Vec2( factor, units ); }

public:
    // KVS_DEPRECATED
    void setOpacity( const kvs::UInt8 opacity ) { m_render_pass.setOpacity( opacity ); }
};

} // end of namespace kvs
