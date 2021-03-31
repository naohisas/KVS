/*****************************************************************************/
/**
 *  @file   FragmentShader.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#pragma once
#include <kvs/ShaderObject>
#include <kvs/ShaderSource>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Fragment shader class.
 */
/*===========================================================================*/
class FragmentShader : public kvs::ShaderObject
{
public:
    using SuperClass = kvs::ShaderObject;

public:
    FragmentShader():
        kvs::ShaderObject( GL_FRAGMENT_SHADER ) {}

    FragmentShader( const kvs::ShaderSource& source ):
        kvs::ShaderObject( GL_FRAGMENT_SHADER )
    {
        SuperClass::create();
        SuperClass::setSource( source );
    }
};

} // end of namespace kvs
