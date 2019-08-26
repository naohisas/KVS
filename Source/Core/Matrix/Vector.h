/****************************************************************************/
/**
 *  @file Vector.h
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: Vector.h 1366 2012-11-29 08:46:38Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#pragma once
#include <iostream>
#include <vector>
#include <cstring>
#include <kvs/DebugNew>
#include <kvs/Assert>
#include <kvs/Math>
#include <kvs/Vector2>
#include <kvs/Vector3>
#include <kvs/Vector4>
#include <kvs/Deprecated>


namespace kvs
{

/*==========================================================================*/
/**
 *  n-D vector class.
 */
/*==========================================================================*/
template <typename T>
class Vector
{
private:
    size_t m_size; ///< Vector size( dimension ).
    T* m_elements; ///< Array of elements.

public:
    static const Vector Zero( const size_t size );
    static const Vector Ones( const size_t size );
    static const Vector Unit( const size_t size, const size_t index = 0 );
    static const Vector Identity( const size_t size );
    static const Vector Constant( const size_t size, const T x );
    static const Vector Random( const size_t size );

public:
    explicit Vector( const size_t size = 0 );
    Vector( const size_t size, const T* elements );
    Vector( const std::vector<T>& other );
    Vector( const kvs::Vector2<T>& other );
    Vector( const kvs::Vector3<T>& other );
    Vector( const kvs::Vector4<T>& other );

    ~Vector();

    Vector( const Vector& other );
    Vector& operator =( const Vector& rhs );

    void setSize( const size_t size );
    void setZero();
    void setOnes();
    void setUnit( const size_t index = 0 );
    void setIdentity();
    void setConstant( const T x );
    void setRandom();

    size_t size() const;

    void swap( Vector& other );
    void normalize();
    void print() const;

    double length() const;
    double length2() const;
    T dot( const Vector& other ) const;
    const Vector normalized() const;

public:
    const T& operator []( const size_t index ) const;
    T&       operator []( const size_t index );

public:
    Vector& operator +=( const Vector& rhs );
    Vector& operator -=( const Vector& rhs );
    Vector& operator *=( const Vector& rhs );
    Vector& operator *=( const T rhs );
    Vector& operator /=( const T rhs );

    const Vector operator -() const;

public:
    friend bool operator ==( const Vector& lhs, const Vector& rhs )
    {
        const size_t size = lhs.size();

        if ( size != rhs.size() )
           return false;

        for ( size_t i = 0; i < size; ++i )
        {
            if ( !kvs::Math::Equal( lhs[i], rhs[i] ) )
                return false;
        }
        return true;
    }

    friend bool operator !=( const Vector& lhs, const Vector& rhs )
    {
        return !( lhs == rhs );
    }

    friend const Vector operator +( const Vector& lhs, const Vector& rhs )
    {
        return Vector( lhs ) += rhs;
    }

    friend const Vector operator -( const Vector& lhs, const Vector& rhs )
    {
        return Vector( lhs ) -= rhs;
    }

    friend const Vector operator *( const Vector& lhs, const Vector& rhs )
    {
        return Vector( lhs ) *= rhs;
    }

    friend const Vector operator *( const Vector& lhs, const T rhs )
    {
        return Vector( lhs ) *= rhs;
    }

    friend const Vector operator *( const T lhs, const Vector& rhs )
    {
        return rhs * lhs;
    }

    friend const Vector operator /( const Vector& lhs, const T rhs )
    {
        return Vector( lhs ) /= rhs;
    }

    friend std::ostream& operator << ( std::ostream& os, const Vector& rhs )
    {
        const size_t size = rhs.size();
        if ( size != 0 )
        {
            for ( size_t i = 0; i < size - 1; ++i )
            {
                os <<  rhs[i] << " ";
            }
            os << rhs[ size - 1 ];
        }
        return os;
    }

public:
    KVS_DEPRECATED( void zero() ) { this->setZero(); }
};

/*==========================================================================*/
/**
 *  Type definition.
 */
/*==========================================================================*/
typedef Vector<int> Veci;
typedef Vector<unsigned int> Vecu;
typedef Vector<float> Vec;
typedef Vector<double> Vecd;


template <typename T>
inline const Vector<T> Vector<T>::Zero( const size_t size )
{
    Vector<T> v( size );
    v.setZero();
    return v;
}

template <typename T>
inline const Vector<T> Vector<T>::Ones( const size_t size )
{
    Vector<T> v( size );
    v.setOnes();
    return v;
}

template <typename T>
inline const Vector<T> Vector<T>::Unit( const size_t size, const size_t index )
{
    Vector<T> v( size );
    v.setUnit( index );
    return v;
}

template <typename T>
inline const Vector<T> Vector<T>::Identity( const size_t size )
{
    Vector<T> v( size );
    v.setIdentity();
    return v;
}

