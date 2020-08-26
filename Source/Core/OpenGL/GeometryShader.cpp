/*****************************************************************************/
/**
 *  @file   GeometryShader.cpp
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#include "GeometryShader.h"

#ifndef GL_GEOMETRY_SHADER
#define GL_GEOMETRY_SHADER 0x8DD9
#endif


namespace kvs
{

GeometryShader::GeometryShader():
    kvs::ShaderObject( GL_GEOMETRY_SHADER )
{
}

GeometryShader::GeometryShader( const kvs::ShaderSource& source ):
    kvs::ShaderObject( GL_GEOMETRY_SHADER )
{
    SuperClass::create();
    SuperClass::setSource( source );
}

} // end of namespace kvs
