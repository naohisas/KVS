/*****************************************************************************/
/**
 *  @file   ShaderObject.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: ShaderObject.cpp 631 2010-10-10 02:15:35Z naohisa.sakamoto $
 */
/*****************************************************************************/
#include "ShaderObject.h"
#include <kvs/DebugNew>
#include <kvs/Message>
#include <kvs/Assert>
#include <vector>


namespace kvs
{

ShaderObject::ShaderObject( const GLenum type ):
    m_id( 0 ),
    m_type( type )
{
}

ShaderObject::~ShaderObject()
{
    this->release();
}

GLuint ShaderObject::id() const
{
    return m_id;
}

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

void ShaderObject::setSource( const kvs::ShaderSource& source ) const
{
    KVS_ASSERT( this->isCreated() );
    const char* code = source.code().c_str();
    KVS_GL_CALL( glShaderSource( m_id, 1, &code, 0 ) );
}

void ShaderObject::create()
{
    this->createID();
}

void ShaderObject::release()
{
    this->deleteID();
}

bool ShaderObject::compile() const
{
    KVS_ASSERT( this->isCreated() );
    KVS_GL_CALL( glCompileShader( m_id ) );
    return this->isCompiled();
}

bool ShaderObject::compile( const kvs::ShaderSource& source ) const
{
    this->setSource( source );
    return this->compile();
}

bool ShaderObject::isCreated() const
{
    return m_id > 0;
}

bool ShaderObject::isValid() const
{
    GLboolean result = GL_FALSE;
    KVS_GL_CALL( result = glIsShader( m_id ) );
    return result == GL_TRUE;
}

bool ShaderObject::isCompiled() const
{
    GLint error = 0;
    KVS_GL_CALL( glGetShaderiv( m_id, GL_COMPILE_STATUS, &error ) );
    return error == GL_TRUE;
}

void ShaderObject::createID()
{
    if ( !this->isValid() )
    {
        KVS_GL_CALL( m_id = glCreateShader( m_type ) );
    }
}

void ShaderObject::deleteID()
{
    if ( this->isValid() )
    {
        KVS_GL_CALL( glDeleteShader( m_id ) );
    }
    m_id = 0;
}


//bool ShaderObject::create( const kvs::ShaderSource& source )
//{
//    this->create();
//    return( this->compile( source ) );
//}

} // end of namespace kvs
