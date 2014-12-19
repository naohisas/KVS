/*****************************************************************************/
/**
 *  @file   RotationMatrix33.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: RotationMatrix33.cpp 602 2010-08-19 02:43:34Z naohisa.sakamoto $
 */
/*****************************************************************************/
#include "RotationMatrix33.h"

namespace kvs
{

template kvs::Matrix33<float> XRotationMatrix33<float>( const float deg );
template kvs::Matrix33<float> YRotationMatrix33<float>( const float deg );
template kvs::Matrix33<float> ZRotationMatrix33<float>( const float deg );
template kvs::Matrix33<float> EulerRotationMatrix33<float>( const float alpha, const float beta, const float gamma );
template kvs::Matrix33<float> RPYRotationMatrix33<float>( const float roll, const float pitch, const float yaw );
template kvs::Matrix33<float> RotationMatrix33<float>( const kvs::Vector3<float>& axis, const float deg );

template kvs::Matrix33<double> XRotationMatrix33<double>( const double deg );
template kvs::Matrix33<double> YRotationMatrix33<double>( const double deg );
template kvs::Matrix33<double> ZRotationMatrix33<double>( const double deg );
template kvs::Matrix33<double> EulerRotationMatrix33<double>( const double alpha, const double beta, const double gamma );
template kvs::Matrix33<double> RPYRotationMatrix33<double>( const double roll, const double pitch, const double yaw );
template kvs::Matrix33<double> RotationMatrix33<double>( const kvs::Vector3<double>& axis, const double deg );

} // end of namespace kvs
