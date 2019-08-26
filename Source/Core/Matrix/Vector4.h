/****************************************************************************/
/**
 *  @file Vector4.h
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
    T m_elements[4]; ///< Elements.

public:
    static const Vector4 Zero() { return Vector4( T(0), T(0), T(0), T(0) ); }
    static const Vector4 Ones() { return Vector4( T(1), T(1), T(1), T(1) ); }
    static const Vector4 UnitX() { return Vector4( T(1), T(0), T(0), T(0) ); }
    static const Vector4 UnitY() { return Vector4( T(0), T(1), T(0), T(0) ); }
    static const Vector4 UnitZ() { return Vector4( T(0), T(0), T(1), T(0) ); }
    static const Vector4 UnitW() { return Vector4( T(0), T(0), T(0), T(1) ); }
    static const Vector4 Identity() { return Vector4( T(1), T(0), T(0), T(0) ); }
    static const Vector4 Constant( const T x ) { return Vector4( x, x, x, x ); }
    static const Vector4 Random() { kvs::Xorshift128 r; return Vector4( T(r()), T(r()), T(r()), T(r()) ); }

public:
    Vector4();
    template <typename U>
    explicit Vector4( const kvs::Vector4<U>& v );
    Vector4( const T x, const T y, const T z, const T w );
    Vector4( const Vector2<T>& other, const T z , const T w );
    Vector4( const Vector3<T>& other, const T w );
    explicit Vector4( const T elements[4] );

    T& x() { return m_elements[0]; }
    T& y() { return m_elements[1]; }
    T& z() { return m_elements[2]; }
    T& w() { return m_elements[3]; }
    const T& x() const { return m_elements[0]; }
    const T& y() const { return m_elements[1]; }
    const T& z() const { return m_elements[2]; }
    const T& w() const { return m_elements[3]; }
    const Vector2<T> xy() const { return Vector2<T>( m_elements[0], m_elements[1] ); }
    const Vector3<T> xyz() const { return Vector3<T>( m_elements[0], m_elements[1], m_elements[2] ); }

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
    double length2() const;
    T dot( const Vector4& other ) const;
    const Vector4 normalized() const;

    const T* data() const;
    T* data();

public:
    const T& operator [] ( const size_t index ) const;
    T&       operator [] ( const size_t index );

public:
    Vector4& operator += ( const Vector4& rhs );
    Vector4& operator -= ( const Vector4& rhs );
    Vector4& operator *= ( const Vector4& rhs );
    Vector4& operator *= ( const T rhs );
    Vector4& operator /= ( const Vector4& rhs );
    Vector4& operator /= ( const T rhs );

    const Vector4 operator -() const;

public:
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
    m_elements[0] = x;
    m_elements[1] = y;
    m_elements[2] = z;
    m_elements[3] = w;
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
    m_elements[0] = other[0];
    m_elements[1] = other[1];
    m_elements[2] = z;
    m_elements[3] = w;
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
    m_elements[0] = other[0];
    m_elements[1] = other[1];
    m_elements[2] = other[2];
    m_elements[3] = w;
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
    m_elements[0] = elements[0];
    m_elements[1] = elements[1];
    m_elements[2] = elements[2];
    m_elements[3] = elements[3];
}

template<typename T>
inline void Vector4<T>::setZero()
{
    m_elements[0] = T(0);
    m_elements[1] = T(0);
    m_elements[2] = T(0);
    m_elements[3] = T(0);
}

template<typename T>
inline void Vector4<T>::setOnes()
{
    m_elements[0] = T(1);
    m_elements[1] = T(1);
    m_elements[2] = T(1);
    m_elements[3] = T(1);
}

template<typename T>
inline void Vector4<T>::setUnitX()
{
    m_elements[0] = T(1);
    m_elements[1] = T(0);
    m_elements[2] = T(0);
    m_elements[3] = T(0);
}

template<typename T>
inline void Vector4<T>::setUnitY()
{
    m_elements[0] = T(0);
    m_elements[1] = T(1);
    m_elements[2] = T(0);
    m_elements[3] = T(0);
}

template<typename T>
inline void Vector4<T>::setUnitZ()
{
    m_elements[0] = T(0);
    m_elements[1] = T(0);
    m_elements[2] = T(1);
    m_elements[3] = T(0);
}

template<typename T>
inline void Vector4<T>::setUnitW()
{
    m_elements[0] = T(0);
    m_elements[1] = T(0);
    m_elements[2] = T(0);
    m_elements[3] = T(1);
}

template<typename T>
inline void Vector4<T>::setIdentity()
{
    m_elements[0] = T(1);
    m_elements[1] = T(0);
    m_elements[2] = T(0);
    m_elements[3] = T(0);
}

template<typename T>
inline void Vector4<T>::setConstant( const T x )
{
    m_elements[0] = x;
    m_elements[1] = x;
    m_elements[2] = x;
    m_elements[3] = x;
}

template<typename T>
inline void Vector4<T>::setRandom()
{
    kvs::Xorshift128 r;
    m_elements[0] = T(r());
    m_elements[1] = T(r());
    m_elements[2] = T(r());
    m_elements[3] = T(r());
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
    std::swap( m_elements[0], other[0] );
    std::swap( m_elements[1], other[1] );
    std::swap( m_elements[2], other[2] );
    std::swap( m_elements[3], other[3] );
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
    return std::sqrt( this->length2() );
}

/*==========================================================================*/
/**
 *  @brief  Calculates a square of a length of this.
 *  @return Square of a length of this.
 */
