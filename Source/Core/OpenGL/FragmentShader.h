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
 *  Fragment shader class.
 */
/*===========================================================================*/
class FragmentShader : public kvs::ShaderObject
{
public:
    using SuperClass = kvs::ShaderObject;

public:
    FragmentShader();
    FragmentShader( const kvs::ShaderSource& source );
};

} // end of namespace kvs
