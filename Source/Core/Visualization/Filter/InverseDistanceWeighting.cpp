/*****************************************************************************/
/**
 *  @file   InverseDistanceWeighting.cpp
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
#include "InverseDistanceWeighting.h"


namespace kvs
{

template class InverseDistanceWeighting<kvs::Real32>;
template class InverseDistanceWeighting<kvs::Vec3>;
template class InverseDistanceWeighting<kvs::Mat3>;

} // end of namespace kvs
