/*****************************************************************************/
/**
 *  @file   StochasticMultipleTetrahedraRenderer.h
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
#ifndef KVS__STOCHASTIC_MULTIPLE_TETRAHEDRA_RENDERER_H_INCLUDE
#define KVS__STOCHASTIC_MULTIPLE_TETRAHEDRA_RENDERER_H_INCLUDE

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
 *  @brief  Stochastic tetrahedra renderer class.
 */
/*===========================================================================*/
class StochasticMultipleTetrahedraRenderer : public kvs::StochasticRendererBase
{
    kvsModule( kvs::StochasticMultipleTetrahedraRenderer, Renderer );
    kvsModuleBaseClass( kvs::StochasticRendererBase );

    friend class StochasticMultipleTetrahedraCompositor;

public:

    class Engine;

public:

    StochasticMultipleTetrahedraRenderer();
    void exec( kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light );
    void setTransferFunction( const kvs::TransferFunction& transfer_function, const size_t index );

protected:

    const kvs::UnstructuredVolumeObject* volume( const size_t index );
    void attachVolume( const kvs::UnstructuredVolumeObject* volume, const size_t index );
    void setExtraTexture( const kvs::Texture2D& extra_texture );
};

/*===========================================================================*/
/**
 *  @brief  Engine class for stochastic polygon renderer.
 */
/*===========================================================================*/
class StochasticMultipleTetrahedraRenderer::Engine : public kvs::StochasticRenderingEngine
{
private:

    const kvs::UnstructuredVolumeObject* m_volume[2]; ///< rendering volumes
    size_t m_random_index[2]; ///< indices used for refering the random texture
    size_t m_value[2]; ///< indices used for refering the values
    bool m_transfer_function_changed[2]; ///< flags for changin transfer function
    kvs::TransferFunction m_transfer_function[2]; ///< transfer functions
    kvs::Texture3D m_preintegration_texture[2]; ///< pre-integration textures
    kvs::ProgramObject m_shader_program[2]; ///< shader programs
    kvs::VertexBufferObject m_vbo[2]; ///< vertex buffer objects
    kvs::IndexBufferObject m_ibo[2]; ///< index buffer objects
    kvs::Texture2D m_decomposition_texture; ///< texture for the tetrahedral decomposition
    kvs::Texture2D m_extra_texture; ///< extra texture (copied from the compositor)

public:

    Engine();
    void release();
    void create( kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light );
    void update( kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light );
    void setup( kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light );
    void draw( kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light );

    void attachVolume( const kvs::UnstructuredVolumeObject* volume, const size_t index )
    {
        m_volume[index] = volume;
    }

    void setTransferFunction( const kvs::TransferFunction& transfer_function, const size_t index )
    {
        m_transfer_function[index] = transfer_function;
        m_transfer_function_changed[index] = true;
    }

    void setExtraTexture( const kvs::Texture2D& extra_texture ) { m_extra_texture = extra_texture; }
    const kvs::UnstructuredVolumeObject* volume( const size_t index ) { return m_volume[index]; }

private:

    void create_shader_program();
    void create_buffer_object();
    void create_preintegration_texture( const size_t index );
    void create_decomposition_texture();
};

} // end of namespace kvs

#endif // KVS__STOCHASTIC_MULTIPLE_TETRAHEDRA_RENDERER_H_INCLUDE
