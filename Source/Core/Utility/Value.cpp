/****************************************************************************/
/**
 *  @file Value.cpp
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: Value.cpp 631 2010-10-10 02:15:35Z naohisa.sakamoto $
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
