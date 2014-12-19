/*****************************************************************************/
/**
 *  @file   BoxMuller.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id$
 */
/*----------------------------------------------------------------------------
 *
 *  Acknowledgements.
 *
 *  This code was developed by Professor Satoshi Tanaka, Ritsumeikan University.
 *  http://www.cg.is.ritsumei.ac.jp/index.html
 */
/*****************************************************************************/
#ifndef KVS__BOX_MULLER_H_INCLUDE
#define KVS__BOX_MULLER_H_INCLUDE

#include <kvs/MersenneTwister>
#include <cmath>


namespace kvs
{

/*==========================================================================*/
/**
 *  BoxMuller class.
 */
/*==========================================================================*/
class BoxMuller
{
public:

    BoxMuller(): m_uniform_random() {}
    explicit BoxMuller( const unsigned long seed ): m_uniform_random( seed ) {}

    double rand();
    double rand( const double mu , const double sigma );
    double randHQ( unsigned int N );
    double randHQ( unsigned int N, const double mu, const double sigma );

    double operator ()();

private:

    kvs::MersenneTwister m_uniform_random ; ///< Uniform random number generator
};

/*===========================================================================*/
/**
 *  @brief  Get a Gaussian random number of N(0,1)
 *  @return random number
 */
/*===========================================================================*/
inline double BoxMuller::rand()
{
    // local variables
    double x1, x2, y ;

    // Generate two uniform random numbers x1, x2 (0 <= xi < 1)
    x1 = m_uniform_random.rand();
    x2 = m_uniform_random.rand();

    // Generate a Gaussian random number y of N(0,1):
    //  "cos" can be replaced with "sin".
    y = std::sqrt( -2.0 * std::log (x1) ) * std::cos ( 2.0 * 3.14159265358979323846 * x2 ) ;

    // result
    return y ;
}

/*===========================================================================*/
/**
 *  @brief  Get a Gaussian random number of N( mu, sigma^2 )
 *  @param  mu [in] mean
 *  @param  sigma [in] variance
 *  @return random number
 */
/*===========================================================================*/
inline double BoxMuller::rand( const double mu , const double sigma )
{
    return this->rand() * sigma + mu;
}

/*===========================================================================*/
/**
 *  @brief  Get a Gaussian random number of N(0, 1) (high-quality version)
 *  @param  N [in] number of iterations
 *  @return random number
 *
 *  Calc an average of N Gaussian random numbers,
 *  y0, y1, ..., y_{N-1}:
 *      y = (y0 + y1 + y2 + ... + y_{N-1} ) / sqrt(N)
 *  This y also becomes a Gaussian random number of N(0,1) with higher
 *  statistical property than yi.
 *  Note: This function is N times slower than rand().
 */
/*===========================================================================*/
inline double BoxMuller::randHQ( unsigned int N )
{
    // square root of N
    double N_sqrt = std::sqrt( (double)N );

    // Calc an average of N Gaussian random numbers y defined by
    //  y = (y0 + y1 + y2 + ... + y_{N-1} ) / sqrt(N)
    double y = 0.0 ;
    for ( int i = 0; i < (int)N ; i++ ) {
        y += rand();
    }
    y /= N_sqrt ;

    // result
    return y ;
}

/*===========================================================================*/
/**
 *  @brief  Get a Gaussian random number of N(mu, sigma^2) (high-quality version).
 *  @param  N [in] number of iterations
 *  @param  mu [in] mean
 *  @param  sigma [in] variance
 *  @return random number
 *
 *  Note: This function is N times slower than rand( sigma, mu ).
 */
/*===========================================================================*/
inline double BoxMuller::randHQ( unsigned int N, const double mu, const double sigma )
{
    return this->randHQ(N) * sigma + mu;
}

/*===========================================================================*/
/**
 *  @brief  This operator is equivalent to rand().
 *  @return random number
 */
/*===========================================================================*/
inline double BoxMuller::operator ()()
{
    return this->rand();
}

} // end of namespace kvs

#endif // KVS__BOX_MULLER_H_INCLUDE
