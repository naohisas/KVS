/*****************************************************************************/
/**
 *  @file   PerspectiveMatrix44.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: PerspectiveMatrix44.cpp 602 2010-08-19 02:43:34Z naohisa.sakamoto $
 */
/*****************************************************************************/
#include "PerspectiveMatrix44.h"

namespace kvs
{

template kvs::Matrix44<float> PerspectiveMatrix44<float>(
    const float fov,
    const float aspect,
    const float near,
    const float far );

template kvs::Matrix44<double> PerspectiveMatrix44<double>(
    const double fov,
    const double aspect,
    const double near,
    const double far );

} // end of namespace kvs
