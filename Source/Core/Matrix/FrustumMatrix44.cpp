/*****************************************************************************/
/**
 *  @file   FrustumMatrix44.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: FrustumMatrix44.cpp 602 2010-08-19 02:43:34Z naohisa.sakamoto $
 */
/*****************************************************************************/
#include "FrustumMatrix44.h"


namespace kvs
{

template kvs::Matrix44<float> FrustumMatrix44<float>(
    const float left,
    const float right,
    const float bottom,
    const float top,
    const float near,
    const float far );

template kvs::Matrix44<double> FrustumMatrix44<double>(
    const double left,
    const double right,
    const double bottom,
    const double top,
    const double near,
    const double far );

} // end of namespace kvs
