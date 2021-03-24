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
    enum DrawingBuffer
    {
        FrontFace,
        BackFace,
        Volume
    };

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
    public:
        BoundingBufferObject() = default;
        kvs::VertexBufferObjectManager& manager() { return m_manager; }
        void release() { m_manager.release(); }
        void create( const kvs::StructuredVolumeObject* volume );
        void draw( const bool back = true, const bool front = true );
    };

    class RenderPass
    {
    private:
        BufferObject& m_buffer_object; ///< buffer object (reference)
        std::string m_vert_shader_file = "RC_ray_caster.vert"; ///< vertex shader file
        std::string m_frag_shader_file = "RC_ray_caster.frag"; ///< fragment shader file
        kvs::ProgramObject m_shader_program{}; ///< shader program
        bool m_enable_jittering = false; ///< frag for stochastic jittering
        float m_step = 0.5f; ///< sampling step
        float m_opaque = 1.0f; ///< opaque value for early ray termination
    public:
        RenderPass( BufferObject& buffer_object ): m_buffer_object( buffer_object ) {}
        virtual ~RenderPass() {}
        BufferObject& bufferObject() { return m_buffer_object; }
        const std::string& vertexShaderFile() const { return m_vert_shader_file; }
        const std::string& fragmentShaderFile() const { return m_frag_shader_file; }
        kvs::ProgramObject& shaderProgram() { return m_shader_program; }
        bool isJitteringEnabled() const { return m_enable_jittering; }
        float step() const { return m_step; }
        float opaque() const { return m_opaque; }
        void setVertexShaderFile( const std::string& file ) { m_vert_shader_file = file; }
        void setFragmentShaderFile( const std::string& file ) { m_frag_shader_file = file; }
        void setShaderFiles( const std::string& vert_file, const std::string& frag_file );
        void setJitteringEnabled( const bool enable = true ) { m_enable_jittering = enable; }
        void setStep( const float step ) { m_step = step; }
        void setOpaque( const float opaque ) { m_opaque = opaque; }
        virtual void release() { m_shader_program.release(); }
        virtual void create( const kvs::Shader::ShadingModel& model, const bool enable );
        virtual void update( const kvs::Shader::ShadingModel& model, const bool enable );
        virtual void setup( const kvs::Shader::ShadingModel& model, const kvs::ObjectBase* object, const kvs::Camera* camera, const kvs::Light* light );
        virtual void draw( const kvs::ObjectBase* object );
    };

    class BoundingRenderPass
    {
    private:
        BoundingBufferObject& m_buffer_object; ///< buffer object (reference)
        kvs::ProgramObject m_shader_program{}; ///< shader program
    public:
        BoundingRenderPass( BoundingBufferObject& buffer_object ): m_buffer_object( buffer_object ) {}
        virtual ~BoundingRenderPass() {}
        BoundingBufferObject& bufferObject() { return m_buffer_object; }
        kvs::ProgramObject& shaderProgram() { return m_shader_program; }
        virtual void release() { m_shader_program.release(); }
        virtual void create();
        virtual void update();
        virtual void setup();
        virtual void draw( const bool back = true, const bool front = true );
    };

private:
    // Variables
    bool m_draw_front_face = true; ///< frag for drawing front face
    bool m_draw_back_face = true; ///< frag for drawing back face
    bool m_draw_volume = true; ///< frag for drawing volume

    // Textures
    kvs::Texture1D m_transfer_function_texture; ///< transfer function texture
    kvs::Texture2D m_jittering_texture; ///< texture for stochastic jittering

    // Framebuffer
    kvs::Texture2D m_color_texture; ///< texture for color buffer
    kvs::Texture2D m_depth_texture; ///< texture for depth buffer

    // Entry/exit framebuffer
    kvs::FrameBufferObject m_entry_exit_framebuffer; ///< framebuffer object for entry/exit point texture
    kvs::Texture2D m_entry_texture; ///< entry point texture
    kvs::Texture2D m_exit_texture; ///< exit point texture

    // Buffer object
    BufferObject m_volume_buffer; ///< volume buffer object
    BoundingBufferObject m_bounding_cube_buffer; ///< bounding cube buffer object

    // Render pass
    RenderPass m_render_pass{ m_volume_buffer };
    BoundingRenderPass m_bounding_render_pass{ m_bounding_cube_buffer };

public:
    RayCastingRenderer();
    RayCastingRenderer( const kvs::TransferFunction& tfunc );
    template <typename ShadingType>
    RayCastingRenderer( const ShadingType shader );

    void exec( kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light );
    void setDrawingBuffer( const DrawingBuffer drawing_buffer );
    void setTransferFunction( const kvs::TransferFunction& tfunc );
    void setSamplingStep( const float step ) { m_render_pass.setStep( step ); }
    void setOpaqueValue( const float opaque ) { m_render_pass.setOpaque( opaque ); }
    void enableJittering() { m_render_pass.setJitteringEnabled( true ); }
    void disableJittering() { m_render_pass.setJitteringEnabled( false ); }

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
