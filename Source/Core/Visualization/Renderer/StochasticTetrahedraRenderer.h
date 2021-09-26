/*****************************************************************************/
/**
 *  @file   StochasticTetrahedraRenderer.h
 *  @author Naohisa Sakamoto
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
    void setEdgeFactor( const float factor );
    const kvs::TransferFunction& transferFunction() const;
    float samplingStep() const;
    void setVertexShaderFile( const std::string& file );
    void setGeometryShaderFile( const std::string& file );
    void setFragmentShaderFile( const std::string& file );
    void setShaderFiles( const std::string& vert_file, const std::string& geom_file, const std::string& frag_file );
};

/*===========================================================================*/
/**
 *  @brief  Engine class for stochastic polygon renderer.
 */
/*===========================================================================*/
class StochasticTetrahedraRenderer::Engine : public kvs::StochasticRenderingEngine
{
public:
    using BaseClass = kvs::StochasticRenderingEngine;

    class TransferFunctionBuffer
    {
        kvs::Texture1D m_texture{}; ///< transfer function texture
    public:
        TransferFunctionBuffer() = default;
        virtual ~TransferFunctionBuffer() { this->release(); }
        const kvs::Texture1D& texture() const { return m_texture; }
        void create( const kvs::TransferFunction& tfunc );
        void update( const kvs::TransferFunction& tfunc );
        void release();
    };

    class PreIntegrationBuffer
    {
        kvs::Texture2D m_texture{}; ///< pre-integration texture
        kvs::Texture1D m_T_texture{}; ///< T function for pre-integration
        kvs::Texture1D m_T_inv_texture{}; ///< inverse function of T for pre-integration
        kvs::Real32 m_T_max = 0.0f; ///< maximum value of T
    public:
        PreIntegrationBuffer() = default;
        virtual ~PreIntegrationBuffer() { this->release(); }
        const kvs::Texture2D& texture() const { return m_texture; }
        const kvs::Texture1D& T() const { return m_T_texture; }
        const kvs::Texture1D& Tinverse() const { return m_T_inv_texture; }
        kvs::Real32 Tmax() const { return m_T_max; }
        int inverseTextureSize() const;
        void create( const kvs::TransferFunction& tfunc );
        void update( const kvs::TransferFunction& tfunc );
        void release();
    };

    class DecompositionBuffer
    {
        kvs::Texture2D m_texture{}; ///< texture for the tetrahedral decomposition
    public:
        DecompositionBuffer() = default;
        virtual ~DecompositionBuffer() { this->release(); }
        const kvs::Texture2D& texture() const { return m_texture; }
        void create();
        void release() { m_texture.release(); }
    };

    class BufferObject
    {
        const kvs::StochasticRenderingEngine* m_engine; ///< pointer to the engine
        kvs::VertexBufferObjectManager m_manager{}; ///< vertex buffer object
    public:
        BufferObject( const kvs::StochasticRenderingEngine* engine ): m_engine( engine ) {}
        virtual ~BufferObject() { this->release(); }
        kvs::VertexBufferObjectManager& manager() { return m_manager; }
        void release() { m_manager.release(); }
        void create(
            const kvs::UnstructuredVolumeObject* volume,
            const kvs::ProgramObject& shader_program );
        void draw( const kvs::UnstructuredVolumeObject* volume );
    };

    class RenderPass
    {
        BufferObject& m_buffer_object; ///< buffer object (reference)
        kvs::Real32 m_sampling_step = 1.0f; ///< sampling step
        std::string m_vert_shader_file = "SR_tetrahedra.vert"; ///< vertex shader file
        std::string m_geom_shader_file = "SR_tetrahedra.geom"; ///< geometry shader file
        std::string m_frag_shader_file = "SR_tetrahedra.frag"; ///< fragment shader file
        kvs::ProgramObject m_shader_program{}; ///< shader program
    public:
        RenderPass( BufferObject& buffer_object ): m_buffer_object( buffer_object ) {}
        virtual ~RenderPass() { this->release(); }
        BufferObject& bufferObject() { return m_buffer_object; }
        kvs::Real32 samplingStep() const { return m_sampling_step; }
        const std::string& vertexShaderFile() const { return m_vert_shader_file; }
        const std::string& geometryShaderFile() const { return m_geom_shader_file; }
        const std::string& fragmentShaderFile() const { return m_frag_shader_file; }
        kvs::ProgramObject& shaderProgram() { return m_shader_program; }
        void setSamplingStep( const kvs::Real32 step ) { m_sampling_step = step; }
        void setVertexShaderFile( const std::string& file ) { m_vert_shader_file = file; }
        void setGeometryShaderFile( const std::string& file ) { m_geom_shader_file = file; }
        void setFragmentShaderFile( const std::string& file ) { m_frag_shader_file = file; }
        void setShaderFiles(
            const std::string& vert_file,
            const std::string& geom_file,
            const std::string& frag_file );
        virtual void release() { m_shader_program.release(); }
        virtual void create(
            const kvs::Shader::ShadingModel& model,
            const bool shading,
            const bool exact );
        virtual void update(
            const kvs::Shader::ShadingModel& model,
            const bool shading,
            const bool exact );
        virtual void setup( const kvs::Shader::ShadingModel& model );
        virtual void draw( const kvs::UnstructuredVolumeObject* volume );
    };

private:
    bool m_transfer_function_changed = true; ///< flag for changin transfer function
    kvs::TransferFunction m_transfer_function{}; ///< transfer function
    TransferFunctionBuffer m_transfer_function_buffer{}; ///< transfer function buffer
    PreIntegrationBuffer m_preintegration_buffer{}; ///< pre-integration buffer
    DecompositionBuffer m_decomposition_buffer{}; ///< decomposition buffer
    BufferObject m_buffer_object{ this }; ///< buffer object
    RenderPass m_render_pass{ m_buffer_object }; ///< render pass
    kvs::Real32 m_edge_factor = 0.0f; ///< edge enhancement factor

public:
    Engine() = default;
    virtual ~Engine() { this->release(); }
    void release();
    void create( kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light );
    void update( kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light );
    void setup( kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light );
    void draw( kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light );

    void setEdgeFactor( const float factor ) { m_edge_factor = factor; }
    void setSamplingStep( const float step ) { m_render_pass.setSamplingStep( step ); }
    void setTransferFunction( const kvs::TransferFunction& transfer_function )
    {
        m_transfer_function = transfer_function;
        m_transfer_function_changed = true;
    }

    float samplingStep() const { return m_render_pass.samplingStep(); }
    const kvs::TransferFunction& transferFunction() const { return m_transfer_function; }

    const std::string& vertexShaderFile() const { return m_render_pass.vertexShaderFile(); }
    const std::string& geometryShaderFile() const { return m_render_pass.geometryShaderFile(); }
    const std::string& fragmentShaderFile() const { return m_render_pass.fragmentShaderFile(); }
    void setVertexShaderFile( const std::string& file ) { m_render_pass.setVertexShaderFile( file ); }
    void setGeometryShaderFile( const std::string& file ) { m_render_pass.setGeometryShaderFile( file ); }
    void setFragmentShaderFile( const std::string& file ) { m_render_pass.setFragmentShaderFile( file ); }
    void setShaderFiles( const std::string& vert, const std::string& geom, const std::string& frag )
    {
        m_render_pass.setShaderFiles( vert, geom, frag );
    }

private:
    void create_transfer_function_texture();
    void update_transfer_function_texture();

    void create_decomposition_texture();

    void create_shader_program();
    void create_buffer_object( const kvs::UnstructuredVolumeObject* volume );
};

} // end of namespace kvs
