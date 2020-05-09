/****************************************************************************/
/**
 *  @file   Quaternion.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: Quaternion.h 1716 2014-02-06 09:30:46Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#ifndef KVS__QUATERNION_H_INCLUDE
#define KVS__QUATERNION_H_INCLUDE

#include <iostream>
#include <iomanip>
#include <kvs/Math>
#include <kvs/Assert>
#include <kvs/Vector4>
#include <kvs/Matrix33>
#include <kvs/Matrix44>
#include <kvs/Deprecated>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Quaternion class.
 */
/*===========================================================================*/
class Quaternion
{
private:

    // Quaternion: Q = w + xi + yj + zk
    //    x = m_elements[0],
    //    y = m_elements[1],
    //    z = m_elements[2],
    //    w = m_elements[3]
    kvs::Vec4 m_elements; ///< elements of quaternion

public:

    static const Quaternion Zero();
    static const Quaternion Identity();
    static const kvs::Vec3 Rotate( const kvs::Vec3& pos, const kvs::Vec3& axis, float rad );
    static const kvs::Vec3 Rotate( const kvs::Vec3& pos, const Quaternion& q );
    static const Quaternion RotationQuaternion( kvs::Vec3 v0, kvs::Vec3 v1 );
    static const Quaternion LinearInterpolation( const Quaternion& q1, const Quaternion& q2, double t, bool for_rotation );
    static const Quaternion SphericalLinearInterpolation( const Quaternion& q1, const Quaternion& q2, double t, bool invert, bool for_rotation );
    static const Quaternion SphericalCubicInterpolation( const Quaternion& q1, const Quaternion& q2, const Quaternion& a, const Quaternion& b, double t, bool for_rotation );
    static const Quaternion SplineInterpolation( const Quaternion& q1, const Quaternion& q2, const Quaternion& q3, const Quaternion& q4, double t, bool for_rotation );
    static const Quaternion Spline( const Quaternion& qnm1, const Quaternion& qn, const Quaternion& qnp1 );

public:

    Quaternion();
    Quaternion( float x, float y, float z, float w );
    Quaternion( const kvs::Vec3& axis, float angle );
    explicit Quaternion( const kvs::Mat3& mat );
    explicit Quaternion( const float elements[4] );

public:

    void set( float x, float y, float z, float w ) { m_elements.set( x, y, z, w ); }

    float& x() { return m_elements[0]; }
    float& y() { return m_elements[1]; }
    float& z() { return m_elements[2]; }
    float& w() { return m_elements[3]; }
    float x() const { return m_elements[0]; }
    float y() const { return m_elements[1]; }
    float z() const { return m_elements[2]; }
    float w() const { return m_elements[3]; }

    void zero() { *this = Zero(); }
    void identity() { *this = Identity(); }
    void conjugate();
    void normalize();
    void invert();

    Quaternion conjugated() const;
    Quaternion normalized() const;
    Quaternion inverted() const;
    Quaternion log() const;
    Quaternion exp() const;

    double dot( const Quaternion& q ) const { return m_elements.dot( q.m_elements ); }
    double length() const { return m_elements.length(); }
    double squaredLength() const { return m_elements.squaredLength(); }

    void toMatrix( kvs::Mat3& m ) const;
    void toMatrix( kvs::Mat4& m ) const;
    void toMatrix( float m[16] ) const;
    kvs::Mat3 toMatrix() const;
    kvs::Vec3 axis() const;
    float angle() const;

public:
    float& operator [] ( size_t index )
    {
        return m_elements[ index ];
    }

    const float& operator [] ( size_t index ) const
    {
        return m_elements[ index ];
    }

    Quaternion& operator += ( const Quaternion& q )
    {
        m_elements += q.m_elements;
        return *this;
    }

    Quaternion& operator -= ( const Quaternion& q )
    {
        m_elements -= q.m_elements;
        return *this;
    }

    Quaternion& operator *= ( float a )
    {
        m_elements *= a;
        return *this;
    }

    Quaternion& operator /= ( float a )
    {
        m_elements /= a;
        return *this;
    }

