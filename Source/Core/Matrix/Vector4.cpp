/****************************************************************************/
/**
 *  @file   Vector4.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: Vector4.cpp 1348 2012-11-13 07:05:59Z s.yamada0808@gmail.com $
 */
/****************************************************************************/
#include "Vector4.h"
#include <kvs/Type>


namespace kvs
{

// Template instantiation.
template class Vector4<kvs::Int8>;
template class Vector4<kvs::UInt8>;
template class Vector4<kvs::Int16>;
template class Vector4<kvs::UInt16>;
template class Vector4<kvs::Int32>;
template class Vector4<kvs::UInt32>;
template class Vector4<kvs::Int64>;
template class Vector4<kvs::UInt64>;
template class Vector4<kvs::Real32>;
template class Vector4<kvs::Real64>;

} // end of namespace kvs
