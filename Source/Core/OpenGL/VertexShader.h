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
    VertexShader():
        kvs::ShaderObject( GL_VERTEX_SHADER ) {}

    VertexShader( const kvs::ShaderSource& source ):
        kvs::ShaderObject( GL_VERTEX_SHADER )
    {
        SuperClass::create();
        SuperClass::setSource( source );
    }
};

} // end of namespace kvs
