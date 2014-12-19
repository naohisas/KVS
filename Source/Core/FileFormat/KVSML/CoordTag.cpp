/*****************************************************************************/
/**
 *  @file   CoordTag.cpp
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: CoordTag.cpp 1342 2012-11-07 13:47:24Z s.yamada0808@gmail.com $
 */
/*****************************************************************************/
#include "CoordTag.h"


namespace kvs
{

namespace kvsml
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new coord tag class.
 */
/*===========================================================================*/
CoordTag::CoordTag():
    kvs::kvsml::TagBase( "Coord" )
{
}

} // end of namespace kvsml

} // end of namespace kvs
