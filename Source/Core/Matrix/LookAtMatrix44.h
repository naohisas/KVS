/*****************************************************************************/
/**
 *  @file   LookAtMatrix44.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: LookAtMatrix44.h 1421 2013-02-27 17:31:12Z s.yamada0808@gmail.com $
 */
/*****************************************************************************/
#pragma once
#include <cmath>
#include <kvs/Matrix44>
#include <kvs/Math>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Return look-at matrix.
 *  @param  eye [in] Camera(eye) position.
 *  @param  up [in] Camera's up-vector.
 *  @param  target [in] Target point.
 *  @return Look-at matrix.
 */
/*===========================================================================*/
template <typename T>
inline kvs::Matrix44<T> LookAtMatrix44(
    const kvs::Vector3<T>& eye,
    const kvs::Vector3<T>& up,
    const kvs::Vector3<T>& target )
{
    kvs::Vector3<T> f = target - eye;
    kvs::Vector3<T> s = f.cross( up.normalized() );
    kvs::Vector3<T> u = s.cross( f );

    f.normalize();
    s.normalize();
    u.normalize();

    const T zero = T(0);
    const T one = T(1);
    const T elements[ 16 ] =
    {
         s.x(),  s.y(),  s.z(), zero,
         u.x(),  u.y(),  u.z(), zero,
        -f.x(), -f.y(), -f.z(), zero,
          zero,   zero,   zero, one
    };

    return kvs::Matrix44<T>( elements );
}

} // end of namespace kvs
