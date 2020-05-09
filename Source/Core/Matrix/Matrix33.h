/****************************************************************************/
/**
 *  @file   Matrix33.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: Matrix33.h 1757 2014-05-04 13:17:37Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#pragma once
#include <iostream>
#include <iomanip>
#include <sstream>
#include <kvs/Assert>
#include <kvs/Math>
#include <kvs/Indent>
#include <kvs/Vector3>
#include <kvs/Deprecated>


namespace kvs
{

/*==========================================================================*/
/**
 *  @brief  3x3 matrix class.
 */
/*==========================================================================*/
template<typename T>
class Matrix33
{
private:
    Vector3<T> m_data[3]; ///< Row vectors.

public:
    static const Matrix33 Zero() { Matrix33 m; m.setZero(); return m; }
    static const Matrix33 Ones() { Matrix33 m; m.setOnes(); return m; }
    static const Matrix33 Identity() { Matrix33 m; m.setIdentity(); return m; }
    static const Matrix33 Constant( const T x ) { Matrix33 m; m.setConstant(x); return m; }
    static const Matrix33 Diagonal( const T x ) { Matrix33 m; m.setDiagonal(x); return m; }
    static const Matrix33 Diagonal( const kvs::Vector3<T>& v ) { Matrix33 m; m.setDiagonal(v); return m; }
    static const Matrix33 Random() { Matrix33 m; m.setRandom(); return m; }
    static const Matrix33 Random( const kvs::UInt32 seed ) { Matrix33 m; m.setRandom( seed ); return m; }
    static const Matrix33 Random( const T min, const T max ) { Matrix33 m; m.setRandom( min, max ); return m; }
    static const Matrix33 Random( const T min, const T max, const kvs::UInt32 seed ) { Matrix33 m; m.setRandom( min, max, seed ); return m; }
    static const Matrix33 Rotation( const Vector3<T>& axis, const double deg );
    static const Matrix33 RotationX( const double deg );
    static const Matrix33 RotationY( const double deg );
    static const Matrix33 RotationZ( const double deg );

public:
    Matrix33();
    Matrix33(
        const T a00, const T a01, const T a02,
        const T a10, const T a11, const T a12,
        const T a20, const T a21, const T a22 );
    Matrix33(
        const Vector3<T>& v0,
        const Vector3<T>& v1,
        const Vector3<T>& v2 );
    explicit Matrix33( const T elements[9] );

    void set(
        const T a00, const T a01, const T a02,
        const T a10, const T a11, const T a12,
        const T a20, const T a21, const T a22 );
    void set(
        const Vector3<T>& v0,
        const Vector3<T>& v1,
        const Vector3<T>& v2 );
    void set( const T elements[9] );
    void setZero();
    void setOnes();
    void setIdentity();
    void setConstant( const T x );
    void setDiagonal( const T x );
    void setDiagonal( const kvs::Vector3<T>& v );
    void setRandom();
    void setRandom( const kvs::UInt32 seed );
    void setRandom( const T min, const T max );
    void setRandom( const T min, const T max, const kvs::UInt32 seed );

    void swap( Matrix33& other );
    void transpose();
    void invert( T* determinant = 0 );

    T trace() const;
    T determinant() const;
    const Matrix33 transposed() const;
    const Matrix33 inverted( T* determinant = 0 ) const;
    bool isSymmetric() const;
    bool isDiagonal() const;

    std::string format(
        const bool newline = false,
        const kvs::Indent& indent = kvs::Indent(0) ) const
    {
        return this->format( ", ", "[", "]", newline, indent );
    }

    std::string format(
        const std::string delim,
        const bool newline = false,
        const kvs::Indent& indent = kvs::Indent(0) ) const
    {
        return this->format( delim, "", "", newline, indent );
    }

    std::string format(
        const std::string bracket_l,
        const std::string bracket_r,
        const bool newline = false,
        const kvs::Indent& indent = kvs::Indent(0) ) const
    {
        return this->format( " ", bracket_l, bracket_r, newline, indent );
    }

