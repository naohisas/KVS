/*****************************************************************************/
/**
 *  @file   ScalingMatrix33.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: ScalingMatrix33.h 1421 2013-02-27 17:31:12Z s.yamada0808@gmail.com $
 */
/*****************************************************************************/
#pragma once
#include <cmath>
#include <kvs/Matrix33>
#include <kvs/Math>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Returns a scaling matrix.
 *  @param  sx [in] Scaling factor of x-direction.
 *  @param  sy [in] Scaling factor of y-direction.
 *  @param  sz [in] Scaling factor of z-direction.
 *  @return Scaling matrix.
 */
/*===========================================================================*/
template <typename T>
inline kvs::Matrix33<T> ScalingMatrix33( const T sx, const T sy, const T sz )
{
    const T zero = T(0);
    const T elements[ 9 ] =
    {
        sx,   zero, zero,
        zero,   sy, zero,
        zero, zero, sz
    };

    return kvs::Matrix33<T>( elements );
}

/*===========================================================================*/
/**
 *  @brief  Returns scaling matrix.
 *  @param  s [in] Scaling factor vector.
 *  @return Scaling matrix.
 */
/*===========================================================================*/
template <typename T>
inline kvs::Matrix33<T> ScalingMatrix33( const kvs::Vector3<T>& s )
{
    return kvs::ScalingMatrix33<T>( s.x(), s.y(), s.z() );
}

/*===========================================================================*/
/**
 *  @brief  Returns scaling matrix.
 *  @param  s [in] Scaling factor.
 *  @return Scaling matrix.
 */
/*===========================================================================*/
template <typename T>
inline kvs::Matrix33<T> ScalingMatrix33( const T s )
{
    return kvs::ScalingMatrix33<T>( s, s, s );
}

} // end of namespace kvs