template <typename T>
inline const Vector<T> Vector<T>::Constant( const size_t size, const T x )
{
    Vector<T> v( size );
    v.setConstant( x );
    return v;
}

template <typename T>
inline const Vector<T> Vector<T>::Random( const size_t size )
{
    Vector<T> v( size );
    v.setRandom();
    return v;
}

/*==========================================================================*/
/**
 *  @brief  Constructs a new Vector.
 *  @param  size [in] Vector size.
 */
/*==========================================================================*/
template <typename T>
inline Vector<T>::Vector( const size_t size ):
    m_size( 0 ),
    m_elements( 0 )
{
    this->setSize( size );
    this->setZero();
//    this->zero();
}

/*==========================================================================*/
/**
 *  @brief  Constructs a new Vector.
 *  @param  size     [in] Size of array.
 *  @param  elements [in] Array of elements.
 */
/*==========================================================================*/
template <typename T>
inline Vector<T>::Vector( const size_t size, const T* elements ):
    m_size( 0 ),
    m_elements( 0 )
{
    this->setSize( size );
    memcpy( m_elements, elements, sizeof( T ) * this->size() );
}

/*==========================================================================*/
/**
 *  @brief  Constructs a new Vector.
 *  @param  other [in] std::vector.
 */
/*==========================================================================*/
template <typename T>
inline Vector<T>::Vector( const std::vector<T>& other ):
    m_size( 0 ),
    m_elements( 0 )
{
    this->setSize( other.size() );
    memcpy( m_elements, &other[0], sizeof(T) * this->size() );
}

template <typename T>
inline Vector<T>::Vector( const kvs::Vector2<T>& other ):
    m_size( 0 ),
    m_elements( 0 )
{
    this->setSize( 2 );
    memcpy( m_elements, other.data(), sizeof(T) * this->size() );
}

template <typename T>
inline Vector<T>::Vector( const kvs::Vector3<T>& other ):
    m_size( 0 ),
    m_elements( 0 )
{
    this->setSize( 3 );
    memcpy( m_elements, other.data(), sizeof(T) * this->size() );
}

template <typename T>
inline Vector<T>::Vector( const kvs::Vector4<T>& other ):
    m_size( 0 ),
    m_elements( 0 )
{
    this->setSize( 4 );
    memcpy( m_elements, other.data(), sizeof(T) * this->size() );
}

/*===========================================================================*/
/**
 *  @brief  Destroys the Vector.
 */
/*===========================================================================*/
template <typename T>
inline Vector<T>::~Vector()
{
    delete [] m_elements;
}

/*==========================================================================*/
/**
 *  @brief  Constructs a copy of other.
 *  @param  other [in] Vector.
 */
/*==========================================================================*/
template <typename T>
inline Vector<T>::Vector( const Vector& other ):
    m_size( 0 ),
    m_elements( 0 )
{
    this->setSize( other.size() );
    memcpy( m_elements, other.m_elements, sizeof( T ) * this->size() );
}

/*==========================================================================*/
/**
 *  @brief  Substitution operator '='.
 *  @param  other [in] Vector.
 */
/*==========================================================================*/
template <typename T>
inline Vector<T>& Vector<T>::operator =( const Vector& rhs )
{
    this->setSize( rhs.size() );
    memcpy( m_elements, rhs.m_elements, sizeof( T ) * this->size() );
    return *this;
}

/*==========================================================================*/
/**
 *  @brief  Sets the size of vector.
 *  @param  size [in] Size of vector.
 */
/*==========================================================================*/
template <typename T>
inline void Vector<T>::setSize( const size_t size )
{
    if ( this->size() != size )
    {
        m_size = size;

        delete [] m_elements;
        m_elements = 0;

        if ( size != 0 )
        {
            m_elements = new T[ size ];
        }
    }

//    this->zero();
    this->setZero();
}

template <typename T>
inline void Vector<T>::setZero()
{
    if ( m_size > 0 )
    {
        for ( size_t i = 0; i < m_size; ++i ) { m_elements[i] = T(0); }
    }
}

template <typename T>
inline void Vector<T>::setOnes()
{
    if ( m_size > 0 )
    {
        for ( size_t i = 0; i < m_size; ++i ) { m_elements[i] = T(1); }
    }
}

template <typename T>
inline void Vector<T>::setUnit( const size_t index )
{
    KVS_ASSERT( index < m_size );
    if ( m_size > 0 )
    {
        this->setZero();
        m_elements[index] = T(1);
    }
}

template <typename T>
inline void Vector<T>::setIdentity()
{
    this->setUnit(0);
}

template <typename T>
inline void Vector<T>::setConstant( const T x )
{
    if ( m_size > 0 )
    {
        for ( size_t i = 0; i < m_size; ++i ) { m_elements[i] = x; }
    }
}

