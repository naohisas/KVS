/****************************************************************************/
/**
 *  @file   Vector.h
 *  @author Naohisa Sakamoto
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
#include <sstream>
#include <vector>
#include <cstring>
#include <algorithm>
#include <numeric>
#include <iterator>
#include <initializer_list>
#include <kvs/DebugNew>
#include <kvs/Assert>
#include <kvs/Math>
#include <kvs/Value>
#include <kvs/Indent>
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
public:
    typedef Vector<T> this_type;
    typedef T value_type;
    typedef std::size_t size_type;
    typedef std::ptrdiff_t difference_type;
    typedef T& reference;
    typedef const T& const_reference;

    // Iterators
    typedef T* iterator;
    typedef const T* const_iterator;
    typedef std::reverse_iterator<iterator> reverse_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

private:
    size_t m_size; ///< Vector size( dimension ).
    value_type* m_data; ///< Array of elements.

public:
    static const Vector Zero( const size_t size );
    static const Vector Ones( const size_t size );
    static const Vector Unit( const size_t size, const size_t index = 0 );
    static const Vector Identity( const size_t size );
    static const Vector Constant( const size_t size, const T x );
    static const Vector Random( const size_t size );
    static const Vector Random( const size_t size, const kvs::UInt32 seed );
    static const Vector Random( const size_t size, const T min, const T max );
    static const Vector Random( const size_t size, const T min, const T max, const kvs::UInt32 seed );

public:
    Vector(): m_size( 0 ), m_data( nullptr ) {}
    ~Vector() { delete [] m_data; }

    explicit Vector( const size_t size );
    Vector( const size_t size, const T* elements );
    Vector( const std::vector<T>& other );
    Vector( const kvs::Vector2<T>& other );
    Vector( const kvs::Vector3<T>& other );
    Vector( const kvs::Vector4<T>& other );

    template <typename InIter>
    Vector( InIter first, InIter last );

    Vector( std::initializer_list<T> list );

    Vector( const Vector& other );
    Vector& operator =( const Vector& rhs );

    Vector( Vector&& other ) noexcept;
    Vector& operator =( Vector&& rhs ) noexcept;

public:
    T* data() { return m_data; }
    const T* data() const { return m_data; }
    size_t size() const { return m_size; }

    iterator begin() { return m_data; }
    iterator end() { return m_data + m_size; }
    const_iterator begin() const { return m_data; }
    const_iterator end() const { return m_data + m_size; }

    void setZero();
    void setOnes();
    void setUnit( const size_t index = 0 );
    void setIdentity();
    void setConstant( const T x );
    void setRandom();
    void setRandom( const kvs::UInt32 seed );
    void setRandom( const T min, const T max );
    void setRandom( const T min, const T max, const kvs::UInt32 seed );

    void resize( const size_t size );
    void swap( Vector& other );
    void normalize();
    double length() const;
    double squaredLength() const;
    T dot( const Vector& other ) const;
    const Vector normalized() const;

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
    Vector& operator +=( const Vector& rhs );
    Vector& operator -=( const Vector& rhs );
    Vector& operator *=( const Vector& rhs );
    Vector& operator *=( const T rhs );
    Vector& operator /=( const Vector& rhs );
    Vector& operator /=( const T rhs );
    const Vector operator -() const;

    friend bool operator ==( const Vector& lhs, const Vector& rhs )
    {
        if ( lhs.size() != rhs.size() ) { return false; }
        return std::equal( lhs.begin(), lhs.end(), rhs.begin() );
    }

    friend bool operator !=( const Vector& lhs, const Vector& rhs )
    {
        return !( lhs == rhs );
    }

    friend const Vector operator +( const Vector& lhs, const Vector& rhs )
    {
        Vector v( lhs ); v += rhs;
        return std::move( v );
    }

    friend const Vector operator -( const Vector& lhs, const Vector& rhs )
    {
        Vector v( lhs ); v -= rhs;
        return std::move( v );
    }

    friend const Vector operator *( const Vector& lhs, const Vector& rhs )
    {
        Vector v( lhs ); v *= rhs;
        return std::move( v );
    }

    friend const Vector operator *( const Vector& lhs, const T rhs )
    {
        Vector v( lhs ); v *= rhs;
        return std::move( v );
    }

    friend const Vector operator *( const T lhs, const Vector& rhs )
    {
        return std::move( rhs * lhs );
    }

    friend const Vector operator /( const Vector& lhs, const Vector& rhs )
    {
        Vector v( lhs ); v /= rhs;
        return std::move( v );
    }

    friend const Vector operator /( const Vector& lhs, const T rhs )
    {
        Vector v( lhs ); v /= rhs;
        return std::move( v );
    }

    friend std::ostream& operator << ( std::ostream& os, const Vector& rhs )
    {
        return os << rhs.format( " ", "", "" );
    }

public:
    KVS_DEPRECATED( void zero() ) { this->setZero(); }
    KVS_DEPRECATED( double length2() const ) { return this->squaredLength(); }
    KVS_DEPRECATED( void print() const ) { std::cout << *this << std::endl; }
    KVS_DEPRECATED( void setSize( const size_t size ) ) { this->resize( size ); }
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
    return std::move( v );
}

template <typename T>
inline const Vector<T> Vector<T>::Ones( const size_t size )
{
    Vector<T> v( size );
    v.setOnes();
    return std::move( v );
}

template <typename T>
inline const Vector<T> Vector<T>::Unit( const size_t size, const size_t index )
{
    Vector<T> v( size );
    v.setUnit( index );
    return std::move( v );
}

template <typename T>
inline const Vector<T> Vector<T>::Identity( const size_t size )
{
    Vector<T> v( size );
    v.setIdentity();
    return std::move( v );
}

template <typename T>
inline const Vector<T> Vector<T>::Constant( const size_t size, const T x )
{
    Vector<T> v( size );
    v.setConstant( x );
    return std::move( v );
}

template <typename T>
inline const Vector<T> Vector<T>::Random( const size_t size )
{
    Vector<T> v( size );
    v.setRandom();
    return std::move( v );
}

template <typename T>
inline const Vector<T> Vector<T>::Random( const size_t size, const kvs::UInt32 seed )
{
    Vector<T> v( size );
    v.setRandom( seed );
    return std::move( v );
}

template <typename T>
inline const Vector<T> Vector<T>::Random( const size_t size, const T min, const T max )
{
    Vector<T> v( size );
    v.setRandom( min, max );
    return std::move( v );
}

template <typename T>
inline const Vector<T> Vector<T>::Random(
    const size_t size,
    const T min,
    const T max,
    const kvs::UInt32 seed )
{
    Vector<T> v( size );
    v.setRandom( min, max, seed );
    return std::move( v );
}

/*==========================================================================*/
/**
 *  @brief  Constructs a new Vector.
 *  @param  size [in] Vector size.
 */
