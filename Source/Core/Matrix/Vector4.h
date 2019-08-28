/****************************************************************************/
/**
 *  @file   Vector4.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: Vector4.h 1757 2014-05-04 13:17:37Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#pragma once
#include <iostream>
#include <kvs/Assert>
#include <kvs/Math>
#include <kvs/Vector2>
#include <kvs/Vector3>
#include <kvs/Deprecated>


namespace kvs
{

/*==========================================================================*/
/**
 *  @brief  4D vector class.
 */
/*==========================================================================*/
template <typename T>
class Vector4
{
private:
    T m_data[4]; ///< Elements.

public:
    static const Vector4 Zero() { return Vector4( T(0), T(0), T(0), T(0) ); }
    static const Vector4 Ones() { return Vector4( T(1), T(1), T(1), T(1) ); }
    static const Vector4 UnitX() { return Vector4( T(1), T(0), T(0), T(0) ); }
    static const Vector4 UnitY() { return Vector4( T(0), T(1), T(0), T(0) ); }
    static const Vector4 UnitZ() { return Vector4( T(0), T(0), T(1), T(0) ); }
    static const Vector4 UnitW() { return Vector4( T(0), T(0), T(0), T(1) ); }
    static const Vector4 Identity() { return Vector4( T(1), T(0), T(0), T(0) ); }
    static const Vector4 Constant( const T x ) { return Vector4( x, x, x, x ); }
    static const Vector4 Random() { Vector4 v; v.setRandom(); return v; }

public:
    Vector4();
    template <typename U>
    explicit Vector4( const kvs::Vector4<U>& v );
    Vector4( const T x, const T y, const T z, const T w );
    Vector4( const Vector2<T>& other, const T z , const T w );
    Vector4( const Vector3<T>& other, const T w );
    explicit Vector4( const T elements[4] );

    T& x() { return m_data[0]; }
    T& y() { return m_data[1]; }
    T& z() { return m_data[2]; }
    T& w() { return m_data[3]; }
    const T& x() const { return m_data[0]; }
    const T& y() const { return m_data[1]; }
    const T& z() const { return m_data[2]; }
    const T& w() const { return m_data[3]; }
    const Vector2<T> xy() const { return Vector2<T>( m_data[0], m_data[1] ); }
    const Vector3<T> xyz() const { return Vector3<T>( m_data[0], m_data[1], m_data[2] ); }
    T* data() { return &m_data[0]; }
    const T* data() const { return &m_data[0]; }

    void set( const T x, const T y, const T z, const T w );
    void set( const Vector2<T>& other, const T z, const T w );
    void set( const Vector3<T>& other, const T w );
    void set( const T elements[4] );
    void setZero();
    void setOnes();
    void setUnitX();
    void setUnitY();
    void setUnitZ();
    void setUnitW();
    void setIdentity();
    void setConstant( const T x );
    void setRandom();

    void swap( Vector4& other );
    void normalize();
    void print() const;
    double length() const;
    double squaredLength() const;
    T dot( const Vector4& other ) const;
    const Vector4 normalized() const;

public:
    const T& operator [] ( const size_t index ) const;
    T& operator [] ( const size_t index );
    Vector4& operator += ( const Vector4& rhs );
    Vector4& operator -= ( const Vector4& rhs );
    Vector4& operator *= ( const Vector4& rhs );
    Vector4& operator *= ( const T rhs );
    Vector4& operator /= ( const Vector4& rhs );
    Vector4& operator /= ( const T rhs );
    const Vector4 operator -() const;

    friend bool operator ==( const Vector4& lhs, const Vector4& rhs )
    {
        return kvs::Math::Equal( lhs[0], rhs[0] ) &&
               kvs::Math::Equal( lhs[1], rhs[1] ) &&
               kvs::Math::Equal( lhs[2], rhs[2] ) &&
               kvs::Math::Equal( lhs[3], rhs[3] );
    }

    friend bool operator !=( const Vector4& lhs, const Vector4& rhs )
    {
        return !( lhs == rhs );
    }

    friend const Vector4 operator +( const Vector4& lhs, const Vector4& rhs )
    {
        return Vector4( lhs ) += rhs;
    }

    friend const Vector4 operator -( const Vector4& lhs, const Vector4& rhs )
    {
        return Vector4( lhs ) -= rhs;
    }

    friend const Vector4 operator *( const Vector4& lhs, const Vector4& rhs )
    {
        return Vector4( lhs ) *= rhs;
    }

    friend const Vector4 operator *( const Vector4& lhs, const T rhs )
    {
        return Vector4( lhs ) *= rhs;
    }

