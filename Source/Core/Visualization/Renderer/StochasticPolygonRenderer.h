/*****************************************************************************/
/**
 *  @file   StochasticPolygonRenderer.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#pragma once
#include <kvs/Module>
#include <kvs/ProgramObject>
#include <kvs/VertexBufferObjectManager>
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
    using BufferObject = kvs::glsl::PolygonRenderer::BufferObject;

private:
    float m_polygon_offset; ///< polygon offset
    kvs::ProgramObject m_shader_program; ///< shader program
    BufferObject m_buffer_object;

public:
    Engine();
    void release();
    void create( kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light );
    void update( kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light );
    void setup( kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light );
    void draw( kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light );
    void setPolygonOffset( const float offset ) { m_polygon_offset = offset; }

private:
    void create_shader_program();
    void create_buffer_object( const kvs::PolygonObject* polygon );
};

} // end of namespace kvs
