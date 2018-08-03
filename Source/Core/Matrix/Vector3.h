/****************************************************************************/
/**
 *  @file Vector3.h
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
#ifndef KVS__VECTOR_3_H_INCLUDE
#define KVS__VECTOR_3_H_INCLUDE

#include <iostream> // For std::cout.
#include <kvs/Assert>
#include <kvs/Math>
#include <kvs/Vector2>
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
    T m_elements[3]; ///< Elements.

public:
    static const Vector3 Zero();
    static const Vector3 All( const T x );

public:
    Vector3();
    template <typename U>
    explicit Vector3( const kvs::Vector3<U>& v );
    Vector3( const T x, const T y, const T z );
    explicit Vector3( const Vector2<T>& other, const T z );
    explicit Vector3( const T elements[3] );

    void set( const T x, const T y, const T z );
    void set( const Vector2<T>& other, const T z );
    void set( const T elements[3] );

    T& x();
    T& y();
    T& z();
    const T& x() const;
    const T& y() const;
    const T& z() const;
    const Vector2<T> xy() const;

    void zero();
    void swap( Vector3& other );
    void normalize();
    void print() const;

    double length() const;
    double length2() const;
    T dot( const Vector3& other ) const;
    const Vector3 cross( const Vector3& other ) const;
    const Vector3 normalized() const;

    const T* data() const;
    T* data();

public:
    const T& operator []( const size_t index ) const;
    T&       operator []( const size_t index );

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
        return os << rhs[0] << " " << rhs[1] << " " << rhs[2];
    }

public:
    KVS_DEPRECATED( explicit Vector3( const T x ) ) { *this = All( x ); }
    KVS_DEPRECATED( void set( const T x ) ) { *this = All( x ); }
};

/*==========================================================================*/
/**
 *  Type definition.
 */
/*==========================================================================*/
typedef Vector3<int>          Vector3i;
typedef Vector3<unsigned int> Vector3u;
typedef Vector3<float>        Vector3f;
typedef Vector3<double>       Vector3d;
typedef Vector3<float>        Vec3;
typedef Vector3<int>          Vec3i;
typedef Vector3<unsigned int> Vec3u;
typedef Vector3<double>       Vec3d;

typedef Vector3<unsigned int> Vector3ui;
typedef Vector3<unsigned int> Vec3ui;

/*===========================================================================*/
/**
 *  @brief  Returns a zero vector.
 */
/*===========================================================================*/
template <typename T>
inline const Vector3<T> Vector3<T>::Zero()
{
    return Vector3( 0, 0, 0 );
}

/*===========================================================================*/
/**
 *  @brief  Returns a vector which all elements are same as x.
 *  @param  x [in] element value
 */
/*===========================================================================*/
template <typename T>
inline const Vector3<T> Vector3<T>::All( const T x )
{
    return Vector3( x, x, x );
}

/*==========================================================================*/
/**
 *  @brief  Constructs a new Vector3.
 */
/*==========================================================================*/
template<typename T>
inline Vector3<T>::Vector3()
{
    this->zero();
}

template <typename T>
template <typename U>
inline Vector3<T>::Vector3( const kvs::Vector3<U>& v )
{
    this->set( static_cast<T>( v.x() ), static_cast<T>( v.y() ), static_cast<T>( v.z() ) );
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
    m_elements[0] = x;
    m_elements[1] = y;
    m_elements[2] = z;
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
    m_elements[0] = other[0];
    m_elements[1] = other[1];
    m_elements[2] = z;
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
    m_elements[0] = elements[0];
    m_elements[1] = elements[1];
    m_elements[2] = elements[2];
}

/*==========================================================================*/
/**
 *  @brief  Returns the first element.
 *  @return Reference of the first element.
 */
/*==========================================================================*/
template<typename T>
inline T& Vector3<T>::x()
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
inline T& Vector3<T>::y()
{
    return m_elements[1];
}

/*==========================================================================*/
/**
 *  @brief  Returns the third element.
 *  @return Reference of the third element.
 */
/*==========================================================================*/
template<typename T>
inline T& Vector3<T>::z()
{
    return m_elements[2];
}

/*==========================================================================*/
/**
 *  @brief  Returns the first element.
 *  @return Reference of the first element.
 */
