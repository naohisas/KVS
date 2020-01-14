/****************************************************************************/
/**
 *  @file Math.h
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: Math.h 1618 2013-07-12 05:39:03Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#ifndef KVS__MATH_H_INCLUDE
#define KVS__MATH_H_INCLUDE

#include <cmath>


namespace kvs
{

namespace Math
{

#define KVS__MATH_TINY_VALUE 1.e-6

const double sqrt2 = 1.4142135623730950488;

const double sqrt2_div_2 = 0.7071067811865475244;

const double pi = 3.1415926535897932384;

const double pi_mul_2 = 6.2831853071795864769;

const double pi_div_2 = 1.5707963267948966192;

const double pi_div_16 = 0.1963495408493620774;

const double golden_ratio = 1.6180339887498948482;

template <typename T>
inline T Max( T a, T b )
{
    return ( a > b ) ? a : b;
}

template <typename T>
inline T Max( T a, T b, T c )
{
    return Max( Max( a, b ), c );
}

template <typename T>
inline T Max( T a, T b, T c, T d )
{
    return Max( Max( Max( a, b ), c ), d );
}

template <typename T>
inline T Min( T a, T b )
{
    return ( a < b ) ? a : b;
}

template <typename T>
inline T Min( T a, T b, T c )
{
    return Min( Min( a, b ), c );
}

template <typename T>
inline T Min( T a, T b, T c, T d )
{
    return Min( Min( Min( a, b ), c ), d );
}

template <typename T>
inline T Abs( T a )
{
    return ( a < 0 ) ? -a : a;
}

template <>
inline unsigned char Abs( unsigned char a )
{
    return a;
}

template <>
inline unsigned int Abs( unsigned int a )
{
    return a;
}

template <>
inline unsigned short Abs( unsigned short a )
{
    return a;
}

template <>
inline unsigned long Abs( unsigned long a )
{
    return a;
}

template <>
inline unsigned long long Abs( unsigned long long a )
{
    return a;
}

template <typename T>
inline int Sgn( T a )
{
    return ( a < 0 ) ? -1 : 1;
}

template <typename T>
inline T Sgn( T a, T b )
{
    return ( b < T( 0 ) ) ? -Abs( a ) : Abs( a );
}

template <typename T>
inline int Round( T a )
{
    return a >= 0 ? ( int )( a + 0.5 ) : -( int )( 0.5 - a );
}

template <typename T>
inline int Floor( T a )
{
    return a >= 0 ? ( int )( a ) : ( int )( a ) - 1;
}

template <typename T>
inline int Ceil( T a )
{
    return a == Floor( a ) ? Floor( a ) : Sgn( a ) + Floor( a );
}

template <typename T>
inline T Clamp( T a, T low, T high )
{
    return a <= low ? low : a <= high ? a : high;
}

template <typename T>
inline bool Equal( T a, T b )
{
    return Abs( a - b ) < KVS__MATH_TINY_VALUE;
}

template <typename T>
inline bool IsZero( T a )
{
    return Equal( a, T( 0 ) );
}

template <typename T>
inline T Square( T a )
{
    return a * a;
}

template <typename T>
inline T Fract( T x )
{
    // T must be a floating-point.
    return x - Floor( x );
}

template <typename T>
inline T Mod( T x, T y )
{
    // T must be a floating-point.
    return x - y * Floor( x / y );
}

template <typename T>
inline T Mix( T x, T y, T a )
{
    // T must be a floating-point.
    return x * ( 1 - a ) + y * a;
}

template <typename T>
inline T Step( T edge, T x )
{
    // T must be a floating-point.
    return x < edge ? T(0) : T(1);
}

template <typename T>
inline T Pythag( T a, T b )
{
    T abs_a = Abs( a );
    T abs_b = Abs( b );

    if ( abs_a > abs_b )
    {
        return abs_a * static_cast<T>( std::sqrt( 1.0 + Square( abs_b / abs_a ) ) );
    }
    else
    {
        return IsZero( abs_b ) ? T( 0 ) :
                abs_b * static_cast<T>( std::sqrt( 1.0 + Square( abs_a / abs_b ) ) );
    }
}

inline unsigned int Log2Largest( unsigned int n )
{
    /*
       MessageAssert( sizeof( unsigned int ) == 4,
       "UTIL_LOG2_LARGEST allows to use on 32 bit CPU only." );
     */
    unsigned int t;
    unsigned int log2;

    if ( n >= 0x10000 ) { log2 = 16; t = 0x1000000; }
    else                { log2 = 0;  t = 0x100;     }

    if ( n >= t ) { log2 += 8; t <<= 4; }
    else          {            t >>= 4; }

    if ( n >= t ) { log2 += 4; t <<= 2; }
    else          {            t >>= 2; }

    if ( n >= t ) { log2 += 2; t <<= 1; }
    else          {            t >>= 1; }

    if ( n >= t ) { log2 += 1; }

    return t;
}

inline unsigned int Log2Smallest( unsigned int n )
{
    /*
       MessageAssert( sizeof( unsigned int ) == 4,
       "UTIL_LOG2_SMALLEST allows to use on 32 bit CPU only." );
     */
    if ( n > 0x80000000 ) { return 32; }

    unsigned int t;
    unsigned int log2;

    if ( n > 0x8000 ) { log2 = 16; t = 0x800000; }
    else              { log2 = 0;  t = 0x80;     }

    if ( n > t ) { log2 += 8; t <<= 4; }
    else         {            t >>= 4; }

    if ( n > t ) { log2 += 4; t <<= 2; }
    else         {            t >>= 2; }

    if ( n > t ) { log2 += 2; t <<= 1; }
    else         {            t >>= 1; }

    if ( n > t ) { log2 += 1; }

    return log2;
}

inline unsigned int Power2Largest( unsigned int n )
{
    return 1 << Log2Largest( n );
}

inline unsigned int Power2Smallest( unsigned int n )
{
    return 1 << Log2Smallest( n );
}

inline bool IsPower2( unsigned int n )
{
    return !( n & ( n - 1 ) );
}

inline float  Deg2Rad( float a )
{
    return a * 0.01745329252f;
}

inline double Deg2Rad( double a )
{
    return a * 0.01745329252;
}

inline float Rad2Deg( float a )
{
    return a * 57.29577951f;
}

inline double Rad2Deg( double a )
{
    return a * 57.29577951;
}

inline bool IsNaN( float a )
{
    return !( a == a );
}

inline bool IsNaN( double a )
{
    return !( a == a );
}

inline float PI()
{
    return 6 * std::asin( 0.5f );
}






#if KVS_ENABLE_DEPRECATED
template <typename T>
inline T Power( T a, T b )
{
    return std::pow( a, b );
}


template <typename T>
inline void Swap( T& a, T& b )
{
    T tmp;
    tmp = a;
    a   = b;
    b   = tmp;
}

template <typename T>
inline void Shift( T& a, T& b, T c )
{
    a = b;
    b = c;
}

template <typename T>
inline void Shift( T& a, T& b, T& c, T d )
{
    a = b;
    b = c;
    c = d;
}

template <typename T>
inline T SquareRoot( T a )
{
    return static_cast<T>( std::sqrt( a ) );
}

#endif

} // end of namespace Math

} // end of namespace kvs

#endif // KVS__MATH_H_INCLUDE
