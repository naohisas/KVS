/*****************************************************************************/
/**
 *  @file   ValueTable.cpp
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#include "ValueTable.h"
#include <kvs/Type>


namespace kvs
{

// Template specialization.
template class ValueTable<kvs::Int8>;
template class ValueTable<kvs::UInt8>;
template class ValueTable<kvs::Int16>;
template class ValueTable<kvs::UInt16>;
template class ValueTable<kvs::Int32>;
template class ValueTable<kvs::UInt32>;
template class ValueTable<kvs::Int64>;
template class ValueTable<kvs::UInt64>;
template class ValueTable<kvs::Real32>;
template class ValueTable<kvs::Real64>;

} // end of namespace kvs
