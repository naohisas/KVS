/*****************************************************************************/
/**
 *  @file   IndexBufferObject.cpp
 *  @author Naohisa Sakamoto
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
