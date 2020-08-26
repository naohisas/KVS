/*****************************************************************************/
/**
 *  @file   VertexShader.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#ifndef KVS__VERTEX_SHADER_H_INCLUDE
#define KVS__VERTEX_SHADER_H_INCLUDE

#include <kvs/ShaderObject>
#include <kvs/ShaderSource>


namespace kvs
{

/*===========================================================================*/
/**
 *  Vertex shader class.
 */
/*===========================================================================*/
class VertexShader : public kvs::ShaderObject
{
public:

    typedef kvs::ShaderObject SuperClass;

public:

    VertexShader();
    VertexShader( const kvs::ShaderSource& source );
};

} // end of namespace kvs

#endif // KVS__VERTEX_SHADER_H_INCLUDE
