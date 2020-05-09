/****************************************************************************/
/**
 *  @file   Matrix44.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: Matrix44.h 1757 2014-05-04 13:17:37Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#pragma once
#include <iostream>
#include <iomanip>
#include <kvs/Assert>
#include <kvs/Math>
#include <kvs/Indent>
#include <kvs/Vector4>
#include <kvs/Deprecated>


namespace kvs
{

/*==========================================================================*/
/**
 *  4x4 matrix class.
 */
/*==========================================================================*/
template<typename T>
class Matrix44
{
private:
    Vector4<T> m_data[4]; ///< Row vectors.

public:
    static const Matrix44 Zero() { Matrix44 m; m.setZero(); return m; }
    static const Matrix44 Ones() { Matrix44 m; m.setOnes(); return m; }
    static const Matrix44 Identity() { Matrix44 m; m.setIdentity(); return m; }
    static const Matrix44 Constant( const T x ) { Matrix44 m; m.setConstant(x); return m; }
    static const Matrix44 Diagonal( const T x ) { Matrix44 m; m.setDiagonal(x); return m; }
    static const Matrix44 Diagonal( const kvs::Vector4<T>& v ) { Matrix44 m; m.setDiagonal(v); return m; }
    static const Matrix44 Random() { Matrix44 m; m.setRandom(); return m; }
    static const Matrix44 Random( const kvs::UInt32 seed ) { Matrix44 m; m.setRandom( seed ); return m; }
    static const Matrix44 Random( const T min, const T max ) { Matrix44 m; m.setRandom( min, max ); return m; }
    static const Matrix44 Random( const T min, const T max, const kvs::UInt32 seed ) { Matrix44 m; m.setRandom( min, max, seed ); return m; }

public:
    Matrix44();
    Matrix44(
        const T a00, const T a01, const T a02, const T a03,
        const T a10, const T a11, const T a12, const T a13,
        const T a20, const T a21, const T a22, const T a23,
        const T a30, const T a31, const T a32, const T a33 );
    Matrix44(
        const Vector4<T>& v0,
        const Vector4<T>& v1,
        const Vector4<T>& v2,
        const Vector4<T>& v3 );
    explicit Matrix44( const T elements[16] );

    void set(
        const T a00, const T a01, const T a02, const T a03,
        const T a10, const T a11, const T a12, const T a13,
        const T a20, const T a21, const T a22, const T a23,
        const T a30, const T a31, const T a32, const T a33 );
    void set(
        const Vector4<T>& v0,
        const Vector4<T>& v1,
        const Vector4<T>& v2,
        const Vector4<T>& v3 );
    void set( const T elements[16] );
    void setZero();
    void setOnes();
    void setIdentity();
    void setConstant( const T x );
    void setDiagonal( const T x );
    void setDiagonal( const kvs::Vector4<T>& v );
    void setRandom();
    void setRandom( const kvs::UInt32 seed );
    void setRandom( const T min, const T max );
    void setRandom( const T min, const T max, const kvs::UInt32 seed );

    void swap( Matrix44& other );
    void transpose();
    void invert( T* determinant = 0 );

    T trace() const;
    T determinant() const;
    const Matrix44 transposed() const;
    const Matrix44 inverted( T* determinant = 0 ) const;
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
    const Vector4<T>& operator []( const size_t index ) const;
    Vector4<T>& operator []( const size_t index );
    Matrix44& operator +=( const Matrix44& rhs );
    Matrix44& operator -=( const Matrix44& rhs );
    Matrix44& operator *=( const Matrix44& rhs );
    Matrix44& operator *=( const T rhs );
    Matrix44& operator /=( const T rhs );
    const Matrix44 operator -() const;

    friend bool operator ==( const Matrix44& lhs, const Matrix44& rhs )
    {
        return lhs[0] == rhs[0] &&
               lhs[1] == rhs[1] &&
               lhs[2] == rhs[2] &&
               lhs[3] == rhs[3];
    }

    friend bool operator !=( const Matrix44& lhs, const Matrix44& rhs )
    {
        return !( lhs == rhs );
    }