    std::string format(
        const std::string delim,
        const std::string bracket_l,
        const std::string bracket_r,
        const bool newline = false,
        const kvs::Indent& indent = kvs::Indent(0) ) const;

public:
    const Vector3<T>& operator []( const size_t index ) const;
    Vector3<T>& operator []( const size_t index );
    Matrix33& operator +=( const Matrix33& rhs );
    Matrix33& operator -=( const Matrix33& rhs );
    Matrix33& operator *=( const Matrix33& rhs );
    Matrix33& operator *=( const T rhs );
    Matrix33& operator /=( const T rhs );
    const Matrix33 operator -() const;

    friend bool operator ==( const Matrix33& lhs, const Matrix33& rhs )
    {
        return lhs[0] == rhs[0] &&
               lhs[1] == rhs[1] &&
               lhs[2] == rhs[2];
    }

    friend bool operator !=( const Matrix33& lhs, const Matrix33& rhs )
    {
        return !( lhs == rhs );
    }

    friend const Matrix33 operator +( const Matrix33& lhs, const Matrix33& rhs )
    {
        return Matrix33( lhs ) += rhs;
    }

    friend const Matrix33 operator -( const Matrix33& lhs, const Matrix33& rhs )
    {
        return Matrix33( lhs ) -= rhs;
    }

    friend const Matrix33 operator *( const Matrix33& lhs, const Matrix33& rhs )
    {
        return Matrix33( lhs ) *= rhs;
    }

    friend const Vector3<T> operator *( const Matrix33& lhs, const Vector3<T>& rhs )
    {
        return Vector3<T>(
            lhs[0].dot( rhs ),
            lhs[1].dot( rhs ),
            lhs[2].dot( rhs ) );
    }

    friend const Vector3<T> operator *( const Vector3<T>& lhs, const Matrix33& rhs )
    {
        return Vector3<T>(
            lhs[0] * rhs[0][0] + lhs[1] * rhs[1][0] + lhs[2] * rhs[2][0],
            lhs[0] * rhs[0][1] + lhs[1] * rhs[1][1] + lhs[2] * rhs[2][1],
            lhs[0] * rhs[0][2] + lhs[1] * rhs[1][2] + lhs[2] * rhs[2][2] );
    }

    friend const Matrix33 operator *( const Matrix33& lhs, const T rhs )
    {
        return Matrix33( lhs ) *= rhs;
    }

    friend const Matrix33 operator *( const T lhs, const Matrix33& rhs )
    {
        return rhs * lhs;
    }

    friend const Matrix33 operator /( const Matrix33& lhs, const T rhs )
    {
        return Matrix33( lhs ) /= rhs;
    }

    friend std::ostream& operator <<( std::ostream& os, const Matrix33& rhs )
    {
        return os << rhs.format( " ", "", "", true );
    }

public:
    KVS_DEPRECATED( static const Matrix33 All( const T x ) ) { return Constant( x ); }
    KVS_DEPRECATED( explicit Matrix33( const T a ) ) { *this = Constant( a ); }
    KVS_DEPRECATED( void set( const T a ) ) { *this = Constant( a ); }
    KVS_DEPRECATED( void zero() ) { this->setZero(); }
    KVS_DEPRECATED( void identity() ) { this->setIdentity(); }
    KVS_DEPRECATED( void print() const ) { std::cout << *this << std::endl; }
};


/*==========================================================================*/
/**
 *  Type definition.
 */
/*==========================================================================*/
typedef Matrix33<float> Matrix33f;
typedef Matrix33<double> Matrix33d;
typedef Matrix33<float> Mat3;
typedef Matrix33<double> Mat3d;


