/****************************************************************************/
/**
 *  @file   Value.cpp
 *  @author Naohisa Sakamoto
 */
/****************************************************************************/
#include "Value.h"
#include <kvs/Type>


namespace kvs
{

// Template specialization.
template class Value<kvs::Int8>;
template class Value<kvs::UInt8>;
template class Value<kvs::Int16>;
template class Value<kvs::UInt16>;
template class Value<kvs::Int32>;
template class Value<kvs::UInt32>;
template class Value<kvs::Int64>;
template class Value<kvs::UInt64>;
template class Value<kvs::Real32>;
template class Value<kvs::Real64>;

} // end of namespace kvs