    friend const Matrix44 operator +( const Matrix44& lhs, const Matrix44& rhs )
    {
        return Matrix44( lhs ) += rhs;
    }

    friend const Matrix44 operator -( const Matrix44& lhs, const Matrix44& rhs )
    {
        return Matrix44( lhs ) -= rhs;
    }

    friend const Matrix44 operator *( const Matrix44& lhs, const Matrix44& rhs )
    {
        return Matrix44( lhs ) *= rhs;
    }

    friend const Vector4<T> operator *( const Matrix44& lhs, const Vector4<T>& rhs )
    {
        return Vector4<T>(
            lhs[0].dot( rhs ),
            lhs[1].dot( rhs ),
            lhs[2].dot( rhs ),
            lhs[3].dot( rhs ) );
    }

    friend const Vector4<T> operator *( const Vector4<T>& lhs, const Matrix44& rhs )
    {
        return Vector4<T>(
            lhs[0] * rhs[0][0] + lhs[1] * rhs[1][0] + lhs[2] * rhs[2][0] + lhs[3] * rhs[3][0],
            lhs[0] * rhs[0][1] + lhs[1] * rhs[1][1] + lhs[2] * rhs[2][1] + lhs[3] * rhs[3][1],
            lhs[0] * rhs[0][2] + lhs[1] * rhs[1][2] + lhs[2] * rhs[2][2] + lhs[3] * rhs[3][2],
            lhs[0] * rhs[0][3] + lhs[1] * rhs[1][3] + lhs[2] * rhs[2][3] + lhs[3] * rhs[3][3] );
    }

    friend const Matrix44 operator *( const Matrix44& lhs, const T rhs )
    {
        return Matrix44( lhs ) *= rhs;
    }

    friend const Matrix44 operator *( const T lhs, const Matrix44& rhs )
    {
        return rhs * lhs;
    }

    friend const Matrix44 operator /( const Matrix44& lhs, const T rhs )
    {
        return Matrix44( lhs ) /= rhs;
    }