template<typename T>
const Matrix33<T> Matrix33<T>::Rotation( const Vector3<T>& axis, const double deg )
{
    const T rad = static_cast<T>( kvs::Math::Deg2Rad( deg ) );
    const T sinA = static_cast<T>( std::sin( rad ) );
    const T cosA = static_cast<T>( std::cos( rad ) );
    const T inv_cosA = 1 - cosA;

    const kvs::Vector3<T> v = axis.normalized();
    const T x = v.x();
    const T y = v.y();
    const T z = v.z();

    return Matrix33(
        inv_cosA * x * x + cosA    , inv_cosA * x * y - sinA * z, inv_cosA * x * z + sinA * y,
        inv_cosA * x * y + sinA * z, inv_cosA * y * y + cosA    , inv_cosA * y * z - sinA * x,
        inv_cosA * x * z - sinA * y, inv_cosA * y * z + sinA * x, inv_cosA * z * z + cosA );
}

template <typename T>
const Matrix33<T> Matrix33<T>::RotationX( const double deg )
{
    const T rad = static_cast<T>( kvs::Math::Deg2Rad( deg ) );
    const T sinA = static_cast<T>( std::sin( rad ) );
    const T cosA = static_cast<T>( std::cos( rad ) );

    return Matrix33(
        1,    0,     0,
        0, cosA, -sinA,
        0, sinA,  cosA );
}

template <typename T>
const Matrix33<T> Matrix33<T>::RotationY( const double deg )
{
    const T rad = static_cast<T>( kvs::Math::Deg2Rad( deg ) );
    const T sinA = static_cast<T>( std::sin( rad ) );
    const T cosA = static_cast<T>( std::cos( rad ) );

    return Matrix33(
         cosA, 0, sinA,
            0, 1,    0,
        -sinA, 0, cosA );
}

template <typename T>
const Matrix33<T> Matrix33<T>::RotationZ( const double deg )
{
    const T rad = static_cast<T>( kvs::Math::Deg2Rad( deg ) );
    const T sinA = static_cast<T>( std::sin( rad ) );
    const T cosA = static_cast<T>( std::cos( rad ) );

    return Matrix33(
         cosA, -sinA, 0,
         sinA,  cosA, 0,
            0,     0, 1 );
}

/*==========================================================================*/
/**
 *  @brief  Constructs a new Matrix33.
 */
/*==========================================================================*/
template<typename T>
inline Matrix33<T>::Matrix33()
{
    this->setZero();
};

/*==========================================================================*/
/**
 *  @brief  Constructs a new Matrix33.
 *  @param  a00 [in] Element.
 *  @param  a01 [in] Element.
 *  @param  a02 [in] Element.
 *  @param  a10 [in] Element.
 *  @param  a11 [in] Element.
 *  @param  a12 [in] Element.
 *  @param  a20 [in] Element.
 *  @param  a21 [in] Element.
 *  @param  a22 [in] Element.
 */
/*==========================================================================*/
template<typename T>
inline Matrix33<T>::Matrix33(
    const T a00, const T a01, const T a02,
    const T a10, const T a11, const T a12,
    const T a20, const T a21, const T a22 )
{
    this->set(
        a00, a01, a02,
        a10, a11, a12,
        a20, a21, a22 );
}

/*==========================================================================*/
/**
 *  @brief  Constructs a new Matrix33.
 *  @param  v0 [in] Vector3.
 *  @param  v1 [in] Vector3.
 *  @param  v2 [in] Vector3.
 */
/*==========================================================================*/
template<typename T>
inline Matrix33<T>::Matrix33(
    const Vector3<T>& v0,
    const Vector3<T>& v1,
    const Vector3<T>& v2 )
{
    this->set( v0, v1, v2 );
}

/*==========================================================================*/
/**
 *  @brief  Constructs a new Matrix33.
 *  @param  elements [in] Array of elements.
 */
/*==========================================================================*/
template<typename T>
inline Matrix33<T>::Matrix33( const T elements[9] )
{
    this->set( elements );
}

/*==========================================================================*/
/**
 *  @brief  Sets the elements.
 *  @param  a00 [in] Element.
 *  @param  a01 [in] Element.
 *  @param  a02 [in] Element.
 *  @param  a10 [in] Element.
 *  @param  a11 [in] Element.
 *  @param  a12 [in] Element.
 *  @param  a20 [in] Element.
 *  @param  a21 [in] Element.
 *  @param  a22 [in] Element.
 */
