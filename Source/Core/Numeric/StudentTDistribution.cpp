#include "StudentTDistribution.h"
#include "BetaFunction.h"
#include <kvs/Math>
#include <kvs/Assert>
#include <cmath>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new StudentTDistribution class.
 *  @param  dof [in] degree of freedom
 */
/*===========================================================================*/
StudentTDistribution::StudentTDistribution( const size_t dof ):
    m_dof( dof )
{
    KVS_ASSERT( dof > 0 );
    kvs::BetaFunction beta;
    m_coef = 1.0 / ( std::sqrt( kvs::Real64( dof ) ) * beta( 0.5, 0.5 * dof ) );
}

/*===========================================================================*/
/**
 *  @brief  Returns probability density function (PDF).
 *  @param  x [in] argument of the PDF
 *  @return PDF value of the student's T distribution
 */
/*===========================================================================*/
kvs::Real64 StudentTDistribution::pdf( const kvs::Real64 t ) const
{
    const size_t n = m_dof;
    if ( n == 1 )
    {
        return 1.0 / ( kvs::Math::PI() * ( 1.0 + t * t ) );
    }

    else if ( n == 2 )
    {
        return 1.0 / std::pow( 2.0 + t * t, 1.5 );
    }

    else if ( n == 3 )
    {
        return 6.0 * std::sqrt( 3.0 ) / ( kvs::Math::PI() * kvs::Math::Square( 3.0 + t * t ) );
    }

    else if ( n > 100 )
    {
        return std::exp( -1.0 * t * t / 2.0 ) / std::sqrt( 2.0 * kvs::Math::PI() );
    }

    else // 4 <= n <= 100
    {
        return m_coef * std::pow( 1.0 + t * t / n, -0.5 * ( n + 1 ) );
    }
}

/*===========================================================================*/
/**
 *  @brief  Returns cumulative distribution function (CDF).
 *  @param  x [in] argument of the CDF
 *  @return CDF value of the student's T distribution
 */
/*===========================================================================*/
kvs::Real64 StudentTDistribution::cdf( const kvs::Real64 t ) const
{
    const size_t n = m_dof;
    if ( n == 1 )
    {
        return 0.5 + ( 1.0 / kvs::Math::PI() ) * std::atan( t );
    }

    else if ( n == 2 )
    {
        return 0.5 + t / ( 2.0 * std::sqrt( 2.0 + t * t ) );
    }

    else
    {
        kvs::BetaFunction beta;
        const kvs::Real64 a = n * 0.5;
        const kvs::Real64 x = ( t + std::sqrt( t * t + n ) ) / ( 2.0 * std::sqrt( t * t + n ) );
        return beta.p( a, a, x );
    }
}

} // end of namespace kvs
