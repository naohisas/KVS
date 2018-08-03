/****************************************************************************/
/**
 *  @file Vector2.h
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
#ifndef KVS__VECTOR_2_H_INCLUDE
#define KVS__VECTOR_2_H_INCLUDE

#include <iostream> // For std::cout.
#include <kvs/Assert>
#include <kvs/Math>
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
    T m_elements[2]; ///< Elements.

public:
    static const Vector2 Zero();
    static const Vector2 All( const T x );

public:
    Vector2();
    template <typename U>
    explicit Vector2( const kvs::Vector2<U>& v );
    Vector2( const T x, const T y );
    explicit Vector2( const T elements[2] );

    void set( const T x, const T y );
    void set( const T elements[2] );

    T& x();
    T& y();
    const T& x() const;
    const T& y() const;

    void zero();
    void swap( Vector2& other );
    void normalize();
    void print() const;

    double length() const;
    double length2() const;
    T dot( const Vector2& other ) const;
    const Vector2 normalized() const;

    const T* data() const;
    T* data();

public:
    const T& operator []( const size_t index ) const;
    T&       operator []( const size_t index );

    Vector2& operator +=( const Vector2& rhs );
    Vector2& operator -=( const Vector2& rhs );
    Vector2& operator *=( const Vector2& rhs );
    Vector2& operator *=( const T rhs );
    Vector2& operator /=( const Vector2& rhs );
    Vector2& operator /=( const T rhs );

    const Vector2 operator -() const;

public:
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
        return os << rhs[0] << " " << rhs[1];
    }

public:
    KVS_DEPRECATED( explicit Vector2( const T x ) ) { *this = All( x ); }
    KVS_DEPRECATED( void set( const T x ) ) { *this = All( x ); }
};

/*==========================================================================*/
/**
 *  Type definition.
 */
/*==========================================================================*/
typedef Vector2<int>          Vector2i;
typedef Vector2<unsigned int> Vector2u;
typedef Vector2<float>        Vector2f;
typedef Vector2<double>       Vector2d;
typedef Vector2<float>        Vec2;
typedef Vector2<unsigned int> Vec2u;
typedef Vector2<int>          Vec2i;
typedef Vector2<double>       Vec2d;

typedef Vector2<unsigned int> Vector2ui;
typedef Vector2<unsigned int> Vec2ui;


/*===========================================================================*/
/**
 *  @brief  Returns a zero vector.
 */
/*===========================================================================*/
template <typename T>
inline const Vector2<T> Vector2<T>::Zero()
{
    return Vector2( 0, 0 );
}

/*===========================================================================*/
/**
 *  @brief  Returns a vector which all elements are same as x.
 *  @param  x [in] element value
 */
/*===========================================================================*/
template <typename T>
inline const Vector2<T> Vector2<T>::All( const T x )
{
    return Vector2( x, x );
}

/*==========================================================================*/
/**
 *  @brief  Constructs a new Vector2.
 */
/*==========================================================================*/
template<typename T>
inline Vector2<T>::Vector2()
{
    this->zero();
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
    m_elements[0] = x;
    m_elements[1] = y;
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
    m_elements[0] = elements[0];
    m_elements[1] = elements[1];
}

/*==========================================================================*/
/**
 *  @brief  Returns the first element.
 *  @return Reference of the first element.
 */
/*==========================================================================*/
template<typename T>
inline T& Vector2<T>::x()
{
    return m_elements[0];
}

/*==========================================================================*/
/**
 *  @brief  Returns the second element.
 *  @return Reference of the second element.
 */
/*==========================================================================*/
template<typename T>
inline T& Vector2<T>::y()
{
    return m_elements[1];
}

/*==========================================================================*/
/**
 *  @brief  Returns the first element.
 *  @return Reference of the first element.
 */
/*==========================================================================*/
template<typename T>
inline const T& Vector2<T>::x() const
{
    return m_elements[0];
}

/*==========================================================================*/
/**
 *  @brief  Returns the second element.
 *  @return Reference of the second element.
 */
/*==========================================================================*/
template<typename T>
inline const T& Vector2<T>::y() const
{
    return m_elements[1];
}

/*==========================================================================*/
/**
 *  @brief  Sets zero to the elements.
 */
/*==========================================================================*/
template<typename T>
inline void Vector2<T>::zero()
{
    m_elements[0] = T( 0 );
    m_elements[1] = T( 0 );
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
    std::swap( m_elements[0], other[0] );
    std::swap( m_elements[1], other[1] );
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
 *  @brief  Prints the elements of this.
 */
/*==========================================================================*/
template<typename T>
inline void Vector2<T>::print() const
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
inline double Vector2<T>::length() const
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
inline double Vector2<T>::length2() const
{
    double result = 0.0;
    result += (double)m_elements[0] * (double)m_elements[0];
    result += (double)m_elements[1] * (double)m_elements[1];
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
    result += m_elements[0] * other[0];
    result += m_elements[1] * other[1];
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

template <typename T>
inline const T* Vector2<T>::data() const
{
    return &m_elements[0];
}

template <typename T>
inline T* Vector2<T>::data()
{
    return &m_elements[0];
}

template<typename T>
inline const T& Vector2<T>::operator []( const size_t index ) const
{
    KVS_ASSERT( index < 2 );
    return m_elements[ index ];
}

template<typename T>
inline T& Vector2<T>::operator []( const size_t index )
{
    KVS_ASSERT( index < 2 );
    return m_elements[ index ];
}

template<typename T>
inline Vector2<T>& Vector2<T>::operator +=( const Vector2& rhs )
{
    m_elements[0] += rhs[0];
    m_elements[1] += rhs[1];
    return *this;
}

template<typename T>
inline Vector2<T>& Vector2<T>::operator -=( const Vector2& rhs )
{
    m_elements[0] -= rhs[0];
    m_elements[1] -= rhs[1];
    return *this;
}

template<typename T>
inline Vector2<T>& Vector2<T>::operator *=( const Vector2& rhs )
{
    m_elements[0] *= rhs[0];
    m_elements[1] *= rhs[1];
    return *this;
}

template<typename T>
inline Vector2<T>& Vector2<T>::operator *=( const T rhs )
{
    m_elements[0] *= rhs;
    m_elements[1] *= rhs;
    return *this;
}

template<typename T>
inline Vector2<T>& Vector2<T>::operator /=( const Vector2& rhs )
{
    m_elements[0] /= rhs[0];
    m_elements[1] /= rhs[1];
    return *this;
}

template<typename T>
inline Vector2<T>& Vector2<T>::operator /=( const T rhs )
{
    m_elements[0] /= rhs;
    m_elements[1] /= rhs;
    return *this;
}

template<typename T>
inline const Vector2<T> Vector2<T>::operator -() const
{
    return Vector2( *this ) *= T( -1 );
}

} // end of namespace kvs

#endif // KVS__VECTOR_2_H_INCLUDE
