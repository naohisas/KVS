/****************************************************************************/
/**
 *  @file MersenneTwister.cpp
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: MersenneTwister.cpp 1365 2012-11-29 08:45:27Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#include "MersenneTwister.h"
#include <cstdio>


namespace kvs
{

/*==========================================================================*/
/**
 *  @brief  Constructs a new MersenneTwister.
 */
/*==========================================================================*/
MersenneTwister::MersenneTwister()
{
    this->setSeed();
}

/*==========================================================================*/
/**
 *  @brief  Constructs a new MersenneTwister.
 *  @param  seed [in] seed value
 */
/*==========================================================================*/
MersenneTwister::MersenneTwister( const unsigned long seed )
{
    this->setSeed( seed );
}

/*==========================================================================*/
/**
 *  @brief  Constructs a new MersenneTwister.
 *  @param  seeds [in] seed values
 *  @param  length [in] number of seed values
 */
/*==========================================================================*/
MersenneTwister::MersenneTwister(
    const unsigned long* const seeds,
    const unsigned long length )
{
    this->setSeed( seeds, length );
}

/*==========================================================================*/
/**
 *  @brief  Sets seed value.
 */
/*==========================================================================*/
void MersenneTwister::setSeed()
{
    // Seed the generator with an array from /dev/urandom if available
    // Otherwise use a hash of time() and clock() values

    // First try getting an array from /dev/urandom
    FILE* urandom = fopen( "/dev/urandom", "rb" );
    if ( urandom )
    {
        unsigned long seeds[N];

        unsigned long* s       = seeds;
        int            i       = N;
        bool           success = true;

        while ( success && i-- )
        {
            success = fread( s++, sizeof( unsigned long ), 1, urandom ) != 0;
        }
        fclose( urandom );

        if ( success )
        {
            this->setSeed( seeds, N );
            return;
        }
    }

    // Was not successful, so use time() and clock() instead
    this->setSeed( this->hash( time( NULL ), clock() ) );
}

/*==========================================================================*/
/**
 *  @brief  Sets seed value.
 *  @param  seed [in] seed value
 */
/*==========================================================================*/
void MersenneTwister::setSeed( const unsigned long seed )
{
    // Seed the generator with a simple uint32
    this->initialize( seed );
    this->reload();
}

/*==========================================================================*/
/**
 *  @brief  Sets seed values.
 *  @param  seeds [in] seed values
 *  @param  length [in] number of seed values
 */
/*==========================================================================*/
void MersenneTwister::setSeed(
    const unsigned long* const seeds,
    const unsigned long        length )
{
    // Seed the generator with an array of uint32's
    // There are 2^19937-1 possible initial states.  This function allows
    // all of those to be accessed by providing at least 19937 bits (with a
    // default seed length of N = 624 uint32's).  Any bits above the lower 32
    // in each element are discarded.
    // Just call setSeed() if you want to get array from /dev/urandom
    this->initialize( 19650218UL );
    int           i = 1;
    unsigned long j = 0;
    int           k = ( N > length ? N : length );

    for ( ; k; --k )
    {
        m_state[i] =
            m_state[i] ^ ( ( m_state[i - 1] ^ ( m_state[i - 1] >> 30 ) ) * 1664525UL );
        m_state[i] += ( seeds[j] & 0xffffffffUL ) + j;
        m_state[i] &= 0xffffffffUL;
        ++i;  ++j;
        if ( i >= N ) { m_state[0] = m_state[N - 1];  i = 1; }
        if ( j >= length ) { j = 0; }
    }

    for ( k = N - 1; k; --k )
    {
        m_state[i] =
            m_state[i] ^ ( ( m_state[i - 1] ^ ( m_state[i - 1] >> 30 ) ) * 1566083941UL );
        m_state[i] -= i;
        m_state[i] &= 0xffffffffUL;
        ++i;
        if ( i >= N ) { m_state[0] = m_state[N - 1];  i = 1; }
    }

    m_state[0] = 0x80000000UL;  // MSB is 1, assuring non-zero initial array
    reload();
}

/*==========================================================================*/
/**
 *  @brief  Initializes state with seed value.
 *  @param  seed [in] seed value
 */
/*==========================================================================*/
void MersenneTwister::initialize( const unsigned long seed )
{
    // Initialize generator state with seed
    // See Knuth TAOCP Vol 2, 3rd Ed, p.106 for multiplier.
    // In previous versions, most significant bits (MSBs) of the seed affect
    // only MSBs of the state array.  Modified 9 Jan 2002 by Makoto Matsumoto.
    unsigned long* s = m_state;
    unsigned long* r = m_state;
    int            i = 1;
    *s++ = seed & 0xffffffffUL;
    for ( ; i < N; ++i )
    {
        *s++ = ( 1812433253UL * ( *r ^ ( *r >> 30 ) ) + i ) & 0xffffffffUL;
        r++;
    }
}

/*==========================================================================*/
/**
 *  @brief  Returns hash value.
 *  @param  t [in] time value
 *  @param  c [in] clock value
 *  @return hash value
 */
/*==========================================================================*/
unsigned long MersenneTwister::hash( std::time_t t, std::clock_t c )
{
    // Get a uint32 from t and c
    // Better than uint32(x) in case x is floating point in [0,1]
    // Based on code by Lawrence Kirby (fred@genesis.demon.co.uk)

    static unsigned long differ = 0; // guarantee time-based seeds will change

    unsigned long  h1 = 0;
    unsigned char* p  = reinterpret_cast<unsigned char*>( &t );
    for ( size_t i = 0; i < sizeof( t ); ++i )
    {
        h1 *= UCHAR_MAX + 2U;
        h1 += p[i];
    }

    unsigned long h2 = 0;
    p = reinterpret_cast<unsigned char*>( &c );
    for ( size_t j = 0; j < sizeof( c ); ++j )
    {
        h2 *= UCHAR_MAX + 2U;
        h2 += p[j];
    }
    return( ( h1 + differ++ ) ^ h2 );
}

} // end of namespace kvs.
