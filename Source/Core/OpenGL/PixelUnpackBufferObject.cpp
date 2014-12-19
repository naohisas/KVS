/*****************************************************************************/
/**
 *  @file   PixelUnpackBufferObject.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: PixelBufferObject.cpp 602 2010-08-19 02:43:34Z naohisa.sakamoto $
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
