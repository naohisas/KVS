/*****************************************************************************/
/**
 *  @file   StochasticUniformGridRenderer.h
 *  @author Naoya Maeda, Naohisa Sakamoto
 */
/*****************************************************************************/
#pragma once
#include <kvs/Module>
#include <kvs/ProgramObject>
#include <kvs/VertexBufferObjectManager>
#include <kvs/FrameBufferObject>
#include <kvs/Texture2D>
#include <kvs/Texture3D>
#include <kvs/TransferFunction>
#include <kvs/RayCastingRenderer>
#include "StochasticRenderingEngine.h"
#include "StochasticRendererBase.h"


namespace kvs
{

class StructuredVolumeObject;

/*===========================================================================*/
/**
 *  @brief  Stochastic uniform grid renderer class.
 */
/*===========================================================================*/
class StochasticUniformGridRenderer : public kvs::StochasticRendererBase
{
    kvsModule( kvs::StochasticUniformGridRenderer, Renderer );
    kvsModuleBaseClass( kvs::StochasticRendererBase );

public:
    class Engine;

public:
    StochasticUniformGridRenderer();
    void setSamplingStep( const float step );
    void setTransferFunction( const kvs::TransferFunction& transfer_function );
    const kvs::TransferFunction& transferFunction() const;
    float samplingStep() const;
};

/*===========================================================================*/
/**
 *  @brief  Engine class for stochastic uniform grid renderer.
 */
/*===========================================================================*/
class StochasticUniformGridRenderer::Engine : public kvs::StochasticRenderingEngine
{
    using BaseClass = kvs::StochasticRenderingEngine;

public:
    using BufferObject = kvs::glsl::RayCastingRenderer::BufferObject;
    using BoundingBufferObject = kvs::glsl::RayCastingRenderer::BoundingBufferObject;

private:
    // Variable
    float m_step; ///< sampling step

    // Transfer function
    bool m_transfer_function_changed; ///< flag for changin transfer function
    kvs::TransferFunction m_transfer_function; ///< transfer function
    kvs::Texture1D m_transfer_function_texture; ///< transfer function texture

    // Buffer object
    BufferObject m_volume_buffer; ///< volume buffer object
    BoundingBufferObject m_bounding_cube_buffer; ///< bounding cube buffer

    // Entry/exit framebuffer
    kvs::FrameBufferObject m_entry_exit_framebuffer; ///< framebuffer object for entry/exit point texture
    kvs::Texture2D m_entry_texture; ///< entry point texture
    kvs::Texture2D m_exit_texture; ///< exit point texture

    // Shader program
    kvs::ProgramObject m_ray_casting_shader; ///< ray casting shader

public:
    Engine();
    void release();
    void create( kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light );
    void update( kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light );
    void setup( kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light );
    void draw( kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light );

    void setSamplingStep( const float step ) { m_step = step; }
    void setTransferFunction( const kvs::TransferFunction& transfer_function )
    {
        m_transfer_function = transfer_function;
        m_transfer_function_changed = true;
    }

    float samplingStep() const { return m_step; }
    const kvs::TransferFunction& transferFunction() const { return m_transfer_function; }

private:
    void create_shader_program( const kvs::Shader::ShadingModel& shading_model, const bool shading_enabled );
    void update_shader_program( const kvs::Shader::ShadingModel& shading_model, const bool shading_enabled );
    void setup_shader_program( const kvs::Shader::ShadingModel& shading_model, const kvs::ObjectBase* object, const kvs::Camera* camera, const kvs::Light* light );

    void create_framebuffer( const size_t width, const size_t height );
    void update_framebuffer( const size_t width, const size_t height );

    void create_buffer_object( const kvs::StructuredVolumeObject* volume );
    void update_buffer_object( const kvs::StructuredVolumeObject* volume );
    void draw_buffer_object( const kvs::StructuredVolumeObject* volume );
};

} // end of namespace kvs