/*==========================================================================*/
template<typename T>
inline const T& Vector3<T>::x() const
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
inline const T& Vector3<T>::y() const
{
    return m_elements[1];
}

/*==========================================================================*/
/**
 *  @brief  Returns the third element.
 *  @return Reference of the third element.
 */
/*==========================================================================*/
template<typename T>
inline const T& Vector3<T>::z() const
{
    return m_elements[2];
}

/*===========================================================================*/
/**
 *  @brief  Returns a vector which has the x and y elements of this vector.
 *  @param  vector which has the x and y elements
 */
/*===========================================================================*/
template <typename T>
inline const Vector2<T> Vector3<T>::xy() const
{
    return Vector2<T>( m_elements[0], m_elements[1] );
}

/*==========================================================================*/
/**
 *  @brief  Sets the elements to zero.
 */
/*==========================================================================*/
template<typename T>
inline void Vector3<T>::zero()
{
    m_elements[0] = T( 0 );
    m_elements[1] = T( 0 );
    m_elements[2] = T( 0 );
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
    std::swap( m_elements[0], other[0] );
    std::swap( m_elements[1], other[1] );
    std::swap( m_elements[2], other[2] );
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
 *  @brief  Prints the elements of this.
 */
/*==========================================================================*/
template<typename T>
inline void Vector3<T>::print() const
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
inline double Vector3<T>::length() const
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
inline double Vector3<T>::length2() const
{
    double result = 0.0;
    result += (double)m_elements[0] * (double)m_elements[0];
    result += (double)m_elements[1] * (double)m_elements[1];
    result += (double)m_elements[2] * (double)m_elements[2];
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
    result += m_elements[0] * other[0];
    result += m_elements[1] * other[1];
    result += m_elements[2] * other[2];
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
    const T x = m_elements[1] * other[2] - m_elements[2] * other[1];
    const T y = m_elements[2] * other[0] - m_elements[0] * other[2];
    const T z = m_elements[0] * other[1] - m_elements[1] * other[0];
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

template <typename T>
inline const T* Vector3<T>::data() const
{
    return &m_elements[0];
}

template <typename T>
inline T* Vector3<T>::data()
{
    return &m_elements[0];
}

template<typename T>
inline const T& Vector3<T>::operator []( const size_t index ) const
{
    KVS_ASSERT( index < 3 );
    return m_elements[ index ];
}

template<typename T>
inline T& Vector3<T>::operator []( const size_t index )
{
    KVS_ASSERT( index < 3 );
    return m_elements[ index ];
}

template<typename T>
inline Vector3<T>& Vector3<T>::operator +=( const Vector3& rhs )
{
    m_elements[0] += rhs[0];
    m_elements[1] += rhs[1];
    m_elements[2] += rhs[2];
    return *this;
}

template<typename T>
inline Vector3<T>& Vector3<T>::operator -=( const Vector3& rhs )
{
    m_elements[0] -= rhs[0];
    m_elements[1] -= rhs[1];
    m_elements[2] -= rhs[2];
    return *this;
}

template<typename T>
inline Vector3<T>& Vector3<T>::operator *=( const Vector3& rhs )
{
    m_elements[0] *= rhs[0];
    m_elements[1] *= rhs[1];
    m_elements[2] *= rhs[2];
    return *this;
}

template<typename T>
inline Vector3<T>& Vector3<T>::operator *=( const T rhs )
{
    m_elements[0] *= rhs;
    m_elements[1] *= rhs;
    m_elements[2] *= rhs;
    return *this;
}

template<typename T>
inline Vector3<T>& Vector3<T>::operator /=( const Vector3& rhs )
{
    m_elements[0] /= rhs[0];
    m_elements[1] /= rhs[1];
    m_elements[2] /= rhs[2];
    return *this;
}

template<typename T>
inline Vector3<T>& Vector3<T>::operator /=( const T rhs )
{
    m_elements[0] /= rhs;
    m_elements[1] /= rhs;
    m_elements[2] /= rhs;
    return *this;
}

template<typename T>
inline const Vector3<T> Vector3<T>::operator -() const
{
    return Vector3( *this ) *= T( -1 );
}

} // end of namespace kvs

#endif // KVS__VECTOR_3_H_INCLUDE
