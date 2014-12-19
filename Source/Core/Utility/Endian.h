/****************************************************************************/
/**
 *  @file Endian.h
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: Endian.h 1146 2012-05-30 11:24:35Z s.yamada0808@gmail.com $
 */
/****************************************************************************/
#ifndef KVS__ENDIAN_H_INCLUDE
#define KVS__ENDIAN_H_INCLUDE

#include <kvs/Type>
#include <string>
#include <utility>


namespace kvs
{

class Endian
{
public:
    static bool IsBig();
    static bool IsLittle();

    static void Swap( kvs::Int8* value );
    static void Swap( kvs::Int16* value );
    static void Swap( kvs::Int32* value );
    static void Swap( kvs::Int64* value );
    static void Swap( kvs::UInt8* value );
    static void Swap( kvs::UInt16* value );
    static void Swap( kvs::UInt32* value );
    static void Swap( kvs::UInt64* value );
    static void Swap( kvs::Real32* value );
    static void Swap( kvs::Real64* value );

    static void Swap( kvs::Int8* values, size_t n );
    static void Swap( kvs::Int16* values, size_t n );
    static void Swap( kvs::Int32* values, size_t n );
    static void Swap( kvs::Int64* values, size_t n );
    static void Swap( kvs::UInt8* values, size_t n );
    static void Swap( kvs::UInt16* values, size_t n );
    static void Swap( kvs::UInt32* values, size_t n );
    static void Swap( kvs::UInt64* values, size_t n );
    static void Swap( kvs::Real32* values, size_t n );
    static void Swap( kvs::Real64* values, size_t n );

#if KVS_ENABLE_DEPRECATED

    enum ByteOrder
    {
        Unknown = -1,
        Little  =  0,
        Big     =  1
    };

    ByteOrder Check();

    static void Swap( kvs::Int8& value );

    static void Swap( kvs::Int16& value );

    static void Swap( kvs::Int32& value );

    static void Swap( kvs::Int64& value );

    static void Swap( kvs::UInt8& value );

    static void Swap( kvs::UInt16& value );

    static void Swap( kvs::UInt32& value );

    static void Swap( kvs::UInt64& value );

    static void Swap( kvs::Real32& value );

    static void Swap( kvs::Real64& value );

    static void Swap( std::string& value );

