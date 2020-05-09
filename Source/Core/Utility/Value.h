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
#pragma once
#include <limits>
#include <cstdlib>
#include <ctime>
#include <kvs/Type>


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
private:
    // Simple random number generator based on Xorshift128
    static kvs::UInt32 m_seeds[4];
    static void set_seed( kvs::UInt32 seed );
    static kvs::UInt32 get_random_seed();
    static kvs::UInt8 get_rand_uint8();
    static kvs::UInt16 get_rand_uint16();
    static kvs::UInt32 get_rand_uint32();
    static kvs::UInt64 get_rand_uint64();
    static kvs::Real32 get_rand_real32();
    static kvs::Real64 get_rand_real64();

private:
    Value();

public:
    static T Zero() { return T(0); }
    static T Min() { return std::numeric_limits<T>::min(); }
    static T Max() { return std::numeric_limits<T>::max(); }
    static T Epsilon() { return std::numeric_limits<T>::epsilon(); }
    static void SetSeed( const kvs::UInt32 seed ) { set_seed( seed ); }
    static void SetRandomSeed() { set_seed( get_random_seed() ); }
    static kvs::UInt32 RandomSeed() { return get_random_seed(); }
    static T Random() { return T(0); }
    static T Random( const T /* min */, const T /* max */ ) { return T(0); }
};

template <typename T>
kvs::UInt32 Value<T>::m_seeds[4] =
{
    123456789,
    362436069,
    521288629,
    88675123
};

template <typename T>
inline void Value<T>::set_seed( kvs::UInt32 seed )
{
    m_seeds[0] = seed = 1812433253U * ( seed ^ ( seed >> 30 ) ) + 1;
    m_seeds[1] = seed = 1812433253U * ( seed ^ ( seed >> 30 ) ) + 2;
    m_seeds[2] = seed = 1812433253U * ( seed ^ ( seed >> 30 ) ) + 3;
    m_seeds[3] = seed = 1812433253U * ( seed ^ ( seed >> 30 ) ) + 4;
}

template <typename T>
inline kvs::UInt32 Value<T>::get_random_seed()
{
    kvs::UInt32 a = static_cast<kvs::UInt32>( std::time(0) );
    kvs::UInt32 b = static_cast<kvs::UInt32>( std::clock() );
    kvs::UInt32 c = kvs::Type::GetID<T>();
    a = a - b; a = a - c; a = a ^ ( c >> 13 );
    b = b - c; b = b - a; b = b ^ ( a << 8  );
    c = c - a; c = c - b; c = c ^ ( b >> 13 );
    a = a - b; a = a - c; a = a ^ ( c >> 12 );
    b = b - c; b = b - a; b = b ^ ( a << 16 );
    c = c - a; c = c - b; c = c ^ ( b >> 5  );
    a = a - b; a = a - c; a = a ^ ( c >> 3  );
    b = b - c; b = b - a; b = b ^ ( a << 10 );
    c = c - a; c = c - b; c = c ^ ( b >> 15 );
    return c;
}

template <typename T>
inline kvs::UInt8 Value<T>::get_rand_uint8()
{
    return kvs::UInt8( get_rand_uint16() >> 8 );
}

template <typename T>
inline kvs::UInt16 Value<T>::get_rand_uint16()
{
    return kvs::UInt16( get_rand_uint32() >> 16 );
}

template <typename T>
inline kvs::UInt32 Value<T>::get_rand_uint32()
{
    kvs::UInt32* s = m_seeds;
    kvs::UInt32 t = ( s[0] ^ ( s[0] << 11 ) );
    s[0] = s[1]; s[1] = s[2]; s[2] = s[3];
    s[3] = ( s[3] ^ ( s[3] >> 19 ) ) ^ ( t ^ ( t >> 8 ) );
    return s[3];
}

template <typename T>
inline kvs::UInt64 Value<T>::get_rand_uint64()
{
    return kvs::UInt64( get_rand_uint32() ) << 32 | get_rand_uint32();
}

template <typename T>
inline kvs::Real32 Value<T>::get_rand_real32()
{
    return get_rand_uint32() * ( 1.0f / kvs::Real32( 4294967296.0 ) );
}

template <typename T>
inline kvs::Real64 Value<T>::get_rand_real64()
{
    return get_rand_uint64() * ( 1.0 / kvs::Real64( 18446744073709551616.0 ) );
}

template <>
inline kvs::Real32 Value<kvs::Real32>::Min()
{
    return -std::numeric_limits<kvs::Real32>::max();
}

template <>
inline kvs::Real64 Value<kvs::Real64>::Min()
{
    return -std::numeric_limits<kvs::Real64>::max();
}

template<>
inline kvs::UInt8 Value<kvs::UInt8>::Random()
{
    // Returns a random number in [0,2^8-1] = [0,255]
    return get_rand_uint8();
}

