/*****************************************************************************/
/**
 *  @file   GeometryShader.h
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
 *  Geometry shader class.
 */
/*===========================================================================*/
class GeometryShader : public kvs::ShaderObject
{
public:
    using SuperClass = kvs::ShaderObject;

public:

    GeometryShader();
    GeometryShader( const kvs::ShaderSource& source );
};

} // end of namespace kvs