    static void Swap( std::string* values, size_t n );

#endif

private:
    static void Swap2Bytes( void* value );
    static void Swap4Bytes( void* value );
    static void Swap8Bytes( void* value );
    static void Swap2Bytes( void* values, size_t n );
    static void Swap4Bytes( void* values, size_t n );
    static void Swap8Bytes( void* values, size_t n );

private:
    Endian();
};

/*===========================================================================*/
/**
 *  @brief  Swaps the two bytes value.
 *  @param  value [in] value in two bytes
 */
/*===========================================================================*/
inline void Endian::Swap2Bytes( void* value )
{
    unsigned char* v = static_cast<unsigned char*>( value );
    std::swap( v[0], v[1] );
}

/*===========================================================================*/
/**
 *  @brief  Swaps the four bytes value.
 *  @param  value [in] value in four bytes
 */
/*===========================================================================*/
inline void Endian::Swap4Bytes( void* value )
{
    unsigned char* v = static_cast<unsigned char*>( value );
    std::swap( v[0], v[3] );
    std::swap( v[1], v[2] );
}

/*===========================================================================*/
/**
 *  @brief  Swaps the eight bytes value.
 *  @param  value [in] value in eight bytes
 */
/*===========================================================================*/
inline void Endian::Swap8Bytes( void* value )
{
    unsigned char* v = static_cast<unsigned char*>( value );
    std::swap( v[0], v[7] );
    std::swap( v[1], v[6] );
    std::swap( v[2], v[5] );
    std::swap( v[3], v[4] );
}

/*===========================================================================*/
/**
 *  @brief  Swaps the array of value in increments of two bytes.
 *  @param  value [in] pointer to the values
 *  @param  n [in] number of elements
 */
/*===========================================================================*/
inline void Endian::Swap2Bytes( void* values, size_t n )
{
    unsigned char* v = static_cast<unsigned char*>( values );
    unsigned char* vend = v + n * 2;
    while ( v != vend )
    {
        Swap2Bytes( v );
        v += 2;
    }
}

/*===========================================================================*/
/**
 *  @brief  Swaps the array of value in increments of four bytes.
 *  @param  value [in] pointer to the values
 *  @param  n [in] number of elements
 */
/*===========================================================================*/
inline void Endian::Swap4Bytes( void* values, size_t n )
{
    unsigned char* v = static_cast<unsigned char*>( values );
    unsigned char* vend = v + n * 4;
    while ( v != vend )
    {
        Swap4Bytes( v );
        v += 4;
    }
}

/*===========================================================================*/
/**
 *  @brief  Swaps the array of value in increments of eight bytes.
 *  @param  value [in] pointer to the values
 *  @param  n [in] number of elements
 */
/*===========================================================================*/
inline void Endian::Swap8Bytes( void* values, size_t n )
{
    unsigned char* v = static_cast<unsigned char*>( values );
    unsigned char* vend = v + n * 8;
    while ( v != vend )
    {
        Swap8Bytes( v );
        v += 8;
    }
}

/*===========================================================================*/
/**
 *  @brief  Returns true if endian type is big endian.
 *  @return true, if big endian
 */
/*===========================================================================*/
inline bool Endian::IsBig()
{
    return !IsLittle();
}

/*===========================================================================*/
/**
 *  @brief  Returns true if endian type is little endian.
 *  @return true, if little endian
 */
/*===========================================================================*/
inline bool Endian::IsLittle()
{
    const int i = 1;
    return *( (char*)&i ) != 0;
}

/*===========================================================================*/
/**
 *  @brief  Swaps 8-bit integer value. (inaction)
 */
/*===========================================================================*/
inline void Endian::Swap( kvs::Int8* )
{
}

/*===========================================================================*/
/**
 *  @brief  Swaps 8-bit unsigned integer value. (inaction)
 */
/*===========================================================================*/
inline void Endian::Swap( kvs::UInt8* )
{
}

/*===========================================================================*/
/**
 *  @brief  Swaps 16-bit integer value.
 *  @param  value [in] 16-bit value
 */
/*===========================================================================*/
inline void Endian::Swap( kvs::Int16* value )
{
    Swap2Bytes( value );
}

/*===========================================================================*/
/**
 *  @brief  Swaps 16-bit integer value.
 *  @param  value [in] 16-bit value
 */
/*===========================================================================*/
inline void Endian::Swap( kvs::UInt16* value )
{
    Swap2Bytes( value );
}

/*===========================================================================*/
/**
 *  @brief  Swaps 32-bit integer value.
 *  @param  value [in] 32-bit value
 */
/*===========================================================================*/
inline void Endian::Swap( kvs::Int32* value )
{
    Swap4Bytes( value );
}

/*===========================================================================*/
/**
 *  @brief  Swaps 32-bit integer value.
 *  @param  value [in] 32-bit value
 */
/*===========================================================================*/
inline void Endian::Swap( kvs::UInt32* value )
{
    Swap4Bytes( value );
}

/*===========================================================================*/
/**
 *  @brief  Swaps 64-bit integer value.
 *  @param  value [in] 64-bit value
 */
/*===========================================================================*/
inline void Endian::Swap( kvs::Int64* value )
{
    Swap8Bytes( value );
}

/*===========================================================================*/
/**
 *  @brief  Swaps 64-bit integer value.
 *  @param  value [in] 64-bit value
 */
/*===========================================================================*/
inline void Endian::Swap( kvs::UInt64* value )
{
    Swap8Bytes( value );
}

/*===========================================================================*/
/**
 *  @brief  Swaps 32-bit floating-point value.
 *  @param  value [in] 32-bit value
 */
/*===========================================================================*/
inline void Endian::Swap( kvs::Real32* value )
{
    Swap4Bytes( value );
}

/*===========================================================================*/
/**
 *  @brief  Swaps 64-bit floating-point value.
 *  @param  value [in] 64-bit value
 */
/*===========================================================================*/
inline void Endian::Swap( kvs::Real64* value )
{
    Swap8Bytes( value );
}

/*===========================================================================*/
/**
 *  @brief  Swaps the array of 8-bit integer value. (inaction)
 */
/*===========================================================================*/
inline void Endian::Swap( kvs::Int8*, size_t )
{
}

/*===========================================================================*/
/**
 *  @brief  Swaps the array of 8-bit integer value. (inaction)
 */
/*===========================================================================*/
inline void Endian::Swap( kvs::UInt8*, size_t )
{
}

/*===========================================================================*/
/**
 *  @brief  Swaps the array of 16-bit integer value.
 *  @param  value [in] 16-bit value
 *  @param  n [in] number of elements
 */
/*===========================================================================*/
inline void Endian::Swap( kvs::Int16* values, size_t n )
{
    Swap2Bytes( values, n );
}

/*===========================================================================*/
/**
 *  @brief  Swaps the array of 16-bit integer value.
 *  @param  value [in] 16-bit value
 *  @param  n [in] number of elements
 */
/*===========================================================================*/
inline void Endian::Swap( kvs::UInt16* values, size_t n )
{
    Swap2Bytes( values, n );
}

/*===========================================================================*/
/**
 *  @brief  Swaps the array of 32-bit integer value.
 *  @param  value [in] 32-bit value
 *  @param  n [in] number of elements
 */
/*===========================================================================*/
inline void Endian::Swap( kvs::Int32* values, size_t n )
{
    Swap4Bytes( values, n );
}

/*===========================================================================*/
/**
 *  @brief  Swaps the array of 32-bit integer value.
 *  @param  value [in] 32-bit value
 *  @param  n [in] number of elements
 */
/*===========================================================================*/
inline void Endian::Swap( kvs::UInt32* values, size_t n )
{
    Swap4Bytes( values, n );
}

/*===========================================================================*/
/**
 *  @brief  Swaps the array of 64-bit integer value.
 *  @param  value [in] 64-bit value
 *  @param  n [in] number of elements
 */
/*===========================================================================*/
inline void Endian::Swap( kvs::Int64* values, size_t n )
{
    Swap8Bytes( values, n );
}

/*===========================================================================*/
/**
 *  @brief  Swaps the array of 64-bit integer value.
 *  @param  value [in] 64-bit value
 *  @param  n [in] number of elements
 */
/*===========================================================================*/
inline void Endian::Swap( kvs::UInt64* values, size_t n )
{
    Swap8Bytes( values, n );
}

/*===========================================================================*/
/**
 *  @brief  Swaps the array of 32-bit floating-point value.
 *  @param  value [in] 32-bit value
 *  @param  n [in] number of elements
 */
/*===========================================================================*/
inline void Endian::Swap( kvs::Real32* values, size_t n )
{
    Swap4Bytes( values, n );
}

/*===========================================================================*/
/**
 *  @brief  Swaps the array of 64-bit floating-point value.
 *  @param  value [in] 64-bit value
 *  @param  n [in] number of elements
 */
/*===========================================================================*/
inline void Endian::Swap( kvs::Real64* values, size_t n )
{
    Swap8Bytes( values, n );
}

#if KVS_ENABLE_DEPRECATED

/*===========================================================================*/
/**
 *  @brief  Checks endianness.
 *  @return endian type (Unknown, Little, Big)
 */
/*===========================================================================*/
inline Endian::ByteOrder Endian::Check()
{
    ByteOrder ret = Unknown;

    int i = 1;
    if ( *( (char*)&i ) ) { ret = Little; }
    else if ( *( (char*)&i + ( sizeof( int ) - 1 ) ) ) { ret = Big; }

    return ret;
}

/*===========================================================================*/
/**
 *  @brief  Swaps 8-bit integer value. (inaction)
 */
/*===========================================================================*/
inline void Endian::Swap( kvs::Int8& )
{
}

/*===========================================================================*/
/**
 *  @brief  Swaps 8-bit unsigned integer value. (inaction)
 */
/*===========================================================================*/
inline void Endian::Swap( kvs::UInt8& )
{
}

/*===========================================================================*/
/**
 *  @brief  Swaps 16-bit integer value.
 *  @param  value [in] 16-bit value
 */
/*===========================================================================*/
inline void Endian::Swap( kvs::Int16& value )
{
    Swap2Bytes( &value );
}

/*===========================================================================*/
/**
 *  @brief  Swaps 16-bit integer value.
 *  @param  value [in] 16-bit value
 */
/*===========================================================================*/
inline void Endian::Swap( kvs::UInt16& value )
{
    Swap2Bytes( &value );
}

/*===========================================================================*/
/**
 *  @brief  Swaps 32-bit integer value.
 *  @param  value [in] 32-bit value
 */
/*===========================================================================*/
inline void Endian::Swap( kvs::Int32& value )
{
    Swap4Bytes( &value );
}

/*===========================================================================*/
/**
 *  @brief  Swaps 32-bit integer value.
 *  @param  value [in] 32-bit value
 */
/*===========================================================================*/
inline void Endian::Swap( kvs::UInt32& value )
{
    Swap4Bytes( &value );
}

/*===========================================================================*/
/**
 *  @brief  Swaps 64-bit integer value.
 *  @param  value [in] 64-bit value
 */
/*===========================================================================*/
inline void Endian::Swap( kvs::Int64& value )
{
    Swap8Bytes( &value );
}

/*===========================================================================*/
/**
 *  @brief  Swaps 64-bit integer value.
 *  @param  value [in] 64-bit value
 */
/*===========================================================================*/
inline void Endian::Swap( kvs::UInt64& value )
{
    Swap8Bytes( &value );
}

/*===========================================================================*/
/**
 *  @brief  Swaps 32-bit floating-point value.
 *  @param  value [in] 32-bit value
 */
/*===========================================================================*/
inline void Endian::Swap( kvs::Real32& value )
{
    Swap4Bytes( &value );
}

/*===========================================================================*/
/**
 *  @brief  Swaps 64-bit floating-point value.
 *  @param  value [in] 64-bit value
 */
/*===========================================================================*/
inline void Endian::Swap( kvs::Real64& value )
{
    Swap8Bytes( &value );
}

/*===========================================================================*/
/**
 *  @brief  Swaps string value. (inaction)
 */
/*===========================================================================*/
inline void Endian::Swap( std::string& )
{
}

/*===========================================================================*/
/**
 *  @brief  Swaps the array of string value. (inaction)
 */
/*===========================================================================*/
inline void Endian::Swap( std::string*, size_t )
{
}

#endif

} // end of namespace kvs

