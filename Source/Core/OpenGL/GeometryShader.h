/*****************************************************************************/
/**
 *  @file   GeometryShader.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#pragma once
#include <kvs/ShaderObject>
#include <kvs/ShaderSource>

#ifndef GL_GEOMETRY_SHADER
#define GL_GEOMETRY_SHADER 0x8DD9
#endif

namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Geometry shader class.
 */
/*===========================================================================*/
class GeometryShader : public kvs::ShaderObject
{
public:
    using SuperClass = kvs::ShaderObject;

public:
    GeometryShader():
        kvs::ShaderObject( GL_GEOMETRY_SHADER ) {}

    GeometryShader( const kvs::ShaderSource& source ):
        kvs::ShaderObject( GL_GEOMETRY_SHADER )
    {
        SuperClass::create();
        SuperClass::setSource( source );
    }
};

} // end of namespace kvs
