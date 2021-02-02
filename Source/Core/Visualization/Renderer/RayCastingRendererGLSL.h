/*****************************************************************************/
/**
 *  @file   RayCastingRendererGLSL.h
 *  @author Naohisa Sakamoto
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
    class BufferObject
    {
    private:
        kvs::Texture3D m_manager{};
    public:
        BufferObject() = default;
        kvs::Texture3D& manager() { return m_manager; }
        void release() { m_manager.release(); }
        void create( const kvs::StructuredVolumeObject* volume,
                     const kvs::TransferFunction& tfunc );
        void draw();
    };

    class BoundingBufferObject
    {
    private:
        kvs::VertexBufferObjectManager m_manager{};
        kvs::ProgramObject m_shader_program{};
    public:
        BoundingBufferObject() = default;
        kvs::VertexBufferObjectManager& manager() { return m_manager; }
        void release() { m_manager.release(); m_shader_program.release(); }
        void create( const kvs::StructuredVolumeObject* volume );
        void draw( const kvs::Mat4& PM, const bool back = true, const bool front = true );
    };

public:
    enum DrawingBuffer
    {
        FrontFace,
        BackFace,
        Volume
    };

private:
    // Variables
    bool m_draw_front_face; ///< frag for drawing front face
    bool m_draw_back_face; ///< frag for drawing back face
    bool m_draw_volume; ///< frag for drawing volume
    bool m_enable_jittering; ///< frag for stochastic jittering
    float m_step; ///< sampling step
    float m_opaque; ///< opaque value for early ray termination

    // Textures
    kvs::Texture1D m_transfer_function_texture; ///< transfer function texture
    kvs::Texture2D m_jittering_texture; ///< texture for stochastic jittering

    // Framebuffer
    kvs::Texture2D m_color_texture; ///< texture for color buffer
    kvs::Texture2D m_depth_texture; ///< texture for depth buffer

    // Buffer object
    BufferObject m_volume_buffer; ///< volume buffer object
    BoundingBufferObject m_bounding_cube_buffer; ///< bounding cube buffer object

    // Entry/exit framebuffer
    kvs::FrameBufferObject m_entry_exit_framebuffer; ///< framebuffer object for entry/exit point texture
    kvs::Texture2D m_entry_texture; ///< entry point texture
    kvs::Texture2D m_exit_texture; ///< exit point texture

    // Shader program
    kvs::ProgramObject m_ray_casting_shader; ///< ray casting shader

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
    void create_shader_program( const kvs::Shader::ShadingModel& shading_model, const bool shading_enabled );
    void update_shader_program( const kvs::Shader::ShadingModel& shading_model, const bool shading_enabled );
    void setup_shader_program( const kvs::Shader::ShadingModel& shading_model, const kvs::ObjectBase* object, const kvs::Camera* camera, const kvs::Light* light );

    void create_framebuffer( const size_t width, const size_t height );
    void update_framebuffer( const size_t width, const size_t height );

    void create_buffer_object( const kvs::StructuredVolumeObject* volume );
    void update_buffer_object( const kvs::StructuredVolumeObject* volume );
    void draw_buffer_object( const kvs::StructuredVolumeObject* volume );
};

} // end of namespace glsl

} // end of namespace kvs
