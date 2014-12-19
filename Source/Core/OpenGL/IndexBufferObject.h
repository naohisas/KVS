/*****************************************************************************/
/**
 *  @file   IndexBufferObject.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: IndexBufferObject.h 634 2010-10-13 07:04:05Z naohisa.sakamoto $
 */
/*****************************************************************************/
#ifndef KVS__INDEX_BUFFER_OBJECT_H_INCLUDE
#define KVS__INDEX_BUFFER_OBJECT_H_INCLUDE

#include <kvs/BufferObject>


namespace kvs
{

/*===========================================================================*/
/**
 *  Index buffer object.
 */
/*===========================================================================*/
class IndexBufferObject : public kvs::BufferObject
{
public:

    IndexBufferObject();
};

} // end of namespace kvs

#endif // KVS__INDEX_BUFFER_OBJECT_H_INCLUDE
