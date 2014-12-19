/*****************************************************************************/
/**
 *  @file   VertexBufferObject.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: VertexBufferObject.cpp 602 2010-08-19 02:43:34Z naohisa.sakamoto $
 */
/*****************************************************************************/
#include "VertexBufferObject.h"


namespace kvs
{

/*===========================================================================*/
/**
 *  Construct a new VertexBufferObject.
 */
/*===========================================================================*/
VertexBufferObject::VertexBufferObject():
    kvs::BufferObject( GL_ARRAY_BUFFER, GL_ARRAY_BUFFER_BINDING, GL_DYNAMIC_DRAW )
{
}

} // end of namespace kvs
