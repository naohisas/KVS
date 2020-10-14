/*****************************************************************************/
/**
 *  @file   StylizedLineRenderer.h
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
#include <kvs/Texture2D>
#include <string>


namespace kvs
{

class StylizedLineRenderer : public kvs::LineRenderer
{
    kvsModule( kvs::StylizedLineRenderer, Renderer );
    kvsModuleBaseClass( kvs::LineRenderer );

public:
    class BufferObject
    {
    private:
        kvs::VertexBufferObjectManager m_manager; ///< VBOs
        kvs::ValueArray<GLint> m_first_array; ///< array of starting indices for the polyline
        kvs::ValueArray<GLsizei> m_count_array; ///< array of the number of indices for the polyline
        kvs::Texture2D m_shape_texture;
        kvs::Texture2D m_diffuse_texture;
        bool m_has_connection;
    public:
        BufferObject(): m_has_connection( false ) {}
        kvs::VertexBufferObjectManager& manager() { return m_manager; }
        void release() { m_manager.release(); }
        void create( const kvs::LineObject* line, const kvs::Real32 halo, const kvs::Real32 radius );
        void draw( const kvs::LineObject* line );
    private:
        void create_shape_texture();
        void create_diffuse_texture();
    };

private:
    std::string m_vert_file; ///< vertex shader file
    std::string m_frag_file; ///< fragment shader file
    size_t m_width; ///< window width
    size_t m_height; ///< window height
    const kvs::ObjectBase* m_object; ///< pointer to the rendering object
    kvs::Shader::ShadingModel* m_shading_model; ///< shading method
    kvs::ProgramObject m_shader_program; ///< shader program

    kvs::Real32 m_radius_size;
    kvs::Real32 m_halo_size;
    BufferObject m_buffer_object;

public:
    StylizedLineRenderer();
    virtual ~StylizedLineRenderer();

    void exec( kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light );

    const std::string& vertexShaderFile() const { return m_vert_file; }
    const std::string& fragmentShaderFile() const { return m_frag_file; }
    kvs::Real32 radiusSize() const { return m_radius_size; }
    kvs::Real32 haloSize() const { return m_halo_size; }

    void setVertexShaderFile( const std::string& vert_file ) { m_vert_file = vert_file; }
    void setFragmentShaderFile( const std::string& frag_file ) { m_frag_file = frag_file; }
    void setRadiusSize( const kvs::Real32 size ) { m_radius_size = size; }
    void setHaloSize( const kvs::Real32 size ) { m_halo_size = size; }

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
inline void StylizedLineRenderer::setShader( const ShadingType shader )
{ this->setShadingModel<ShadingType>( shader ); }

} // end of namespace kvs