    friend const Vector4 operator *( const T lhs, const Vector4& rhs )
    {
        return rhs * lhs;
    }

    friend const Vector4 operator /( const Vector4& lhs, const Vector4& rhs )
    {
        return Vector4( lhs ) /= rhs;
    }

    friend const Vector4 operator /( const Vector4& lhs, const T rhs )
    {
        return Vector4( lhs ) /= rhs;
    }

    friend std::ostream& operator <<( std::ostream& os, const Vector4& rhs )
    {
        return os << rhs[0] << " " << rhs[1] << " " << rhs[2] << " " << rhs[3];
    }

public:
    KVS_DEPRECATED( static const Vector4 All( const T x ) ) { return Constant( x ); }
    KVS_DEPRECATED( explicit Vector4( const T x ) ) { *this = Constant( x ); }
    KVS_DEPRECATED( void set( const T x ) ) { *this = Constant( x ); }
    KVS_DEPRECATED( void zero() ) { this->setZero(); }
    KVS_DEPRECATED( double length2() const ) { return this->squaredLength(); }
};


/*==========================================================================*/
/**
 *  Type definition.
 */
/*==========================================================================*/
typedef Vector4<int> Vector4i;
typedef Vector4<unsigned int> Vector4u;
typedef Vector4<float> Vector4f;
typedef Vector4<double> Vector4d;
typedef Vector4<float> Vec4;
typedef Vector4<int> Vec4i;
typedef Vector4<unsigned int> Vec4u;
typedef Vector4<double> Vec4d;
typedef Vector4<unsigned int> Vector4ui;
typedef Vector4<unsigned int> Vec4ui;


/*==========================================================================*/
/**
 *  @brief  Constructs a new Vector4.
 */
/*==========================================================================*/
template<typename T>
inline Vector4<T>::Vector4()
{
    this->setZero();
}

template <typename T>
template <typename U>
inline Vector4<T>::Vector4( const kvs::Vector4<U>& v )
{
    this->set( static_cast<T>( v.x() ), static_cast<T>( v.y() ), static_cast<T>( v.z() ), static_cast<T>( v.w() ) );
}

/*==========================================================================*/
/**
 *  @brief  Constructs a new Vector4.
 *  @param  x [in] Element.
 *  @param  y [in] Element.
 *  @param  z [in] Element.
 *  @param  w [in] Element.
 */
/*==========================================================================*/
template<typename T>
inline Vector4<T>::Vector4( const T x, const T y, const T z, const T w )
{
    this->set( x, y, z, w );
}

/*==========================================================================*/
/**
 *  @brief  Constructs a new Vector4.
 *  @param  other [in] Vector2.
 *  @param  z     [in] Element.
 *  @param  w     [in] Element.
 */
/*==========================================================================*/
template<typename T>
inline Vector4<T>::Vector4( const Vector2<T>& other, const T z , const T w )
{
    this->set( other, z, w );
}

/*==========================================================================*/
/**
 *  @brief  Constructs a new Vector4.
 *  @param  other [in] Vector3.
 *  @param  w     [in] Element.
 */
/*==========================================================================*/
template<typename T>
inline Vector4<T>::Vector4( const Vector3<T>& other, const T w )
{
    this->set( other, w );
}

/*==========================================================================*/
/**
 *  @brief  Constructs a new Vector4.
 *  @param  elements [in] Array of elements.
 */
/*==========================================================================*/
template<typename T>
inline Vector4<T>::Vector4( const T elements[4] )
{
    this->set( elements );
}

/*==========================================================================*/
/**
 *  @brief  Sets the elements.
 *  @param  x [in] Element.
 *  @param  y [in] Element.
 *  @param  z [in] Element.
 *  @param  w [in] Element.
 */
/*==========================================================================*/
template<typename T>
inline void Vector4<T>::set( const T x, const T y, const T z, const T w )
{
    m_data[0] = x;
    m_data[1] = y;
    m_data[2] = z;
    m_data[3] = w;
}

/*==========================================================================*/
/**
 *  @brief  Sets the elements.
 *  @param  other [in] Vector2.
 *  @param  z     [in] Element.
 *  @param  w     [in] Element.
 */
/*==========================================================================*/
template<typename T>
inline void Vector4<T>::set( const Vector2<T>& other, const T z, const T w )
{
    m_data[0] = other[0];
    m_data[1] = other[1];
    m_data[2] = z;
    m_data[3] = w;
}

/*==========================================================================*/
/**
 *  @brief  Sets the elements.
 *  @param  other [in] Vector3.
 *  @param  w     [in] Element.
 */
