/****************************************************************************/
/**
 *  @file   Vector3.cpp
 *  @author Naohisa Sakamoto
 */
/****************************************************************************/
#include "Vector3.h"
#include <kvs/Type>


namespace kvs
{

// Template instantiation.
template class Vector3<kvs::Int8>;
template class Vector3<kvs::UInt8>;
template class Vector3<kvs::Int16>;
template class Vector3<kvs::UInt16>;
template class Vector3<kvs::Int32>;
template class Vector3<kvs::UInt32>;
template class Vector3<kvs::Int64>;
template class Vector3<kvs::UInt64>;
template class Vector3<kvs::Real32>;
template class Vector3<kvs::Real64>;

} // end of namespace kvs
