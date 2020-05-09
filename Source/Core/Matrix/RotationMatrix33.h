/*****************************************************************************/
/**
 *  @file   RotationMatrix33.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: RotationMatrix33.h 1421 2013-02-27 17:31:12Z s.yamada0808@gmail.com $
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
 *  @brief  Returns x-axis rotation matrix (right-hand system).
 *  @param  deg [in] Rotation angle in degree.
 *  @return Rotation matrix.
 */
/*===========================================================================*/
template <typename T>
inline kvs::Matrix33<T> XRotationMatrix33( const T deg )
{
    const T rad  = kvs::Math::Deg2Rad( deg );
    const T sinA = static_cast<T>( sin( rad ) );
    const T cosA = static_cast<T>( cos( rad ) );

    const T elements[ 9 ] =
    {
        1,    0,     0,
        0, cosA, -sinA,
        0, sinA,  cosA
    };

    return kvs::Matrix33<T>( elements );
}

/*===========================================================================*/
/**
 *  @brief  Returns y-axis rotation matrix.
 *  @param  deg [in] Rotation angle ( degree ).
 *  @return Rotation matrix.
 */
/*===========================================================================*/
template <typename T>
inline kvs::Matrix33<T> YRotationMatrix33( const T deg )
{
    const T rad  = kvs::Math::Deg2Rad( deg );
    const T sinA = static_cast<T>( sin( rad ) );
    const T cosA = static_cast<T>( cos( rad ) );

    const T elements[ 9 ] =
    {
         cosA, 0, sinA,
            0, 1,    0,
        -sinA, 0, cosA
    };

    return kvs::Matrix33<T>( elements );
}

/*===========================================================================*/
/**
 *  @brief  Returns z-axis rotation matrix.
 *  @param  deg [in] Rotation angle ( degree ).
 *  @return Rotation matrix.
 */
/*===========================================================================*/
template <typename T>
inline kvs::Matrix33<T> ZRotationMatrix33( const T deg )
{
    const T rad  = kvs::Math::Deg2Rad( deg );
    const T sinA = static_cast<T>( sin( rad ) );
    const T cosA = static_cast<T>( cos( rad ) );

    const T elements[ 9 ] =
    {
         cosA, -sinA, 0,
         sinA,  cosA, 0,
            0,     0, 1
    };

    return kvs::Matrix33<T>( elements );
}

/*===========================================================================*/
/**
 *  @brief  Returns euler rotation matrix.
 *  @param  alpha [in] alpha ( degree ).
 *  @param  beta  [in] beta ( degree ).
 *  @param  gamma [in] gamma ( degree ).
 *  @return Rotation matrix.
 */
/*===========================================================================*/
template <typename T>
inline kvs::Matrix33<T> EulerRotationMatrix33( const T alpha, const T beta, const T gamma )
{
    const T alpha_rad = kvs::Math::Deg2Rad( alpha );
    const T beta_rad  = kvs::Math::Deg2Rad( beta );
    const T gamma_rad = kvs::Math::Deg2Rad( gamma );

    const T sinA = static_cast<T>( std::sin( alpha_rad ) );
    const T cosA = static_cast<T>( std::cos( alpha_rad ) );
    const T sinB = static_cast<T>( std::sin( beta_rad )  );
    const T cosB = static_cast<T>( std::cos( beta_rad )  );
    const T sinC = static_cast<T>( std::sin( gamma_rad ) );
    const T cosC = static_cast<T>( std::cos( gamma_rad ) );

    const T elements[ 9 ] =
    {
        cosA * cosB * cosC - sinA * sinC, -cosA * cosB * sinC - sinA * cosC, cosA * sinB,
        sinA * cosB * cosC + cosA * sinC, -sinA * cosB * sinC + cosA * cosC, sinA * sinB,
                            -sinB * cosC,                      -sinB * sinC,        cosB
    };

    return kvs::Matrix33<T>( elements );
}

/*===========================================================================*/
/**
 *  @brief  Returns roll-pitch-yaw rotation matrix.
 *  @param  roll  [in] Rolling angle ( degree ).
 *  @param  pitch [in] Pitching angle ( degree ).
 *  @param  yaw   [in] Yawing angle ( degree ).
 *  @return Rotation matrix.
 */
/*===========================================================================*/
template <typename T>
inline kvs::Matrix33<T> RPYRotationMatrix33( const T roll, const T pitch, const T yaw )
{
    const T roll_rad  = kvs::Math::Deg2Rad( roll );
    const T pitch_rad = kvs::Math::Deg2Rad( pitch );
    const T yaw_rad   = kvs::Math::Deg2Rad( yaw );

    const T sinA = static_cast<T>( std::sin( roll_rad ) );
    const T cosA = static_cast<T>( std::cos( roll_rad ) );
    const T sinB = static_cast<T>( std::sin( pitch_rad )  );
    const T cosB = static_cast<T>( std::cos( pitch_rad )  );
    const T sinC = static_cast<T>( std::sin( yaw_rad ) );
    const T cosC = static_cast<T>( std::cos( yaw_rad ) );

    const T elements[ 9 ] =
    {
        cosA * cosB, cosA * sinB * sinC - sinA * cosC, cosA * sinB * cosC + sinA * sinC,
        sinA * cosB, sinA * sinB * sinC + cosA * cosC, sinA * sinB * cosC - cosA * sinC,
              -sinB,                      cosB * sinC,                      cosB * cosC
    };

    return kvs::Matrix33<T>( elements );
}

/*===========================================================================*/
/**
 *  @brief  Returns rotation matrix with an arbitrary axis.
 *  @param  axis [in] Rotation axis.
 *  @param  deg  [in] Rotation angle ( degree ).
 *  @return Rotation matrix.
 */
/*===========================================================================*/
template <typename T>
inline kvs::Matrix33<T> RotationMatrix33( const kvs::Vector3<T>& axis, const T deg )
{
    const T rad      = kvs::Math::Deg2Rad( deg );
    const T sinA     = std::sin( rad );
    const T cosA     = std::cos( rad );
    const T inv_cosA = 1 - cosA;

    const kvs::Vector3<T> v = axis.normalized();

    const T x = v.x();
    const T y = v.y();
    const T z = v.z();

    const T elements[ 9 ] =
    {
        inv_cosA * x * x + cosA    , inv_cosA * x * y - sinA * z, inv_cosA * x * z + sinA * y,
        inv_cosA * x * y + sinA * z, inv_cosA * y * y + cosA    , inv_cosA * y * z - sinA * x,
        inv_cosA * x * z - sinA * y, inv_cosA * y * z + sinA * x, inv_cosA * z * z + cosA
    };

    return kvs::Matrix33<T>( elements );
}

} // end of namespace kvs
