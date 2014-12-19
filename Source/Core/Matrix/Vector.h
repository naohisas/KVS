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
#ifndef KVS__VECTOR_H_INCLUDE
#define KVS__VECTOR_H_INCLUDE

#include <iostream>
#include <vector>
#include <cstring>
#include <kvs/DebugNew>
#include <kvs/Assert>
#include <kvs/Math>


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
    size_t m_size;     ///< Vector size( dimension ).
    T*     m_elements; ///< Array of elements.

public:
    explicit Vector( const size_t size = 0 );
    Vector( const size_t size, const T* elements );
    Vector( const std::vector<T>& std_vector );
    ~Vector();

    Vector( const Vector& other );
    Vector& operator =( const Vector& rhs );

    void setSize( const size_t size );

    size_t size() const;

    void zero();
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
};

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
    this->zero();
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
 *  @param  std_vector [in] std::vector.
 */
/*==========================================================================*/
template <typename T>
inline Vector<T>::Vector( const std::vector<T>& std_vector ):
    m_size( 0 ),
    m_elements( 0 )
{
    this->setSize( std_vector.size() );
    memcpy( m_elements, &std_vector[0], sizeof( T ) * this->size() );
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

    this->zero();
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
 *  @brief  Sets the elements to zero.
 */
/*==========================================================================*/
template <typename T>
inline void Vector<T>::zero()
{
    const size_t size = this->size();
    T* const     v    = m_elements;
    for ( size_t i = 0; i < size; ++i )
    {
        v[i] = T( 0 );
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
    const size_t   size = this->size();
    const T* const v    = m_elements;

    double result = 0.0;
    for ( size_t i = 0; i < size; ++i )
    {
        result += (double)v[i] * (double)v[i];
    }
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

    const size_t   size = this->size();
    const T* const v    = m_elements;

    T result( 0 );
    for ( size_t i = 0; i < size; ++i )
    {
        result += v[i] * other[i];
    }
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
    T* const     v    = m_elements;
    for ( size_t i = 0; i < size; ++i )
    {
        v[i] += rhs[i];
    }
    return *this;
}

template <typename T>
inline Vector<T>& Vector<T>::operator -=( const Vector& rhs )
{
    KVS_ASSERT( this->size() == rhs.size() );

    const size_t size = this->size();
    T* const     v    = m_elements;
    for ( size_t i = 0; i < size; ++i )
    {
        v[i] -= rhs[i];
    }
    return *this;
}

template <typename T>
inline Vector<T>& Vector<T>::operator *=( const Vector& rhs )
{
    KVS_ASSERT( this->size() == rhs.size() );

    const size_t size = this->size();
    T* const     v    = m_elements;
    for ( size_t i = 0; i < size; ++i )
    {
        v[i] *= rhs[i];
    }
    return *this;
}

template <typename T>
inline Vector<T>& Vector<T>::operator *=( const T rhs )
{
    const size_t size = this->size();
    T* const     v    = m_elements;
    for ( size_t i = 0; i < size; ++i )
    {
        v[i] *= rhs;
    }
    return *this;
}

template <typename T>
inline Vector<T>& Vector<T>::operator /=( const T rhs )
{
    const size_t size = this->size();
    T* const     v    = m_elements;
    for ( size_t i = 0; i < size; ++i )
    {
        v[i] /= rhs;
    }
    return *this;
}

template<typename T>
inline const Vector<T> Vector<T>::operator -() const
{
    return Vector( *this ) *= T( -1 );
}

} // end of namespace kvs

#endif // KVS__VECTOR_H_INCLUDE
