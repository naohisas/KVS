#include "ExponentialDistribution.h"
#include <kvs/Assert>
#include <cmath>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new ExponentialDistribution class.
 *  @param  lambda [in] lambda
 */
/*===========================================================================*/
ExponentialDistribution::ExponentialDistribution( const kvs::Real64 lambda ):
    m_lambda( lambda )
{
    KVS_ASSERT( lambda > 0.0 );
}

/*===========================================================================*/
/**
 *  @brief  Returns probability density function (PDF).
 *  @param  x [in] argument of the PDF
 *  @return PDF value of the exponential distribution
 */
/*===========================================================================*/
kvs::Real64 ExponentialDistribution::pdf( const kvs::Real64 x ) const
{
    return ( x < 0.0 ) ? 0.0 : m_lambda * std::exp( -1.0 * m_lambda * x );
}

/*===========================================================================*/
/**
 *  @brief  Returns cumulative distribution function (CDF).
 *  @param  x [in] argument of the CDF
 *  @return CDF value of the exponential distribution
 */
/*===========================================================================*/
kvs::Real64 ExponentialDistribution::cdf( const kvs::Real64 x ) const
{
    return ( x < 0.0 ) ? 0.0 : 1.0 - std::exp( -1.0 * m_lambda * x );
}

} // end of namespace kvs