/*==========================================================================*/
template<typename T>
inline double Vector4<T>::length2() const
{
    double result = 0.0;
    result += (double)m_elements[0] * (double)m_elements[0];
    result += (double)m_elements[1] * (double)m_elements[1];
    result += (double)m_elements[2] * (double)m_elements[2];
    result += (double)m_elements[3] * (double)m_elements[3];
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
    result += m_elements[0] * other[0];
    result += m_elements[1] * other[1];
    result += m_elements[2] * other[2];
    result += m_elements[3] * other[3];
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
    return m_elements[ index ];
}

template<typename T>
inline T& Vector4<T>::operator []( const size_t index )
{
    KVS_ASSERT( index < 4 );
    return m_elements[ index ];
}

template<typename T>
inline Vector4<T>& Vector4<T>::operator +=( const Vector4& rhs )
{
    m_elements[0] += rhs[0];
    m_elements[1] += rhs[1];
    m_elements[2] += rhs[2];
    m_elements[3] += rhs[3];
    return *this;
}

template<typename T>
inline Vector4<T>& Vector4<T>::operator -=( const Vector4& rhs )
{
    m_elements[0] -= rhs[0];
    m_elements[1] -= rhs[1];
    m_elements[2] -= rhs[2];
    m_elements[3] -= rhs[3];
    return *this;
}

template<typename T>
inline Vector4<T>& Vector4<T>::operator *=( const Vector4& rhs )
{
    m_elements[0] *= rhs[0];
    m_elements[1] *= rhs[1];
    m_elements[2] *= rhs[2];
    m_elements[3] *= rhs[3];
    return *this;
}

template<typename T>
inline Vector4<T>& Vector4<T>::operator *=( const T rhs )
{
    m_elements[0] *= rhs;
    m_elements[1] *= rhs;
    m_elements[2] *= rhs;
    m_elements[3] *= rhs;
    return *this;
}

template<typename T>
inline Vector4<T>& Vector4<T>::operator /=( const Vector4& rhs )
{
    m_elements[0] /= rhs[0];
    m_elements[1] /= rhs[1];
    m_elements[2] /= rhs[2];
    m_elements[3] /= rhs[3];
    return *this;
}

template<typename T>
inline Vector4<T>& Vector4<T>::operator /=( const T rhs )
{
    m_elements[0] /= rhs;
    m_elements[1] /= rhs;
    m_elements[2] /= rhs;
    m_elements[3] /= rhs;
    return *this;
}

template<typename T>
inline const Vector4<T> Vector4<T>::operator -() const
{
    return Vector4( *this ) *= T( -1 );
}

} // end of namespace kvs
