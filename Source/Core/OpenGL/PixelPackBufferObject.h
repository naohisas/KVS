/*****************************************************************************/
/**
 *  @file   PixelPackBufferObject.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: PixelBufferObject.h 634 2010-10-13 07:04:05Z naohisa.sakamoto $
 */
/*****************************************************************************/
#ifndef KVS__PIXEL_PACK_BUFFER_OBJECT_H_INCLUDE
#define KVS__PIXEL_PACK_BUFFER_OBJECT_H_INCLUDE

#include <kvs/BufferObject>


namespace kvs
{

/*===========================================================================*/
/**
 *  Pixel buffer object.
 */
/*===========================================================================*/
class PixelPackBufferObject : public kvs::BufferObject
{
public:

    PixelPackBufferObject();
};

} // end of namespace kvs

#endif // KVS__PIXEL_PACK_BUFFER_OBJECT_H_INCLUDE
