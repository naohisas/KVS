/*****************************************************************************/
/**
 *  @file   VertexBufferObject.cpp
 *  @author Naohisa Sakamoto
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
