/*****************************************************************************/
/**
 *  @file   AnyValueTable.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id$
 */
/*****************************************************************************/
#include "AnyValueTable.h"
#include <kvs/Type>
#include <string>


namespace kvs
{

template AnyValueTable::AnyValueTable( const kvs::ValueTable<kvs::Int8>& table );
template AnyValueTable::AnyValueTable( const kvs::ValueTable<kvs::UInt8>& table );
template AnyValueTable::AnyValueTable( const kvs::ValueTable<kvs::Int16>& table );
template AnyValueTable::AnyValueTable( const kvs::ValueTable<kvs::UInt16>& table );
template AnyValueTable::AnyValueTable( const kvs::ValueTable<kvs::Int32>& table );
template AnyValueTable::AnyValueTable( const kvs::ValueTable<kvs::UInt32>& table );
template AnyValueTable::AnyValueTable( const kvs::ValueTable<kvs::Int64>& table );
template AnyValueTable::AnyValueTable( const kvs::ValueTable<kvs::UInt64>& table );
template AnyValueTable::AnyValueTable( const kvs::ValueTable<kvs::Real32>& table );
template AnyValueTable::AnyValueTable( const kvs::ValueTable<kvs::Real64>& table );
#if KVS_ENABLE_DEPRECATED
template AnyValueTable::AnyValueTable( const kvs::ValueTable<std::string>& table );
#endif

}
