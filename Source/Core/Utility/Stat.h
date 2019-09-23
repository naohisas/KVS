#pragma once
#include <cmath>
#include <numeric>
#include <kvs/Assert>
#include <kvs/ValueArray>
#include <kvs/ValueTable>
#include <kvs/Math>
#include <kvs/Value>


namespace kvs
{

namespace Stat
{

/*===========================================================================*/
/**
 *  @brief  Returns summation of elements of the given array.
 *  @param  values [in] array
 *  @return summation
 */
/*===========================================================================*/
template <typename T>
T Sum( const kvs::ValueArray<T>& values )
{
    return std::accumulate( values.begin(), values.end(), T(0) );
}

/*===========================================================================*/
/**
 *  @brief  Returns mean value of elements of the given array.
 *  @param  values [in] array
 *  @return mean value
 */
/*===========================================================================*/
template <typename T>
T Mean( const kvs::ValueArray<T>& values )
{
    KVS_ASSERT( values.size() != 0 );
    return std::accumulate( values.begin(), values.end(), T(0) ) / values.size();
}

/*===========================================================================*/
/**
 *  @brief  Returns variance of the given array.
 *  @param  values [in] array
 *  @return variance
 */
/*===========================================================================*/
template <typename T>
T Var( const kvs::ValueArray<T>& values )
{
    KVS_ASSERT( values.size() - 1 != 0 );

    const size_t n = values.size();
    T sum = T(0);
    T sum2 = T(0);
    for ( size_t i = 0; i < n; i++ )
    {
        sum += values[i];
        sum2 += values[i] * values[i];
    }

    return ( sum2 - ( sum * sum ) / n ) / ( n - 1 );
}

/*===========================================================================*/
/**
 *  @brief  Returns population variance of the given array.
 *  @param  values [in] array
 *  @return population variance
 */
/*===========================================================================*/
template <typename T>
T VarP( const kvs::ValueArray<T>& values )
{
    KVS_ASSERT( values.size() != 0 );
    const size_t n = values.size();
    return ( n == 1 ) ? T(0) : Var( values ) * ( n - 1 ) / n;
}

/*===========================================================================*/
/**
 *  @brief  Returns variance of the given array with shifting data.
 *  @param  values [in] array
 *  @return variance
 */
/*===========================================================================*/
template <typename T>
T ShiftedVar( const kvs::ValueArray<T>& values )
{
    KVS_ASSERT( values.size() - 1 != 0 );

    const T k = values[0];
    const size_t n = values.size();
    T sum = T(0);
    T sum2 = T(0);
    for ( size_t i = 0; i < n; i++ )
    {
        sum += values[i] - k;
        sum2 += ( values[i] - k ) * ( values[i] - k );
    }

    return ( sum2 - ( sum * sum ) / n ) / ( n - 1 );
}

/*===========================================================================*/
/**
 *  @brief  Returns population variance of the given array with shifting data.
 *  @param  values [in] array
 *  @return population variance
 */
/*===========================================================================*/
template <typename T>
T ShiftedVarP( const kvs::ValueArray<T>& values )
{
    KVS_ASSERT( values.size() != 0 );
    const size_t n = values.size();
    return ( n == 1 ) ? T(0) : ShiftedVar( values ) * ( n - 1 ) / n;
}

/*===========================================================================*/
/**
 *  @brief  Returns variance calculated by using two-pass algorithm.
 *  @param  values [in] array
 *  @return variance
 */
/*===========================================================================*/
template <typename T>
T TwoPassVar( const kvs::ValueArray<T>& values )
{
    KVS_ASSERT( values.size() - 1 != 0 );

    const T mean = Mean( values );
    const size_t n = values.size();
    T sum = T(0);
    for ( size_t i = 0; i < n; i++ )
    {
        sum += ( values[i] - mean ) * ( values[i] - mean );
    }

    return sum / ( n - 1 );
}

/*===========================================================================*/
/**
 *  @brief  Returns population variance calculated by using two-pass algorithm.
 *  @param  values [in] array
 *  @return population variance
 */
/*===========================================================================*/
template <typename T>
T TwoPassVarP( const kvs::ValueArray<T>& values )
{
    KVS_ASSERT( values.size() != 0 );
    const size_t n = values.size();
    return ( n == 1 ) ? T(0) : TwoPassVar( values ) * ( n - 1 ) / n;
}

/*===========================================================================*/
/**
 *  @brief  Returns compensated variance for the roundoff error.
 *  @param  values [in] array
 *  @return compensated variance
 */
/*===========================================================================*/
template <typename T>
T CompensatedVar( const kvs::ValueArray<T>& values )
{
    KVS_ASSERT( values.size() - 1 != 0 );

    const T mean = Mean( values );
    const size_t n = values.size();
    T sum = T(0);
    T sum2 = T(0);
    for ( size_t i = 0; i < n; i++ )
    {
        sum += values[i] - mean;
        sum2 += ( values[i] - mean ) * ( values[i] - mean );
    }

    return ( sum2 - ( sum * sum ) / n ) / ( n - 1 );
}

/*===========================================================================*/
/**
 *  @brief  Returns compensated variance of population for the roundoff error.
 *  @param  values [in] array
 *  @return compensated population variance
 */
/*===========================================================================*/
template <typename T>
T CompensatedVarP( const kvs::ValueArray<T>& values )
{
    KVS_ASSERT( values.size() != 0 );
    const size_t n = values.size();
    return ( n == 1 ) ? T(0) : CompensatedVar( values ) * ( n - 1 ) / n;
}

/*===========================================================================*/
/**
 *  @brief  Returns variance calculated by using Welford's online algorithm.
 *  @param  values [in] array
 *  @return variance
 */
/*===========================================================================*/
template <typename T>
T OnlineVar( const kvs::ValueArray<T>& values )
{
    KVS_ASSERT( values.size() - 1 != 0 );

    const size_t n = values.size();
    T mean = T(0);
    T m2 = T(0);
    for ( size_t i = 0; i < n; i++ )
    {
        const T delta = values[i] - mean;
        mean += delta / ( i + 1 );
        m2 += delta * ( values[i] - mean );
    }

    return m2 / ( n - 1 );
}

/*===========================================================================*/
/**
 *  @brief  Returns population variance calculated by using Welford's online algorithm.
 *  @param  values [in] array
 *  @return population variance
 */
/*===========================================================================*/
template <typename T>
T OnlineVarP( const kvs::ValueArray<T>& values )
{
    KVS_ASSERT( values.size() != 0 );
    const size_t n = values.size();
    return ( n == 1 ) ? T(0) : OnlineVar( values ) * ( n - 1 ) / n;
}

/*===========================================================================*/
/**
 *  @brief  Returns covariance of the given arrays.
 *  @param  values1 [in] array1
 *  @param  values2 [in] array2
 *  @return covariance
 */
/*===========================================================================*/
template <typename T>
T Cov( const kvs::ValueArray<T>& values1, const kvs::ValueArray<T>& values2 )
{
    KVS_ASSERT( values1.size() - 1 != 0 );
    KVS_ASSERT( values1.size() == values2.size() );

    const size_t n = values1.size();
    T sum1 = T(0);
    T sum2 = T(0);
    T sum12 = T(0);
    for ( size_t i = 0; i < n; i++ )
    {
        sum1 += values1[i];
        sum2 += values2[i];
        sum12 += values1[i] * values2[i];
    }

    return ( sum12 - ( sum1 * sum2 ) / n ) / ( n - 1 );
}

/*===========================================================================*/
/**
 *  @brief  Returns population covariance the given arrays.
 *  @param  values1 [in] array1
 *  @param  values2 [in] array2
 *  @return population covariance
 */
/*===========================================================================*/
template <typename T>
T CovP( const kvs::ValueArray<T>& values1, const kvs::ValueArray<T>& values2 )
{
    KVS_ASSERT( values1.size() != 0 );
    KVS_ASSERT( values1.size() == values2.size() );
    const size_t n = values1.size();
    return ( n == 1 ) ? T(0) : Cov( values1, values2 ) * ( n - 1 ) / n;
}

/*===========================================================================*/
/**
 *  @brief  Returns covariance of the given arrays with shifting data.
 *  @param  values1 [in] array1
 *  @param  values2 [in] array2
 *  @return covariance
 */
/*===========================================================================*/
template <typename T>
T ShiftedCov( const kvs::ValueArray<T>& values1, const kvs::ValueArray<T>& values2 )
{
    KVS_ASSERT( values1.size() - 1 != 0 );
    KVS_ASSERT( values1.size() == values2.size() );

    const T k1 = values1[0];
    const T k2 = values2[0];
    const size_t n = values1.size();
    T sum12 = T(0);
    T sum1 = T(0);
    T sum2 = T(0);
    for ( size_t i = 0; i < n; i++ )
    {
        sum1 += values1[i] - k1;
        sum2 += values2[i] - k2;
        sum12 += ( values1[i] - k1 ) * ( values2[i] - k2 );
    }

    return ( sum12 - ( sum1 * sum2 ) / n ) / ( n - 1 );
}

/*===========================================================================*/
/**
 *  @brief  Returns population covariance of the given arrays with shifting data.
 *  @param  values1 [in] array1
 *  @param  values2 [in] array2
 *  @return population covariance
 */
/*===========================================================================*/
template <typename T>
T ShiftedCovP( const kvs::ValueArray<T>& values1, const kvs::ValueArray<T>& values2 )
{
    KVS_ASSERT( values1.size() != 0 );
    KVS_ASSERT( values1.size() == values2.size() );
    const size_t n = values1.size();
    return ( n == 1 ) ? T(0) : ShiftedCov( values1, values2 ) * ( n - 1 ) / n;
}

/*===========================================================================*/
/**
 *  @brief  Returns covariance calculated by using two-pass algorithm.
 *  @param  values1 [in] array1
 *  @param  values2 [in] array2
 *  @return covariance
 */
/*===========================================================================*/
template <typename T>
T TwoPassCov( const kvs::ValueArray<T>& values1, const kvs::ValueArray<T>& values2 )
{
    KVS_ASSERT( values1.size() - 1 != 0 );
    KVS_ASSERT( values1.size() == values2.size() );

    const size_t n = values1.size();
    const T mean1 = Sum( values1 ) / n;
    const T mean2 = Sum( values2 ) / n;
    T cov = T(0);
    for ( size_t i = 0; i < n; i++ )
    {
        cov += ( values1[i] - mean1 ) * ( values2[i] - mean2 ) / ( n - 1 );
    }

    return cov;
}

/*===========================================================================*/
/**
 *  @brief  Returns population covariance calculated by using two-pass algorithm.
 *  @param  values1 [in] array1
 *  @param  values2 [in] array2
 *  @return population covariance
 */
/*===========================================================================*/
template <typename T>
T TwoPassCovP( const kvs::ValueArray<T>& values1, const kvs::ValueArray<T>& values2 )
{
    KVS_ASSERT( values1.size() != 0 );
    KVS_ASSERT( values1.size() == values2.size() );
    const size_t n = values1.size();
    return ( n == 1 ) ? T(0) : TwoPassCov( values1, values2 ) * ( n - 1 ) / n;
}

/*===========================================================================*/
/**
 *  @brief  Returns covariance calculated by using Welford's online algorithm.
 *  @param  values1 [in] array1
 *  @param  values2 [in] array2
 *  @return covariance
 */
/*===========================================================================*/
template <typename T>
T OnlineCov( const kvs::ValueArray<T>& values1, const kvs::ValueArray<T>& values2 )
{
    KVS_ASSERT( values1.size() - 1 != 0 );
    KVS_ASSERT( values1.size() == values2.size() );

    const size_t n = values1.size();
    T mean1 = T(0);
    T mean2 = T(0);
    T cov = T(0);
    for ( size_t i = 0; i < n; i++ )
    {
        const T delta1 = ( values1[i] - mean1 ) / ( i + 1 );
        const T delta2 = ( values2[i] - mean2 ) / ( i + 1 );
        mean1 += delta1;
        mean2 += delta2;
        cov += i * delta1 * delta2 - cov / ( i + 1 );
    }

    return cov * n / ( n - 1 );
}

/*===========================================================================*/
/**
 *  @brief  Returns population covariance calculated by using Welford's online algorithm.
 *  @param  values1 [in] array1
 *  @param  values2 [in] array2
 *  @return population covariance
 */
/*===========================================================================*/
template <typename T>
T OnlineCovP( const kvs::ValueArray<T>& values1, const kvs::ValueArray<T>& values2 )
{
    KVS_ASSERT( values1.size() != 0 );
    KVS_ASSERT( values1.size() == values2.size() );
    const size_t n = values1.size();
    return ( n == 1 ) ? T(0) : OnlineCov( values1, values2 ) * ( n - 1 ) / n;
}

/*===========================================================================*/
/**
 *  @brief  Returns standard deviation of the given array.
 *  @param  values [in] array
 *  @return standard deviation
 */
/*===========================================================================*/
template <typename T>
T StdDev( const kvs::ValueArray<T>& values )
{
    return T( std::sqrt( Var( values ) ) );
}

/*===========================================================================*/
/**
 *  @brief  Returns standard deviation of the given array using population variance.
 *  @param  values [in] array
 *  @return standard deviation
 */
/*===========================================================================*/
template <typename T>
T StdDevP( const kvs::ValueArray<T>& values )
{
    return T( std::sqrt( VarP( values ) ) );
}

/*===========================================================================*/
/**
 *  @brief  Returns standard deviation of the given array using the specified variance calculation function.
 *  @param  values [in] array
 *  @param  VarF [in] variance calculation function
 *  @return standard deviation
 */
/*===========================================================================*/
template <typename T, typename F>
T StdDev( const kvs::ValueArray<T>& values, F VarF )
{
    return T( std::sqrt( VarF( values ) ) );
}

/*===========================================================================*/
/**
 *  @brief  Returns correlation between the given arrays with a lag.
 *  @param  values1 [in] array 1
 *  @param  values2 [in] array 2
 *  @param  lag [in] lag
 *  @return correlation
 */
/*===========================================================================*/
template <typename T>
T Corr( const kvs::ValueArray<T>& values1, const kvs::ValueArray<T>& values2, const size_t lag = 0 )
{
    KVS_ASSERT( values1.size() != 0 );
    KVS_ASSERT( values1.size() > lag );
    KVS_ASSERT( values1.size() == values2.size() );

    const size_t k = lag;
    const size_t n = values1.size();
    T sum1 = T(0), sum11 = T(0);
    T sum2 = T(0), sum22 = T(0);
    T sum12 = T(0);
    for ( size_t i = k; i < n; i++ )
    {
        sum1 += values1[i];
        sum11 += values1[i] * values1[i];
        sum2 += values2[i-k];
        sum22 += values2[i-k] * values2[i-k];
        sum12 += values1[i] * values2[i-k];
    }

    const T var1 = sum11 - ( sum1 * sum1 ) / n;
    const T var2 = sum22 - ( sum2 * sum2 ) / n;
    const T cov = sum12 - ( sum1 * sum2 ) / n;
    return cov / std::sqrt( var1 * var2 );
}

/*===========================================================================*/
/**
 *  @brief  Returns auto-correlation for the given array with a lag.
 *  @param  values [in] array
 *  @param  lag [in] lag
 *  @return auto-correlation
 */
/*===========================================================================*/
template <typename T>
T AutoCorr( const kvs::ValueArray<T>& values, const size_t lag )
{
    return Corr( values, values, lag );
}

/*===========================================================================*/
/**
 *  @brief  Returns array of auto-correlation for the given array.
 *  @param  values [in] array
 *  @return array of auto-correlation
 */
/*===========================================================================*/
template <typename T>
kvs::ValueArray<T> AutoCorr( const kvs::ValueArray<T>& values )
{
    const size_t n = values.size();
    kvs::ValueArray<T> corrs( n );
    for ( size_t i = 0; i < n; i++ ) { corrs[i] = AutoCorr( values, i ); }
    return corrs;
}

/*===========================================================================*/
/**
 *  @brief  Returns cross-correlation between the given arrays with a lag.
 *  @param  values1 [in] array 1
 *  @param  values2 [in] array 2
 *  @param  lag [in] lag
 *  @return cross-correlation
 */
/*===========================================================================*/
template <typename T>
T CrossCorr( const kvs::ValueArray<T>& values1, const kvs::ValueArray<T>& values2, const size_t lag )
{
    return Corr( values1, values2, lag );
}

/*===========================================================================*/
/**
 *  @brief  Returns array of cross-correlation between the given arrays.
 *  @param  values1 [in] array 1
 *  @param  values2 [in] array 2
 *  @return array of cross-correlation
 */
/*===========================================================================*/
template <typename T>
kvs::ValueArray<T> CrossCorr( const kvs::ValueArray<T>& values1, const kvs::ValueArray<T>& values2 )
{
    const size_t n = values1.size();
    kvs::ValueArray<T> corrs( n );
    for ( size_t i = 0; i < n; i++ ) { corrs[i] = CrossCorr( values1, values2, i ); }
    return corrs;
}

/*===========================================================================*/
/**
 *  @brief  Standardize the given array.
 *  @param  values [in/out] array
 */
/*===========================================================================*/
template <typename T>
void Standardize( kvs::ValueArray<T>& values )
{
    KVS_ASSERT( values.size() - 1 != 0 );

    const size_t n = values.size();
    T sum = T(0), sum2 = T(0);
    for ( size_t i = 0; i < n; i++ )
    {
        sum += values[i];
        sum2 += values[i] * values[i];
    }

    const kvs::Real64 var = kvs::Real64( ( sum2 - ( sum * sum ) / n ) ) / ( n - 1 );
    const kvs::Real64 stddev = std::sqrt( var );
    const kvs::Real64 mean = kvs::Real64( sum ) / n;
    for ( size_t i = 0; i < n; i++ )
    {
        values[i] = ( values[i] - mean ) / stddev;
    }
}

} // end of namespace Stat

} // end of namespace kvs
