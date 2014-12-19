/*****************************************************************************/
/**
 *  @file   LookAtMatrix44.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: LookAtMatrix44.cpp 602 2010-08-19 02:43:34Z naohisa.sakamoto $
 */
/*****************************************************************************/
#include "LookAtMatrix44.h"


namespace kvs
{

template kvs::Matrix44<float> LookAtMatrix44<float>(
    const kvs::Vector3<float>& eye,
    const kvs::Vector3<float>& up,
    const kvs::Vector3<float>& target );

template kvs::Matrix44<double> LookAtMatrix44<double>(
    const kvs::Vector3<double>& eye,
    const kvs::Vector3<double>& up,
    const kvs::Vector3<double>& target );

} // end of namespace kvs
