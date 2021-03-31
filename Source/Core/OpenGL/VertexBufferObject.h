/*****************************************************************************/
/**
 *  @file   VertexBufferObject.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#pragma once
#include <kvs/BufferObject>


namespace kvs
{

/*===========================================================================*/
/**
 *  Vertex buffer object.
 */
/*===========================================================================*/
class VertexBufferObject : public kvs::BufferObject
{
public:
    VertexBufferObject():
        kvs::BufferObject(
            GL_ARRAY_BUFFER,
            GL_ARRAY_BUFFER_BINDING,
            GL_DYNAMIC_DRAW ) {}

};

} // end of namespace kvs
