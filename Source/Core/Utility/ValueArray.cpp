/****************************************************************************/
/**
 *  @file   ValueArray.cpp
 *  @author Naohisa Sakamoto
 */
/****************************************************************************/
#include "ValueArray.h"
#include <kvs/Type>


namespace kvs
{

// Template specialization.
template class ValueArray<kvs::Int8>;
template class ValueArray<kvs::UInt8>;
template class ValueArray<kvs::Int16>;
template class ValueArray<kvs::UInt16>;
template class ValueArray<kvs::Int32>;
template class ValueArray<kvs::UInt32>;
template class ValueArray<kvs::Int64>;
template class ValueArray<kvs::UInt64>;
template class ValueArray<kvs::Real32>;
template class ValueArray<kvs::Real64>;

} // end of namespace kvs
