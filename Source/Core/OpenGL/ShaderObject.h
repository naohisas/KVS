/*****************************************************************************/
/**
 *  @file   ShaderObject.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#pragma once
#include <kvs/ShaderSource>
#include <kvs/GL>
#include <kvs/Deprecated>


namespace kvs
{

/*===========================================================================*/
/**
 *  Shader object class.
 */
/*===========================================================================*/
class ShaderObject
{
private:
    GLuint m_id = 0; ///< shader ID
    GLenum m_type = 0; ///< shader type (GL_VERTEX_SHADER, GL_GEOMETRY_SHADER or GL_FRAGMENT_SHADER)

public:
    ShaderObject( const GLenum type ): m_type( type ) {}
    virtual ~ShaderObject() { this->release(); }

    GLuint id() const { return m_id; }
    std::string log() const;
    std::string source() const;
    void setSource( const kvs::ShaderSource& source ) const;

    void create() { this->createID(); }
    void release() { this->deleteID(); }
    bool compile() const;
    bool compile( const kvs::ShaderSource& source ) const;

    bool isCreated() const;
    bool isValid() const;
    bool isCompiled() const;

protected:
    ShaderObject();
    void createID();
    void deleteID();
};

} // end of namespace kvs
