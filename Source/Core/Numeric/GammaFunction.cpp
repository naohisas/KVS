#include "GammaFunction.h"
#include <kvs/Assert>
#include <limits>
#include <cmath>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Returns gamma function.
 *  @param  x [in] argument for the gamma function
 *  @return gamma value
 */
/*===========================================================================*/
kvs::Real64 GammaFunction::operator () ( const kvs::Real64 x ) const
{
    return this->gamma( x );
}

/*===========================================================================*/
/**
 *  @brief  Returns natural logarithm of the gamma function.
 *  @param  x [in] argument for gamma function
 *  @return log of gamma function
 */
/*===========================================================================*/
kvs::Real64 GammaFunction::log( const kvs::Real64 x ) const
{
    KVS_ASSERT( x > 0.0 );

    static const kvs::Real64 coef[6] = {
        76.18009172947146,
        -86.50532032941677,
        24.01409824083091,
        -1.231739572450155,
        0.1208650973866179e-2,
        -0.5395239384953e-5
    };

    const kvs::Real64 temp = ( x + 5.5 ) - ( x + 0.5 ) * std::log( x + 5.5 );
    kvs::Real64 ser = 1.000000000190015;
    kvs::Real64 y = x;
    for ( size_t i = 0; i < 6; i++ ) { ser += coef[i] / ( y + 1.0 ); y = y + 1.0; }

    return -1.0 * temp + std::log( 2.5066282746310005 * ser / x );
}

/*===========================================================================*/
/**
 *  @brief  Returns regularized lower incomplete gamma function.
 *  @param  a [in] argument for the gamma function
 *  @param  x [in] upper integral limit
 *  @return regularized lower integral gamma function
 */
/*===========================================================================*/
kvs::Real64 GammaFunction::p( const kvs::Real64 a, const kvs::Real64 x ) const
{
    return this->regularizedLowerIncompleteGamma( a, x );
}

/*===========================================================================*/
/**
 *  @brief  Returns regularized upper incomplete gamma function.
 *  @param  a [in] argument for the gamma function
 *  @param  x [in] lower integral limit
 *  @return regularized upper integral gamma function
 */
/*===========================================================================*/
kvs::Real64 GammaFunction::q( const kvs::Real64 a, const kvs::Real64 x ) const
{
    return this->regularizedUpperIncompleteGamma( a, x );
}

/*===========================================================================*/
/**
 *  @brief  Returns gamma function.
 *  @param  x [in] argument for the gamma function
 *  @return gamma value
 */
/*===========================================================================*/
kvs::Real64 GammaFunction::gamma( const kvs::Real64 x ) const
{
    return std::exp( this->log( x ) );
}

/*===========================================================================*/
/**
 *  @brief  Returns digamma function.
 *  @param  x [in] argument for the gamma function
 *  @return digamma value
 */
/*===========================================================================*/
kvs::Real64 GammaFunction::digamma( const kvs::Real64 x ) const
{
    KVS_ASSERT( x > 0.0 );

    kvs::Real64 z = x;
    kvs::Real64 r = 0.0;
    while ( z <= 5 ) { r -= 1.0 / z; z += 1; }

    const kvs::Real64 f = 1.0 / ( z * z );
    const kvs::Real64 t =
        f * ( -1.0 / 12.0 +
              f * ( 1.0 / 120.0 +
                    f * ( -1.0 / 252.0 +
                          f * ( 1.0 / 240.0 +
                                f * ( -1.0 / 132.0 +
                                      f * ( 691.0 / 32760.0 +
                                            f * ( -1.0 / 12.0 +
                                                  f * 3617.0 / 8160.0 ) ) ) ) ) ) );

    return r + std::log( z ) - 0.5 / z + t;
}

/*===========================================================================*/
/**
 *  @brief  Returns lower incomplete gamma function.
 *  @param  a [in] argument for the gamma function
 *  @param  x [in] upper integral limit
 *  @return lower integral gamma function
 */
/*===========================================================================*/
kvs::Real64 GammaFunction::lowerIncompleteGamma( const kvs::Real64 a, const kvs::Real64 x ) const
{
    KVS_ASSERT( a > 0.0 );
    KVS_ASSERT( x >= 0.0 );
    if ( x <= 0.0 ) { return 0.0; }
    return this->series( a, x ) * std::exp( a * std::log(x) - x );
}

