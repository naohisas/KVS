/*****************************************************************************/
/**
 *  @file   StochasticLineRenderer.h
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
private:
    kvs::UInt8 m_line_opacity; ///< line opacity
    kvs::ValueArray<GLint> m_first_array; ///< array of starting indices for the polyline
    kvs::ValueArray<GLsizei> m_count_array; ///< array of the number of indices for the polyline
    bool m_has_connection; ///< check flag for the connection array
    float m_line_offset; ///< line offset
    kvs::ProgramObject m_shader_program; ///< shader program
    kvs::VertexBufferObjectManager m_vbo_manager; ///< vertex buffer object manager

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