/*==========================================================================*/
template<typename T>
inline void Vector4<T>::set( const Vector3<T>& other, const T w )
{
    m_data[0] = other[0];
    m_data[1] = other[1];
    m_data[2] = other[2];
    m_data[3] = w;
}

/*==========================================================================*/
/**
 *  @brief  Sets the elements.
 *  @param  elements [in] Array of elements.
 */
/*==========================================================================*/
template<typename T>
inline void Vector4<T>::set( const T elements[4] )
{
    m_data[0] = elements[0];
    m_data[1] = elements[1];
    m_data[2] = elements[2];
    m_data[3] = elements[3];
}

template<typename T>
inline void Vector4<T>::setZero()
{
    m_data[0] = T(0);
    m_data[1] = T(0);
    m_data[2] = T(0);
    m_data[3] = T(0);
}

template<typename T>
inline void Vector4<T>::setOnes()
{
    m_data[0] = T(1);
    m_data[1] = T(1);
    m_data[2] = T(1);
    m_data[3] = T(1);
}

template<typename T>
inline void Vector4<T>::setUnitX()
{
    m_data[0] = T(1);
    m_data[1] = T(0);
    m_data[2] = T(0);
    m_data[3] = T(0);
}

template<typename T>
inline void Vector4<T>::setUnitY()
{
    m_data[0] = T(0);
    m_data[1] = T(1);
    m_data[2] = T(0);
    m_data[3] = T(0);
}

template<typename T>
inline void Vector4<T>::setUnitZ()
{
    m_data[0] = T(0);
    m_data[1] = T(0);
    m_data[2] = T(1);
    m_data[3] = T(0);
}

template<typename T>
inline void Vector4<T>::setUnitW()
{
    m_data[0] = T(0);
    m_data[1] = T(0);
    m_data[2] = T(0);
    m_data[3] = T(1);
}

template<typename T>
inline void Vector4<T>::setIdentity()
{
    m_data[0] = T(1);
    m_data[1] = T(0);
    m_data[2] = T(0);
    m_data[3] = T(0);
}

template<typename T>
inline void Vector4<T>::setConstant( const T x )
{
    m_data[0] = x;
    m_data[1] = x;
    m_data[2] = x;
    m_data[3] = x;
}

template<typename T>
inline void Vector4<T>::setRandom()
{
    static kvs::Xorshift128 r( static_cast<kvs::UInt32>( time(0) ) );
    m_data[0] = T( 2.0 * r() - 1.0 ); // in [-1,1]
    m_data[1] = T( 2.0 * r() - 1.0 ); // in [-1,1]
    m_data[2] = T( 2.0 * r() - 1.0 ); // in [-1,1]
    m_data[3] = T( 2.0 * r() - 1.0 ); // in [-1,1]
}

template<>
inline void Vector4<kvs::Int8>::setRandom()
{
    static kvs::Xorshift128 r( static_cast<kvs::UInt32>( time(0)*2 ) );
    m_data[0] = kvs::Int8( r.randInteger() % ( UCHAR_MAX + 1 ) ); // in [CHAR_MIN, CHAR_MAX]
    m_data[1] = kvs::Int8( r.randInteger() % ( UCHAR_MAX + 1 ) ); // in [CHAR_MIN, CHAR_MAX]
    m_data[2] = kvs::Int8( r.randInteger() % ( UCHAR_MAX + 1 ) ); // in [CHAR_MIN, CHAR_MAX]
    m_data[3] = kvs::Int8( r.randInteger() % ( UCHAR_MAX + 1 ) ); // in [CHAR_MIN, CHAR_MAX]
}

template<>
inline void Vector4<kvs::UInt8>::setRandom()
{
    static kvs::Xorshift128 r( static_cast<kvs::UInt32>( time(0)*3 ) );
    m_data[0] = kvs::UInt8( r.randInteger() % ( UCHAR_MAX + 1 ) ); // in [0, UINT_MAX]
    m_data[1] = kvs::UInt8( r.randInteger() % ( UCHAR_MAX + 1 ) ); // in [0, UINT_MAX]
    m_data[2] = kvs::UInt8( r.randInteger() % ( UCHAR_MAX + 1 ) ); // in [0, UINT_MAX]
    m_data[3] = kvs::UInt8( r.randInteger() % ( UCHAR_MAX + 1 ) ); // in [0, UINT_MAX]
}

