/*****************************************************************************/
/**
 *  @file   PointRendererGLSL.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#pragma once
#include <kvs/DebugNew>
#include <kvs/Module>
#include <kvs/PointObject>
#include <kvs/Shader>
#include <kvs/ProgramObject>
#include <kvs/VertexBufferObjectManager>
#include <kvs/Deprecated>
#include <string>


namespace kvs
{

namespace glsl
{

/*===========================================================================*/
/**
 *  @brief  Point renderer class with VBO
 */
/*===========================================================================*/
class PointRenderer : public kvs::PointRenderer
{
    kvsModule( kvs::glsl::PointRenderer, Renderer );
    kvsModuleBaseClass( kvs::PointRenderer );

public:
    class BufferObject
    {
    private:
        kvs::VertexBufferObjectManager m_manager{}; ///< VBOs
    public:
        BufferObject() = default;
        virtual ~BufferObject() { this->release(); }
        kvs::VertexBufferObjectManager& manager() { return m_manager; }
        void release() { m_manager.release(); }
        void create( const kvs::ObjectBase* object );
        void draw( const kvs::ObjectBase* object );
    };

    class RenderPass
    {
    private:
        BufferObject& m_buffer_object; ///< buffer object (reference)
        std::string m_vert_shader_file = "shader.vert"; ///< vertex shader file
        std::string m_frag_shader_file = "shader.frag"; ///< fragment shader file
        kvs::ProgramObject m_shader_program{}; ///< shader program
    public:
        RenderPass( BufferObject& buffer_object ): m_buffer_object( buffer_object ) {}
        virtual ~RenderPass() { this->release(); }
        BufferObject& bufferObject() { return m_buffer_object; }
        const std::string& vertexShaderFile() const { return m_vert_shader_file; }
        const std::string& fragmentShaderFile() const { return m_frag_shader_file; }
        kvs::ProgramObject& shaderProgram() { return m_shader_program; }
        void setVertexShaderFile( const std::string& file ) { m_vert_shader_file = file; }
        void setFragmentShaderFile( const std::string& file ) { m_frag_shader_file = file; }
        void setShaderFiles( const std::string& vert_file, const std::string& frag_file );
        virtual void release() { m_shader_program.release(); }
        virtual void create( const kvs::Shader::ShadingModel& model, const bool enable );
        virtual void update( const kvs::Shader::ShadingModel& model, const bool enable );
        virtual void setup( const kvs::Shader::ShadingModel& model );
        virtual void draw( const kvs::ObjectBase* object );
    };

private:
    size_t m_width = 0; ///< window width
    size_t m_height = 0; ///< window height
    const kvs::ObjectBase* m_object = nullptr; ///< pointer to the rendering object
    kvs::Shader::ShadingModel* m_shading_model = nullptr; ///< shading method

    BufferObject m_buffer_object{}; ///< buffer object
    RenderPass m_render_pass{ m_buffer_object }; ///< render pass

public:
    PointRenderer(): m_shading_model( new kvs::Shader::Lambert() ) {}
    virtual ~PointRenderer() { if ( m_shading_model ) { delete m_shading_model; } }

    void exec( kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light );

    const std::string& vertexShaderFile() const { return m_render_pass.vertexShaderFile(); }
    const std::string& fragmentShaderFile() const { return m_render_pass.fragmentShaderFile(); }
    void setVertexShaderFile( const std::string& file ) { m_render_pass.setVertexShaderFile( file ); }
    void setFragmentShaderFile( const std::string& file ) { m_render_pass.setFragmentShaderFile( file ); }
    void setShaderFiles( const std::string& vert_file, const std::string& frag_file )
    {
        this->setVertexShaderFile( vert_file );
        this->setFragmentShaderFile( frag_file );
    }

    template <typename Model>
    void setShadingModel( const Model model )
    {
        if ( m_shading_model ) { delete m_shading_model; }
        m_shading_model = new Model( model );
        if ( !m_shading_model )
        {
            kvsMessageError("Cannot create a specified shading model.");
        }
    }

protected:
    kvs::Shader::ShadingModel& shadingModel() { return *m_shading_model; }
    kvs::ProgramObject& shader() { return m_render_pass.shaderProgram(); }

    RenderPass& renderPass() { return m_render_pass; }
    BufferObject& bufferObject() { return m_buffer_object; }

    bool isWindowCreated() { return m_width == 0 && m_height == 0; }
    bool isWindowResized( size_t w, size_t h ) { return m_width != w || m_height != h; }
    bool isObjectChanged( const kvs::ObjectBase* o ) { return m_object != o; }
    void setWindowSize( size_t w, size_t h ) { m_width = w; m_height = h; }

    void createBufferObject( const kvs::ObjectBase* object );
    void updateBufferObject( const kvs::ObjectBase* object );
    void drawBufferObject( const kvs::Camera* camera );

public:
    template <typename ShadingType>
    KVS_DEPRECATED( void setShader( const ShadingType shader ) );
    KVS_DEPRECATED( void setOffset( const float offset ) ) { BaseClass::setDepthOffset( offset ); }
};

template <typename ShadingType>
inline void PointRenderer::setShader( const ShadingType shader )
{ this->setShadingModel<ShadingType>( shader ); }

} // end of namespace glsl

} // end of namespace kvs
