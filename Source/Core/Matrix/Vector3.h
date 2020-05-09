/****************************************************************************/
/**
 *  @file   Vector3.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: Vector3.h 1757 2014-05-04 13:17:37Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#pragma once
#include <iostream>
#include <sstream>
#include <kvs/Assert>
#include <kvs/Math>
#include <kvs/Vector2>
#include <kvs/Value>
#include <kvs/Indent>
#include <kvs/Deprecated>


namespace kvs
{

/*==========================================================================*/
/**
 *  @brief  3D vector class.
 */
/*==========================================================================*/
template<typename T>
class Vector3
{
private:
    T m_data[3]; ///< Elements.

public:
    static const Vector3 Zero() { return Vector3( T(0), T(0), T(0) ); }
    static const Vector3 Ones() { return Vector3( T(1), T(1), T(1) ); }
    static const Vector3 UnitX() { return Vector3( T(1), T(0), T(0) ); }
    static const Vector3 UnitY() { return Vector3( T(0), T(1), T(0) ); }
    static const Vector3 UnitZ() { return Vector3( T(0), T(0), T(1) ); }
    static const Vector3 Identity() { return Vector3( T(1), T(0), T(0) ); }
    static const Vector3 Constant( const T x ) { return Vector3( x, x, x ); }
    static const Vector3 Random() { Vector3 v; v.setRandom(); return v; }
    static const Vector3 Random( const kvs::UInt32 seed ) { Vector3 v; v.setRandom( seed ); return v; }
    static const Vector3 Random( const T min, const T max ) { Vector3 v; v.setRandom( min, max ); return v; }
    static const Vector3 Random( const T min, const T max, const kvs::UInt32 seed ) { Vector3 v; v.setRandom( min, max, seed ); return v; }

public:
    Vector3();
    template <typename U>
    explicit Vector3( const kvs::Vector3<U>& v );
    Vector3( const T x, const T y, const T z );
    explicit Vector3( const Vector2<T>& other, const T z );
    explicit Vector3( const T elements[3] );

    T& x() { return m_data[0]; }
    T& y() { return m_data[1]; }
    T& z() { return m_data[2]; }
    const T& x() const { return m_data[0]; }
    const T& y() const { return m_data[1]; }
    const T& z() const { return m_data[2]; }
    const Vector2<T> xy() const { return Vector2<T>( m_data[0], m_data[1] ); }
    T* data() { return &m_data[0]; }
    const T* data() const { return &m_data[0]; }

    void set( const T x, const T y, const T z );
    void set( const Vector2<T>& other, const T z );
    void set( const T elements[3] );
    void setZero();
    void setOnes();
    void setUnitX();
    void setUnitY();
    void setUnitZ();
    void setIdentity();
    void setConstant( const T x );
    void setRandom();
    void setRandom( const kvs::UInt32 seed );
    void setRandom( const T min, const T max );
    void setRandom( const T min, const T max, const kvs::UInt32 seed );

    void swap( Vector3& other );
    void normalize();
    double length() const;
    double squaredLength() const;
    T dot( const Vector3& other ) const;
    const Vector3 cross( const Vector3& other ) const;
    const Vector3 normalized() const;

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
    const T& operator []( const size_t index ) const;
    T& operator []( const size_t index );
    Vector3& operator +=( const Vector3& rhs );
    Vector3& operator -=( const Vector3& rhs );
    Vector3& operator *=( const Vector3& rhs );
    Vector3& operator *=( const T rhs );
    Vector3& operator /= ( const Vector3& rhs );
    Vector3& operator /=( const T rhs );
    const Vector3 operator -() const;

    friend bool operator ==( const Vector3& lhs, const Vector3& rhs )
    {
        return kvs::Math::Equal( lhs[0], rhs[0] ) &&
               kvs::Math::Equal( lhs[1], rhs[1] ) &&
               kvs::Math::Equal( lhs[2], rhs[2] );
    }

    friend bool operator !=( const Vector3& lhs, const Vector3& rhs )
    {
        return !( lhs == rhs );
    }