#if 0

#define KVS_STATIC_ASSERT( expr, mes ) { char dummy[ ( expr ) ? 1 : -1 ]; (void)dummy; }
//#define KVS_STATIC_ASSERT( expr, mes ) static_assert( ( expr ), ( mes ) )


namespace kvs
{

namespace detail
{

template <typename T, T Value>
struct integral_constant
{
    static const T value = Value;
    typedef T value_type;
};

typedef integral_constant<bool, true> true_type;
typedef integral_constant<bool, false> false_type;

} // detail

} // kvs


namespace kvs
{

class Endian
{

public:

/*===========================================================================*/
/**
 *  @brief  Returns true if the endian type is little endian.
 *  @return true, if little endian
 */
/*===========================================================================*/
    static bool IsLittle()
    {
        const int i = 1;
        return *( (char*)&i ) != 0;
    }

/*===========================================================================*/
/**
 *  @brief  Returns true if the endian type is big endian.
 *  @return true, if big endian
 */
/*===========================================================================*/
    static bool IsBig()
    {
        return !IsLittle();
    }

/*===========================================================================*/
/**
 *  @brief  Swaps a value of type Type.
 *  @param  value [in] pointer to the value of type Type
 */
/*===========================================================================*/
    template <typename T>
    static void Swap( T* value )
    {
        KVS_STATIC_ASSERT( is_supported<T>::value, "not supported" );
        SwapBytes<sizeof( T )>( value );
    }

/*===========================================================================*/
/**
 *  @brief  Swaps an array of value of type Type.
 *  @param  values [in] pointer to the array
 *  @param  n [in] number of the elements
 */
/*===========================================================================*/
    template <typename T>
    static void Swap( T* values, size_t n )
    {
        KVS_STATIC_ASSERT( is_supported<T>::value, "not supported" );
        SwapBytes<sizeof( T )>( values, n );
    }

private:

/*===========================================================================*/
/**
 *  @brief  Swaps the endianess of the NBytes bytes value which \p value points.
 *  @param  value [in] pointer to the value
 */
/*===========================================================================*/
    template <size_t NBytes>
    static void SwapBytes( void* )
    {
        KVS_STATIC_ASSERT( false, "unexpected byte size" );
    }