template<>
inline kvs::UInt16 Value<kvs::UInt16>::Random()
{
    // Returns a random number in [0,2^16-1] = [0,65535]
    return get_rand_uint16();
}

template<>
inline kvs::UInt32 Value<kvs::UInt32>::Random()
{
    // Returns a random number in [0,2^32-1] = [0,4294967295]
    return get_rand_uint32();
}

template<>
inline kvs::UInt64 Value<kvs::UInt64>::Random()
{
    // Returns a random number in [0,2^64-1] = [0,18446744073709551615]
    return get_rand_uint64();
}

template<>
inline kvs::Int8 Value<kvs::Int8>::Random()
{
    // Returns a random number in [-2^7,2^7-1] = [-128,127]
    return kvs::Int8( get_rand_uint8() );
}

template<>
inline kvs::Int16 Value<kvs::Int16>::Random()
{
    // Returns a random number in [-2^15,2^15-1] = [-32768,32767]
    return kvs::Int16( get_rand_uint16() );
}

template<>
inline kvs::Int32 Value<kvs::Int32>::Random()
{
    // Returns a random number in [-2^31,2^31-1] = [-2147483648,2147483647]
    return kvs::Int32( get_rand_uint32() );
}

template<>
inline kvs::Int64 Value<kvs::Int64>::Random()
{
    // Returns a random number in [-2^63,2^63-1] = [-9223372036854775808,9223372036854775807]
    return kvs::Int64( get_rand_uint64() );
}

template<>
inline kvs::Real32 Value<kvs::Real32>::Random()
{
    // Returns a random number in [0,1]
    return get_rand_real32();
}

template<>
inline kvs::Real64 Value<kvs::Real64>::Random()
{
    // Returns a random number in [0,1]
    return get_rand_real64();
}

template<>
inline kvs::UInt8 Value<kvs::UInt8>::Random( const kvs::UInt8 min, const kvs::UInt8 max )
{
    // Returns a random number in [min,max]
    return min + kvs::UInt8( get_rand_uint8() * ( max - min + 1.0 ) / ( 1.0 + Value<kvs::UInt8>::Max() ) );
}

template<>
inline kvs::UInt16 Value<kvs::UInt16>::Random( const kvs::UInt16 min, const kvs::UInt16 max )
{
    // Returns a random number in [min,max]
    return min + kvs::UInt16( get_rand_uint16() * ( max - min + 1.0 ) / ( 1.0 + Value<kvs::UInt16>::Max() ) );
}

template<>
inline kvs::UInt32 Value<kvs::UInt32>::Random( const kvs::UInt32 min, const kvs::UInt32 max )
{
    // Returns a random number in [min,max]
    return min + kvs::UInt32( get_rand_uint32() * ( max - min + 1.0 ) / ( 1.0 + Value<kvs::UInt32>::Max() ) );
}

template<>
inline kvs::UInt64 Value<kvs::UInt64>::Random( const kvs::UInt64 min, const kvs::UInt64 max )
{
    // Returns a random number in [min,max]
    return min + kvs::UInt64( get_rand_uint64() * ( max - min + 1.0 ) / ( 1.0 + Value<kvs::UInt64>::Max() ) );
}

template<>
inline kvs::Int8 Value<kvs::Int8>::Random( const kvs::Int8 min, const kvs::Int8 max )
{
    // Returns a random number in [min,max]
    return min + kvs::Int8( get_rand_uint8() * ( max - min + 1.0 ) / ( 1.0 + Value<kvs::UInt8>::Max() ) );
}

template<>
inline kvs::Int16 Value<kvs::Int16>::Random( const kvs::Int16 min, const kvs::Int16 max )
{
    // Returns a random number in [min,max]
    return min + kvs::Int16( get_rand_uint16() * ( max - min + 1.0 ) / ( 1.0 + Value<kvs::UInt16>::Max() ) );
}

template<>
inline kvs::Int32 Value<kvs::Int32>::Random( const kvs::Int32 min, const kvs::Int32 max )
{
    // Returns a random number in [min,max]
    return min + kvs::UInt32( get_rand_uint32() * ( max - min + 1.0 ) / ( 1.0 + Value<kvs::UInt32>::Max() ) );
}

template<>
inline kvs::Int64 Value<kvs::Int64>::Random( const kvs::Int64 min, const kvs::Int64 max )
{
    // Returns a random number in [min,max]
    return min + kvs::Int64( get_rand_uint64() * ( max - min + 1.0 ) / ( 1.0 + Value<kvs::UInt64>::Max() ) );
}

template<>
inline kvs::Real32 Value<kvs::Real32>::Random( const kvs::Real32 min, const kvs::Real32 max )
{
    // Returns a random number in [min,max]
    return min + get_rand_real32() * ( max - min );
}

template<>
inline kvs::Real64 Value<kvs::Real64>::Random( const kvs::Real64 min, const kvs::Real64 max )
{
    // Returns a random number in [min,max]
    return min + get_rand_real64() * ( max - min );
}

} // end of namespace kvs
