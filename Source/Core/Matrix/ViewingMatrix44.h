/*****************************************************************************/
/**
 *  @file   ViewingMatrix.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: ViewingMatrix44.h 1421 2013-02-27 17:31:12Z s.yamada0808@gmail.com $
 */
/*****************************************************************************/
#pragma once
#include <cmath>
#include <kvs/Matrix44>
#include <kvs/Vector3>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Return viewing matrix.
 *  @param  c [in] camera position in the world coordinate.
 *  @param  u [in] up-vector in the world coordinate
 *  @param  g [in] gaze point in the world coordinate
 *  @return Vewing matrix
 */
/*===========================================================================*/
template <typename T>
inline kvs::Matrix44<T> ViewingMatrix44(
    const kvs::Vector3<T>& c,
    const kvs::Vector3<T>& u,
    const kvs::Vector3<T>& g )
{
    kvs::Vector3<T> d = g - c;
    kvs::Vector3<T> r = d.cross( u );
    kvs::Vector3<T> f = r.cross( d );

    d.normalize();
    r.normalize();
    f.normalize();

    const T elements[ 16 ] =
    {
         r.x(),  r.y(),  r.z(), -r.dot( c ),
         f.x(),  f.y(),  f.z(), -f.dot( c ),
        -d.x(), -d.y(), -d.z(),  d.dot( c ),
             0,      0,      0,             1
    };

    return kvs::Matrix44<T>( elements );
}

} // end of namespace kvs
