/*****************************************************************************/
/**
 *  @file   ConnectionTag.cpp
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: ConnectionTag.cpp 1344 2012-11-07 14:59:01Z s.yamada0808@gmail.com $
 */
/*****************************************************************************/
#include "ConnectionTag.h"


namespace kvs
{

namespace kvsml
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new coord tag class.
 */
/*===========================================================================*/
ConnectionTag::ConnectionTag():
    kvs::kvsml::TagBase( "Connection" )
{
}

} // end of namespace kvsml

} // end of namespace kvs
