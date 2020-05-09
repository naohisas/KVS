/****************************************************************************/
/**
 *  @file   Vector2.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: Vector2.h 1757 2014-05-04 13:17:37Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#pragma once
#include <iostream>
#include <sstream>
#include <climits>
#include <kvs/Assert>
#include <kvs/Math>
#include <kvs/Value>
#include <kvs/Indent>
#include <kvs/Deprecated>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  2D vector class.
 */
/*===========================================================================*/
template<typename T>
class Vector2
{
private:
    T m_data[2]; ///< Elements.

public:
    static const Vector2 Zero() { return Vector2( T(0), T(0) ); }
    static const Vector2 Ones() { return Vector2( T(1), T(1) ); }
    static const Vector2 UnitX() { return Vector2( T(1), T(0) ); }
    static const Vector2 UnitY() { return Vector2( T(0), T(1) ); }
    static const Vector2 Identity() { return Vector2( T(1), T(0) ); }
    static const Vector2 Constant( const T x ) { return Vector2( x, x ); }
    static const Vector2 Random() { Vector2 v; v.setRandom(); return v; }
    static const Vector2 Random( const kvs::UInt32 seed ) { Vector2 v; v.setRandom( seed ); return v; }
    static const Vector2 Random( const T min, const T max ) { Vector2 v; v.setRandom( min, max ); return v; }
    static const Vector2 Random( const T min, const T max, const kvs::UInt32 seed ) { Vector2 v; v.setRandom( min, max, seed ); return v; }

public:
    Vector2();
    template <typename U>
    explicit Vector2( const kvs::Vector2<U>& v );
    Vector2( const T x, const T y );
    explicit Vector2( const T elements[2] );

    T& x() { return m_data[0]; }
    T& y() { return m_data[1]; }
    const T& x() const { return m_data[0]; }
    const T& y() const { return m_data[1]; }
    T* data() { return &m_data[0]; }
    const T* data() const { return &m_data[0]; }

    void set( const T x, const T y );
    void set( const T elements[2] );
    void setZero();
    void setOnes();
    void setUnitX();
    void setUnitY();
    void setIdentity();
    void setConstant( const T x );
    void setRandom();
    void setRandom( const kvs::UInt32 seed );
    void setRandom( const T min, const T max );
    void setRandom( const T min, const T max, const kvs::UInt32 seed );

    void swap( Vector2& other );
    void normalize();
    double length() const;
    double squaredLength() const;
    T dot( const Vector2& other ) const;
    const Vector2 normalized() const;

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
    Vector2& operator +=( const Vector2& rhs );
    Vector2& operator -=( const Vector2& rhs );
    Vector2& operator *=( const Vector2& rhs );
    Vector2& operator *=( const T rhs );
    Vector2& operator /=( const Vector2& rhs );
    Vector2& operator /=( const T rhs );
    const Vector2 operator -() const;

    friend bool operator ==( const Vector2& lhs, const Vector2& rhs )
    {
        return kvs::Math::Equal( lhs[0], rhs[0] ) &&
               kvs::Math::Equal( lhs[1], rhs[1] );
    }

    friend bool operator !=( const Vector2& lhs, const Vector2& rhs )
    {
        return !( lhs == rhs );
    }

    friend const Vector2 operator +( const Vector2& lhs, const Vector2& rhs )
    {
        return Vector2( lhs ) += rhs;
    }

    friend const Vector2 operator -( const Vector2& lhs, const Vector2& rhs )
    {
        return Vector2( lhs ) -= rhs;
    }

    friend const Vector2 operator *( const Vector2& lhs, const Vector2& rhs )
    {
        return Vector2( lhs ) *= rhs;
    }

    friend const Vector2 operator *( const Vector2& lhs, const T rhs )
    {
        return Vector2( lhs ) *= rhs;
    }

    friend const Vector2 operator *( const T lhs, const Vector2& rhs )
    {
        return rhs * lhs;
    }

    friend const Vector2 operator /( const Vector2& lhs, const Vector2& rhs )
    {
        return Vector2( lhs ) /= rhs;
    }

    friend const Vector2 operator /( const Vector2& lhs, const T rhs )
    {
        return Vector2( lhs ) /= rhs;
    }

