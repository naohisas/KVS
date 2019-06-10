#include "ChiSquaredDistribution.h"
#include "GammaFunction.h"
#include <kvs/Assert>
#include <cmath>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new ChiSquaredDistribution class.
 *  @param  dof [in] degree of freedom
 */
/*===========================================================================*/
ChiSquaredDistribution::ChiSquaredDistribution( const size_t dof ):
    m_dof( dof )
{
    kvs::GammaFunction gamma;
    m_coef1 = 1.0 / gamma( 0.5 * dof );
    m_coef2 = m_coef1 / std::pow( 2.0, 0.5 * dof );
}

/*===========================================================================*/
/**
 *  @brief  Returns probability density function (PDF).
 *  @param  x [in] argument of the PDF
 *  @return PDF value of the chi-squared distribution
 */
/*===========================================================================*/
kvs::Real64 ChiSquaredDistribution::pdf( const kvs::Real64 x ) const
{
    KVS_ASSERT( x >= 0.0 );
    return m_coef2 * std::pow( x, 0.5 * m_dof - 1.0 ) * std::exp( -0.5 * x );
}

/*===========================================================================*/
/**
 *  @brief  Returns cumulative distribution function (CDF).
 *  @param  x [in] argument of the CDF
 *  @return CDF value of the chi-squared distribution
 */
/*===========================================================================*/
kvs::Real64 ChiSquaredDistribution::cdf( const kvs::Real64 x ) const
{
    KVS_ASSERT( x >= 0.0 );
    kvs::GammaFunction gamma;
    return m_coef1 * gamma.lowerIncompleteGamma( 0.5 * m_dof, 0.5 * x );
}

} // end of namespace kvs
