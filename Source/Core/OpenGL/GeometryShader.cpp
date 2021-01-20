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

/*===========================================================================*/
/**
 *  @brief  Creates a geometry shader object.
 */
/*===========================================================================*/
GeometryShader::GeometryShader():
    kvs::ShaderObject( GL_GEOMETRY_SHADER )
{
}

/*===========================================================================*/
/**
 *  @brief  Creates a geometry shader object.
 *  @param  source [in] shader source
 */
/*===========================================================================*/
GeometryShader::GeometryShader( const kvs::ShaderSource& source ):
    kvs::ShaderObject( GL_GEOMETRY_SHADER )
{
    SuperClass::create();
    SuperClass::setSource( source );
}

} // end of namespace kvs