/*==========================================================================*/
template<typename T>
inline void Matrix33<T>::set(
    const T a00, const T a01, const T a02,
    const T a10, const T a11, const T a12,
    const T a20, const T a21, const T a22 )
{
    m_data[0].set( a00, a01, a02 );
    m_data[1].set( a10, a11, a12 );
    m_data[2].set( a20, a21, a22 );
}

/*==========================================================================*/
/**
 *  @brief  Sets the elements.
 *  @param  v0 [in] 1st row vector.
 *  @param  v1 [in] 2nd row vector.
 *  @param  v2 [in] 3rd row vector.
 */
/*==========================================================================*/
template<typename T>
inline void Matrix33<T>::set(
    const Vector3<T>& v0,
    const Vector3<T>& v1,
    const Vector3<T>& v2 )
{
    m_data[0] = v0;
    m_data[1] = v1;
    m_data[2] = v2;
}

/*==========================================================================*/
/**
 *  @brief  Sets the elements.
 *  @param  elements [in] Array of elements.
 */
/*==========================================================================*/
template<typename T>
inline void Matrix33<T>::set( const T elements[9] )
{
    m_data[0].set( elements     );
    m_data[1].set( elements + 3 );
    m_data[2].set( elements + 6 );
}

template<typename T>
inline void Matrix33<T>::setZero()
{
    m_data[0].setZero();
    m_data[1].setZero();
    m_data[2].setZero();
}

template<typename T>
inline void Matrix33<T>::setOnes()
{
    m_data[0].setOnes();
    m_data[1].setOnes();
    m_data[2].setOnes();
}

template<typename T>
inline void Matrix33<T>::setIdentity()
{
    m_data[0].setUnitX();
    m_data[1].setUnitY();
    m_data[2].setUnitZ();
}

template<typename T>
inline void Matrix33<T>::setConstant( const T x )
{
    m_data[0].setConstant(x);
    m_data[1].setConstant(x);
    m_data[2].setConstant(x);
}

template<typename T>
inline void Matrix33<T>::setDiagonal( const T x )
{
    m_data[0].setZero(); m_data[0][0] = x;
    m_data[1].setZero(); m_data[1][1] = x;
    m_data[2].setZero(); m_data[2][2] = x;
}

template<typename T>
inline void Matrix33<T>::setDiagonal( const kvs::Vector3<T>& v )
{
    m_data[0].setZero(); m_data[0][0] = v[0];
    m_data[1].setZero(); m_data[1][1] = v[1];
    m_data[2].setZero(); m_data[2][2] = v[2];
}

template<typename T>
inline void Matrix33<T>::setRandom()
{
    m_data[0].setRandom();
    m_data[1].setRandom();
    m_data[2].setRandom();
}

template<typename T>
inline void Matrix33<T>::setRandom( const kvs::UInt32 seed )
{
    m_data[0].setRandom( seed );
    m_data[1].setRandom();
    m_data[2].setRandom();
}

template<typename T>
inline void Matrix33<T>::setRandom( const T min, const T max )
{
    m_data[0].setRandom( min, max );
    m_data[1].setRandom( min, max );
    m_data[2].setRandom( min, max );
}

template<typename T>
inline void Matrix33<T>::setRandom( const T min, const T max, const kvs::UInt32 seed )
{
    m_data[0].setRandom( min, max, seed );
    m_data[1].setRandom( min, max );
    m_data[2].setRandom( min, max );
}

/*==========================================================================*/
/**
 *  @brief  Swaps this and other.
 *  @param  other [in,out] Matrix33.
 */
/*==========================================================================*/
template<typename T>
inline void Matrix33<T>::swap( Matrix33& other )
{
    m_data[0].swap( other[0] );
    m_data[1].swap( other[1] );
    m_data[2].swap( other[2] );
}

/*==========================================================================*/
/**
 *  @brief  Transposes this matrix.
 *  @return Transposed matrix.
 */