/*==========================================================================*/
template <typename T>
inline Vector<T>::Vector( const size_t size ):
    m_size( size ),
    m_data( new T [ size ] )
{
    this->setZero();
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
    m_size( size ),
    m_data( new T [ size ] )
{
    std::memcpy( m_data, elements, sizeof( T ) * this->size() );
}

/*==========================================================================*/
/**
 *  @brief  Constructs a new Vector.
 *  @param  other [in] std::vector.
 */
/*==========================================================================*/
template <typename T>
inline Vector<T>::Vector( const std::vector<T>& other ):
    m_size( other.size() ),
    m_data( new T [ other.size() ] )
{
    std::memcpy( m_data, &other[0], sizeof(T) * this->size() );
}

template <typename T>
inline Vector<T>::Vector( const kvs::Vector2<T>& other ):
    m_size( 2 ),
    m_data( new T [2] )
{
    std::memcpy( m_data, other.data(), sizeof(T) * this->size() );
}

template <typename T>
inline Vector<T>::Vector( const kvs::Vector3<T>& other ):
    m_size( 3 ),
    m_data( new T [3] )
{
    std::memcpy( m_data, other.data(), sizeof(T) * this->size() );
}

template <typename T>
inline Vector<T>::Vector( const kvs::Vector4<T>& other ):
    m_size( 4 ),
    m_data( new T [4] )
{
    std::memcpy( m_data, other.data(), sizeof(T) * this->size() );
}

template <typename T>
template <typename InIter>
inline Vector<T>::Vector( InIter first, InIter last ):
    m_size( std::distance( first, last ) ),
    m_data( new T [ m_size ] )
{
    std::copy( first, last, this->begin() );
}

template <typename T>
inline Vector<T>::Vector( std::initializer_list<T> list ):
    m_size( std::distance( list.begin(), list.end() ) ),
    m_data( new T [ m_size ] )
{
    std::copy( list.begin(), list.end(), this->begin() );
}

/*==========================================================================*/
/**
 *  @brief  Constructs a copy of other.
 *  @param  other [in] Vector.
 */
/*==========================================================================*/
template <typename T>
inline Vector<T>::Vector( const Vector& other ):
    m_size( other.size() ),
    m_data( new T [ other.size() ] )
{
    std::memcpy( m_data, other.m_data, sizeof(T) * this->size() );
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
    if ( this != &rhs )
    {
        if ( m_size != rhs.size() )
        {
            delete [] m_data;
            m_size = rhs.size();
            m_data = new T [ rhs.size() ];
        }

        std::memcpy( m_data, rhs.m_data, sizeof(T) * m_size );
    }

    return *this;
}

template <typename T>
inline Vector<T>::Vector( Vector&& other ) noexcept:
    m_size( other.m_size ),
    m_data( other.m_data )
{
    other.m_size = 0;
    other.m_data = nullptr;
}

template <typename T>
inline Vector<T>& Vector<T>::operator =( Vector&& rhs ) noexcept
{
    if ( this != &rhs )
    {
        m_size = rhs.m_size;
        m_data = rhs.m_data;

        rhs.m_size = 0;
        rhs.m_data = nullptr;
    }

    return *this;
}

template <typename T>
inline void Vector<T>::setZero()
{
    std::memset( m_data, 0, sizeof(T) * m_size );
}

template <typename T>
inline void Vector<T>::setOnes()
{
    std::fill( this->data(), this->data() + this->size(), T(1) );
}

template <typename T>
inline void Vector<T>::setUnit( const size_t index )
{
    KVS_ASSERT( index < m_size );
    if ( m_size > 0 ) { this->setZero(); m_data[index] = T(1); }
}

template <typename T>
inline void Vector<T>::setIdentity()
{
    this->setUnit(0);
}

template <typename T>
inline void Vector<T>::setConstant( const T x )
{
    std::fill( this->data(), this->data() + this->size(), x );
}


template<typename T>
inline void Vector<T>::setRandom()
{
    if ( m_size > 0 )
    {
        static bool flag = true;
        if ( flag ) { kvs::Value<T>::SetRandomSeed(); flag = false; }
        const_iterator last = this->end();
        for ( iterator v = this->begin(); v != last; ++v ) { *v = kvs::Value<T>::Random(); }
    }
}

template<typename T>
inline void Vector<T>::setRandom( const kvs::UInt32 seed )
{
    if ( m_size > 0 )
    {
        if ( seed > 0 ) kvs::Value<T>::SetSeed( seed );
        else kvs::Value<T>::SetRandomSeed();
        const_iterator last = this->end();
        for ( iterator v = this->begin(); v != last; ++v ) { *v = kvs::Value<T>::Random(); }
    }
}

template<typename T>
inline void Vector<T>::setRandom( const T min, const T max )
{
    if ( m_size > 0 )
    {
        static bool flag = true;
        if ( flag ) { kvs::Value<T>::SetRandomSeed(); flag = false; }
        const_iterator last = this->end();
        for ( iterator v = this->begin(); v != last; ++v ) { *v = kvs::Value<T>::Random( min, max ); }
    }
}

template<typename T>
inline void Vector<T>::setRandom( const T min, const T max, const kvs::UInt32 seed )
{
    if ( m_size > 0 )
    {
        if ( seed > 0 ) kvs::Value<T>::SetSeed( seed );
        else kvs::Value<T>::SetRandomSeed();
        const_iterator last = this->end();
        for ( iterator v = this->begin(); v != last; ++v ) { *v = kvs::Value<T>::Random( min, max ); }
    }
}

template <typename T>
inline void Vector<T>::resize( const size_t size )
{
    if ( this->size() != size )
    {
        delete [] m_data;
        m_size = size;
        m_data = new T [ size ];
        std::memset( m_data, 0, sizeof(T) * m_size );
    }
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
    std::swap( m_data, other.m_data );
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
 *  @brief  Calculates a length of this.
 *  @return Length of this.
 */
/*==========================================================================*/
template <typename T>
inline double Vector<T>::length() const
{
    return std::sqrt( this->squaredLength() );
}

/*==========================================================================*/
/**
 *  @brief  Calculates a square of a length of this.
 *  @return Square of a length of this.
 */
/*==========================================================================*/
template <typename T>
inline double Vector<T>::squaredLength() const
{
    return std::inner_product( this->begin(), this->end(), this->begin(), double(0) );
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
    return std::inner_product( this->begin(), this->end(), other.begin(), static_cast<T>(0) );
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

/*==========================================================================*/
/**
 *  @brief  Prints the elements as a formatted string.
 *  @param delim [in] delimiter
 *  @param bracket_l [in] left bracket
 *  @param bracket_r [in] right bracket
 */
/*==========================================================================*/
template<typename T>
inline std::string Vector<T>::format(
    const std::string delim,
    const std::string bracket_l,
    const std::string bracket_r ) const
{
    std::ostringstream os;
    if ( m_size == 0 ) { os << bracket_l << " " << bracket_r; }
    else
    {
        os << bracket_l;
        os << m_data[0];
        for ( size_t i = 1; i < m_size; ++i ) { os << delim << m_data[i]; }
        os << bracket_r;
    }
    return os.str();
}

template <typename T>
inline const T& Vector<T>::operator []( const size_t index ) const
{
    KVS_ASSERT( index < this->size() );
    return *( m_data + index );
}

template <typename T>
inline T& Vector<T>::operator []( const size_t index )
{
    KVS_ASSERT( index < this->size() );
    return *( m_data + index );
}

template <typename T>
inline Vector<T>& Vector<T>::operator +=( const Vector& rhs )
{
    KVS_ASSERT( this->size() == rhs.size() );
    const_iterator r = rhs.begin();
    const_iterator last = this->end();
    for ( iterator v = this->begin(); v != last; ++v, ++r ) { *v += *r; }
    return *this;
}

template <typename T>
inline Vector<T>& Vector<T>::operator -=( const Vector& rhs )
{
    KVS_ASSERT( this->size() == rhs.size() );
    const_iterator r = rhs.begin();
    const_iterator last = this->end();
    for ( iterator v = this->begin(); v != last; ++v, ++r ) { *v -= *r; }
    return *this;
}

template <typename T>
inline Vector<T>& Vector<T>::operator *=( const Vector& rhs )
{
    KVS_ASSERT( this->size() == rhs.size() );
    const_iterator r = rhs.begin();
    const_iterator last = this->end();
    for ( iterator v = this->begin(); v != last; ++v, ++r ) { *v *= *r; }
    return *this;
}

template <typename T>
inline Vector<T>& Vector<T>::operator *=( const T rhs )
{
    const_iterator last = this->end();
    for ( iterator v = this->begin(); v != last; ++v ) { *v *= rhs; }
    return *this;
}

template<typename T>
inline Vector<T>& Vector<T>::operator /=( const Vector& rhs )
{
    const_iterator r = rhs.begin();
    const_iterator last = this->end();
    for ( iterator v = this->begin(); v != last; ++v, ++r ) { *v /= *r; }
    return *this;
}

template <typename T>
inline Vector<T>& Vector<T>::operator /=( const T rhs )
{
    const_iterator last = this->end();
    for ( iterator v = this->begin(); v != last; ++v ) { *v /= rhs; }
    return *this;
}

template<typename T>
inline const Vector<T> Vector<T>::operator -() const
{
    Vector v( *this ); v *= T( -1 );
    return std::move( v );
}

} // end of namespace kvs

namespace std
{

template <typename T>
inline void swap( kvs::Vector<T>& lhs, kvs::Vector<T>& rhs )
{
    lhs.swap( rhs );
}

} // end of namespace std
