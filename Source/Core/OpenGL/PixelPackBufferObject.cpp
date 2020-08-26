/*****************************************************************************/
/**
 *  @file   PixelPackBufferObject.cpp
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#include "PixelPackBufferObject.h"


namespace kvs
{

/*===========================================================================*/
/**
 *  Construct a new PixelPackBufferObject.
 */
/*===========================================================================*/
PixelPackBufferObject::PixelPackBufferObject():
    kvs::BufferObject( GL_PIXEL_PACK_BUFFER, GL_PIXEL_PACK_BUFFER_BINDING, GL_DYNAMIC_DRAW )
{
}

} // end of namespace kvs