template <typename T>
inline void Vector<T>::setRandom()
{
    if ( m_size > 0 )
    {
        kvs::Xorshift128 r;
        for ( size_t i = 0; i < m_size; ++i ) { m_elements[i] = T(r()); }
    }
}

/*==========================================================================*/
/**
 *  @brief  Returns the size of vector.
 *  @return Size of vector.
 */
/*==========================================================================*/
template <typename T>
inline size_t Vector<T>::size() const
{
    return m_size;
}

/*==========================================================================*/
/**
 *  @brief  Swaps this and other.
 *  @param  other [in,out] Vector.
 */
/*==========================================================================*/
template<typename T>
inline void Vector<T>::swap( Vector& other )
{
    std::swap( m_size, other.m_size );
    std::swap( m_elements, other.m_elements );
}

/*==========================================================================*/
/**
 *  @brief  Normalizes this.
 *  @return Normalized Vector.
 */
/*==========================================================================*/
template <typename T>
inline void Vector<T>::normalize()
{
    const double length = this->length();
    const T normalize_factor = length > 0.0 ? static_cast<T>( 1.0 / length ) : T( 0 );
    *this *= normalize_factor;
}

/*==========================================================================*/
/**
 *  @brief  Prints the elements.
 */
/*==========================================================================*/
template <typename T>
inline void Vector<T>::print() const
{
    std::cout << *this << std::endl;
}

/*==========================================================================*/
/**
 *  @brief  Calculates a length of this.
 *  @return Length of this.
 */
/*==========================================================================*/
template <typename T>
inline double Vector<T>::length() const
{
    return std::sqrt( this->length2() );
}

/*==========================================================================*/
/**
 *  @brief  Calculates a square of a length of this.
 *  @return Square of a length of this.
 */
/*==========================================================================*/
template <typename T>
inline double Vector<T>::length2() const
{
    const size_t size = this->size();
    const T* const v = m_elements;

    double result = 0.0;
    for ( size_t i = 0; i < size; ++i ) { result += (double)v[i] * (double)v[i]; }
    return result;
}

/*==========================================================================*/
/**
 *  @brief  Calculates a dot product.
 *  @param  other [in] Vector.
 *  @return Dot product.
 */
/*==========================================================================*/
template <typename T>
inline T Vector<T>::dot( const Vector<T>& other ) const
{
    KVS_ASSERT( this->size() == other.size() );

    const size_t size = this->size();
    const T* const v = m_elements;

    T result( 0 );
    for ( size_t i = 0; i < size; ++i ) { result += v[i] * other[i]; }
    return result;
}

/*==========================================================================*/
/**
 *  @brief  Copies this and normalizes it.
 *  @return Normalized Vector.
 */
/*==========================================================================*/
template <typename T>
inline const Vector<T> Vector<T>::normalized() const
{
    const double length = this->length();
    const T normalize_factor = length > 0.0 ? static_cast<T>( 1.0 / length ) : T( 0 );
    return *this * normalize_factor;
}

template <typename T>
inline const T& Vector<T>::operator []( const size_t index ) const
{
    KVS_ASSERT( index < this->size() );
    return m_elements[ index ];
}

template <typename T>
inline T& Vector<T>::operator []( const size_t index )
{
    KVS_ASSERT( index < this->size() );
    return m_elements[ index ];
}

template <typename T>
inline Vector<T>& Vector<T>::operator +=( const Vector& rhs )
{
    KVS_ASSERT( this->size() == rhs.size() );
    const size_t size = this->size();
    T* const v = m_elements;
    for ( size_t i = 0; i < size; ++i ) { v[i] += rhs[i]; }
    return *this;
}

template <typename T>
inline Vector<T>& Vector<T>::operator -=( const Vector& rhs )
{
    KVS_ASSERT( this->size() == rhs.size() );
    const size_t size = this->size();
    T* const v = m_elements;
    for ( size_t i = 0; i < size; ++i ) { v[i] -= rhs[i]; }
    return *this;
}

template <typename T>
inline Vector<T>& Vector<T>::operator *=( const Vector& rhs )
{
    KVS_ASSERT( this->size() == rhs.size() );
    const size_t size = this->size();
    T* const v = m_elements;
    for ( size_t i = 0; i < size; ++i ) { v[i] *= rhs[i]; }
    return *this;
}

template <typename T>
inline Vector<T>& Vector<T>::operator *=( const T rhs )
{
    const size_t size = this->size();
    T* const v = m_elements;
    for ( size_t i = 0; i < size; ++i ) { v[i] *= rhs; }
    return *this;
}

template <typename T>
inline Vector<T>& Vector<T>::operator /=( const T rhs )
{
    const size_t size = this->size();
    T* const v = m_elements;
    for ( size_t i = 0; i < size; ++i ) { v[i] /= rhs; }
    return *this;
}

template<typename T>
inline const Vector<T> Vector<T>::operator -() const
{
    return Vector( *this ) *= T( -1 );
}

} // end of namespace kvs
