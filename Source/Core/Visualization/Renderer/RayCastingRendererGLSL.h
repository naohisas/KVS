/*****************************************************************************/
/**
 *  @file   RayCastingRendererGLSL.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: RayCastingRenderer.h 1472 2013-04-01 09:59:41Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#pragma once
#include <kvs/Module>
#include <kvs/VolumeRendererBase>
#include <kvs/Texture1D>
#include <kvs/Texture2D>
#include <kvs/Texture3D>
#include <kvs/FrameBufferObject>
#include <kvs/VertexBufferObjectManager>
#include <kvs/ObjectBase>
#include <kvs/Camera>
#include <kvs/Light>
#include <kvs/StructuredVolumeObject>
#include <kvs/ProgramObject>
#include <kvs/ShaderSource>


namespace kvs
{

namespace glsl
{

/*===========================================================================*/
/**
 *  @brief  RayCastingRenderer class.
 */
/*===========================================================================*/
class RayCastingRenderer : public kvs::VolumeRendererBase
{
    kvsModule( kvs::glsl::RayCastingRenderer, Renderer );
    kvsModuleBaseClass( kvs::VolumeRendererBase );

public:
    enum DrawingBuffer
    {
        FrontFace,
        BackFace,
        Volume
    };

private:
    bool m_draw_front_face; ///< frag for drawing front face
    bool m_draw_back_face; ///< frag for drawing back face
    bool m_draw_volume; ///< frag for drawing volume
    bool m_enable_jittering; ///< frag for stochastic jittering
    float m_step; ///< sampling step
    float m_opaque; ///< opaque value for early ray termination
    kvs::Texture1D m_transfer_function_texture; ///< transfer function texture
    kvs::Texture2D m_jittering_texture; ///< texture for stochastic jittering
    kvs::Texture2D m_entry_texture; ///< entry point texture
    kvs::Texture2D m_exit_texture; ///< exit point texture
    kvs::Texture2D m_color_texture; ///< texture for color buffer
    kvs::Texture2D m_depth_texture; ///< texture for depth buffer
    kvs::Texture3D m_volume_texture; ///< volume data (3D texture)
    kvs::FrameBufferObject m_entry_exit_framebuffer; ///< framebuffer object for entry/exit point texture
    kvs::VertexBufferObjectManager m_bounding_cube_buffer; ///< bounding cube (VBO)
    kvs::ProgramObject m_ray_casting_shader; ///< ray casting shader
    kvs::ProgramObject m_bounding_cube_shader; ///< bounding cube shader

public:
    RayCastingRenderer();
    RayCastingRenderer( const kvs::TransferFunction& tfunc );
    template <typename ShadingType>
    RayCastingRenderer( const ShadingType shader );

    void exec( kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light );
    void setDrawingBuffer( const DrawingBuffer drawing_buffer );
    void setTransferFunction( const kvs::TransferFunction& tfunc );
    void setSamplingStep( const float step ) { m_step = step; }
    void setOpaqueValue( const float opaque ) { m_opaque = opaque; }
    void enableJittering() { m_enable_jittering = true; }
    void disableJittering() { m_enable_jittering = false; }

private:
    void initialize_shader( const kvs::StructuredVolumeObject* volume );
    void initialize_jittering_texture();
    void initialize_bounding_cube_buffer( const kvs::StructuredVolumeObject* volume );
    void initialize_transfer_function_texture();
    void initialize_volume_texture( const kvs::StructuredVolumeObject* volume );
    void initialize_framebuffer( const size_t width, const size_t height );
    void update_framebuffer( const size_t width, const size_t height );
    void draw_bounding_cube_buffer();
    void draw_quad( const float opacity );
};

} // end of namespace glsl

} // end of namespace kvs
