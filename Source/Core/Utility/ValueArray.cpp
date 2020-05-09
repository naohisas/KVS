/****************************************************************************/
/**
 *  @file ValueArray.cpp
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: ValueArray.cpp 1137 2012-05-10 00:32:24Z s.yamada0808@gmail.com $
 */
/****************************************************************************/
#include "ValueArray.h"
#include <kvs/Type>
#include <string>


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
//template class ValueArray<std::string>;

} // end of namespace kvs
