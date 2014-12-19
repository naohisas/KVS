/*****************************************************************************/
/**
 *  @file   StochasticMultivariateRenderer.h
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
#ifndef KVS__STOCHASTIC_MULTIVARITE_TETRAHEDRA_RENDERER_H_INCLUDE
#define KVS__STOCHASTIC_MULTIVARITE_TETRAHEDRA_RENDERER_H_INCLUDE

#include <kvs/Module>
#include <kvs/TransferFunction>
#include <kvs/Texture3D>
#include <kvs/ProgramObject>
#include <kvs/VertexBufferObject>
#include <kvs/IndexBufferObject>
#include "StochasticRenderingEngine.h"
#include "StochasticRendererBase.h"


namespace kvs
{

class UnstructuredVolumeObject;

/*===========================================================================*/
/**
 *  @brief  Stochastic multivariate tetrahedra renderer class.
 */
/*===========================================================================*/
class StochasticMultivariateTetrahedraRenderer : public kvs::StochasticRendererBase
{
    kvsModule( kvs::StochasticMultivariateTetrahedraRenderer, Renderer );
    kvsModuleBaseClass( kvs::StochasticRendererBase );

public:

    class Engine;

public:

    StochasticMultivariateTetrahedraRenderer();
    void showComponent( const size_t index );
    void hideComponent( const size_t index );
    void setTransferFunction( const kvs::TransferFunction& transfer_function, const size_t index );
};

/*===========================================================================*/
/**
 *  @brief  Engine class for stochastic polygon renderer.
 */
/*===========================================================================*/
class StochasticMultivariateTetrahedraRenderer::Engine : public kvs::StochasticRenderingEngine
{
private:

    size_t m_random_index; ///< index used for refering the random texture
    size_t m_value; ///< index used for refering the values
    bool m_show_component[2]; ///< flags for showing the component
    bool m_transfer_function_changed[2]; ///< flags for changin transfer function
    kvs::TransferFunction m_transfer_function[2]; ///< transfer functions for the component
    kvs::Texture3D m_preintegration_texture[2]; ///< pre-integration tables for the component #1
    kvs::Texture2D m_decomposition_texture; ///< texture for the tetrahedral decomposition
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

    void showComponent( const size_t index ) { m_show_component[index] = true; }
    void hideComponent( const size_t index ) { m_show_component[index] = false; }
    void setTransferFunction( const kvs::TransferFunction& transfer_function, const size_t index )
    {
        m_transfer_function[index] = transfer_function;
        m_transfer_function_changed[index] = true;
    }

private:

    void create_shader_program();
    void create_buffer_object( const kvs::UnstructuredVolumeObject* volume );
    void create_preintegration_texture( const size_t index );
    void create_decomposition_texture();
};

} // end of namespace kvs

#endif // KVS__STOCHASTIC_MULTIVARIATE_TETRAHEDRA_RENDERER_H_INCLUDE
