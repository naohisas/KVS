/****************************************************************************/
/**
 *  @file Matrix33.h
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
#ifndef KVS__MATRIX_33_H_INCLUDE
#define KVS__MATRIX_33_H_INCLUDE

#include <iostream>
#include <kvs/Assert>
#include <kvs/Math>
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
    Vector3<T> m_rows[3]; ///< Row vectors.

public:
    static const Matrix33 Zero();
    static const Matrix33 Identity();
    static const Matrix33 Diagonal( const T x );
    static const Matrix33 All( const T x );
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

public:
    void set(
        const T a00, const T a01, const T a02,
        const T a10, const T a11, const T a12,
        const T a20, const T a21, const T a22 );
    void set(
        const Vector3<T>& v0,
        const Vector3<T>& v1,
        const Vector3<T>& v2 );
    void set( const T elements[9] );

    void zero();
    void identity();
    void swap( Matrix33& other );
    void transpose();
    void invert( T* determinant = 0 );
    void print() const;

    T trace() const;
    T determinant() const;
    const Matrix33 transposed() const;
    const Matrix33 inverted( T* determinant = 0 ) const;

public:
    const Vector3<T>& operator []( const size_t index ) const;
    Vector3<T>&       operator []( const size_t index );

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
        return os << rhs[0] << "\n" << rhs[1] << "\n" << rhs[2];
    }

public:
    KVS_DEPRECATED( explicit Matrix33( const T a ) ) { *this = All( a ); }
    KVS_DEPRECATED( void set( const T a ) ) { *this = All( a ); }
};

/*==========================================================================*/
/**
 *  Type definition.
 */
/*==========================================================================*/
typedef Matrix33<float>  Matrix33f;
typedef Matrix33<double> Matrix33d;
typedef Matrix33<float>  Mat3;
typedef Matrix33<double> Mat3d;


/*===========================================================================*/
/**
 *  @brief  Returns an identity matrix.
 */
/*===========================================================================*/
template<typename T>
const Matrix33<T> Matrix33<T>::Identity()
{
    return Matrix33( 1, 0, 0,
                     0, 1, 0,
                     0, 0, 1 );
}

/*===========================================================================*/
/**
 *  @brief  Returns a zero matrix.
 */
/*===========================================================================*/
template<typename T>
const Matrix33<T> Matrix33<T>::Zero()
{
    return Matrix33( 0, 0, 0,
                     0, 0, 0,
                     0, 0, 0 );
}

/*===========================================================================*/
/**
 *  @brief  Returns a matrix which all elements are same as x.
 *  @param  x [in] element value
 */
/*===========================================================================*/
template<typename T>
const Matrix33<T> Matrix33<T>::All( const T x )
{
    return Matrix33( x, x, x,
                     x, x, x,
                     x, x, x );
}

/*===========================================================================*/
/**
 *  @brief  Returns a diagonal matrix which all diagonal elements are same as x.
 *  @param  x [in] element value
 */
/*===========================================================================*/
template<typename T>
const Matrix33<T> Matrix33<T>::Diagonal( const T x )
{
    return Identity() * x;
}

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
    this->zero();
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
    m_rows[0].set( a00, a01, a02 );
    m_rows[1].set( a10, a11, a12 );
    m_rows[2].set( a20, a21, a22 );
}

/*==========================================================================*/
/**
 *  @brief  Sets the elements.
 *  @param  v0 [in] Vector3.
 *  @param  v1 [in] Vector3.
 *  @param  v2 [in] Vector3.
 */
/*==========================================================================*/
template<typename T>
inline void Matrix33<T>::set(
    const Vector3<T>& v0,
    const Vector3<T>& v1,
    const Vector3<T>& v2 )
{
    m_rows[0] = v0;
    m_rows[1] = v1;
    m_rows[2] = v2;
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
    m_rows[0].set( elements     );
    m_rows[1].set( elements + 3 );
    m_rows[2].set( elements + 6 );
}

/*==========================================================================*/
/**
 *  @brief  Sets the elements to zero.
 */
/*==========================================================================*/
template<typename T>
inline void Matrix33<T>::zero()
{
    *this = Zero();
}

/*==========================================================================*/
/**
 *  @brief  Sets this matrix to an identity matrix.
 */
