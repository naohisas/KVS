/*****************************************************************************/
/**
 *  @file   DataValueTag.cpp
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: DataValueTag.cpp 1344 2012-11-07 14:59:01Z s.yamada0808@gmail.com $
 */
/*****************************************************************************/
#include "DataValueTag.h"
#include <kvs/IgnoreUnusedVariable>


namespace kvs
{

namespace kvsml
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new data value class.
 */
/*===========================================================================*/
DataValueTag::DataValueTag():
    kvs::kvsml::TagBase( "DataValue" )
{
}

} // end of namespace kvsml

} // end of namespace kvs
