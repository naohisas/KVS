/*****************************************************************************/
/**
 *  @file   PerspectiveMatrix44.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: PerspectiveMatrix44.h 1421 2013-02-27 17:31:12Z s.yamada0808@gmail.com $
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
 *  @brief  Return perspective matrix (right-hand system).
 *  @param  fov [i] Field-of-view along a y-axis in degree.
 *  @param  aspect [in] Aspect ratio.
 *  @param  near [in] Near position.
 *  @param  far [in] Far position.
 */
/*===========================================================================*/
template <typename T>
inline kvs::Matrix44<T> PerspectiveMatrix44(
    const T fov,
    const T aspect,
    const T znear,
    const T zfar )
{
    const T rad  = kvs::Math::Deg2Rad( fov / 2 );
    const T sinA = std::sin( rad );
    const T cosA = std::cos( rad );

    KVS_ASSERT( !( kvs::Math::IsZero( sinA ) ) );
    KVS_ASSERT( !( kvs::Math::IsZero( aspect ) ) );
    KVS_ASSERT( !( kvs::Math::IsZero( zfar - znear ) ) );

    const T cotA = cosA / sinA;
    const T elements[16] =
    {
        cotA / aspect,    0,                                    0,                                        0,
                    0, cotA,                                    0,                                        0,
                    0,    0, -( zfar + znear ) / ( zfar - znear ), -( zfar * znear * 2 ) / ( zfar - znear ),
                    0,    0,                                   -1,                                        0
    };

    return kvs::Matrix44<T>( elements );
}

} // end of namespace kvs