/*==========================================================================*/
template<typename T>
inline void Matrix33<T>::identity()
{
    *this = Identity();
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
    m_rows[0].swap( other[0] );
    m_rows[1].swap( other[1] );
    m_rows[2].swap( other[2] );
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
    std::swap( m_rows[0][1], m_rows[1][0] );
    std::swap( m_rows[0][2], m_rows[2][0] );
    std::swap( m_rows[1][2], m_rows[2][1] );
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
        m_rows[1][1] * m_rows[2][2] - m_rows[1][2] * m_rows[2][1],
        m_rows[1][0] * m_rows[2][2] - m_rows[1][2] * m_rows[2][0],
        m_rows[1][0] * m_rows[2][1] - m_rows[1][1] * m_rows[2][0],
        m_rows[0][1] * m_rows[2][2] - m_rows[0][2] * m_rows[2][1],
        m_rows[0][0] * m_rows[2][2] - m_rows[0][2] * m_rows[2][0],
        m_rows[0][0] * m_rows[2][1] - m_rows[0][1] * m_rows[2][0],
        m_rows[0][1] * m_rows[1][2] - m_rows[0][2] * m_rows[1][1],
        m_rows[0][0] * m_rows[1][2] - m_rows[0][2] * m_rows[1][0],
        m_rows[0][0] * m_rows[1][1] - m_rows[0][1] * m_rows[1][0], };

    const T det33 =
        m_rows[0][0] * det22[0] - m_rows[0][1] * det22[1] + m_rows[0][2] * det22[2];

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
 *  @brief  Prints the elements of this.
 */
/*==========================================================================*/
template<typename T>
inline void Matrix33<T>::print() const
{
    std::cout << *this << std::endl;
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
    return m_rows[0][0] + m_rows[1][1] + m_rows[2][2];
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
    return m_rows[0].cross( m_rows[1] ).dot( m_rows[2] );
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
inline const Vector3<T>& Matrix33<T>::operator []( const size_t index ) const
{
    KVS_ASSERT( index < 3 );
    return m_rows[ index ];
}

template<typename T>
inline Vector3<T>& Matrix33<T>::operator []( const size_t index )
{
    KVS_ASSERT( index < 3 );
    return m_rows[ index ];
}

template<typename T>
inline Matrix33<T>& Matrix33<T>::operator +=( const Matrix33& rhs )
{
    m_rows[0] += rhs[0];
    m_rows[1] += rhs[1];
    m_rows[2] += rhs[2];
    return *this;
}

template<typename T>
inline Matrix33<T>& Matrix33<T>::operator -=( const Matrix33& rhs )
{
    m_rows[0] -= rhs[0];
    m_rows[1] -= rhs[1];
    m_rows[2] -= rhs[2];
    return *this;
}

template<typename T>
inline Matrix33<T>& Matrix33<T>::operator *=( const Matrix33& rhs )
{
    this->set(
        m_rows[0][0] * rhs[0][0] + m_rows[0][1] * rhs[1][0] + m_rows[0][2] * rhs[2][0],
        m_rows[0][0] * rhs[0][1] + m_rows[0][1] * rhs[1][1] + m_rows[0][2] * rhs[2][1],
        m_rows[0][0] * rhs[0][2] + m_rows[0][1] * rhs[1][2] + m_rows[0][2] * rhs[2][2],
        m_rows[1][0] * rhs[0][0] + m_rows[1][1] * rhs[1][0] + m_rows[1][2] * rhs[2][0],
        m_rows[1][0] * rhs[0][1] + m_rows[1][1] * rhs[1][1] + m_rows[1][2] * rhs[2][1],
        m_rows[1][0] * rhs[0][2] + m_rows[1][1] * rhs[1][2] + m_rows[1][2] * rhs[2][2],
        m_rows[2][0] * rhs[0][0] + m_rows[2][1] * rhs[1][0] + m_rows[2][2] * rhs[2][0],
        m_rows[2][0] * rhs[0][1] + m_rows[2][1] * rhs[1][1] + m_rows[2][2] * rhs[2][1],
        m_rows[2][0] * rhs[0][2] + m_rows[2][1] * rhs[1][2] + m_rows[2][2] * rhs[2][2] );
    return *this;
}

template<typename T>
inline Matrix33<T>& Matrix33<T>::operator *=( const T rhs )
{
    m_rows[0] *= rhs;
    m_rows[1] *= rhs;
    m_rows[2] *= rhs;
    return *this;
}

template<typename T>
inline Matrix33<T>& Matrix33<T>::operator /=( const T rhs )
{
    m_rows[0] /= rhs;
    m_rows[1] /= rhs;
    m_rows[2] /= rhs;
    return *this;
}

template<typename T>
inline const Matrix33<T> Matrix33<T>::operator -() const
{
    return Matrix33( *this ) *= T( -1 );
}

} // end of namespace kvs

#endif // KVS__MATRIX_33_H_INCLUDE