    template <>
    static void SwapBytes<1>( void* )
    {
    }

    template <>
    static void SwapBytes<2>( void* value )
    {
        unsigned char* v = static_cast<unsigned char*>( value );
        std::swap( v[0], v[1] );
    }

    template <>
    static void SwapBytes<4>( void* value )
    {
        unsigned char* v = static_cast<unsigned char*>( value );
        std::swap( v[0], v[3] );
        std::swap( v[1], v[2] );
    }

    template <>
    static void SwapBytes<8>( void* value )
    {
        unsigned char* v = static_cast<unsigned char*>( value );
        std::swap( v[0], v[7] );
        std::swap( v[1], v[6] );
        std::swap( v[2], v[5] );
        std::swap( v[3], v[4] );
    }

/*===========================================================================*/
/**
 *  @brief  Swaps an array of value of which size is NByte bytes.
 *  @param  values [in] pointer to the array head
 *  @param  n [in] number of the elements
 */
/*===========================================================================*/
    template <size_t NBytes>
    static void SwapBytes( void* values, size_t n )
    {
        unsigned char* v = static_cast<unsigned char*>( values );
        unsigned char* vend = v + n * NBytes;
        while ( v != vend )
        {
            SwapBytes<NBytes>( v );
            v += NBytes;
        }
    }

    template <typename T>
    struct is_supported : detail::false_type {};
    template <> struct is_supported<char> :                 detail::true_type {};
    template <> struct is_supported<signed char> :          detail::true_type {};
    template <> struct is_supported<short> :                detail::true_type {};
    template <> struct is_supported<int> :                  detail::true_type {};
    template <> struct is_supported<long> :                 detail::true_type {};
    template <> struct is_supported<long long> :            detail::true_type {};
    template <> struct is_supported<unsigned char> :        detail::true_type {}; 
    template <> struct is_supported<unsigned short> :       detail::true_type {};
    template <> struct is_supported<unsigned int> :         detail::true_type {};
    template <> struct is_supported<unsigned long> :        detail::true_type {};
    template <> struct is_supported<unsigned long long> :   detail::true_type {};
    template <> struct is_supported<float> :                detail::true_type {};
    template <> struct is_supported<double> :               detail::true_type {};

private:

    Endian();

};

} // end of namespace kvs

#endif

#endif // KVS__ENDIAN_H_INCLUDE
