/*****************************************************************************/
/**
 *  @file   PolygonRendererGLSL.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#pragma once
#include <kvs/DebugNew>
#include <kvs/Module>
#include <kvs/PolygonObject>
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
 *  @brief  Polygon renderer class.
 */
/*===========================================================================*/
class PolygonRenderer : public kvs::PolygonRenderer
{
    kvsModule( kvs::glsl::PolygonRenderer, Renderer );
    kvsModuleBaseClass( kvs::PolygonRenderer );

public:
    class BufferObject
    {
    private:
        kvs::VertexBufferObjectManager m_manager{}; ///< VBOs
    public:
        BufferObject() = default;
        kvs::VertexBufferObjectManager& manager() { return m_manager; }
        void release() { m_manager.release(); }
        void create( const kvs::PolygonObject* polygon );
        void draw( const kvs::PolygonObject* polygon );
    };

    class RenderPass
    {
    private:
        BufferObject& m_buffer_object; ///< buffer object (reference)
        float m_polygon_offset = 0.0f; ///< polygon offset
        std::string m_vert_shader_file = "shader.vert"; ///< vertex shader file
        std::string m_frag_shader_file = "shader.frag"; ///< fragment shader file
        kvs::ProgramObject m_shader_program{}; ///< shader program
    public:
        RenderPass( BufferObject& buffer_object ): m_buffer_object( buffer_object ) {}
        virtual ~RenderPass() {}
        BufferObject& bufferObject() { return m_buffer_object; }
        float polygonOffset() const { return m_polygon_offset; }
        const std::string& vertexShaderFile() const { return m_vert_shader_file; }
        const std::string& fragmentShaderFile() const { return m_frag_shader_file; }
        kvs::ProgramObject& shaderProgram() { return m_shader_program; }
        void setPolygonOffset( const float offset ) { m_polygon_offset = offset; }
        void setVertexShaderFile( const std::string& file ) { m_vert_shader_file = file; }
        void setFragmentShaderFile( const std::string& file ) { m_frag_shader_file = file; }
        void setShaderFiles( const std::string& vert_file, const std::string& frag_file );
        virtual void release() { m_shader_program.release(); }
        virtual void create( const kvs::Shader::ShadingModel& model, const bool enable );
        virtual void update( const kvs::Shader::ShadingModel& model, const bool enable );
        virtual void setup( const kvs::Shader::ShadingModel& model );
        virtual void draw( const kvs::PolygonObject* polygon );
    };

private:
    size_t m_width; ///< window width
    size_t m_height; ///< window height
    const kvs::ObjectBase* m_object; ///< pointer to the rendering object
    kvs::Shader::ShadingModel* m_shading_model; ///< shading method

    RenderPass m_render_pass; ///< render pass
    BufferObject m_buffer_object; ///< buffer object

public:
    PolygonRenderer();
    virtual ~PolygonRenderer();

    void exec( kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light );

    const std::string& vertexShaderFile() const { return m_render_pass.vertexShaderFile(); }
    const std::string& fragmentShaderFile() const { return m_render_pass.fragmentShaderFile(); }
    float polygonOffset() const { return m_render_pass.polygonOffset(); }
    void setVertexShaderFile( const std::string& file ) { m_render_pass.setVertexShaderFile( file ); }
    void setFragmentShaderFile( const std::string& file ) { m_render_pass.setFragmentShaderFile( file ); }
    void setPolygonOffset( const float offset ) { m_render_pass.setPolygonOffset( offset ); }
    void setShaderFiles( const std::string& vert_file, const std::string& frag_file )
    {
        this->setVertexShaderFile( vert_file );
        this->setFragmentShaderFile( frag_file );
    }

    template <typename Model>
    void setShadingModel( const Model model )
    {
        if ( m_shading_model ) { delete m_shading_model; m_shading_model = NULL; }
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

    void createShaderProgram();
    void updateShaderProgram();
    void setupShaderProgram();
    void createBufferObject( const kvs::ObjectBase* object );
    void updateBufferObject( const kvs::ObjectBase* object );
    void drawBufferObject( const kvs::Camera* camera );

public:
    template <typename ShadingType>
    KVS_DEPRECATED( void setShader( const ShadingType shader ) );
};

template <typename ShadingType>
inline void PolygonRenderer::setShader( const ShadingType shader )
{ this->setShadingModel<ShadingType>( shader ); }

} // end of namespace glsl

} // end of namespace kvs
