/*****************************************************************************/
/**
 *  @file   VertexShader.h
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
 *  Vertex shader class.
 */
/*===========================================================================*/
class VertexShader : public kvs::ShaderObject
{
public:
    using SuperClass = kvs::ShaderObject;

public:
    VertexShader();
    VertexShader( const kvs::ShaderSource& source );
};

} // end of namespace kvs
