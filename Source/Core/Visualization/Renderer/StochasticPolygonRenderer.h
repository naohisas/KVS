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
    void setPolygonOffset( const float polygon_offset );
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
        void draw( const kvs::PolygonObject* polygon );
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
    void setPolygonOffset( const float offset ) { m_render_pass.setPolygonOffset( offset ); }
};

} // end of namespace kvs
