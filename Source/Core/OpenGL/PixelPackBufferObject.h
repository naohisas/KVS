/*****************************************************************************/
/**
 *  @file   PixelPackBufferObject.h
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
class PixelPackBufferObject : public kvs::BufferObject
{
public:
    PixelPackBufferObject():
        kvs::BufferObject(
            GL_PIXEL_PACK_BUFFER,
            GL_PIXEL_PACK_BUFFER_BINDING,
            GL_DYNAMIC_DRAW ) {}

};

} // end of namespace kvs
