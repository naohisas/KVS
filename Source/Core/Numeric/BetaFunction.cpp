#include "BetaFunction.h"
#include "GammaFunction.h"
#include <kvs/Assert>
#include <cmath>
#include <limits>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Returns beta function.
 *  @param  a [in] fisrt argument for the beta function
 *  @param  b [in] second argument for the beta function
 *  @return beta value
 */
/*===========================================================================*/
kvs::Real64 BetaFunction::operator () ( const kvs::Real64 a, const kvs::Real64 b ) const
{
    return this->beta( a, b );
}

/*===========================================================================*/
/**
 *  @brief  Returns natural logarithm of the beta function.
 *  @param  a [in] fisrt argument for the beta function
 *  @param  b [in] second argument for the beta function
 *  @return log of beta function
 */
/*===========================================================================*/
kvs::Real64 BetaFunction::log( const kvs::Real64 a, const kvs::Real64 b ) const
{
    kvs::GammaFunction gamma;
    return gamma.log(a) + gamma.log(b) - gamma.log(a+b);
}

/*===========================================================================*/
/**
 *  @brief  Returns regularized lower incomplete beta function.
 *  @param  a [in] fisrt argument for the beta function
 *  @param  b [in] second argument for the beta function
 *  @param  x [in] upper integral limit
 *  @return regularized lower integral beta function
 */
/*===========================================================================*/
kvs::Real64 BetaFunction::p( const kvs::Real64 a, const kvs::Real64 b, const kvs::Real64 x ) const
{
    return this->regularizedLowerIncompleteBeta( a, b, x );
}

/*===========================================================================*/
/**
 *  @brief  Returns regularized upper incomplete beta function.
 *  @param  a [in] fisrt argument for the beta function
 *  @param  b [in] second argument for the beta function
 *  @param  x [in] lower integral limit
 *  @return regularized upper integral beta function
 */
/*===========================================================================*/
kvs::Real64 BetaFunction::q( const kvs::Real64 a, const kvs::Real64 b, const kvs::Real64 x ) const
{
    return this->regularizedUpperIncompleteBeta( a, b, x );
}

/*===========================================================================*/
/**
 *  @brief  Returns beta function.
 *  @param  a [in] fisrt argument for the beta function
 *  @param  b [in] second argument for the beta function
 *  @return beta value
 */
/*===========================================================================*/
kvs::Real64 BetaFunction::beta( const kvs::Real64 a, const kvs::Real64 b ) const
{
    kvs::GammaFunction gamma;
    return std::exp( gamma.log(a) + gamma.log(b) - gamma.log(a+b) );
}

/*===========================================================================*/
/**
 *  @brief  Returns lower incomplete beta function.
 *  @param  a [in] fisrt argument for the beta function
 *  @param  b [in] second argument for the beta function
 *  @param  x [in] upper integral limit
 *  @return lower integral beta function
 */
/*===========================================================================*/
kvs::Real64 BetaFunction::lowerIncompleteBeta(
    const kvs::Real64 a,
    const kvs::Real64 b,
    const kvs::Real64 x ) const
{
    return this->regularizedLowerIncompleteBeta( a, b, x ) * this->beta( a, b );
}

/*===========================================================================*/
/**
 *  @brief  Returns upper incomplete beta function.
 *  @param  a [in] fisrt argument for the beta function
 *  @param  b [in] second argument for the beta function
 *  @param  x [in] lower integral limit
 *  @return upper integral beta function
 */
/*===========================================================================*/
kvs::Real64 BetaFunction::upperIncompleteBeta(
    const kvs::Real64 a,
    const kvs::Real64 b,
    const kvs::Real64 x ) const
{
    return this->regularizedUpperIncompleteBeta( a, b, x ) * this->beta( a, b );
}

/*===========================================================================*/
/**
 *  @brief  Returns regularized upper incomplete beta function.
 *  @param  a [in] fisrt argument for the beta function
 *  @param  b [in] second argument for the beta function
 *  @param  x [in] lower integral limit
 *  @return regularized upper integral beta function
 */
/*===========================================================================*/
kvs::Real64 BetaFunction::regularizedUpperIncompleteBeta(
    const kvs::Real64 a,
    const kvs::Real64 b,
    const kvs::Real64 x ) const
{
    return 1.0 - this->regularizedLowerIncompleteBeta( a, b, x );
}

/*===========================================================================*/
/**
 *  @brief  Returns regularized lower incomplete beta function.
 *  @param  a [in] fisrt argument for the beta function
 *  @param  b [in] second argument for the beta function
 *  @param  x [in] upper integral limit
 *  @return regularized lower integral beta function
 */
/*===========================================================================*/
kvs::Real64 BetaFunction::regularizedLowerIncompleteBeta(
    const kvs::Real64 a,
    const kvs::Real64 b,
    const kvs::Real64 x ) const
{
    KVS_ASSERT( 0.0 <= x && x <= 1.0 );

    kvs::Real64 bt = 0.0;
    if ( x == 0.0 || x == 1.0 ) { bt = 0.0; }
    else if ( ( x > 0.0 ) && ( x < 1.0 ) )
    {
        kvs::GammaFunction gamma;
        bt = gamma( a + b ) * std::pow( x, a ) * std::pow( 1.0 - x, b ) / ( gamma(a) * gamma(b) );
    }

    if (  x < ( a + 1.0 ) / ( a + b + 2.0 ) )
    {
        return bt * this->continued_fraction( a, b, x ) / a;
    }
    else
    {
        return 1.0 - bt * this->continued_fraction( b, a, 1.0 - x ) / b;
    }
}

kvs::Real64 BetaFunction::continued_fraction( const kvs::Real64 a, const kvs::Real64 b, const kvs::Real64 x ) const
{
    const size_t max_iterations = 100;
    const kvs::Real64 epsilon = 1.0e-8;
    const kvs::Real64 fpmin = 1.0e-30;

    kvs::Real64 qab = a + b;
    kvs::Real64 qap = a + 1.0;
    kvs::Real64 qam = a - 1.0;
    kvs::Real64 c = 1.0;
    kvs::Real64 d = 1.0 - qab * x / qap;
    if ( std::abs(d) < fpmin ) { d = fpmin; }
    d = 1.0 / d;
    kvs::Real64 h = d;
    bool converged = false;
    for ( size_t m = 1; m <= max_iterations; m++ )
    {
        const kvs::Real64 m2 = 2.0 * m;
        kvs::Real64 aa = m * ( b - m ) * x / ( ( qam + m2 ) * ( a + m2 ) );

        d = 1.0 + aa * d;
        if ( std::abs(d) < fpmin ) { d = fpmin; }

        c = 1.0 + aa / c;
        if ( std::abs(c) < fpmin ) { c = fpmin; }

        d = 1.0 / d;
        h *= d * c;
        aa = -1.0 * ( a + m ) * ( qab + m ) * x / ( ( a + m2 ) * ( qap + m2 ) );

        d = 1.0 + aa * d;
        if ( std::abs(d) < fpmin ) { d = fpmin; }

        c = 1.0 + aa / c;
        if ( std::abs(c) < fpmin ) { c = fpmin; }

        d = 1.0  /d;
        const kvs::Real64 del = d * c;
        h *= del;
        if ( std::abs( del - 1.0 ) < epsilon ) { converged = true; break; }
    }

    if ( converged ) { return h; }
    else { return std::numeric_limits<kvs::Real64>::quiet_NaN(); }
}

} // end of namespace kvs