    Quaternion& operator *= ( const Quaternion& q )
    {
        float x = this->x();
        float y = this->y();
        float z = this->z();
        float w = this->w();

        this->set( w * q.x() + x * q.w() + y * q.z() - z * q.y(),
                   w * q.y() - x * q.z() + y * q.w() + z * q.x(),
                   w * q.z() + x * q.y() - y * q.x() + z * q.w(),
                   w * q.w() - x * q.x() - y * q.y() - z * q.z() );

        return *this;
    }

    friend bool operator == ( const Quaternion& a, const Quaternion& b )
    {
        return a.m_elements == b.m_elements;
    }

    friend bool operator != ( const Quaternion& a, const Quaternion& b )
    {
        return !( a == b );
    }

    friend Quaternion operator - ( const Quaternion& a )
    {
        return Quaternion( -a[0], -a[1], -a[2], -a[3] );
    }

    friend Quaternion operator + ( const Quaternion& a, const Quaternion& b )
    {
        return Quaternion( a ) += b;
    }

    friend Quaternion operator - ( const Quaternion& a, const Quaternion& b )
    {
        return Quaternion( a ) -= b;
    }

    friend Quaternion operator * ( const Quaternion& q, float s )
    {
        return Quaternion( q ) *= s;
    }

    friend Quaternion operator * ( float s, const Quaternion& q )
    {
        return Quaternion( q ) *= s;
    }

    friend Quaternion operator / ( const Quaternion& q, float s )
    {
        return Quaternion( q ) /= s;
    }

    friend Quaternion operator * ( const Quaternion& a, const Quaternion& b )
    {
        return Quaternion( a ) *= b;
    }

    friend std::ostream& operator << ( std::ostream& os, const Quaternion& q )
    {
        const size_t width     = 8;
        const size_t precision = 3;

        const std::ios_base::fmtflags original_flags( os.flags() );

        os.setf( std::ios::fixed );
        os.setf( std::ios::showpoint );

        os << std::setw(width) << std::setprecision(precision) << q.w() << " + ";
        os << std::setw(width) << std::setprecision(precision) << q.x() << " i + ";
        os << std::setw(width) << std::setprecision(precision) << q.y() << " j + ";
        os << std::setw(width) << std::setprecision(precision) << q.z() << " k";

        os.flags( original_flags );

        return os;
    }

public:

    KVS_DEPRECATED( static Vector3<float> rotate(
        const kvs::Vec3& pos,
        const kvs::Vec3& axis, float rad ) )
    {
        return Quaternion::Rotate( pos, axis, rad );
    }

    KVS_DEPRECATED( static Vector3<float> rotate(
        const kvs::Vec3& pos,
        const Quaternion& q ) )
    {
        return Quaternion::Rotate( pos, q );
    }

    KVS_DEPRECATED( static Quaternion rotationQuaternion(
        kvs::Vec3 v0,
        kvs::Vec3 v1 ) )
    {
        return Quaternion::RotationQuaternion( v0, v1 );
    }

    KVS_DEPRECATED( static Quaternion linearInterpolation(
        const Quaternion& q1,
        const Quaternion& q2,
        double t,
        bool for_rotation ) )
    {
        return Quaternion::LinearInterpolation( q1, q2, t, for_rotation );
    }

    KVS_DEPRECATED( static Quaternion sphericalLinearInterpolation(
        const Quaternion& q1,
        const Quaternion& q2,
        double t,
        bool invert,
        bool for_rotation ) )
    {
        return Quaternion::SphericalLinearInterpolation( q1, q2, t, invert, for_rotation );
    }

    KVS_DEPRECATED( static Quaternion sphericalCubicInterpolation(
        const Quaternion& q1,
        const Quaternion& q2,
        const Quaternion& a,
        const Quaternion& b,
        double t,
        bool for_rotation ) )
    {
        return Quaternion::SphericalCubicInterpolation( q1, q2, a, b, t, for_rotation );
    }

