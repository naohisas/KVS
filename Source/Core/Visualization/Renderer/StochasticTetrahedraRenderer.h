/*****************************************************************************/
/**
 *  @file   StochasticTetrahedraRenderer.h
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
/*----------------------------------------------------------------------------
 *
 * References:
 * [1] Naohisa Sakamoto, Koji Koyamada, "A Stochastic Approach for Rendering
 *     Multiple Irregular Volumes", In Proc. of IEEE Pacific Visualization
 *     2014 (VisNotes), pp.272-276, 2014.3.
 */
/*****************************************************************************/
#pragma once
#include <kvs/Module>
#include <kvs/TransferFunction>
#include <kvs/Texture1D>
#include <kvs/Texture2D>
#include <kvs/ProgramObject>
#include <kvs/VertexBufferObjectManager>
#include <kvs/StochasticRenderingEngine>
#include <kvs/StochasticRendererBase>


namespace kvs
{

class UnstructuredVolumeObject;

/*===========================================================================*/
/**
 *  @brief  Stochastic tetrahedra renderer class.
 */
/*===========================================================================*/
class StochasticTetrahedraRenderer : public kvs::StochasticRendererBase
{
    kvsModule( kvs::StochasticTetrahedraRenderer, Renderer );
    kvsModuleBaseClass( kvs::StochasticRendererBase );

public:
    class Engine;

public:
    StochasticTetrahedraRenderer();
    void setTransferFunction( const kvs::TransferFunction& transfer_function );
    void setSamplingStep( const float sampling_step );
    const kvs::TransferFunction& transferFunction() const;
    float samplingStep() const;
};

/*===========================================================================*/
/**
 *  @brief  Engine class for stochastic polygon renderer.
 */
/*===========================================================================*/
class StochasticTetrahedraRenderer::Engine : public kvs::StochasticRenderingEngine
{
private:
    bool m_transfer_function_changed; ///< flag for changin transfer function
    kvs::TransferFunction m_transfer_function; ///< transfer function
    kvs::Texture2D m_preintegration_texture; ///< pre-integration texture
    kvs::Texture2D m_decomposition_texture; ///< texture for the tetrahedral decomposition
    kvs::Texture1D m_transfer_function_texture; ///< transfer function texture
    kvs::Texture1D m_T_texture; ///< T function for pre-integration
    kvs::Texture1D m_inv_T_texture; ///< inverse function of T for pre-integration
    kvs::ProgramObject m_shader_program; ///< shader program
    kvs::VertexBufferObjectManager m_vbo_manager; ///< vertex buffer object manager
    float m_sampling_step; ///< sampling step
    float m_maxT; ///< maximum value of T

public:
    Engine();
    void release();
    void create( kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light );
    void update( kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light );
    void setup( kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light );
    void draw( kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light );

    void setSamplingStep( const float sampling_step ) { m_sampling_step = sampling_step; }
    void setTransferFunction( const kvs::TransferFunction& transfer_function )
    {
        m_transfer_function = transfer_function;
        m_transfer_function_changed = true;
    }

    float samplingStep() const { return m_sampling_step; }
    const kvs::TransferFunction& transferFunction() const { return m_transfer_function; }

private:
    void create_shader_program();
    void create_buffer_object( const kvs::UnstructuredVolumeObject* volume );
    void create_preintegration_texture();
    void create_decomposition_texture();
};

} // end of namespace kvs
