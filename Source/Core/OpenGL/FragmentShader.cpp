/*****************************************************************************/
/**
 *  @file   FragmentShader.cpp
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#include "FragmentShader.h"


namespace kvs
{

/*===========================================================================*/
/**
 *  Constructs a new FragmentShader class.
 *  @param  source [in] shader source
 */
/*===========================================================================*/
FragmentShader::FragmentShader( const kvs::ShaderSource& source ):
    kvs::ShaderObject( GL_FRAGMENT_SHADER )
{
    SuperClass::create();
    SuperClass::setSource( source );
}

} // end of namespace kvs
