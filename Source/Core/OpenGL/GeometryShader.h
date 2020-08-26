/*****************************************************************************/
/**
 *  @file   GeometryShader.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#ifndef KVS__GEOMETRY_SHADER_H_INCLUDE
#define KVS__GEOMETRY_SHADER_H_INCLUDE

#include <kvs/ShaderObject>
#include <kvs/ShaderSource>


namespace kvs
{

/*===========================================================================*/
/**
 *  Geometry shader class.
 */
/*===========================================================================*/
class GeometryShader : public kvs::ShaderObject
{
public:

    typedef kvs::ShaderObject SuperClass;

public:

    GeometryShader();
    GeometryShader( const kvs::ShaderSource& source );
};

} // end of namespace kvs

#endif // KVS__GEOMETRY_SHADER_H_INCLUDE
