/*****************************************************************************/
/**
 *  @file   IndexBufferObject.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: IndexBufferObject.cpp 602 2010-08-19 02:43:34Z naohisa.sakamoto $
 */
/*****************************************************************************/
#include "IndexBufferObject.h"


namespace kvs
{

/*===========================================================================*/
/**
 *  Construct a new IndexBufferObject.
 */
/*===========================================================================*/
IndexBufferObject::IndexBufferObject():
    kvs::BufferObject( GL_ELEMENT_ARRAY_BUFFER, GL_ELEMENT_ARRAY_BUFFER_BINDING, GL_DYNAMIC_DRAW )
{
}

} // end of namespace kvs
