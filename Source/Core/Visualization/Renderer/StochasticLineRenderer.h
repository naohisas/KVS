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
    /*KVS_DEPRECATED*/ void setOpacity( const kvs::UInt8 opacity );
};

/*===========================================================================*/
/**
 *  @brief  Engine class for stochastic line renderer.
 */
/*===========================================================================*/
class StochasticLineRenderer::Engine : public kvs::StochasticRenderingEngine
{
    using BaseClass = kvs::StochasticRenderingEngine;
    using BufferObject = kvs::glsl::LineRenderer::BufferObject;

private:
    kvs::UInt8 m_line_opacity; ///< line opacity
    float m_line_offset; ///< line offset
    kvs::ProgramObject m_shader_program; ///< shader program
    BufferObject m_buffer_object;

public:
    Engine();
    void release();
    void create( kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light );
    void update( kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light );
    void setup( kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light );
    void draw( kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light );

    void setLineOffset( const float offset ) { m_line_offset = offset; }

public:
    /* KVS_DEPRECATED */ void setOpacity( const kvs::UInt8 opacity ){ m_line_opacity = opacity; }

private:
    void create_shader_program();
    void create_buffer_object( const kvs::LineObject* line );
};

} // end of namespace kvs
