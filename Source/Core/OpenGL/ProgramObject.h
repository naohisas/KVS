/*****************************************************************************/
/**
 *  @file   ProgramObject.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: ProgramObject.h 1149 2012-06-02 13:27:56Z s.yamada0808@gmail.com $
 */
/*****************************************************************************/
#ifndef KVS__PROGRAM_OBJECT_H_INCLUDE
#define KVS__PROGRAM_OBJECT_H_INCLUDE

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
    GLuint m_id; ///< shader ID
    GLenum m_geom_input_type; ///< input type for geometry shader
    GLenum m_geom_output_type; ///< output type for geometry shader
    GLint m_geom_output_vertices; ///< number of vertices for geometry shader
    mutable bool m_is_bound; ///< binding flag

public:
    class Binder;

public:
    ProgramObject();
    virtual ~ProgramObject();

    GLuint id() const;
    std::string log() const;

    void create();
    void release();
    void attach( const kvs::ShaderObject& shader ) const;
    void detach( const kvs::ShaderObject& shader ) const;
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

    void setGeometryInputType( const GLint type );
    void setGeometryOutputType( const GLint type );
    void setGeometryOutputVertices( const GLint value );

protected:
    void createID();
    void deleteID();
    void setParameter( GLenum pname, GLint value );

public:
    /*KVS_DEPRECATED*/ bool link( const kvs::VertexShader& vertex_shader, const kvs::FragmentShader& fragment_shader );
    /*KVS_DEPRECATED*/ void create( const kvs::ShaderSource& vertex_source, const kvs::ShaderSource& fragment_source );
    KVS_DEPRECATED( void clear() ) { this->release(); }
    KVS_DEPRECATED( void setUniformValuei( const GLchar* name, const GLint v0 ) );
    KVS_DEPRECATED( void setUniformValuei( const GLchar* name, const GLint v0, const GLint v1 ) );
    KVS_DEPRECATED( void setUniformValuei( const GLchar* name, const GLint v0, const GLint v1, const GLint v2 ) );
    KVS_DEPRECATED( void setUniformValuei( const GLchar* name, const GLint v0, const GLint v1, const GLint v2, const GLint v3 ) );
    KVS_DEPRECATED( void setUniformValuei( const GLchar* name, const kvs::Vector2i& v ) );
    KVS_DEPRECATED( void setUniformValuei( const GLchar* name, const kvs::Vector3i& v ) );
    KVS_DEPRECATED( void setUniformValuei( const GLchar* name, const kvs::Vector4i& v ) );
    KVS_DEPRECATED( void setUniformValuef( const GLchar* name, const GLfloat v0 ) );
    KVS_DEPRECATED( void setUniformValuef( const GLchar* name, const GLfloat v0, const GLfloat v1 ) );
    KVS_DEPRECATED( void setUniformValuef( const GLchar* name, const GLfloat v0, const GLfloat v1, const GLfloat v2 ) );
    KVS_DEPRECATED( void setUniformValuef( const GLchar* name, const GLfloat v0, const GLfloat v1, const GLfloat v2, const GLfloat v3 ) );
    KVS_DEPRECATED( void setUniformValuef( const GLchar* name, const kvs::Vector2f& v ) );
    KVS_DEPRECATED( void setUniformValuef( const GLchar* name, const kvs::Vector3f& v ) );
    KVS_DEPRECATED( void setUniformValuef( const GLchar* name, const kvs::Vector4f& v ) );
};

class ProgramObject::Binder
{
    const kvs::ProgramObject& m_po;

public:

    Binder( const kvs::ProgramObject& po );
    ~Binder();

private:

    Binder( const Binder& );
    Binder& operator =( const Binder& );
};

} // end of namespace kvs

#endif // KVS__PROGRAM_OBJECT_H_INCLUDE
