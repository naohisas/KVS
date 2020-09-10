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
        kvs::VertexBufferObjectManager m_manager; ///< VBOs
    public:
        BufferObject() {}
        kvs::VertexBufferObjectManager& manager() { return m_manager; }
        void create() { m_manager.create(); }
        void release() { m_manager.release(); }
        void set( const kvs::PolygonObject* polygon );
        void draw( const kvs::PolygonObject* polygon );
    };

private:
    std::string m_vert_file; ///< vertex shader file
    std::string m_frag_file; ///< fragment shader file
    size_t m_width; ///< window width
    size_t m_height; ///< window height
    const kvs::ObjectBase* m_object; ///< pointer to the rendering object
    kvs::Shader::ShadingModel* m_shading_model; ///< shading method
    kvs::ProgramObject m_shader_program; ///< shader program

    float m_polygon_offset; ///< polygon offset
    BufferObject m_buffer_object;

public:
    PolygonRenderer();
    virtual ~PolygonRenderer();

    void exec( kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light );

    const std::string& vertexShaderFile() const { return m_vert_file; }
    const std::string& fragmentShaderFile() const { return m_frag_file; }
    float polygonOffset() const { return m_polygon_offset; }

    void setVertexShaderFile( const std::string& vert_file ) { m_vert_file = vert_file; }
    void setFragmentShaderFile( const std::string& frag_file ) { m_frag_file = frag_file; }
    void setPolygonOffset( const float offset ) { m_polygon_offset = offset; }

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
    KVS_DEPRECATED( void setShader( const ShadingType shader ) )
    { this->setShadingModel<ShadingType>( shader ); }
};

} // end of namespace glsl

} // end of namespace kvs
