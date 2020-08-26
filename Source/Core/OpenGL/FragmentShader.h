/*****************************************************************************/
/**
 *  @file   FragmentShader.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#ifndef KVS__FRAGMENT_SHADER_H_INCLUDE
#define KVS__FRAGMENT_SHADER_H_INCLUDE

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

    typedef kvs::ShaderObject SuperClass;

public:

    FragmentShader();
    FragmentShader( const kvs::ShaderSource& source );
};

} // end of namespace kvs

#endif // KVS__FRAGMENT_SHADER_H_INCLUDE
