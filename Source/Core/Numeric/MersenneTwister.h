/****************************************************************************/
/**
 *  @file MersenneTwister.h
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: MersenneTwister.h 1365 2012-11-29 08:45:27Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#pragma once
#include <ctime>
#include <climits>
#ifdef _MSC_VER
#pragma warning (disable : 4146)
#endif


namespace kvs
{

/*==========================================================================*/
/**
 *  MersenneTwister class.
 */
/*==========================================================================*/
class MersenneTwister
{
private:
    enum { N = 624 };
    enum { M = 397 }; // period parameter
    unsigned long m_state[N]; ///< internal state
    unsigned long* m_next; ///< next value to get from state
    int m_left; ///< number of values left before reload needed

public:
    MersenneTwister();
    explicit MersenneTwister( const unsigned long seed );
    explicit MersenneTwister( const unsigned long* const seeds, const unsigned long length = N );

    void setSeed();
    void setSeed( const unsigned long seed );
    void setSeed( const unsigned long* const seeds, const unsigned long length = N );

    double rand();
    double rand( const double n );
    double rand53();
    unsigned long randInteger();
    unsigned long randInteger( const unsigned long n );
    double operator ()();

private:
    void initialize( const unsigned long seed );
    void reload( void );
    unsigned long high_bit( const unsigned long u ) const;
    unsigned long low_bit( const unsigned long u ) const;
    unsigned long low_bits( const unsigned long u ) const;
    unsigned long mix_bits( const unsigned long u, const unsigned long v ) const;
    unsigned long twist( const unsigned long m, const unsigned long s0, const unsigned long s1 ) const;
    static unsigned long hash( std::time_t t, std::clock_t c );
};

/*==========================================================================*/
/**
 *  @brief  Returns uniform random number.
 *  @return uniform random number in [0,1]
 */
/*==========================================================================*/
inline double MersenneTwister::rand()
{
    // 0xffffffff = 4294967295 = 2^32 - 1 (=UINT_MAX)
    // 0x7fffffff = 2147483647 = 2^31 - 1 (=INT_MAX)
    // [0,0xffffffff]: randInteger()
    // [0,0x7fffffff]: long( randInteger() >> 1 )
    // [0,1]: randInteger() * ( 1.0 / 4294967296.0 )
    // [0,1): randInteger() * ( 1.0 / 4294967295.0 )
    // (0,1): double( randInteger() ) + 0.5 ) * ( 1.0 / 4294967296.0 )
    return double ( this->randInteger() ) * ( 1.0 / 4294967295.0 ); // in [0,1]
}

/*==========================================================================*/
/**
 *  @brief  Returns uniform random number.
 *  @param  n [in] maximum value
 *  @return uniform random number in [0,n]
 */
/*==========================================================================*/
inline double MersenneTwister::rand( const double n )
{
    return this->rand() * n;
}

/*==========================================================================*/
/**
 *  @brief  Returns uniform random number (53-bit precision).
 *  @return uniform random number in [0,1)
 */
/*==========================================================================*/
inline double MersenneTwister::rand53()
{
    // Generate a uniform random number (53 bits precision)
    // Based on code by Isaku Wasa.
    // http://www001.upp.so-net.ne.jp/isaku/zmtrand.c.html
    const unsigned long a = this->randInteger() >> 5;
    const unsigned long b = this->randInteger() >> 6;
    return ( a * 67108864.0 + b ) * ( 1.0 / 9007199254740992.0 );
}

/*==========================================================================*/
/**
 *  @brief  Returns uniform random number (32-bit integer).
 *  @return uniform random number in [0,0xffffffff] = [0,UINT_MAX]
 */
/*==========================================================================*/
inline unsigned long MersenneTwister::randInteger()
{
    // Pull a 32-bit integer from the generator state
    // Every other access function simply transforms
    // the numbers extracted here

    if ( m_left == 0 ) { this->reload(); }
    --m_left;

    unsigned long s1;
    s1  = *m_next++;
    s1 ^= ( s1 >> 11 );
    s1 ^= ( s1 <<  7 ) & 0x9d2c5680UL;
    s1 ^= ( s1 << 15 ) & 0xefc60000UL;
    return s1 ^ ( s1 >> 18 );
}

/*==========================================================================*/
/**
 *  @brief  Returns uniform random number.
 *  @param  n [in] maximum number
 *  @return uniform random number in [0,n]
 */
/*==========================================================================*/
inline unsigned long MersenneTwister::randInteger( const unsigned long n )
{
    // Find which bits are used in n
    // Optimized by Magnus Jonsson (magnus@smartelectronix.com)
    unsigned long used = n;
    used |= used >> 1;
    used |= used >> 2;
    used |= used >> 4;
    used |= used >> 8;
    used |= used >> 16;

    // Draw numbers until one is found in [0,n]
    unsigned long i;
    do
    {
        i = this->randInteger() & used;  // toss unused bits to shorten search
    }
    while ( i > n );

    return i;
}

/*==========================================================================*/
/**
 *  @brief  Returns uniform random number.
 *  @return uniform random number
 */
/*==========================================================================*/
inline double MersenneTwister::operator ()()
{
    return this->rand();
}

/*==========================================================================*/
/**
 *  @brief  Generates N new values.
 */
/*==========================================================================*/
inline void MersenneTwister::reload()
{
    // Generate N new values in state
    // Made clearer and faster by Matthew Bellew (matthew.bellew@home.com)
    unsigned long* p = m_state;
    int i;
    for ( i = N - M; i--; ++p ) { *p = twist( p[M], p[0], p[1] ); }
    for ( i = M; --i; ++p ) { *p = twist( p[M - N], p[0], p[1] ); }
    *p = twist( p[M - N], p[0], m_state[0] );
    m_left = N, m_next = m_state;
}

/*==========================================================================*/
/**
 *  @brief  Returns high bit value.
 *  @param  u [in] input value
 *  @return high bit value
 */
/*==========================================================================*/
inline unsigned long MersenneTwister::high_bit( const unsigned long u ) const
{
    return u & 0x80000000UL;
}

/*==========================================================================*/
/**
 *  @brief  Returns low bit value.
 *  @param  u [in] input value
 *  @return low bit value
 */
/*==========================================================================*/
inline unsigned long MersenneTwister::low_bit( const unsigned long u ) const
{
    return u & 0x00000001UL;
}

/*==========================================================================*/
/**
 *  @brief  Returns low bit value.
 *  @param  u [in] input value
 *  @return low bit value
 */
/*==========================================================================*/
inline unsigned long MersenneTwister::low_bits( const unsigned long u ) const
{
    return u & 0x7fffffffUL;
}

/*==========================================================================*/
/**
 *  @brief  Returns mixed bit value of input values.
 *  @param  u [in] input value 1
 *  @param  v [in] input value 2
 *  @return mixed bit value
 */
/*==========================================================================*/
inline unsigned long MersenneTwister::mix_bits( const unsigned long u, const unsigned long v ) const
{
    return high_bit( u ) | low_bits( v );
}

/*==========================================================================*/
/**
 *  @brief  Returns twisted bit value of input values.
 *  @param  m [in] input value
 *  @param  s0 [in] input sub-value 0
 *  @param  s1 [in] input sub-value 1
 *  @return twisted bit value
 */
/*==========================================================================*/
inline unsigned long MersenneTwister::twist(
    const unsigned long m,
    const unsigned long s0,
    const unsigned long s1 ) const
{
    return m ^ ( mix_bits( s0, s1 ) >> 1 ) ^ ( -low_bit( s1 ) & 0x9908b0dfUL );
}

} // end of namespace kvs
