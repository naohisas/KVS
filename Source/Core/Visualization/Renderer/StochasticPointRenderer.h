/*****************************************************************************/
/**
 *  @file   StochasticPointRenderer.h
 *  @author Naohisa Sakamoto
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
#pragma once
#include <kvs/Module>
#include <kvs/ProgramObject>
#include <kvs/VertexBufferObjectManager>
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
private:
    kvs::UInt8 m_point_opacity; ///< point opacity
    bool m_has_normal; ///< check flag for the normal array
    kvs::ProgramObject m_shader_program; ///< shader program
    kvs::VertexBufferObjectManager m_vbo_manager; ///< vertex buffer object manager

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
