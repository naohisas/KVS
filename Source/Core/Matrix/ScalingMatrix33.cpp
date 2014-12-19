/*****************************************************************************/
/**
 *  @file   ScalingMatrix33.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: ScalingMatrix33.cpp 602 2010-08-19 02:43:34Z naohisa.sakamoto $
 */
/*****************************************************************************/
#include "ScalingMatrix33.h"

namespace kvs
{

template kvs::Matrix33<float> ScalingMatrix33<float>( const float sx, const float sy, const float sz );
template kvs::Matrix33<float> ScalingMatrix33<float>( const kvs::Vector3<float>& s );
template kvs::Matrix33<float> ScalingMatrix33<float>( const float s );

template kvs::Matrix33<double> ScalingMatrix33<double>( const double sx, const double sy, const double sz );
template kvs::Matrix33<double> ScalingMatrix33<double>( const kvs::Vector3<double>& s );
template kvs::Matrix33<double> ScalingMatrix33<double>( const double s );

} // end of namespace kvs
