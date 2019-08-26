/*****************************************************************************/
/**
 *  @file   FrustumMatrix44.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: FrustumMatrix44.h 1421 2013-02-27 17:31:12Z s.yamada0808@gmail.com $
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
 *  @brief  Return view-frustum matrix (right-hand system).
 *  @param  left [in] Left position.
 *  @param  right [in] Right position.
 *  @param  bottom [in] Bottom position.
 *  @param  top [in] Top position.
 *  @param  near [in] Near position.
 *  @param  far [in] Far position.
 *  @return View-frustum matrix.
 */
/*===========================================================================*/
template <typename T>
inline kvs::Matrix44<T> FrustumMatrix44(
    const T left,
    const T right,
    const T bottom,
    const T top,
    const T znear,
    const T zfar )
{
    KVS_ASSERT( !( kvs::Math::IsZero( right - left ) ) );
    KVS_ASSERT( !( kvs::Math::IsZero( top - bottom ) ) );
    KVS_ASSERT( !( kvs::Math::IsZero( zfar - znear ) ) );

    const T elements[16] =
    {
        2 * znear / ( right - left ),                            0,      ( right + left ) / ( right - left ),                                    0,
                                   0, 2 * znear / ( top - bottom ),      ( top + bottom ) / ( top - bottom ),                                    0,
                                   0,                            0,     -( zfar + znear ) / ( zfar - znear ), -2 * zfar * znear / ( zfar - znear ),
                                   0,                            0,                                       -1,                                    0
    };

    return kvs::Matrix44<T>( elements );
}

} // end of namespace kvs
