#include "FisherFDistribution.h"
#include "BetaFunction.h"
#include "GammaFunction.h"
#include <kvs/Type>
#include <kvs/Math>
#include <kvs/ValueArray>
#include <kvs/Assert>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new FisherFDistribution class.
 *  @param  dof1 [in] degree of freedom 1
 *  @param  dof2 [in] degree of freedom 2
 */
/*===========================================================================*/
FisherFDistribution::FisherFDistribution( const size_t dof1, const size_t dof2 ):
    m_dof1( dof1 ),
    m_dof2( dof2 )
{
    KVS_ASSERT( dof1 > 0.0 );
    KVS_ASSERT( dof2 > 0.0 );
    kvs::GammaFunction gamma;
    const kvs::Real64 g0 = gamma( ( dof1 + dof2 ) / 2.0 );
    const kvs::Real64 g1 = gamma( dof1 / 2.0 );
    const kvs::Real64 g2 = gamma( dof2 / 2.0 );
    m_coef = std::pow( double( dof1 ) / dof2, dof1 / 2.0 ) * g0 / ( g1 * g2 );
}

/*===========================================================================*/
/**
 *  @brief  Returns probability density function (PDF).
 *  @param  x [in] argument of the PDF
 *  @return PDF value of the fisher's F distribution
 */
/*===========================================================================*/
kvs::Real64 FisherFDistribution::pdf( const kvs::Real64 x ) const
{
    KVS_ASSERT( x >= 0.0 );
    const kvs::Real64 term1 = std::pow( x, 0.5 * m_dof1 - 1.0 );
    const kvs::Real64 term2 = std::pow( 1.0 + x * m_dof1 / m_dof2, 0.5 * ( m_dof1 + m_dof2 ) );
    return m_coef * term1 / term2;
}

/*===========================================================================*/
/**
 *  @brief  Returns cumulative distribution function (CDF).
 *  @param  x [in] argument of the CDF
 *  @return CDF value of the fisher's F distribution
 */
/*===========================================================================*/
kvs::Real64 FisherFDistribution::cdf( const kvs::Real64 x ) const
{
    KVS_ASSERT( x >= 0.0 );
    kvs::BetaFunction beta;
    return beta.p( 0.5 * m_dof1, 0.5 * m_dof2, ( m_dof1 * x ) / ( m_dof1 * x + m_dof2 ) );
}

} // end of namespace kvs
