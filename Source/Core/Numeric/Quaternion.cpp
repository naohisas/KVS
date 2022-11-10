/****************************************************************************/
/**
 *  @file   Quaternion.cpp
 *  @author Naohisa Sakamoto
 */
/****************************************************************************/
#include "Quaternion.h"


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Returns zero quaternion.
 *  @return zero quaternion
 */
/*===========================================================================*/
const Quaternion Quaternion::Zero()
{
    return Quaternion( 0, 0, 0, 0 );
}

/*===========================================================================*/
/**
 *  @brief  Returns identity quaternion.
 *  @return identity quaternion
 */
/*===========================================================================*/
const Quaternion Quaternion::Identity()
{
    return Quaternion( 0, 0, 0, 1 );
}

/*===========================================================================*/
/**
 *  @brief  Returns rotated point.
 *  @param  pos [in] point position
 *  @param  axis [in] rotation axis
 *  @param  rad [in] rotation angle
 *  @return rotated point
 */
/*===========================================================================*/
const kvs::Vec3 Quaternion::Rotate(
    const kvs::Vec3& pos,
    const kvs::Vec3& axis,
    float rad )
{
    const Quaternion p( pos.x(), pos.y(), pos.z(), 0.0f );
    const Quaternion rotate_quat( axis, rad );
    const Quaternion rotate_conj = rotate_quat.conjugated();
    const Quaternion rotate_pos = rotate_quat * p * rotate_conj;
    return rotate_pos.m_elements.xyz();
}

/*===========================================================================*/
/**
 *  @brief  Returns rotated point.
 *  @param  pos [in] point position
 *  @param  q [in] quaternion
 *  @return rotated point
 */
/*===========================================================================*/
const kvs::Vec3 Quaternion::Rotate(
    const kvs::Vec3& pos,
    const kvs::Quaternion& q )
{
    const Quaternion p( pos.x(), pos.y(), pos.z(), 0.0f );
    const Quaternion rotate_conj = q.conjugated();
    const Quaternion rotate_pos = q * p * rotate_conj;
    return rotate_pos.m_elements.xyz();
}

/*===========================================================================*/
/**
 *  @brief  Returns rotation quaternion.
 *  @param  v0 [in] point 0
 *  @param  v1 [in] point 1
 *  @return rotation quaternion
 */
/*===========================================================================*/
const Quaternion Quaternion::RotationQuaternion(
    kvs::Vec3 v0,
    kvs::Vec3 v1 )
{
    Quaternion q;

    v0.normalize();
    v1.normalize();

    kvs::Vec3 c = v0.cross( v1 );
    float d = v0.x() * v1.x() + v0.y() * v1.y() + v0.z() * v1.z();
    double s = std::sqrt( double( ( 1 + d ) * 2.0 ) );

    q.x() = float( c.x() / s );
    q.y() = float( c.y() / s );
    q.z() = float( c.z() / s );
    q.w() = float( s / 2.0 );

    return q;
}

const Quaternion Quaternion::LinearInterpolation(
    const Quaternion& q1,
    const Quaternion& q2,
    double t,
    bool normalize )
{
    auto q = q1 + float(t) * ( q2 - q1 );
    if ( normalize ) { q.normalize(); }
    return q;
}

const Quaternion Quaternion::SplineInterpolation(
    const Quaternion& qn_m1,
    const Quaternion& qn,
    const Quaternion& qn_p1 )
{
    auto tmp_m1 = qn_m1; tmp_m1.normalize();
    auto tmp_p1 = qn_p1; tmp_p1.normalize();
    auto qn_i = qn.conjugated(); qn_i.normalize();
    return qn * ( ( ( qn_i * tmp_m1 ).log() + ( qn_i * tmp_p1 ).log() ) * -0.25f ).exp();
}

const Quaternion Quaternion::SphericalLinearInterpolation(
    const Quaternion& q1,
    const Quaternion& q2,
    double t,
    bool invert,
    bool normalize )
{
    auto tmp1 = q1; tmp1.normalize();
    auto tmp2 = q2; tmp2.normalize();
    double dot = tmp1.dot( tmp2 );

    // dot = cos( theta )
    // if (dot < 0), q1 and q2 are more than 90 degrees apart,
    // so we can invert one to reduce spining
    Quaternion q3;
    if ( invert && dot < 0 ) { q3 = -q2; dot = -dot; }
    else { q3 = q2; }

    const float phi = std::acos( dot );
    if ( phi > 0 && phi < kvs::Math::pi )
//    if ( (  invert && dot < 0.95 ) ||
//         ( !invert && dot > -0.95 && dot < 0.95 ) )
    {
//        double angle   = std::acos( dot );
//        double sina    = std::sin( angle );
//        double sinat   = std::sin( angle * t );
//        double sinaomt = std::sin( angle * (1-t) );
        const float sin_p = std::sin( phi );
        const float sin_pt = std::sin( phi * t );
        const float sin_p1t = std::sin( phi * ( 1 - t ) );
        auto q = ( q1 * sin_p1t + q3 * sin_pt ) / sin_p;
        if ( normalize ) { q.normalize(); }
        return q;
    }
    // if the angle is small, use linear interpolation
    else
    {
        return Quaternion::LinearInterpolation( q1, q3, t, normalize );
    }
}

const Quaternion Quaternion::SphericalCubicInterpolation(
    const Quaternion& q1,
    const Quaternion& q2,
    const Quaternion& a,
    const Quaternion& b,
    double t,
    bool normalize )
{
    auto c = Quaternion::SphericalLinearInterpolation( q1, q2, t, false, normalize );
    auto d = Quaternion::SphericalLinearInterpolation(  a,  b, t, false, normalize );
    return Quaternion::SphericalLinearInterpolation( c, d, 2.0 * t * ( 1 - t ), false, normalize );
}

const Quaternion Quaternion::SphericalQuadrangleInterpolation(
    const Quaternion& q1,
    const Quaternion& q2,
    const Quaternion& q3,
    const Quaternion& q4,
    double t,
    bool normalize )
{
    auto a = Quaternion::SplineInterpolation( q1, q2, q3 );
    auto b = Quaternion::SplineInterpolation( q2, q3, q4 );
    return Quaternion::SphericalCubicInterpolation( q2, q3, a, b, t, normalize );
}

} // end of namespace kvs
