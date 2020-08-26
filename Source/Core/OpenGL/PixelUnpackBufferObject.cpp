/*****************************************************************************/
/**
 *  @file   PixelUnpackBufferObject.cpp
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#include "PixelUnpackBufferObject.h"


namespace kvs
{

/*===========================================================================*/
/**
 *  Construct a new PixelUnpackBufferObject.
 *  @param  operation_type [in] operation type
 */
/*===========================================================================*/
PixelUnpackBufferObject::PixelUnpackBufferObject():
    kvs::BufferObject( GL_PIXEL_UNPACK_BUFFER, GL_PIXEL_UNPACK_BUFFER_BINDING, GL_DYNAMIC_DRAW )
{
}

} // end of namespace kvs