/*===========================================================================*/
/**
 *  @brief  Returns upper incomplete gamma function.
 *  @param  a [in] argument for the gamma function
 *  @param  x [in] lower integral limit
 *  @return upper integral gamma function
 */
/*===========================================================================*/
kvs::Real64 GammaFunction::upperIncompleteGamma( const kvs::Real64 a, const kvs::Real64 x ) const
{
    KVS_ASSERT( a > 0.0 );
    KVS_ASSERT( x >= 0.0 );
    if ( x <= 0.0 ) { return this->gamma(a); }
    return this->continued_fraction( a, x ) * std::exp( a * std::log(x) - x );
}

/*===========================================================================*/
/**
 *  @brief  Returns regularized lower incomplete gamma function.
 *  @param  a [in] argument for the gamma function
 *  @param  x [in] upper integral limit
 *  @return regularized lower integral gamma function
 */
/*===========================================================================*/
kvs::Real64 GammaFunction::regularizedLowerIncompleteGamma( const kvs::Real64 a, const kvs::Real64 x ) const
{
    KVS_ASSERT( a > 0.0 );
    KVS_ASSERT( x >= 0.0 );
    if ( x <= 0.0 ) { return 0.0; }
    if ( x < a + 1.0 ) { return this->series( a, x ) * std::exp( a * std::log(x) - x - this->log(a) ); }
    return 1.0 - this->continued_fraction( a, x ) * std::exp( a * std::log(x) - x - this->log(a) );
}

/*===========================================================================*/
/**
 *  @brief  Returns regularized upper incomplete gamma function.
 *  @param  a [in] argument for the gamma function
 *  @param  x [in] lower integral limit
 *  @return regularized upper integral gamma function
 */
/*===========================================================================*/
kvs::Real64 GammaFunction::regularizedUpperIncompleteGamma( const kvs::Real64 a, const kvs::Real64 x ) const
{
    KVS_ASSERT( a > 0.0 );
    KVS_ASSERT( x >= 0.0 );
    if ( x <= 0.0 ) { return 1.0; }
    if ( x < a + 1.0 ) { return 1.0 - this->series( a, x ) * std::exp( a * std::log(x) - x - this->log(a) ); }
    return this->continued_fraction( a, x ) * std::exp( a * std::log(x) - x - this->log(a) );
}

kvs::Real64 GammaFunction::series( const kvs::Real64 a, const kvs::Real64 x ) const
{
    KVS_ASSERT( a > 0.0 );
    KVS_ASSERT( x >= 0.0 );

    const size_t max_iterations = 500;
    const kvs::Real64 epsilon = 3.0e-7;
    kvs::Real64 del = 1.0 / a;
    kvs::Real64 sum = del;
    kvs::Real64 ap = a;
    for ( size_t i = 1; i <= max_iterations; i++ )
    {
        ++ap;
        del *= x / ap;
        sum += del;
        if ( std::abs( del ) < std::abs( sum ) * epsilon ) { return sum; }
    }

    kvsMessageError("Not converged.");
    return std::numeric_limits<kvs::Real64>::quiet_NaN();
}

kvs::Real64 GammaFunction::continued_fraction( const kvs::Real64 a, const kvs::Real64 x ) const
{
    KVS_ASSERT( a > 0.0 );
    KVS_ASSERT( x >= 0.0 );

    const size_t max_iterations = 500;
    const kvs::Real64 epsilon = 3.0e-7;
    const kvs::Real64 fpmin = 1.0e-30;
    kvs::Real64 b = x + 1.0 - a;
    kvs::Real64 c = 1.0 / fpmin;
    kvs::Real64 d = 1.0 / b;
    kvs::Real64 h = d;
    for ( size_t i = 1; i <= max_iterations; i++ )
    {
        kvs::Real64 an = i * ( a - i );
        b += 2.0;

        d = an * d + b;
        if ( std::abs( d ) < fpmin ) { d = fpmin; }

        c = b + an / c;
        if ( std::abs( c ) < fpmin ) { c = fpmin; }

        d = 1.0 / d;
        kvs::Real64 del = d * c;
        h *= del;
        if ( std::abs( del - 1.0 ) < epsilon ) { return h; }
    }

    kvsMessageError("Not converged.");
    return std::numeric_limits<kvs::Real64>::quiet_NaN();
}

} // end of namespace kvs