/*==========================================================================*/
template<typename T>
inline void Matrix33<T>::transpose()
{
    std::swap( m_data[0][1], m_data[1][0] );
    std::swap( m_data[0][2], m_data[2][0] );
    std::swap( m_data[1][2], m_data[2][1] );
}

/*==========================================================================*/
/**
 *  @brief  Inverts this matrix.
 *  @param  determinant [out] calculated determinant
 *  @return Inverse matrix.
 */
/*==========================================================================*/
template<typename T>
inline void Matrix33<T>::invert( T* determinant )
{
    const T det22[9] = {
        m_data[1][1] * m_data[2][2] - m_data[1][2] * m_data[2][1],
        m_data[1][0] * m_data[2][2] - m_data[1][2] * m_data[2][0],
        m_data[1][0] * m_data[2][1] - m_data[1][1] * m_data[2][0],
        m_data[0][1] * m_data[2][2] - m_data[0][2] * m_data[2][1],
        m_data[0][0] * m_data[2][2] - m_data[0][2] * m_data[2][0],
        m_data[0][0] * m_data[2][1] - m_data[0][1] * m_data[2][0],
        m_data[0][1] * m_data[1][2] - m_data[0][2] * m_data[1][1],
        m_data[0][0] * m_data[1][2] - m_data[0][2] * m_data[1][0],
        m_data[0][0] * m_data[1][1] - m_data[0][1] * m_data[1][0], };

    const T det33 =
        m_data[0][0] * det22[0] - m_data[0][1] * det22[1] + m_data[0][2] * det22[2];

    if ( determinant ) *determinant = det33;

    this->set(
        +det22[0], -det22[3], +det22[6],
        -det22[1], +det22[4], -det22[7],
        +det22[2], -det22[5], +det22[8] );

    const T det_inverse = static_cast<T>( 1.0 / det33 );
    *this *= det_inverse;
}

/*==========================================================================*/
/**
 *  @brief  Prints the elements as a formatted string.
 *  @param  delim [in] delimiter
 *  @param  bracket_l [in] left bracket
 *  @param  bracket_r [in] right bracket
 *  @param  newline [in] flag for newline for each row
 *  @param  indent [in] indent for each row
 */
/*==========================================================================*/
template<typename T>
inline std::string Matrix33<T>::format(
    const std::string delim,
    const std::string bracket_l,
    const std::string bracket_r,
    const bool newline,
    const kvs::Indent& indent ) const
{
    std::ostringstream os;
    os << indent << bracket_l;
    {
        const std::string offset( bracket_l.size(), ' ' );
        os << m_data[0].format( delim, bracket_l, bracket_r );
        os << delim; if ( newline ) { os << std::endl << indent << offset; }
        os << m_data[1].format( delim, bracket_l, bracket_r );
        os << delim; if ( newline ) { os << std::endl << indent << offset; }
        os << m_data[2].format( delim, bracket_l, bracket_r );
    }
    os << bracket_r;
    return os.str();
}

/*==========================================================================*/
/**
 *  @brief  Calculates the trace of this matrix.
 *  @return Trace of this matrix.
 */
/*==========================================================================*/
template<typename T>
inline T Matrix33<T>::trace() const
{
    return m_data[0][0] + m_data[1][1] + m_data[2][2];
}

/*==========================================================================*/
/**
 *  @brief  Calculates the determinant of this matrix.
 *  @return Determinant of this matrix.
 */
/*==========================================================================*/
template<typename T>
inline T Matrix33<T>::determinant() const
{
    return m_data[0].cross( m_data[1] ).dot( m_data[2] );
}

/*==========================================================================*/
/**
 *  @brief  Copies this and transposes it.
 *  @return Transposed matrix.
 */
/*==========================================================================*/
template<typename T>
inline const Matrix33<T> Matrix33<T>::transposed() const
{
    Matrix33 result( *this );
    result.transpose();
    return result;
}

/*==========================================================================*/
/**
 *  @brief  Copies this and inverts it.
 *  @param  determinant [out] calculated determinant
 *  @return Inverse matrix.
 */
