/*****************************************************************************/
/**
 *  @file   ProgramObject.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#pragma once
#include <kvs/ShaderObject>
#include <kvs/VertexShader>
#include <kvs/GeometryShader>
#include <kvs/FragmentShader>
#include <kvs/Vector2>
#include <kvs/Vector3>
#include <kvs/Vector4>
#include <kvs/Matrix22>
#include <kvs/Matrix33>
#include <kvs/Matrix44>
#include <kvs/ValueArray>
#include <kvs/Deprecated>


namespace kvs
{

/*===========================================================================*/
/**
 *  Program object class.
 */
/*===========================================================================*/
class ProgramObject
{
private:
    GLuint m_id = 0; ///< program object ID
    GLenum m_geom_input_type = 0; ///< input type for geometry shader
    GLenum m_geom_output_type = 0; ///< output type for geometry shader
    GLint m_geom_output_vertices = 0; ///< number of vertices for geometry shader
    mutable bool m_is_bound = false; ///< binding flag

public:
    class Binder;

public:
    ProgramObject() = default;
    virtual ~ProgramObject() { this->release(); }

    GLuint id() const { return m_id; }
    std::string log() const;

    void create() { this->createID(); }
    void release() { this->deleteID(); }
    void attach( const kvs::ShaderObject& shader ) const;
    void detach( const kvs::ShaderObject& shader ) const;
    void detach() const;
    bool link() const;
    void build( const kvs::ShaderSource& vert_src, const kvs::ShaderSource& frag_src );
    void build( const kvs::ShaderSource& vert_src, const kvs::ShaderSource& geom_src, const kvs::ShaderSource& frag_src );

    void bind() const;
    void unbind() const;
    bool isCreated() const;
    bool isValid() const;
    bool isBound() const;
    bool isLinked() const;

    GLint uniformLocation( const GLchar* name );
    GLint attributeLocation( const GLchar* name );

    void setUniform( const GLchar* name, const GLint value );
    void setUniform( const GLchar* name, const kvs::Vector2i& value );
    void setUniform( const GLchar* name, const kvs::Vector3i& value );
    void setUniform( const GLchar* name, const kvs::Vector4i& value );
    void setUniform( const GLchar* name, const GLfloat value );
    void setUniform( const GLchar* name, const kvs::Vector2f& value );
    void setUniform( const GLchar* name, const kvs::Vector3f& value );
    void setUniform( const GLchar* name, const kvs::Vector4f& value );
    void setUniform( const GLchar* name, const kvs::Matrix22f& value );
    void setUniform( const GLchar* name, const kvs::Matrix33f& value );
    void setUniform( const GLchar* name, const kvs::Matrix44f& value );
    void setUniform( const GLchar* name, const kvs::ValueArray<GLint>& values, const int dim = 1 );
    void setUniform( const GLchar* name, const kvs::ValueArray<GLfloat>& values, const int dim = 1 );

    void setGeometryInputType( const GLint type ) { m_geom_input_type = type; }
    void setGeometryOutputType( const GLint type ) { m_geom_output_type = type; }
    void setGeometryOutputVertices( const GLint value ) { m_geom_output_vertices = value; }

protected:
    void createID();
    void deleteID();
    void setParameter( GLenum pname, GLint value );

public:
    KVS_DEPRECATED( bool link( const kvs::VertexShader& vertex_shader, const kvs::FragmentShader& fragment_shader ) );
    KVS_DEPRECATED( void create( const kvs::ShaderSource& vertex_source, const kvs::ShaderSource& fragment_source ) );
};

class ProgramObject::Binder
{
    const kvs::ProgramObject& m_po;
public:
    Binder( const kvs::ProgramObject& po );
    ~Binder();
    Binder( const Binder& ) = delete;
    Binder& operator =( const Binder& ) = delete;
};

} // end of namespace kvs
