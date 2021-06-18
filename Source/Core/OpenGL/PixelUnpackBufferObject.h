/*****************************************************************************/
/**
 *  @file   PixelUnpackBufferObject.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#pragma once
#include <kvs/BufferObject>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Pixel buffer object.
 */
/*===========================================================================*/
class PixelUnpackBufferObject : public kvs::BufferObject
{
public:
    PixelUnpackBufferObject():
        kvs::BufferObject(
            GL_PIXEL_UNPACK_BUFFER,
            GL_PIXEL_UNPACK_BUFFER_BINDING,
            GL_DYNAMIC_DRAW ) {}
};

} // end of namespace kvs
