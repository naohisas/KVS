/*****************************************************************************/
/**
 *  @file   ScalingMatrix33.cpp
 *  @author Naohisa Sakamoto
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
