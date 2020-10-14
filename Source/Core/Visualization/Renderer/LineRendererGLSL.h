/*****************************************************************************/
/**
 *  @file   LineRendererGLSL.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#pragma once
#include <kvs/DebugNew>
#include <kvs/Module>
#include <kvs/LineObject>
#include <kvs/LineRenderer>
#include <kvs/Shader>
#include <kvs/ProgramObject>
#include <kvs/VertexBufferObjectManager>
#include <kvs/Deprecated>
#include <string>


namespace kvs
{

namespace glsl
{

class LineRenderer : public kvs::LineRenderer
{
    kvsModule( kvs::glsl::LineRenderer, Renderer );
    kvsModuleBaseClass( kvs::LineRenderer );

public:
    class BufferObject
    {
    private:
        kvs::VertexBufferObjectManager m_manager; ///< VBOs
        kvs::ValueArray<GLint> m_first_array; ///< array of starting indices for the polyline
        kvs::ValueArray<GLsizei> m_count_array; ///< array of the number of indices for the polyline
    public:
        BufferObject() {}
        kvs::VertexBufferObjectManager& manager() { return m_manager; }
        void release() { m_manager.release(); }
        void create( const kvs::LineObject* line );
        void draw( const kvs::LineObject* line );
    };

private:
    std::string m_vert_file; ///< vertex shader file
    std::string m_frag_file; ///< fragment shader file
    size_t m_width; ///< window width
    size_t m_height; ///< window height
    const kvs::ObjectBase* m_object; ///< pointer to the rendering object
    kvs::Shader::ShadingModel* m_shading_model; ///< shading method
    kvs::ProgramObject m_shader_program; ///< shader program

    float m_line_width;
    kvs::Vec2 m_line_width_range;
    float m_outline_width;
    kvs::RGBColor m_outline_color;
    BufferObject m_buffer_object;

public:
    LineRenderer();
    virtual ~LineRenderer();

    void exec( kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light );

    const std::string& vertexShaderFile() const { return m_vert_file; }
    const std::string& fragmentShaderFile() const { return m_frag_file; }
    float outlineWidth() const { return m_outline_width; }
    const kvs::RGBColor& outlineColor() const { return m_outline_color; }

    void setVertexShaderFile( const std::string& vert_file ) { m_vert_file = vert_file; }
    void setFragmentShaderFile( const std::string& frag_file ) { m_frag_file = frag_file; }
    void setOutlineWidth( const float width ) { m_outline_width = width; }
    void setOutlineColor( const kvs::RGBColor color ) { m_outline_color = color; }

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
    kvs::ProgramObject& shader() { return m_shader_program; }

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
inline void LineRenderer::setShader( const ShadingType shader )
{ this->setShadingModel<ShadingType>( shader ); }

} // end of namespace glsl

} // end of namespace kvs
