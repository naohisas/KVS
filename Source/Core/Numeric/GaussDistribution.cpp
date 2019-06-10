#include "GaussDistribution.h"
#include "GammaFunction.h"
#include <kvs/Assert>
#include <kvs/Math>
#include <cmath>


namespace
{

inline kvs::Real64 Erf( const kvs::Real64 x )
{
    // If the compiler supports C++11, this function can be replaced with std::erf().
    kvs::GammaFunction gamma;
    return x < 0.0 ? -1.0 * gamma.p( 0.5, x*x ) : gamma.p( 0.5, x*x );
}

}

namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new GaussDistribution class.
 *  @param  mean [in] mean value
 *  @param  sdev [in] standard deviation
 */
/*===========================================================================*/
GaussDistribution::GaussDistribution( const kvs::Real64 mean, const kvs::Real64 sdev ):
    m_mean( mean ),
    m_sdev( sdev )
{
    KVS_ASSERT( sdev != 0.0 );
}

/*===========================================================================*/
/**
 *  @brief  Returns probability density function (PDF).
 *  @param  x [in] argument of the PDF
 *  @return PDF value of the gauss distribution
 */
/*===========================================================================*/
kvs::Real64 GaussDistribution::pdf( const kvs::Real64 x ) const
{
    static const kvs::Real64 root_2pi = std::sqrt( 2.0 * kvs::Math::PI() );
    return std::exp( -1.0 * kvs::Math::Square( ( x - m_mean ) / m_sdev ) ) / ( root_2pi * m_sdev );
}

/*===========================================================================*/
/**
 *  @brief  Returns cumulative distribution function (CDF).
 *  @param  x [in] argument of the CDF
 *  @return CDF value of the gauss distribution
 */
/*===========================================================================*/
kvs::Real64 GaussDistribution::cdf( const kvs::Real64 x ) const
{
    static const kvs::Real64 root_2 = std::sqrt( 2.0 );
    return 0.5 * ( 1.0 + ::Erf( ( x - m_mean ) / ( root_2 * m_sdev ) ) );
}

} // end of namespace kvs
