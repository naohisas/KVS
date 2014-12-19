/****************************************************************************/
/**
 *  @file Value.h
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: Value.h 1707 2014-01-27 07:37:04Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#ifndef KVS__VALUE_H_INCLUDE
#define KVS__VALUE_H_INCLUDE

#include <limits>
#if KVS_ENABLE_DEPRECATED
#include <kvs/Endian>
#endif

namespace kvs
{

/*==========================================================================*/
/**
 *  Value class.
 */
/*==========================================================================*/
template<typename T>
class Value
{
#if KVS_ENABLE_DEPRECATED

protected:

    T m_value; ///< value

public:

    Value();

    Value( T value );

    Value( const Value<T>& value );

    ~Value();

public:

    T operator () () const;

    Value<T>& operator = ( const Value<T>& other );

    Value<T>& operator += ( const Value<T>& other );

    Value<T>& operator -= ( const Value<T>& other );

    Value<T>& operator *= ( const Value<T>& other );

    Value<T>& operator /= ( const Value<T>& other );

public:

    void swapByte();
#else
private:
    Value();
#endif
public:

    static T Zero();

    static T Min();

    static T Max();

    static T Epsilon();
};
#if KVS_ENABLE_DEPRECATED
template<typename T>
inline Value<T>::Value()
{
    m_value = Value<T>::Zero();
}

template<typename T>
inline Value<T>::Value( T value ):
    m_value( value )
{
}

template<typename T>
inline Value<T>::Value( const Value<T>& value ):
    m_value( value.m_value )
{
}

template<typename T>
inline Value<T>::~Value()
{
}

template<typename T>
inline T Value<T>::operator () () const
{
    return m_value;
}

template<typename T>
inline Value<T>& Value<T>::operator = ( const Value<T>& other )
{
    m_value = other.m_value;
    return *this;
}

template<typename T>
inline Value<T>& Value<T>::operator += ( const Value<T>& other )
{
    m_value = static_cast<T>( m_value + other.m_value );
    return *this;
}

template<typename T>
inline Value<T>& Value<T>::operator -= ( const Value<T>& other )
{
    m_value = static_cast<T>( m_value - other.m_value );
    return *this;
}

template<typename T>
inline Value<T>& Value<T>::operator *= ( const Value<T>& other )
{
    m_value = static_cast<T>( m_value * other.m_value );
    return *this;
}

template<typename T>
inline Value<T>& Value<T>::operator /= ( const Value<T>& other )
{
    m_value = static_cast<T>( m_value / other.m_value );
    return *this;
}

template<typename T>
inline void Value<T>::swapByte()
{
    kvs::Endian::Swap( &m_value );
}
#endif
template<typename T>
inline T Value<T>::Zero()
{
    return T( 0 );
}

template<typename T>
inline T Value<T>::Min()
{
    return std::numeric_limits<T>::min();
}

template <>
inline float Value<float>::Min()
{
    return -std::numeric_limits<float>::max();
}

template <>
inline double Value<double>::Min()
{
    return -std::numeric_limits<double>::max();
}

template <>
inline long double Value<long double>::Min()
{
    return -std::numeric_limits<long double>::max();
}

template<typename T>
inline T Value<T>::Max()
{
    return std::numeric_limits<T>::max();
}

template<typename T>
inline T Value<T>::Epsilon()
{
    return std::numeric_limits<T>::epsilon();
}
#if KVS_ENABLE_DEPRECATED
template<typename T>
inline bool operator == ( const Value<T>& other1, const Value<T>& other2 )
{
    return other1.m_value == other1.m_value;
}

template<typename T>
inline bool operator < ( const Value<T>& other1, const Value<T>& other2 )
{
    return other1.m_value < other1.m_value;
}

template<typename T>
inline bool operator!= ( const Value<T>& other1, const Value<T>& other2 )
{
    return !( other1 == other2 );
}

template<typename T>
inline bool operator > ( const Value<T>& other1, const Value<T>& other2 )
{
    return other2 < other1;
}

template<typename T>
inline bool operator <= ( const Value<T>& other1, const Value<T>& other2 )
{
    return !( other2 < other1 );
}

template<typename T>
inline bool operator >= ( const Value<T>& other1, const Value<T>& other2 )
{
    return !( other1 < other2 );
}

template<typename T>
inline Value<T> operator + ( const Value<T>& other1, const Value<T>& other2 )
{
    Value<T> result( other1 ); result += other2;
    return result;
}

template<typename T>
inline Value<T> operator - ( const Value<T>& other1, const Value<T>& other2 )
{
    Value<T> result( other1 ); result -= other2;
    return result;
}

template<typename T>
inline Value<T> operator * ( const Value<T>& other1, const Value<T>& other2 )
{
    Value<T> result( other1 ); result *= other2;
    return result;
}

template<typename T>
inline Value<T> operator / ( const Value<T>& other1, const Value<T>& other2 )
{
    Value<T> result( other1 ); result /= other2;
    return result;
}
#endif
} // end of namespace kvs

#endif // KVS__VALUE_H_INCLUDE