    friend const Vector3 operator +( const Vector3& lhs, const Vector3& rhs )
    {
        return Vector3( lhs ) += rhs;
    }

    friend const Vector3 operator -( const Vector3& lhs, const Vector3& rhs )
    {
        return Vector3( lhs ) -= rhs;
    }

    friend const Vector3 operator *( const Vector3& lhs, const Vector3& rhs )
    {
        return Vector3( lhs ) *= rhs;
    }

    friend const Vector3 operator *( const Vector3& lhs, const T rhs )
    {
        return Vector3( lhs ) *= rhs;
    }

    friend const Vector3 operator *( const T lhs, const Vector3& rhs )
    {
        return rhs * lhs;
    }

    friend const Vector3 operator /( const Vector3& lhs, const Vector3& rhs )
    {
        return Vector3( lhs ) /= rhs;
    }

    friend const Vector3 operator /( const Vector3& lhs, const T rhs )
    {
        return Vector3( lhs ) /= rhs;
    }

    friend std::ostream& operator <<( std::ostream& os, const Vector3& rhs )
    {
        return os << rhs.format( " ", "", "" );
    }

public:
    KVS_DEPRECATED( static const Vector3 All( const T x ) ) { return Constant( x ); }
    KVS_DEPRECATED( explicit Vector3( const T x ) ) { *this = Constant( x ); }
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
typedef Vector3<int> Vector3i;
typedef Vector3<unsigned int> Vector3u;
typedef Vector3<float> Vector3f;
typedef Vector3<double> Vector3d;
typedef Vector3<float> Vec3;
typedef Vector3<int> Vec3i;
typedef Vector3<unsigned int> Vec3u;
typedef Vector3<double> Vec3d;
typedef Vector3<unsigned int> Vector3ui;
typedef Vector3<unsigned int> Vec3ui;


/*==========================================================================*/
/**
 *  @brief  Constructs a new Vector3.
 */
/*==========================================================================*/
template<typename T>
inline Vector3<T>::Vector3()
{
    this->setZero();
}

template <typename T>
template <typename U>
inline Vector3<T>::Vector3( const kvs::Vector3<U>& v )
{
    this->set(
        static_cast<T>( v.x() ),
        static_cast<T>( v.y() ),
        static_cast<T>( v.z() ) );
}

/*==========================================================================*/
/**
 *  @brief  Constructs a new Vector3.
 *  @param  x [in] Element.
 *  @param  y [in] Element.
 *  @param  z [in] Element.
 */
/*==========================================================================*/
template<typename T>
inline Vector3<T>::Vector3( const T x, const T y, const T z )
{
    this->set( x, y, z );
}

/*==========================================================================*/
/**
 *  @brief  Constructs a new Vector3.
 *  @param  other [in] Vector2.
 *  @param  z     [in] Element.
 */
/*==========================================================================*/
template<typename T>
inline Vector3<T>::Vector3( const Vector2<T>& other, const T z )
{
    this->set( other, z );
}

/*==========================================================================*/
/**
 *  @brief  Constructs a new Vector3.
 *  @param  elements [in] Array of elements.
 */
/*==========================================================================*/
template<typename T>
inline Vector3<T>::Vector3( const T elements[3] )
{
    this->set( elements );
}

/*==========================================================================*/
/**
 *  @brief  Sets the elements.
 *  @param  x [in] Element.
 *  @param  y [in] Element.
 *  @param  z [in] Element.
 */
/*==========================================================================*/
template<typename T>
inline void Vector3<T>::set( const T x, const T y, const T z )
{
    m_data[0] = x;
    m_data[1] = y;
    m_data[2] = z;
}

/*==========================================================================*/
/**
 *  @brief  Sets the elements.
 *  @param  other [in] Vector2.
 *  @param  z     [in] Element.
 */
/*==========================================================================*/
template<typename T>
inline void Vector3<T>::set( const Vector2<T>& other, const T z )
{
    m_data[0] = other[0];
    m_data[1] = other[1];
    m_data[2] = z;
}

/*==========================================================================*/
/**
 *  @brief  Sets the elements.
 *  @param  elements [in] Array of elements.
 */
/*==========================================================================*/
template<typename T>
inline void Vector3<T>::set( const T elements[3] )
{
    m_data[0] = elements[0];
    m_data[1] = elements[1];
    m_data[2] = elements[2];
}

template<typename T>
inline void Vector3<T>::setZero()
{
    m_data[0] = T(0);
    m_data[1] = T(0);
    m_data[2] = T(0);
}

template<typename T>
inline void Vector3<T>::setOnes()
{
    m_data[0] = T(1);
    m_data[1] = T(1);
    m_data[2] = T(1);
}

template<typename T>
inline void Vector3<T>::setUnitX()
{
    m_data[0] = T(1);
    m_data[1] = T(0);
    m_data[2] = T(0);
}

template<typename T>
inline void Vector3<T>::setUnitY()
{
    m_data[0] = T(0);
    m_data[1] = T(1);
    m_data[2] = T(0);
}

template<typename T>
inline void Vector3<T>::setUnitZ()
{
    m_data[0] = T(0);
    m_data[1] = T(0);
    m_data[2] = T(1);
}

template<typename T>
inline void Vector3<T>::setIdentity()
{
    m_data[0] = T(1);
    m_data[1] = T(0);
    m_data[2] = T(0);
}

template<typename T>
inline void Vector3<T>::setConstant( const T x )
{
    m_data[0] = x;
    m_data[1] = x;
    m_data[2] = x;
}

template<typename T>
inline void Vector3<T>::setRandom()
{
    static bool flag = true;
    if ( flag ) { kvs::Value<T>::SetRandomSeed(); flag = false; }
    m_data[0] = kvs::Value<T>::Random();
    m_data[1] = kvs::Value<T>::Random();
    m_data[2] = kvs::Value<T>::Random();
}

template<typename T>
inline void Vector3<T>::setRandom( const kvs::UInt32 seed )
{
    if ( seed > 0 ) kvs::Value<T>::SetSeed( seed );
    else kvs::Value<T>::SetRandomSeed();
    m_data[0] = kvs::Value<T>::Random();
    m_data[1] = kvs::Value<T>::Random();
    m_data[2] = kvs::Value<T>::Random();
}

template<typename T>
inline void Vector3<T>::setRandom( const T min, const T max )
{
    static bool flag = true;
    if ( flag ) { kvs::Value<T>::SetRandomSeed(); flag = false; }
    m_data[0] = kvs::Value<T>::Random( min, max );
    m_data[1] = kvs::Value<T>::Random( min, max );
    m_data[2] = kvs::Value<T>::Random( min, max );
}

template<typename T>
inline void Vector3<T>::setRandom( const T min, const T max, const kvs::UInt32 seed )
{
    if ( seed > 0 ) kvs::Value<T>::SetSeed( seed );
    else kvs::Value<T>::SetRandomSeed();
    m_data[0] = kvs::Value<T>::Random( min, max );
    m_data[1] = kvs::Value<T>::Random( min, max );
    m_data[2] = kvs::Value<T>::Random( min, max );
}

/*==========================================================================*/
/**
 *  @brief  Swaps this and other.
 *  @param  other [in,out] Vector3.
 */
/*==========================================================================*/
template<typename T>
inline void Vector3<T>::swap( Vector3& other )
{
    std::swap( m_data[0], other[0] );
    std::swap( m_data[1], other[1] );
    std::swap( m_data[2], other[2] );
}

/*==========================================================================*/
/**
 *  @brief  Normalizes this.
 *  @return Normalized Vector3.
 */
/*==========================================================================*/
template<typename T>
inline void Vector3<T>::normalize()
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
inline double Vector3<T>::length() const
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
inline double Vector3<T>::squaredLength() const
{
    double result = 0.0;
    result += static_cast<double>( m_data[0] * m_data[0] );
    result += static_cast<double>( m_data[1] * m_data[1] );
    result += static_cast<double>( m_data[2] * m_data[2] );
    return result;
}

/*==========================================================================*/
/**
 *  @brief  Calculates a dot product.
 *  @param  other [in] Vector3.
 *  @return Dot product.
 */
/*==========================================================================*/
template<typename T>
inline T Vector3<T>::dot( const Vector3& other ) const
{
    T result( 0 );
    result += m_data[0] * other[0];
    result += m_data[1] * other[1];
    result += m_data[2] * other[2];
    return result;
}

/*==========================================================================*/
/**
 *  @brief  Calculates a cross product.
 *  @param  other [in] Vector3.
 *  @return Cross product.
 */
/*==========================================================================*/
template<typename T>
inline const Vector3<T> Vector3<T>::cross( const Vector3& other ) const
{
    const T x = m_data[1] * other[2] - m_data[2] * other[1];
    const T y = m_data[2] * other[0] - m_data[0] * other[2];
    const T z = m_data[0] * other[1] - m_data[1] * other[0];
    return Vector3<T>( x, y, z );
}

/*===========================================================================*/
/**
 *  @brief  Returns the normalized vector.
 *  @return normalized vector
 */
/*===========================================================================*/
template<typename T>
inline const Vector3<T> Vector3<T>::normalized() const
{
    const double length = this->length();
    const T normalize_factor = length > 0.0 ? static_cast<T>( 1.0 / length ) : T( 0 );
    return *this * normalize_factor;
}

/*==========================================================================*/
/**
 *  @brief  Prints the elements as a formatted string.
 *  @param delim [in] delimiter
 *  @param bracket_l [in] left bracket
 *  @param bracket_r [in] right bracket
 */
/*==========================================================================*/
template<typename T>
inline std::string Vector3<T>::format(
    const std::string delim,
    const std::string bracket_l,
    const std::string bracket_r ) const
{
    std::ostringstream os;
    os << bracket_l
       << m_data[0] << delim
       << m_data[1] << delim
       << m_data[2]
       << bracket_r;
    return os.str();
}

template<typename T>
inline const T& Vector3<T>::operator []( const size_t index ) const
{
    KVS_ASSERT( index < 3 );
    return m_data[ index ];
}

template<typename T>
inline T& Vector3<T>::operator []( const size_t index )
{
    KVS_ASSERT( index < 3 );
    return m_data[ index ];
}

template<typename T>
inline Vector3<T>& Vector3<T>::operator +=( const Vector3& rhs )
{
    m_data[0] += rhs[0];
    m_data[1] += rhs[1];
    m_data[2] += rhs[2];
    return *this;
}

template<typename T>
inline Vector3<T>& Vector3<T>::operator -=( const Vector3& rhs )
{
    m_data[0] -= rhs[0];
    m_data[1] -= rhs[1];
    m_data[2] -= rhs[2];
    return *this;
}

template<typename T>
inline Vector3<T>& Vector3<T>::operator *=( const Vector3& rhs )
{
    m_data[0] *= rhs[0];
    m_data[1] *= rhs[1];
    m_data[2] *= rhs[2];
    return *this;
}

template<typename T>
inline Vector3<T>& Vector3<T>::operator *=( const T rhs )
{
    m_data[0] *= rhs;
    m_data[1] *= rhs;
    m_data[2] *= rhs;
    return *this;
}

template<typename T>
inline Vector3<T>& Vector3<T>::operator /=( const Vector3& rhs )
{
    m_data[0] /= rhs[0];
    m_data[1] /= rhs[1];
    m_data[2] /= rhs[2];
    return *this;
}

template<typename T>
inline Vector3<T>& Vector3<T>::operator /=( const T rhs )
{
    m_data[0] /= rhs;
    m_data[1] /= rhs;
    m_data[2] /= rhs;
    return *this;
}

template<typename T>
inline const Vector3<T> Vector3<T>::operator -() const
{
    return Vector3( *this ) *= T( -1 );
}

} // end of namespace kvs
