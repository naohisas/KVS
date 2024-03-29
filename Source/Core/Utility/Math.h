/****************************************************************************/
/**
 *  @file   Math.h
 *  @author Naohisa Sakamoto
 */
/****************************************************************************/
#pragma once
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
    n |= n >> 1;
    n |= n >> 2;
    n |= n >> 4;
    n |= n >> 8;
    n |= n >> 16;
    return n ^ ( n >> 1 );
}

inline unsigned int Log2Smallest( unsigned int n )
{
    n--;
    n |= n >> 1;
    n |= n >> 2;
    n |= n >> 4;
    n |= n >> 8;
    n |= n >> 16;
    n++;
    return n;
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

inline size_t BitToByte( size_t value )
{
    return value >> 3;
}

inline size_t ByteToBit( size_t value )
{
    return value << 3;
}

} // end of namespace Math

} // end of namespace kvs