template<>
inline void Vector4<kvs::Int16>::setRandom()
{
    static kvs::Xorshift128 r( static_cast<kvs::UInt32>( time(0)*4 ) );
    m_data[0] = kvs::Int16( r.randInteger() % ( USHRT_MAX + 1 ) ); // in [SHRT_MIN, SHRT_MAX]
    m_data[1] = kvs::Int16( r.randInteger() % ( USHRT_MAX + 1 ) ); // in [SHRT_MIN, SHRT_MAX]
    m_data[2] = kvs::Int16( r.randInteger() % ( USHRT_MAX + 1 ) ); // in [SHRT_MIN, SHRT_MAX]
    m_data[3] = kvs::Int16( r.randInteger() % ( USHRT_MAX + 1 ) ); // in [SHRT_MIN, SHRT_MAX]
}

template<>
inline void Vector4<kvs::UInt16>::setRandom()
{
    static kvs::Xorshift128 r( static_cast<kvs::UInt32>( time(0)*5 ) );
    m_data[0] = kvs::UInt16( r.randInteger() % ( USHRT_MAX + 1 ) ); // in [0, UINT_MAX]
    m_data[1] = kvs::UInt16( r.randInteger() % ( USHRT_MAX + 1 ) ); // in [0, UINT_MAX]
    m_data[2] = kvs::UInt16( r.randInteger() % ( USHRT_MAX + 1 ) ); // in [0, UINT_MAX]
    m_data[3] = kvs::UInt16( r.randInteger() % ( USHRT_MAX + 1 ) ); // in [0, UINT_MAX]
}

template<>
inline void Vector4<kvs::Int32>::setRandom()
{
    static kvs::Xorshift128 r( static_cast<kvs::UInt32>( time(0)*6 ) );
    m_data[0] = kvs::Int32( r.randInteger() ); // in [INT_MIN, INT_MAX]
    m_data[1] = kvs::Int32( r.randInteger() ); // in [INT_MIN, INT_MAX]
    m_data[2] = kvs::Int32( r.randInteger() ); // in [INT_MIN, INT_MAX]
    m_data[3] = kvs::Int32( r.randInteger() ); // in [INT_MIN, INT_MAX]
}

template<>
inline void Vector4<kvs::UInt32>::setRandom()
{
    static kvs::Xorshift128 r( static_cast<kvs::UInt32>( time(0)*7 ) );
    m_data[0] = r.randInteger(); // in [0, UINT_MAX]
    m_data[1] = r.randInteger(); // in [0, UINT_MAX]
    m_data[2] = r.randInteger(); // in [0, UINT_MAX]
    m_data[3] = r.randInteger(); // in [0, UINT_MAX]
}

template<>
inline void Vector4<kvs::Int64>::setRandom()
{
    static kvs::Xorshift128 r( static_cast<kvs::UInt32>( time(0)*8 ) );
    m_data[0] = kvs::Int64( ( kvs::UInt64( r.randInteger() ) << 32 ) | kvs::UInt64( r.randInteger() ) ); // in [LONG_MIN, LONG_MAX]
    m_data[1] = kvs::Int64( ( kvs::UInt64( r.randInteger() ) << 32 ) | kvs::UInt64( r.randInteger() ) ); // in [LONG_MIN, LONG_MAX]
    m_data[2] = kvs::Int64( ( kvs::UInt64( r.randInteger() ) << 32 ) | kvs::UInt64( r.randInteger() ) ); // in [LONG_MIN, LONG_MAX]
    m_data[3] = kvs::Int64( ( kvs::UInt64( r.randInteger() ) << 32 ) | kvs::UInt64( r.randInteger() ) ); // in [LONG_MIN, LONG_MAX]
}

template<>
inline void Vector4<kvs::UInt64>::setRandom()
{
    static kvs::Xorshift128 r( static_cast<kvs::UInt32>( time(0)*9 ) );
    m_data[0] = ( kvs::UInt64( r.randInteger() ) << 32 ) | kvs::UInt64( r.randInteger() ); // in [0, ULONG_MAX]
    m_data[1] = ( kvs::UInt64( r.randInteger() ) << 32 ) | kvs::UInt64( r.randInteger() ); // in [0, ULONG_MAX]
    m_data[2] = ( kvs::UInt64( r.randInteger() ) << 32 ) | kvs::UInt64( r.randInteger() ); // in [0, ULONG_MAX]
    m_data[3] = ( kvs::UInt64( r.randInteger() ) << 32 ) | kvs::UInt64( r.randInteger() ); // in [0, ULONG_MAX]
}

/*==========================================================================*/
/**
 *  @brief  Swaps this and other.
 *  @param  other [in,out] Vector4.
 */