    KVS_DEPRECATED( static Quaternion splineInterpolation(
        const Quaternion& q1,
        const Quaternion& q2,
        const Quaternion& q3,
        const Quaternion& q4,
        double t,
        bool for_rotation ) )
    {
        return Quaternion::SplineInterpolation( q1, q2, q3, q4, t, for_rotation );
    }

    KVS_DEPRECATED( static Quaternion spline(
        const Quaternion& qnm1,
        const Quaternion& qn,
        const Quaternion& qnp1 ) )
    {
        return Quaternion::Spline( qnm1, qn, qnp1 );
    }

    KVS_DEPRECATED( double length2() const ) { return this->squaredLength(); }
};

/*===========================================================================*/
/**
 *  @brief  Constructs a new Quaternion class.
 */
/*===========================================================================*/
inline Quaternion::Quaternion():
    m_elements( 0, 0, 0, 0 )
{
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new Quaternion class.
 *  @param  x [in] element of i-term
 *  @param  y [in] element of j-term
 *  @param  z [in] element of k-term
 *  @param  w [in] real number
 */
/*===========================================================================*/
inline Quaternion::Quaternion( float x, float y, float z, float w ):
    m_elements( x, y, z, w )
{
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new Quaternion class.
 *  @param  axis [in] rotation axis
 *  @param  angle [in] rotation angle
 */
/*===========================================================================*/
inline Quaternion::Quaternion( const kvs::Vector3<float>& axis, float angle )
{
    float s = static_cast<float>( std::sin( angle * 0.5 ) );
    float w = static_cast<float>( std::cos( angle * 0.5 ) );

    kvs::Vec3 n = axis.normalized();
    kvs::Vec3 v = s * n;
    m_elements.set( v, w );
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new Quaternion class.
 *  @param  m [in] rotation matrix
 */
/*===========================================================================*/
inline Quaternion::Quaternion( const kvs::Matrix33<float>& m )
{
    double trace = double( m.trace() + 1.0 );

    float x, y, z, w;
    if ( trace >= 1.0 )
    {
        double sqrt_trace = std::sqrt( trace );

        x = float( ( m[1][2] - m[2][1] ) * 0.5 / sqrt_trace );
        y = float( ( m[2][0] - m[0][2] ) * 0.5 / sqrt_trace );
        z = float( ( m[0][1] - m[1][0] ) * 0.5 / sqrt_trace );
        w = float( sqrt_trace * 0.5 );
    }
    else
    {
        if ( m[0][0] > m[1][1] && m[0][0] > m[2][2] )
        {
            x = float( std::sqrt( double( m[0][0] - m[1][1] - m[2][2] ) + 1.0 ) * 0.5 );
            y = float( ( m[0][1] + m[1][0] ) * 0.25 / x );
            z = float( ( m[0][2] + m[2][0] ) * 0.25 / x );
            w = float( ( m[1][2] + m[2][1] ) * 0.25 / x );
        }
        else if ( m[1][1] > m[2][2] )
        {
            y = float( std::sqrt( double( m[1][1] - m[2][2] - m[0][0] ) + 1.0 ) * 0.5 );
            z = float( ( m[1][2] + m[2][1] ) * 0.25 / y );
            x = float( ( m[1][0] + m[0][1] ) * 0.25 / y );
            w = float( ( m[2][0] + m[0][2] ) * 0.25 / y );
        }
        else
        {
            z = float( std::sqrt( double( m[2][2] - m[0][0] - m[1][1] ) + 1.0 ) * 0.5 );
            x = float( ( m[2][0] + m[0][2] ) * 0.25 / z );
            y = float( ( m[2][1] + m[1][2] ) * 0.25 / z );
            w = float( ( m[0][1] + m[1][0] ) * 0.25 / z );
        }
    }
    m_elements.set( x, y, z, w );
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new Quaternion class.
 *  @param  elements [in] elements of rotation matrix
 */
/*===========================================================================*/
inline Quaternion::Quaternion( const float elements[4] ) :
    m_elements( elements )
{
}

/*===========================================================================*/
/**
 *  @brief  Conjugates the quaternion.
 */
/*===========================================================================*/
inline void Quaternion::conjugate()
{
    m_elements[0] *= -1.0f;
    m_elements[1] *= -1.0f;
    m_elements[2] *= -1.0f;
}

/*===========================================================================*/
/**
 *  @brief  Normalizes the quaternion.
 */
/*===========================================================================*/
inline void Quaternion::normalize()
{
    float n = static_cast<float>( this->length() );
    if ( n > 0 ) { m_elements /= n; }
}

/*===========================================================================*/
/**
 *  @brief  Inverts the quaternion.
 */
/*===========================================================================*/
inline void Quaternion::invert()
{
    float n = static_cast<float>( this->squaredLength() );
    if ( n > 0 ) { this->conjugate(); m_elements /= n; }
}

/*===========================================================================*/
/**
 *  @brief  Returns conjugated quaternion.
 *  @return conjugated quaternion
 */
/*===========================================================================*/
inline Quaternion Quaternion::conjugated() const
{
    Quaternion result( *this );
    result.conjugate();
    return result;
}

/*===========================================================================*/
/**
 *  @brief  Returns normalized quaternion.
 *  @return normalized quaternion
 */
/*===========================================================================*/
inline Quaternion Quaternion::normalized() const
{
    Quaternion result( *this );
    result.normalize();
    return result;
}

/*===========================================================================*/
/**
 *  @brief  Returns inverted quaternion.
 *  @return inverted quaternion
 */
/*===========================================================================*/
inline Quaternion Quaternion::inverted() const
{
    Quaternion result( *this );
    result.invert();
    return result;
}

/*===========================================================================*/
/**
 *  @brief  Returns logarithm of quaternion.
 *  @return logarithm of quaternion
 */
/*===========================================================================*/
inline Quaternion Quaternion::log() const
{
    Quaternion result( *this );

    double theta = std::acos( double( result.w() ) );
    double sin_theta = std::sin( theta );
    if ( sin_theta > 0 )
    {
        result.x() = float( theta * result.x() / sin_theta );
        result.y() = float( theta * result.y() / sin_theta );
        result.z() = float( theta * result.z() / sin_theta );
        result.w() = 0;
    }
    else
    {
        result.x() = 0;
        result.y() = 0;
        result.z() = 0;
        result.w() = 0;
    }

    return result;
}

/*===========================================================================*/
/**
 *  @brief  Returns exponential of quaternion.
 *  @return exponential of quaternion
 */
/*===========================================================================*/
inline Quaternion Quaternion::exp() const
{
    Quaternion result( *this );

    double theta2 = m_elements.xyz().squaredLength();
    double theta = std::sqrt( theta2 );
    double cos_theta = std::cos( theta );
    if ( theta > 0 )
    {
        double sin_theta = std::sin( theta );
        result.x() = float( sin_theta * result.x() / theta );
        result.y() = float( sin_theta * result.y() / theta );
        result.z() = float( sin_theta * result.z() / theta );
        result.w() = float( cos_theta );
    }
    else
    {
        result.x() = 0;
        result.y() = 0;
        result.z() = 0;
        result.w() = float( cos_theta );
    }

    return *this;
}

/*===========================================================================*/
/**
 *  @brief  Calculates 3x3 matrix of the quaternion.
 *  @param  m [in/out] 3x3 matrix
 */
/*===========================================================================*/
inline void Quaternion::toMatrix( kvs::Mat3& m ) const
{
    float length_2 = static_cast<float>( this->squaredLength() );
    float s = ( length_2 > 0 ) ? float(2) / length_2 : 0;

    float xx = this->x() * this->x() * s;
    float yy = this->y() * this->y() * s;
    float zz = this->z() * this->z() * s;
    float xy = this->x() * this->y() * s;
    float xz = this->x() * this->z() * s;
    float yz = this->y() * this->z() * s;
    float wx = this->w() * this->x() * s;
    float wy = this->w() * this->y() * s;
    float wz = this->w() * this->z() * s;

    m[0][0] = 1.0f - ( yy + zz );
    m[1][0] = xy + wz;
    m[2][0] = xz - wy;

    m[0][1] = xy - wz;
    m[1][1] = 1.0f - ( xx + zz );
    m[2][1] = yz + wx;

    m[0][2] = xz + wy;
    m[1][2] = yz - wx;
    m[2][2] = 1.0f - ( xx + yy );
}

/*===========================================================================*/
/**
 *  @brief  Calculates 4x4 matrix of the quaternion.
 *  @param  m [in/out] 4x4 matrix
 */
/*===========================================================================*/
inline void Quaternion::toMatrix( kvs::Mat4& m ) const
{
    float length_2 = static_cast<float>( this->squaredLength() );
    float s = ( length_2 > float(0) ) ? float(2) / length_2 : float(0);

    float xx = this->x() * this->x() * s;
    float yy = this->y() * this->y() * s;
    float zz = this->z() * this->z() * s;
    float xy = this->x() * this->y() * s;
    float xz = this->x() * this->z() * s;
    float yz = this->y() * this->z() * s;
    float wx = this->w() * this->x() * s;
    float wy = this->w() * this->y() * s;
    float wz = this->w() * this->z() * s;

    m[0][0] = 1.0f - ( yy + zz );
    m[1][0] = xy + wz;
    m[2][0] = xz - wy;
    m[3][0] = 0.0f;

    m[0][1] = xy - wz;
    m[1][1] = 1.0f - ( xx + zz );
    m[2][1] = yz + wx;
    m[3][1] = 0.0f;

    m[0][2] = xz + wy;
    m[1][2] = yz - wx;
    m[2][2] = 1.0f - ( xx + yy );
    m[3][2] = 0.0f;

    m[0][3] = 0.0f;
    m[1][3] = 0.0f;
    m[2][3] = 0.0f;
    m[3][3] = 1.0f;
}

/*===========================================================================*/
/**
 *  @brief  Calculates 4x4 matrix of the quaternion.
 *  @param  m [in/out] 4x4 matrix
 */
/*===========================================================================*/
inline void Quaternion::toMatrix( float m[16] ) const
{
    float length_2 = static_cast<float>( this->squaredLength() );
    float s = ( length_2 > 0 ) ? float(2) / length_2 : float(0);

    float xx = this->x() * this->x() * s;
    float yy = this->y() * this->y() * s;
    float zz = this->z() * this->z() * s;
    float xy = this->x() * this->y() * s;
    float xz = this->x() * this->z() * s;
    float yz = this->y() * this->z() * s;
    float wx = this->w() * this->x() * s;
    float wy = this->w() * this->y() * s;
    float wz = this->w() * this->z() * s;

    m[0]  = 1.0f - ( yy + zz );
    m[1]  = xy + wz;
    m[2]  = xz - wy;
    m[3]  = 0.0f;

    m[4]  = xy - wz;
    m[5]  = 1.0f - ( xx + zz );
    m[6]  = yz + wx;
    m[7]  = 0.0f;

    m[8]  = xz + wy;
    m[9]  = yz - wx;
    m[10] = 1.0f - ( xx + yy );
    m[11] = 0.0f;

    m[12] = 0.0f;
    m[13] = 0.0f;
    m[14] = 0.0f;
    m[15] = 1.0f;
}

/*===========================================================================*/
/**
 *  @brief  Returns 3x3 matrix of the quaternion.
 *  @return 3x3 matrix
 */
/*===========================================================================*/
inline kvs::Mat3 Quaternion::toMatrix() const
{
    kvs::Mat3 result; this->toMatrix( result );
    return result;
}

/*===========================================================================*/
/**
 *  @brief  Returns rotation axis.
 *  @return rotation axis
 */
/*===========================================================================*/
inline kvs::Vec3 Quaternion::axis() const
{
    float s = float( std::sin( double( this->angle() ) * 0.5 ) );
    return m_elements.xyz() / s;
}

/*===========================================================================*/
/**
 *  @brief  Returns rotation angle.
 *  @return rotation angle
 */
/*===========================================================================*/
inline float Quaternion::angle() const
{
    return float( std::acos( double( this->w() ) ) * 2.0 );
}

} // end of namespace kvs

#endif // KVS__QUATERNION_H_INCLUDE