    friend std::ostream& operator <<( std::ostream& os, const Matrix44& rhs )
    {
        return os << rhs.format( " ", "", "", true );
    }

public:
    KVS_DEPRECATED( static const Matrix44 All( const T x ) ) { return Constant( x ); }
    KVS_DEPRECATED( explicit Matrix44( const T a ) ) { *this = Constant( a ); }
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
typedef Matrix44<float> Matrix44f;
typedef Matrix44<double> Matrix44d;
typedef Matrix44<float> Mat4;
typedef Matrix44<double> Mat4d;


/*==========================================================================*/
/**
 *  @brief  Constructs a new Matrix44.
 */
/*==========================================================================*/
template<typename T>
inline Matrix44<T>::Matrix44()
{
    this->setZero();
};

/*==========================================================================*/
/**
 *  @brief  Constructs a new Matrix44.
 *  @param  a00 [in] Element.
 *  @param  a01 [in] Element.
 *  @param  a02 [in] Element.
 *  @param  a03 [in] Element.
 *  @param  a10 [in] Element.
 *  @param  a11 [in] Element.
 *  @param  a12 [in] Element.
 *  @param  a13 [in] Element.
 *  @param  a20 [in] Element.
 *  @param  a21 [in] Element.
 *  @param  a22 [in] Element.
 *  @param  a23 [in] Element.
 *  @param  a30 [in] Element.
 *  @param  a31 [in] Element.
 *  @param  a32 [in] Element.
 *  @param  a33 [in] Element.
 */
/*==========================================================================*/
template<typename T>
inline Matrix44<T>::Matrix44(
    const T a00, const T a01, const T a02, const T a03,
    const T a10, const T a11, const T a12, const T a13,
    const T a20, const T a21, const T a22, const T a23,
    const T a30, const T a31, const T a32, const T a33 )
{
    this->set(
        a00, a01, a02, a03,
        a10, a11, a12, a13,
        a20, a21, a22, a23,
        a30, a31, a32, a33 );
}

/*==========================================================================*/
/**
 *  @brief  Constructs a new Matrix44.
 *  @param  v0 [in] 1st row vector.
 *  @param  v1 [in] 2nd row vector.
 *  @param  v2 [in] 3rd row vector.
 *  @param  v3 [in] 4th row vector.
 */
/*==========================================================================*/
template<typename T>
inline Matrix44<T>::Matrix44(
    const Vector4<T>& v0,
    const Vector4<T>& v1,
    const Vector4<T>& v2,
    const Vector4<T>& v3 )
{
    this->set( v0, v1, v2, v3 );
}

/*==========================================================================*/
/**
 *  @brief  Constructs a new Matrix44.
 *  @param  elements [in] Array of elements.
 */
/*==========================================================================*/
template<typename T>
inline Matrix44<T>::Matrix44( const T elements[16] )
{
    this->set( elements );
}

/*==========================================================================*/
/**
 *  @brief  Sets the elements.
 *  @param  a00 [in] Element.
 *  @param  a01 [in] Element.
 *  @param  a02 [in] Element.
 *  @param  a03 [in] Element.
 *  @param  a10 [in] Element.
 *  @param  a11 [in] Element.
 *  @param  a12 [in] Element.
 *  @param  a13 [in] Element.
 *  @param  a20 [in] Element.
 *  @param  a21 [in] Element.
 *  @param  a22 [in] Element.
 *  @param  a23 [in] Element.
 *  @param  a30 [in] Element.
 *  @param  a31 [in] Element.
 *  @param  a32 [in] Element.
 *  @param  a33 [in] Element.
 */
/*==========================================================================*/
template<typename T>
inline void Matrix44<T>::set(
    const T a00, const T a01, const T a02, const T a03,
    const T a10, const T a11, const T a12, const T a13,
    const T a20, const T a21, const T a22, const T a23,
    const T a30, const T a31, const T a32, const T a33 )
{
    m_data[0].set( a00, a01, a02, a03 );
    m_data[1].set( a10, a11, a12, a13 );
    m_data[2].set( a20, a21, a22, a23 );
    m_data[3].set( a30, a31, a32, a33 );
}

/*==========================================================================*/
/**
 *  @brief  Sets the elements.
 *  @param  v0 [in] 1st row vector.
 *  @param  v1 [in] 2nd row vector.
 *  @param  v2 [in] 3rd row vector.
 *  @param  v3 [in] 4th row vector.
 */
/*==========================================================================*/
template<typename T>
inline void Matrix44<T>::set(
    const Vector4<T>& v0,
    const Vector4<T>& v1,
    const Vector4<T>& v2,
    const Vector4<T>& v3 )
{
    m_data[0] = v0;
    m_data[1] = v1;
    m_data[2] = v2;
    m_data[3] = v3;
}

/*==========================================================================*/
/**
 *  @brief  Sets the elements.
 *  @param  elements [in] Array of elements.
 */
/*==========================================================================*/
template<typename T>
inline void Matrix44<T>::set( const T elements[16] )
{
    m_data[0].set( elements      );
    m_data[1].set( elements +  4 );
    m_data[2].set( elements +  8 );
    m_data[3].set( elements + 12 );
}

template<typename T>
inline void Matrix44<T>::setZero()
{
    m_data[0].setZero();
    m_data[1].setZero();
    m_data[2].setZero();
    m_data[3].setZero();
}

template<typename T>
inline void Matrix44<T>::setOnes()
{
    m_data[0].setOnes();
    m_data[1].setOnes();
    m_data[2].setOnes();
    m_data[4].setOnes();
}

template<typename T>
inline void Matrix44<T>::setIdentity()
{
    m_data[0].setUnitX();
    m_data[1].setUnitY();
    m_data[2].setUnitZ();
    m_data[3].setUnitW();
}

template<typename T>
inline void Matrix44<T>::setConstant( const T x )
{
    m_data[0].setConstant(x);
    m_data[1].setConstant(x);
    m_data[2].setConstant(x);
    m_data[3].setConstant(x);
}

template<typename T>
inline void Matrix44<T>::setDiagonal( const T x )
{
    m_data[0].setZero(); m_data[0][0] = x;
    m_data[1].setZero(); m_data[1][1] = x;
    m_data[2].setZero(); m_data[2][2] = x;
    m_data[3].setZero(); m_data[3][3] = x;
}

template<typename T>
inline void Matrix44<T>::setDiagonal( const kvs::Vector4<T>& v )
{
    m_data[0].setZero(); m_data[0][0] = v[0];
    m_data[1].setZero(); m_data[1][1] = v[1];
    m_data[2].setZero(); m_data[2][2] = v[2];
    m_data[3].setZero(); m_data[3][3] = v[3];
}

template<typename T>
inline void Matrix44<T>::setRandom()
{
    m_data[0].setRandom();
    m_data[1].setRandom();
    m_data[2].setRandom();
    m_data[3].setRandom();
}

template<typename T>
inline void Matrix44<T>::setRandom( const kvs::UInt32 seed )
{
    m_data[0].setRandom( seed );
    m_data[1].setRandom();
    m_data[2].setRandom();
    m_data[3].setRandom();
}

template<typename T>
inline void Matrix44<T>::setRandom( const T min, const T max )
{
    m_data[0].setRandom( min, max );
    m_data[1].setRandom( min, max );
    m_data[2].setRandom( min, max );
    m_data[3].setRandom( min, max );
}

template<typename T>
inline void Matrix44<T>::setRandom( const T min, const T max, const kvs::UInt32 seed )
{
    m_data[0].setRandom( min, max, seed );
    m_data[1].setRandom( min, max );
    m_data[2].setRandom( min, max );
    m_data[3].setRandom( min, max );
}

/*==========================================================================*/
/**
 *  @brief  Swaps this and other.
 *  @param  other [in,out] Matrix44.
 */
/*==========================================================================*/
template<typename T>
inline void Matrix44<T>::swap( Matrix44& other )
{
    m_data[0].swap( other[0] );
    m_data[1].swap( other[1] );
    m_data[2].swap( other[2] );
    m_data[3].swap( other[3] );
}

/*==========================================================================*/
/**
 *  @brief  Transposes this matrix.
 *  @return Transposed matrix.
 */
/*==========================================================================*/
template<typename T>
inline void Matrix44<T>::transpose()
{
    std::swap( m_data[0][1], m_data[1][0] );
    std::swap( m_data[0][2], m_data[2][0] );
    std::swap( m_data[0][3], m_data[3][0] );
    std::swap( m_data[1][2], m_data[2][1] );
    std::swap( m_data[1][3], m_data[3][1] );
    std::swap( m_data[2][3], m_data[3][2] );
}

/*==========================================================================*/
/**
 *  @brief  Inverts this matrix.
 *  @param  determinat [out] calculated determinant
 *  @return Inverse matrix.
 */
/*==========================================================================*/
template<typename T>
inline void Matrix44<T>::invert( T* determinant )
{
    const T det22upper[6] = {
        m_data[0][2] * m_data[1][3] - m_data[0][3] * m_data[1][2],
        m_data[0][1] * m_data[1][3] - m_data[0][3] * m_data[1][1],
        m_data[0][0] * m_data[1][3] - m_data[0][3] * m_data[1][0],
        m_data[0][1] * m_data[1][2] - m_data[0][2] * m_data[1][1],
        m_data[0][0] * m_data[1][2] - m_data[0][2] * m_data[1][0],
        m_data[0][0] * m_data[1][1] - m_data[0][1] * m_data[1][0], };

    const T det22lower[6] = {
        m_data[2][2] * m_data[3][3] - m_data[2][3] * m_data[3][2],
        m_data[2][1] * m_data[3][3] - m_data[2][3] * m_data[3][1],
        m_data[2][0] * m_data[3][3] - m_data[2][3] * m_data[3][0],
        m_data[2][1] * m_data[3][2] - m_data[2][2] * m_data[3][1],
        m_data[2][0] * m_data[3][2] - m_data[2][2] * m_data[3][0],
        m_data[2][0] * m_data[3][1] - m_data[2][1] * m_data[3][0], };

    const T det33[16] = {
        m_data[1][1] * det22lower[0] - m_data[1][2] * det22lower[1] + m_data[1][3] * det22lower[3],
        m_data[1][0] * det22lower[0] - m_data[1][2] * det22lower[2] + m_data[1][3] * det22lower[4],
        m_data[1][0] * det22lower[1] - m_data[1][1] * det22lower[2] + m_data[1][3] * det22lower[5],
        m_data[1][0] * det22lower[3] - m_data[1][1] * det22lower[4] + m_data[1][2] * det22lower[5],
        m_data[0][1] * det22lower[0] - m_data[0][2] * det22lower[1] + m_data[0][3] * det22lower[3],
        m_data[0][0] * det22lower[0] - m_data[0][2] * det22lower[2] + m_data[0][3] * det22lower[4],
        m_data[0][0] * det22lower[1] - m_data[0][1] * det22lower[2] + m_data[0][3] * det22lower[5],
        m_data[0][0] * det22lower[3] - m_data[0][1] * det22lower[4] + m_data[0][2] * det22lower[5],
        m_data[3][1] * det22upper[0] - m_data[3][2] * det22upper[1] + m_data[3][3] * det22upper[3],
        m_data[3][0] * det22upper[0] - m_data[3][2] * det22upper[2] + m_data[3][3] * det22upper[4],
        m_data[3][0] * det22upper[1] - m_data[3][1] * det22upper[2] + m_data[3][3] * det22upper[5],
        m_data[3][0] * det22upper[3] - m_data[3][1] * det22upper[4] + m_data[3][2] * det22upper[5],
        m_data[2][1] * det22upper[0] - m_data[2][2] * det22upper[1] + m_data[2][3] * det22upper[3],
        m_data[2][0] * det22upper[0] - m_data[2][2] * det22upper[2] + m_data[2][3] * det22upper[4],
        m_data[2][0] * det22upper[1] - m_data[2][1] * det22upper[2] + m_data[2][3] * det22upper[5],
        m_data[2][0] * det22upper[3] - m_data[2][1] * det22upper[4] + m_data[2][2] * det22upper[5], };

    const T det44 =
        m_data[0][0] * det33[0] - m_data[0][1] * det33[1] + m_data[0][2] * det33[2] - m_data[0][3] * det33[3];

    if ( determinant ) *determinant = det44;

    this->set(
        +det33[0], -det33[4], +det33[ 8], -det33[12],
        -det33[1], +det33[5], -det33[ 9], +det33[13],
        +det33[2], -det33[6], +det33[10], -det33[14],
        -det33[3], +det33[7], -det33[11], +det33[15] );

    const T det_inverse = static_cast<T>( 1.0 / det44 );
    *this *= det_inverse;
}

/*==========================================================================*/
/**
 *  @brief  Prints the elements with the specified delimiter and brackets.
 *  @param  delim [in] delimiter
 *  @param  bracket_l [in] left bracket
 *  @param  bracket_r [in] right bracket
 *  @param  newline [in] flag for newline for each row
 *  @param  indent [in] indent for each row
 */
/*==========================================================================*/
template<typename T>
inline std::string Matrix44<T>::format(
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
        os << delim; if ( newline ) { os << std::endl << indent << offset; }
        os << m_data[3].format( delim, bracket_l, bracket_r );
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
inline T Matrix44<T>::trace() const
{
    return m_data[0][0] + m_data[1][1] + m_data[2][2] + m_data[3][3];
}

/*==========================================================================*/
/**
 *  @brief  Calculates the determinant of this matrix.
 *  @return Determinant of this matrix.
 */
/*==========================================================================*/
template<typename T>
inline T Matrix44<T>::determinant() const
{
    const T det22lower[6] = {
        m_data[2][2] * m_data[3][3] - m_data[2][3] * m_data[3][2],
        m_data[2][1] * m_data[3][3] - m_data[2][3] * m_data[3][1],
        m_data[2][0] * m_data[3][3] - m_data[2][3] * m_data[3][0],
        m_data[2][1] * m_data[3][2] - m_data[2][2] * m_data[3][1],
        m_data[2][0] * m_data[3][2] - m_data[2][2] * m_data[3][0],
        m_data[2][0] * m_data[3][1] - m_data[2][1] * m_data[3][0], };

    const T det33[4] = {
        m_data[1][1] * det22lower[0] - m_data[1][2] * det22lower[1] + m_data[1][3] * det22lower[3],
        m_data[1][0] * det22lower[0] - m_data[1][2] * det22lower[2] + m_data[1][3] * det22lower[4],
        m_data[1][0] * det22lower[1] - m_data[1][1] * det22lower[2] + m_data[1][3] * det22lower[5],
        m_data[1][0] * det22lower[3] - m_data[1][1] * det22lower[4] + m_data[1][2] * det22lower[5], };

    const T det44 =
        m_data[0][0] * det33[0] - m_data[0][1] * det33[1] + m_data[0][2] * det33[2] - m_data[0][3] * det33[3];

    return det44;
}

/*==========================================================================*/
/**
 *  @brief  Copies this and transposes it.
 *  @return Transposed matrix.
 */
/*==========================================================================*/
template<typename T>
inline const Matrix44<T> Matrix44<T>::transposed() const
{
    Matrix44 result( *this );
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
inline const Matrix44<T> Matrix44<T>::inverted( T* determinant ) const
{
    Matrix44 result( *this );
    result.invert( determinant );
    return result;
}

template<typename T>
inline bool Matrix44<T>::isSymmetric() const
{
    if ( !kvs::Math::Equal( m_data[0][1], m_data[1][0] ) ) { return false; }
    if ( !kvs::Math::Equal( m_data[0][2], m_data[2][0] ) ) { return false; }
    if ( !kvs::Math::Equal( m_data[0][3], m_data[3][0] ) ) { return false; }
    if ( !kvs::Math::Equal( m_data[1][2], m_data[2][1] ) ) { return false; }
    if ( !kvs::Math::Equal( m_data[1][3], m_data[3][1] ) ) { return false; }
    if ( !kvs::Math::Equal( m_data[2][3], m_data[3][2] ) ) { return false; }
    return true;
}

template<typename T>
inline bool Matrix44<T>::isDiagonal() const
{
    if ( !kvs::Math::IsZero( m_data[0][1] ) ||
         !kvs::Math::IsZero( m_data[1][0] ) ) { return false; }
    if ( !kvs::Math::IsZero( m_data[0][2] ) ||
         !kvs::Math::IsZero( m_data[2][0] ) ) { return false; }
    if ( !kvs::Math::IsZero( m_data[0][3] ) ||
         !kvs::Math::IsZero( m_data[3][0] ) ) { return false; }
    if ( !kvs::Math::IsZero( m_data[1][2] ) ||
         !kvs::Math::IsZero( m_data[2][1] ) ) { return false; }
    if ( !kvs::Math::IsZero( m_data[1][3] ) ||
         !kvs::Math::IsZero( m_data[3][1] ) ) { return false; }
    if ( !kvs::Math::IsZero( m_data[2][3] ) ||
         !kvs::Math::IsZero( m_data[3][2] ) ) { return false; }
    return true;
}

template<typename T>
inline const Vector4<T>& Matrix44<T>::operator []( const size_t index ) const
{
    KVS_ASSERT( index < 4 );
    return m_data[ index ];
}

template<typename T>
inline Vector4<T>& Matrix44<T>::operator []( const size_t index )
{
    KVS_ASSERT( index < 4 );
    return m_data[ index ];
}

template<typename T>
inline Matrix44<T>& Matrix44<T>::operator +=( const Matrix44& rhs )
{
    m_data[0] += rhs[0];
    m_data[1] += rhs[1];
    m_data[2] += rhs[2];
    m_data[3] += rhs[3];
    return *this;
}

template<typename T>
inline Matrix44<T>& Matrix44<T>::operator -=( const Matrix44& rhs )
{
    m_data[0] -= rhs[0];
    m_data[1] -= rhs[1];
    m_data[2] -= rhs[2];
    m_data[3] -= rhs[3];
    return *this;
}

template<typename T>
inline Matrix44<T>& Matrix44<T>::operator *=( const Matrix44& rhs )
{
    this->set(
        m_data[0][0] * rhs[0][0] + m_data[0][1] * rhs[1][0] + m_data[0][2] * rhs[2][0] + m_data[0][3] * rhs[3][0],
        m_data[0][0] * rhs[0][1] + m_data[0][1] * rhs[1][1] + m_data[0][2] * rhs[2][1] + m_data[0][3] * rhs[3][1],
        m_data[0][0] * rhs[0][2] + m_data[0][1] * rhs[1][2] + m_data[0][2] * rhs[2][2] + m_data[0][3] * rhs[3][2],
        m_data[0][0] * rhs[0][3] + m_data[0][1] * rhs[1][3] + m_data[0][2] * rhs[2][3] + m_data[0][3] * rhs[3][3],
        m_data[1][0] * rhs[0][0] + m_data[1][1] * rhs[1][0] + m_data[1][2] * rhs[2][0] + m_data[1][3] * rhs[3][0],
        m_data[1][0] * rhs[0][1] + m_data[1][1] * rhs[1][1] + m_data[1][2] * rhs[2][1] + m_data[1][3] * rhs[3][1],
        m_data[1][0] * rhs[0][2] + m_data[1][1] * rhs[1][2] + m_data[1][2] * rhs[2][2] + m_data[1][3] * rhs[3][2],
        m_data[1][0] * rhs[0][3] + m_data[1][1] * rhs[1][3] + m_data[1][2] * rhs[2][3] + m_data[1][3] * rhs[3][3],
        m_data[2][0] * rhs[0][0] + m_data[2][1] * rhs[1][0] + m_data[2][2] * rhs[2][0] + m_data[2][3] * rhs[3][0],
        m_data[2][0] * rhs[0][1] + m_data[2][1] * rhs[1][1] + m_data[2][2] * rhs[2][1] + m_data[2][3] * rhs[3][1],
        m_data[2][0] * rhs[0][2] + m_data[2][1] * rhs[1][2] + m_data[2][2] * rhs[2][2] + m_data[2][3] * rhs[3][2],
        m_data[2][0] * rhs[0][3] + m_data[2][1] * rhs[1][3] + m_data[2][2] * rhs[2][3] + m_data[2][3] * rhs[3][3],
        m_data[3][0] * rhs[0][0] + m_data[3][1] * rhs[1][0] + m_data[3][2] * rhs[2][0] + m_data[3][3] * rhs[3][0],
        m_data[3][0] * rhs[0][1] + m_data[3][1] * rhs[1][1] + m_data[3][2] * rhs[2][1] + m_data[3][3] * rhs[3][1],
        m_data[3][0] * rhs[0][2] + m_data[3][1] * rhs[1][2] + m_data[3][2] * rhs[2][2] + m_data[3][3] * rhs[3][2],
        m_data[3][0] * rhs[0][3] + m_data[3][1] * rhs[1][3] + m_data[3][2] * rhs[2][3] + m_data[3][3] * rhs[3][3] );
    return *this;
}

template<typename T>
inline Matrix44<T>& Matrix44<T>::operator *=( const T rhs )
{
    m_data[0] *= rhs;
    m_data[1] *= rhs;
    m_data[2] *= rhs;
    m_data[3] *= rhs;
    return *this;
}

template<typename T>
inline Matrix44<T>& Matrix44<T>::operator /=( const T rhs )
{
    m_data[0] /= rhs;
    m_data[1] /= rhs;
    m_data[2] /= rhs;
    m_data[3] /= rhs;
    return *this;
}

template<typename T>
inline const Matrix44<T> Matrix44<T>::operator -() const
{
    return Matrix44( *this ) *= T( -1 );
}

} // end of namespace kvs
