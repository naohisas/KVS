/*****************************************************************************/
/**
 *  @file   GeometryShader.cpp
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#include "GeometryShader.h"


namespace kvs
{

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
