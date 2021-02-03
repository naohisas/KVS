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
    /*KVS_DEPRECATED*/ void setOpacity( const kvs::UInt8 opacity );
};

/*===========================================================================*/
/**
 *  @brief  Engine class for stochastic point renderer.
 */
/*===========================================================================*/
class StochasticPointRenderer::Engine : public kvs::StochasticRenderingEngine
{
    using BaseClass = kvs::StochasticRenderingEngine;
    using BufferObject = kvs::glsl::PointRenderer::BufferObject;

private:
    kvs::UInt8 m_point_opacity; ///< point opacity
    kvs::ProgramObject m_shader_program; ///< shader program
    BufferObject m_buffer_object;

public:
    Engine();
    void release();
    void create( kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light );
    void update( kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light );
    void setup( kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light );
    void draw( kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light );

public:
    /*KVS_DEPRECATED*/ void setOpacity( const kvs::UInt8 opacity ) { m_point_opacity = opacity; }

private:
    void create_shader_program();
    void create_buffer_object( const kvs::PointObject* point );
};

} // end of namespace kvs