/*==========================================================================*/
template<typename T>
inline void Vector4<T>::swap( Vector4& other )
{
    std::swap( m_data[0], other[0] );
    std::swap( m_data[1], other[1] );
    std::swap( m_data[2], other[2] );
    std::swap( m_data[3], other[3] );
}

/*==========================================================================*/
/**
 *  @brief  Normalizes this.
 *  @return Normalized Vector4.
 */
/*==========================================================================*/
template<typename T>
inline void Vector4<T>::normalize()
{
    const double length = this->length();
    const T normalize_factor = length > 0.0 ? static_cast<T>( 1.0 / length ) : T( 0 );
    *this *= normalize_factor;
}

/*==========================================================================*/
/**
 *  @brief  Prints the elements of this.
 */
/*==========================================================================*/
template<typename T>
inline void Vector4<T>::print() const
{
    std::cout << *this << std::endl;
}

/*==========================================================================*/
/**
 *  @brief  Calculates a length of this.
 *  @return Length of this.
 */
/*==========================================================================*/
template<typename T>
inline double Vector4<T>::length() const
{
    return std::sqrt( this->squaredLength() );
}

/*==========================================================================*/
/**
 *  @brief  Calculates a square of a length of this.
 *  @return Square of a length of this.
 */
/*==========================================================================*/
template<typename T>
inline double Vector4<T>::squaredLength() const
{
    double result = 0.0;
    result += static_cast<double>( m_data[0] * m_data[0] );
    result += static_cast<double>( m_data[1] * m_data[1] );
    result += static_cast<double>( m_data[2] * m_data[2] );
    result += static_cast<double>( m_data[3] * m_data[3] );
    return result;
}

/*==========================================================================*/
/**
 *  @brief  Calculates a dot product.
 *  @param  other [in] Vector4.
 *  @return Dot product.
 */
/*==========================================================================*/
template<typename T>
inline T Vector4<T>::dot( const Vector4& other ) const
{
    T result( 0 );
    result += m_data[0] * other[0];
    result += m_data[1] * other[1];
    result += m_data[2] * other[2];
    result += m_data[3] * other[3];
    return result;
}

/*===========================================================================*/
/**
 *  @brief  Returns the normalized vector.
 *  @return normalized vector
 */
/*===========================================================================*/
template<typename T>
inline const Vector4<T> Vector4<T>::normalized() const
{
    const double length = this->length();
    const T normalize_factor = length > 0.0 ? static_cast<T>( 1.0 / length ) : T( 0 );
    return *this * normalize_factor;
}

template<typename T>
inline const T& Vector4<T>::operator []( const size_t index ) const
{
    KVS_ASSERT( index < 4 );
    return m_data[ index ];
}

template<typename T>
inline T& Vector4<T>::operator []( const size_t index )
{
    KVS_ASSERT( index < 4 );
    return m_data[ index ];
}

template<typename T>
inline Vector4<T>& Vector4<T>::operator +=( const Vector4& rhs )
{
    m_data[0] += rhs[0];
    m_data[1] += rhs[1];
    m_data[2] += rhs[2];
    m_data[3] += rhs[3];
    return *this;
}

template<typename T>
inline Vector4<T>& Vector4<T>::operator -=( const Vector4& rhs )
{
    m_data[0] -= rhs[0];
    m_data[1] -= rhs[1];
    m_data[2] -= rhs[2];
    m_data[3] -= rhs[3];
    return *this;
}

template<typename T>
inline Vector4<T>& Vector4<T>::operator *=( const Vector4& rhs )
{
    m_data[0] *= rhs[0];
    m_data[1] *= rhs[1];
    m_data[2] *= rhs[2];
    m_data[3] *= rhs[3];
    return *this;
}

template<typename T>
inline Vector4<T>& Vector4<T>::operator *=( const T rhs )
{
    m_data[0] *= rhs;
    m_data[1] *= rhs;
    m_data[2] *= rhs;
    m_data[3] *= rhs;
    return *this;
}

template<typename T>
inline Vector4<T>& Vector4<T>::operator /=( const Vector4& rhs )
{
    m_data[0] /= rhs[0];
    m_data[1] /= rhs[1];
    m_data[2] /= rhs[2];
    m_data[3] /= rhs[3];
    return *this;
}

template<typename T>
inline Vector4<T>& Vector4<T>::operator /=( const T rhs )
{
    m_data[0] /= rhs;
    m_data[1] /= rhs;
    m_data[2] /= rhs;
    m_data[3] /= rhs;
    return *this;
}

template<typename T>
inline const Vector4<T> Vector4<T>::operator -() const
{
    return Vector4( *this ) *= T( -1 );
}

} // end of namespace kvs