    friend std::ostream& operator <<( std::ostream& os, const Vector2& rhs )
    {
        return os << rhs.format( " ", "", "" );
    }

public:
    KVS_DEPRECATED( static const Vector2 All( const T x ) ) { return Constant( x ); }
    KVS_DEPRECATED( explicit Vector2( const T x ) ) { *this = Constant( x ); }
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
typedef Vector2<float> Vec2;
typedef Vector2<unsigned int> Vec2u;
typedef Vector2<int> Vec2i;
typedef Vector2<double> Vec2d;
typedef Vector2<int> Vector2i;
typedef Vector2<unsigned int> Vector2u;
typedef Vector2<float> Vector2f;
typedef Vector2<double> Vector2d;
typedef Vector2<unsigned int> Vector2ui;
typedef Vector2<unsigned int> Vec2ui;


/*==========================================================================*/
/**
 *  @brief  Constructs a new Vector2.
 */
/*==========================================================================*/
template<typename T>
inline Vector2<T>::Vector2()
{
    this->setZero();
}

template <typename T>
template <typename U>
inline Vector2<T>::Vector2( const kvs::Vector2<U>& v )
{
    this->set( static_cast<T>( v.x() ), static_cast<T>( v.y() ) );
}

/*==========================================================================*/
/**
 *  @brief  Constructs a new Vector2.
 *  @param  x [in] Element.
 *  @param  y [in] Element.
 */
/*==========================================================================*/
template<typename T>
inline Vector2<T>::Vector2( const T x, const T y )
{
    this->set( x, y );
}

/*==========================================================================*/
/**
 *  @brief  Constructs a new Vector2.
 *  @param  elements [in] Array of elements.
 */
/*==========================================================================*/
template<typename T>
inline Vector2<T>::Vector2( const T elements[2] )
{
    this->set( elements );
}

/*==========================================================================*/
/**
 *  @brief  Sets the elements.
 *  @param  x [in] Element.
 *  @param  y [in] Element.
 */
/*==========================================================================*/
template<typename T>
inline void Vector2<T>::set( const T x, const T y )
{
    m_data[0] = x;
    m_data[1] = y;
}

/*==========================================================================*/
/**
 *  @brief  Sets the elements.
 *  @param  elements [in] Array of elements.
 */
/*==========================================================================*/
template<typename T>
inline void Vector2<T>::set( const T elements[2] )
{
    m_data[0] = elements[0];
    m_data[1] = elements[1];
}

template<typename T>
inline void Vector2<T>::setZero()
{
    m_data[0] = T(0);
    m_data[1] = T(0);
}

template<typename T>
inline void Vector2<T>::setOnes()
{
    m_data[0] = T(1);
    m_data[1] = T(1);
}

template<typename T>
inline void Vector2<T>::setUnitX()
{
    m_data[0] = T(1);
    m_data[1] = T(0);
}

template<typename T>
inline void Vector2<T>::setUnitY()
{
    m_data[0] = T(0);
    m_data[1] = T(1);
}

template<typename T>
inline void Vector2<T>::setIdentity()
{
    m_data[0] = T(1);
    m_data[1] = T(0);
}

template<typename T>
inline void Vector2<T>::setConstant( const T x )
{
    m_data[0] = x;
    m_data[1] = x;
}

template<typename T>
inline void Vector2<T>::setRandom()
{
    static bool flag = true;
    if ( flag ) { kvs::Value<T>::SetRandomSeed(); flag = false; }
    m_data[0] = kvs::Value<T>::Random();
    m_data[1] = kvs::Value<T>::Random();
}

template<typename T>
inline void Vector2<T>::setRandom( const kvs::UInt32 seed )
{
    if ( seed > 0 ) kvs::Value<T>::SetSeed( seed );
    else kvs::Value<T>::SetRandomSeed();
    m_data[0] = kvs::Value<T>::Random();
    m_data[1] = kvs::Value<T>::Random();
}

template<typename T>
inline void Vector2<T>::setRandom( const T min, const T max )
{
    static bool flag = true;
    if ( flag ) { kvs::Value<T>::SetRandomSeed(); flag = false; }
    m_data[0] = kvs::Value<T>::Random( min, max );
    m_data[1] = kvs::Value<T>::Random( min, max );
}

template<typename T>
inline void Vector2<T>::setRandom( const T min, const T max, const kvs::UInt32 seed )
{
    if ( seed > 0 ) kvs::Value<T>::SetSeed( seed );
    else kvs::Value<T>::SetRandomSeed();
    m_data[0] = kvs::Value<T>::Random( min, max );
    m_data[1] = kvs::Value<T>::Random( min, max );
}

/*==========================================================================*/
/**
 *  @brief  Swaps this and other.
 *  @param  other [in,out] Vector2.
 */
/*==========================================================================*/
template<typename T>
inline void Vector2<T>::swap( Vector2& other )
{
    std::swap( m_data[0], other[0] );
    std::swap( m_data[1], other[1] );
}

/*==========================================================================*/
/**
 *  @brief  Normalizes this.
 */
/*==========================================================================*/
template<typename T>
inline void Vector2<T>::normalize()
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
inline double Vector2<T>::length() const
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
inline double Vector2<T>::squaredLength() const
{
    double result = 0.0;
    result += static_cast<double>( m_data[0] * m_data[0] );
    result += static_cast<double>( m_data[1] * m_data[1] );
    return result;
}

/*==========================================================================*/
/**
 *  @brief  Calculates a dot product.
 *  @param  other [in] Vector2.
 *  @return Dot product.
 */
/*==========================================================================*/
template<typename T>
inline T Vector2<T>::dot( const Vector2& other ) const
{
    T result( 0 );
    result += m_data[0] * other[0];
    result += m_data[1] * other[1];
    return result;
}

/*===========================================================================*/
/**
 *  @brief  Returns the normalized vector.
 *  @return normalized vector
 */
/*===========================================================================*/
template<typename T>
inline const Vector2<T> Vector2<T>::normalized() const
{
    const double length = this->length();
    const T normalize_factor = length > 0.0 ? static_cast<T>( 1.0 / length ) : T( 0 );
    return *this * normalize_factor;
}

/*==========================================================================*/
/**
 *  @brief  Prints the elements as a formatted string.
 *  @param  delim [in] delimiter
 *  @param  bracket_l [in] left bracket
 *  @param  bracket_r [in] right bracket
 */
/*==========================================================================*/
template<typename T>
inline std::string Vector2<T>::format(
    const std::string delim,
    const std::string bracket_l,
    const std::string bracket_r ) const
{
    std::ostringstream os;
    os << bracket_l
       << m_data[0] << delim
       << m_data[1]
       << bracket_r;
    return os.str();
}

template<typename T>
inline const T& Vector2<T>::operator []( const size_t index ) const
{
    KVS_ASSERT( index < 2 );
    return m_data[ index ];
}

template<typename T>
inline T& Vector2<T>::operator []( const size_t index )
{
    KVS_ASSERT( index < 2 );
    return m_data[ index ];
}

template<typename T>
inline Vector2<T>& Vector2<T>::operator +=( const Vector2& rhs )
{
    m_data[0] += rhs[0];
    m_data[1] += rhs[1];
    return *this;
}

template<typename T>
inline Vector2<T>& Vector2<T>::operator -=( const Vector2& rhs )
{
    m_data[0] -= rhs[0];
    m_data[1] -= rhs[1];
    return *this;
}

template<typename T>
inline Vector2<T>& Vector2<T>::operator *=( const Vector2& rhs )
{
    m_data[0] *= rhs[0];
    m_data[1] *= rhs[1];
    return *this;
}

template<typename T>
inline Vector2<T>& Vector2<T>::operator *=( const T rhs )
{
    m_data[0] *= rhs;
    m_data[1] *= rhs;
    return *this;
}

template<typename T>
inline Vector2<T>& Vector2<T>::operator /=( const Vector2& rhs )
{
    m_data[0] /= rhs[0];
    m_data[1] /= rhs[1];
    return *this;
}

template<typename T>
inline Vector2<T>& Vector2<T>::operator /=( const T rhs )
{
    m_data[0] /= rhs;
    m_data[1] /= rhs;
    return *this;
}

template<typename T>
inline const Vector2<T> Vector2<T>::operator -() const
{
    return Vector2( *this ) *= T( -1 );
}

} // end of namespace kvs
