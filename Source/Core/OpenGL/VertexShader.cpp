/*****************************************************************************/
/**
 *  @file   VertexShader.cpp
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#include "VertexShader.h"


namespace kvs
{

/*===========================================================================*/
/**
 *  Construct a new VertexShader class.
 */
/*===========================================================================*/
VertexShader::VertexShader():
    kvs::ShaderObject( GL_VERTEX_SHADER )
{
}

/*===========================================================================*/
/**
 *  Construct a new VertexShader class.
 *  @param  source [in] shader source code
 */
/*===========================================================================*/
VertexShader::VertexShader( const kvs::ShaderSource& source ):
    kvs::ShaderObject( GL_VERTEX_SHADER )
{
    SuperClass::create();
    SuperClass::setSource( source );
}


} // end of namespace kvs
