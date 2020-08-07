/****************************************************************************/
/**
 *  @file   Endian.h
 *  @author Naohisa Sakamoto
 */
/****************************************************************************/
#pragma once
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

} // end of namespace kvs
