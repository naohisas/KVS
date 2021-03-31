/*****************************************************************************/
/**
 *  @file   IndexBufferObject.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#pragma once
#include <kvs/BufferObject>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Index buffer object.
 */
/*===========================================================================*/
class IndexBufferObject : public kvs::BufferObject
{
public:
    IndexBufferObject():
        kvs::BufferObject(
            GL_ELEMENT_ARRAY_BUFFER,
            GL_ELEMENT_ARRAY_BUFFER_BINDING,
            GL_DYNAMIC_DRAW ) {}
};

} // end of namespace kvs
