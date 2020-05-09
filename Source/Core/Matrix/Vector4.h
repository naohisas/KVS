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
#include <sstream>
#include <kvs/Assert>
#include <kvs/Math>
#include <kvs/Value>
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
    static const Vector4 Random( const kvs::UInt32 seed ) { Vector4 v; v.setRandom( seed ); return v; }
    static const Vector4 Random( const T min, const T max ) { Vector4 v; v.setRandom( min, max ); return v; }
    static const Vector4 Random( const T min, const T max, const kvs::UInt32 seed ) { Vector4 v; v.setRandom( min, max, seed ); return v; }

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
    void setRandom( const kvs::UInt32 seed );
    void setRandom( const T min, const T max );
    void setRandom( const T min, const T max, const kvs::UInt32 seed );

    void swap( Vector4& other );
    void normalize();
    double length() const;
    double squaredLength() const;
    T dot( const Vector4& other ) const;
    const Vector4 normalized() const;

    std::string format() const
    {
        return this->format( ", ", "[", "]" );
    }

    std::string format(
        const std::string delim ) const
    {
        return this->format( delim, "", "" );
    }

    std::string format(
        const std::string bracket_l,
        const std::string bracket_r ) const
    {
        return this->format( " ", bracket_l, bracket_r );
    }

    std::string format(
        const std::string delim,
        const std::string bracket_l,
        const std::string bracket_r ) const;

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
        return os << rhs.format( " ", "", "" );
    }

public:
    KVS_DEPRECATED( static const Vector4 All( const T x ) ) { return Constant( x ); }
    KVS_DEPRECATED( explicit Vector4( const T x ) ) { *this = Constant( x ); }
    KVS_DEPRECATED( void set( const T x ) ) { *this = Constant( x ); }
    KVS_DEPRECATED( void zero() ) { this->setZero(); }
    KVS_DEPRECATED( double length2() const ) { return this->squaredLength(); }
    KVS_DEPRECATED( void print() const ) { std::cout << *this << std::endl; }
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
    this->set(
        static_cast<T>( v.x() ),
        static_cast<T>( v.y() ),
        static_cast<T>( v.z() ),
        static_cast<T>( v.w() ) );
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
    static bool flag = true;
    if ( flag ) { kvs::Value<T>::SetRandomSeed(); flag = false; }
    m_data[0] = kvs::Value<T>::Random();
    m_data[1] = kvs::Value<T>::Random();
    m_data[2] = kvs::Value<T>::Random();
    m_data[3] = kvs::Value<T>::Random();
}

template<typename T>
inline void Vector4<T>::setRandom( const kvs::UInt32 seed )
{
    if ( seed > 0 ) kvs::Value<T>::SetSeed( seed );
    else kvs::Value<T>::SetRandomSeed();
    m_data[0] = kvs::Value<T>::Random();
    m_data[1] = kvs::Value<T>::Random();
    m_data[2] = kvs::Value<T>::Random();
    m_data[3] = kvs::Value<T>::Random();
}

template<typename T>
inline void Vector4<T>::setRandom( const T min, const T max )
{
    static bool flag = true;
    if ( flag ) { kvs::Value<T>::SetRandomSeed(); flag = false; }
    m_data[0] = kvs::Value<T>::Random( min, max );
    m_data[1] = kvs::Value<T>::Random( min, max );
    m_data[2] = kvs::Value<T>::Random( min, max );
    m_data[3] = kvs::Value<T>::Random( min, max );
}

template<typename T>
inline void Vector4<T>::setRandom( const T min, const T max, const kvs::UInt32 seed )
{
    if ( seed > 0 ) kvs::Value<T>::SetSeed( seed );
    else kvs::Value<T>::SetRandomSeed();
    m_data[0] = kvs::Value<T>::Random( min, max );
    m_data[1] = kvs::Value<T>::Random( min, max );
    m_data[2] = kvs::Value<T>::Random( min, max );
    m_data[3] = kvs::Value<T>::Random( min, max );
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

/*==========================================================================*/
/**
 *  @brief  Prints the elements with a formatted string.
 *  @param delim [in] delimiter
 *  @param bracket_l [in] left bracket
 *  @param bracket_r [in] right bracket
 */
/*==========================================================================*/
template<typename T>
inline std::string Vector4<T>::format(
    const std::string delim,
    const std::string bracket_l,
    const std::string bracket_r ) const
{
    std::ostringstream os;
    os << bracket_l
       << m_data[0] << delim
       << m_data[1] << delim
       << m_data[2] << delim
       << m_data[3]
       << bracket_r;
    return os.str();
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
