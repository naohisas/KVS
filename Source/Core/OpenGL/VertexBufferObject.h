/*****************************************************************************/
/**
 *  @file   VertexBufferObject.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: VertexBufferObject.h 634 2010-10-13 07:04:05Z naohisa.sakamoto $
 */
/*****************************************************************************/
#ifndef KVS__VERTEX_BUFFER_OBJECT_H_INCLUDE
#define KVS__VERTEX_BUFFER_OBJECT_H_INCLUDE

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

    VertexBufferObject();
};

} // end of namespace kvs

#endif // KVS__GLEW__VERTEX_BUFFER_OBJECT_H_INCLUDE
