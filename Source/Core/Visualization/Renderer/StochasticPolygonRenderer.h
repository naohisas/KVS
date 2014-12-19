/*****************************************************************************/
/**
 *  @file   StochasticPolygonRenderer.h
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
#ifndef KVS__STOCHASTIC_POLYGON_RENDERER_H_INCLUDE
#define KVS__STOCHASTIC_POLYGON_RENDERER_H_INCLUDE

#include <kvs/Module>
#include <kvs/ProgramObject>
#include <kvs/VertexBufferObject>
#include <kvs/IndexBufferObject>
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
private:

    bool m_has_normal; ///< check flag for the normal array
    bool m_has_connection; ///< check flag for the connection array
    size_t m_random_index; ///< index used for refering the random texture
    float m_polygon_offset; ///< polygon offset
    kvs::ProgramObject m_shader_program; ///< shader program
    kvs::VertexBufferObject m_vbo; ///< vertex buffer object
    kvs::IndexBufferObject m_ibo; ///< index buffer object

public:

    Engine();
    void release();
    void create( kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light );
    void update( kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light );
    void setup( kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light );
    void draw( kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light );

    void setPolygonOffset( const float offset )
    {
        m_polygon_offset = offset;
    }

private:

    void create_shader_program();
    void create_buffer_object( const kvs::PolygonObject* polygon );
};

} // end of namespace kvs

#endif // KVS__STOCHASTIC_POLYGON_RENDERER_H_INCLUDE