/*==========================================================================*/
template<typename T>
inline const Matrix33<T> Matrix33<T>::inverted( T* determinant ) const
{
    Matrix33 result( *this );
    result.invert( determinant );
    return result;
}

template<typename T>
inline bool Matrix33<T>::isSymmetric() const
{
    if ( !kvs::Math::Equal( m_data[0][1], m_data[1][0] ) ) { return false; }
    if ( !kvs::Math::Equal( m_data[0][2], m_data[2][0] ) ) { return false; }
    if ( !kvs::Math::Equal( m_data[1][2], m_data[2][1] ) ) { return false; }
    return true;
}

template<typename T>
inline bool Matrix33<T>::isDiagonal() const
{
    if ( !kvs::Math::IsZero( m_data[0][1] ) ||
         !kvs::Math::IsZero( m_data[1][0] ) ) { return false; }
    if ( !kvs::Math::IsZero( m_data[0][2] ) ||
         !kvs::Math::IsZero( m_data[2][0] ) ) { return false; }
    if ( !kvs::Math::IsZero( m_data[1][2] ) ||
         !kvs::Math::IsZero( m_data[2][1] ) ) { return false; }
    return true;
}

template<typename T>
inline const Vector3<T>& Matrix33<T>::operator []( const size_t index ) const
{
    KVS_ASSERT( index < 3 );
    return m_data[ index ];
}

template<typename T>
inline Vector3<T>& Matrix33<T>::operator []( const size_t index )
{
    KVS_ASSERT( index < 3 );
    return m_data[ index ];
}

template<typename T>
inline Matrix33<T>& Matrix33<T>::operator +=( const Matrix33& rhs )
{
    m_data[0] += rhs[0];
    m_data[1] += rhs[1];
    m_data[2] += rhs[2];
    return *this;
}

template<typename T>
inline Matrix33<T>& Matrix33<T>::operator -=( const Matrix33& rhs )
{
    m_data[0] -= rhs[0];
    m_data[1] -= rhs[1];
    m_data[2] -= rhs[2];
    return *this;
}

template<typename T>
inline Matrix33<T>& Matrix33<T>::operator *=( const Matrix33& rhs )
{
    this->set(
        m_data[0][0] * rhs[0][0] + m_data[0][1] * rhs[1][0] + m_data[0][2] * rhs[2][0],
        m_data[0][0] * rhs[0][1] + m_data[0][1] * rhs[1][1] + m_data[0][2] * rhs[2][1],
        m_data[0][0] * rhs[0][2] + m_data[0][1] * rhs[1][2] + m_data[0][2] * rhs[2][2],
        m_data[1][0] * rhs[0][0] + m_data[1][1] * rhs[1][0] + m_data[1][2] * rhs[2][0],
        m_data[1][0] * rhs[0][1] + m_data[1][1] * rhs[1][1] + m_data[1][2] * rhs[2][1],
        m_data[1][0] * rhs[0][2] + m_data[1][1] * rhs[1][2] + m_data[1][2] * rhs[2][2],
        m_data[2][0] * rhs[0][0] + m_data[2][1] * rhs[1][0] + m_data[2][2] * rhs[2][0],
        m_data[2][0] * rhs[0][1] + m_data[2][1] * rhs[1][1] + m_data[2][2] * rhs[2][1],
        m_data[2][0] * rhs[0][2] + m_data[2][1] * rhs[1][2] + m_data[2][2] * rhs[2][2] );
    return *this;
}

template<typename T>
inline Matrix33<T>& Matrix33<T>::operator *=( const T rhs )
{
    m_data[0] *= rhs;
    m_data[1] *= rhs;
    m_data[2] *= rhs;
    return *this;
}

template<typename T>
inline Matrix33<T>& Matrix33<T>::operator /=( const T rhs )
{
    m_data[0] /= rhs;
    m_data[1] /= rhs;
    m_data[2] /= rhs;
    return *this;
}

template<typename T>
inline const Matrix33<T> Matrix33<T>::operator -() const
{
    return Matrix33( *this ) *= T(-1);
}

} // end of namespace kvs
