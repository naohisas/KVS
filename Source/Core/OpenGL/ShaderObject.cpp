/*****************************************************************************/
/**
 *  @file   ShaderObject.cpp
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#include "ShaderObject.h"
#include <kvs/DebugNew>
#include <kvs/Message>
#include <kvs/Assert>
#include <vector>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Construct a shader object.
 *  @param  type [in] shader type
 */
/*===========================================================================*/
ShaderObject::ShaderObject( const GLenum type ):
    m_id( 0 ),
    m_type( type )
{
}

/*===========================================================================*/
/**
 *  @brief  Destroys a shader object.
 */
/*===========================================================================*/
ShaderObject::~ShaderObject()
{
    this->release();
}

/*===========================================================================*/
/**
 *  @brief  Returns the information log for a shader object.
 *  @return information log
 */
/*===========================================================================*/
std::string ShaderObject::log() const
{
    KVS_ASSERT( this->isCreated() );
//    GLint length = 0;
//    KVS_GL_CALL( glGetShaderiv( m_id, GL_INFO_LOG_LENGTH, &length ) );
//    if ( length == 0 ) return "";
    GLint length = 512;
    std::vector<char> buffer( length );
    KVS_GL_CALL( glGetShaderInfoLog( m_id, length, NULL, &buffer[0] ) );
    return std::string( buffer.begin(), buffer.end() );
}

/*===========================================================================*/
/**
 *  @brief  Returns the shader source code from a shader object.
 *  @return shader source code
 */
/*===========================================================================*/
std::string ShaderObject::source() const
{
    KVS_ASSERT( this->isCreated() );
    GLint length = 0;
    KVS_GL_CALL( glGetShaderiv( m_id, GL_SHADER_SOURCE_LENGTH, &length ) );
    if ( length == 0 ) return "";
    std::vector<char> buffer( length );
    KVS_GL_CALL( glGetShaderSource( m_id, length, NULL, &buffer[0] ) );
    return std::string( buffer.begin(), buffer.end() );
}

/*===========================================================================*/
/**
 *  @brief  Sets the shade source in a shade object.
 *  @param  source [in] shader source
 */
/*===========================================================================*/
void ShaderObject::setSource( const kvs::ShaderSource& source ) const
{
    KVS_ASSERT( this->isCreated() );
    const char* code = source.code().c_str();
    KVS_GL_CALL( glShaderSource( m_id, 1, &code, 0 ) );
}

/*===========================================================================*/
/**
 *  @brief  Create a shader object.
 */
/*===========================================================================*/
void ShaderObject::create()
{
    this->createID();
}

/*===========================================================================*/
/**
 *  @brief  Release a shader object.
 */
/*===========================================================================*/
void ShaderObject::release()
{
    this->deleteID();
}

/*===========================================================================*/
/**
 *  @brief  Compiles a shader object.
 */
/*===========================================================================*/
bool ShaderObject::compile() const
{
    KVS_ASSERT( this->isCreated() );
    KVS_GL_CALL( glCompileShader( m_id ) );
    return this->isCompiled();
}

/*===========================================================================*/
/**
 *  @brief  Compiles a shader object by the specified shade source.
 *  @param  source [in] shader source
 *  @return true if the shade is compiled successfully
 */
/*===========================================================================*/
bool ShaderObject::compile( const kvs::ShaderSource& source ) const
{
    this->setSource( source );
    return this->compile();
}

/*===========================================================================*/
/**
 *  @brief  Determines if the shader object is created.
 *  @return true if the shader object is created
 */
/*===========================================================================*/
bool ShaderObject::isCreated() const
{
    return m_id > 0;
}

/*===========================================================================*/
/**
 *  @brief  Determins if a name corresponds to a shader object.
 *  @return true if the shader ID is the name of a shader object previously created
 */
/*===========================================================================*/
bool ShaderObject::isValid() const
{
    GLboolean result = GL_FALSE;
    KVS_GL_CALL( result = glIsShader( m_id ) );
    return result == GL_TRUE;
}

/*===========================================================================*/
/**
 *  @brief  Determines if the shader object is compiled.
 *  @return true if the shader object is compiled
 */
/*===========================================================================*/
bool ShaderObject::isCompiled() const
{
    GLint error = 0;
    KVS_GL_CALL( glGetShaderiv( m_id, GL_COMPILE_STATUS, &error ) );
    return error == GL_TRUE;
}

/*===========================================================================*/
/**
 *  @brief  Creates a shader object.
 */
/*===========================================================================*/
void ShaderObject::createID()
{
    if ( !this->isValid() )
    {
        KVS_GL_CALL( m_id = glCreateShader( m_type ) );
    }
}

/*===========================================================================*/
/**
 *  @brief  Deletes a shader object.
 */
/*===========================================================================*/
void ShaderObject::deleteID()
{
    if ( this->isValid() )
    {
        KVS_GL_CALL( glDeleteShader( m_id ) );
    }
    m_id = 0;
}

} // end of namespace kvs
