/*****************************************************************************/
/**
 *  @file   Stat.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
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
 *  @brief  Returns mean value of elements of the given array using online algorithm.
 *  @param  values [in] array
 *  @return mean value
 */
/*===========================================================================*/
template <typename T>
T OnlineMean( const kvs::ValueArray<T>& values )
{
    KVS_ASSERT( values.size() != 0 );
    T mean = 0;
    size_t i = 1;
    for ( const auto& value : values )
    {
        const auto delta = value - mean;
        mean += delta / i++;
    }
    return mean;
}

/*===========================================================================*/
/**
 *  @brief Mean value calculation class with incremental updating.
 */
/*===========================================================================*/
class IncrementalMean
{
private:
    double m_value = 0.0; ///< mean
    size_t m_count = 0; ///< number of samples (added values)

public:
    IncrementalMean() = default;
    double value() const { return m_value; }
    size_t count() const { return m_count; }
    template <typename T>
    void add( const T v )
    {
        m_count++;
        const auto delta = static_cast<double>( v ) - m_value;
        m_value += delta / m_count;
    }
};

/*===========================================================================*/
/**
 *  @brief  Returns variance of the given array.
 *  @param  values [in] value array
 *  @param  mean [in/out] mean value
 *  @return variance
 */
/*===========================================================================*/
template <typename T>
T Var( const kvs::ValueArray<T>& values, T* mean = nullptr )
{
    KVS_ASSERT( values.size() > 1 );

    T sum = T(0), sum2 = T(0);
    for ( const auto& value : values )
    {
        sum += value;
        sum2 += value * value;
    }

    const auto n = values.size();
    if ( mean ) *mean = sum / n;
    return ( sum2 - ( sum * sum ) / n ) / ( n - 1 );
}

/*===========================================================================*/
/**
 *  @brief  Returns population variance of the given array.
 *  @param  values [in] array
 *  @param  mean [in/out] mean value
 *  @return population variance
 */
/*===========================================================================*/
template <typename T>
T VarP( const kvs::ValueArray<T>& values, T* mean = nullptr )
{
    KVS_ASSERT( values.size() != 0 );
    const auto n = values.size();
    if ( n == 1 ) { if ( mean ) *mean = values[0]; return T(0); }
    return Var( values, mean ) * ( n - 1 ) / n;
}

/*===========================================================================*/
/**
 *  @brief  Returns variance of the given array with shifting data.
 *  @param  values [in] array
 *  @param  mean [in/out] mean value
 *  @return variance
 */
/*===========================================================================*/
template <typename T>
T ShiftedVar( const kvs::ValueArray<T>& values, T* mean = nullptr )
{
    KVS_ASSERT( values.size() - 1 != 0 );

    const T k = values[0];
    T sum = T(0), sum2 = T(0);
    for ( const auto& value : values )
    {
        sum += value - k;
        sum2 += ( value - k ) * ( value - k );
    }

    const auto n = values.size();
    if ( mean ) *mean = sum / n;
    return ( sum2 - ( sum * sum ) / n ) / ( n - 1 );
}

/*===========================================================================*/
/**
 *  @brief  Returns population variance of the given array with shifting data.
 *  @param  values [in] array
 *  @param  mean [in/out] mean value
 *  @return population variance
 */
/*===========================================================================*/
template <typename T>
T ShiftedVarP( const kvs::ValueArray<T>& values, T* mean = nullptr )
{
    KVS_ASSERT( values.size() != 0 );
    const auto n = values.size();
    if ( n == 1 ) { if ( mean ) *mean = values[0]; return T(0); }
    return ShiftedVar( values, mean ) * ( n - 1 ) / n;
}

/*===========================================================================*/
/**
 *  @brief  Returns variance calculated by using two-pass algorithm.
 *  @param  values [in] array
 *  @param  mean [in/out] mean value
 *  @return variance
 */
/*===========================================================================*/
template <typename T>
T TwoPassVar( const kvs::ValueArray<T>& values, T* mean = nullptr )
{
    KVS_ASSERT( values.size() - 1 != 0 );

    const auto m = Mean<T>( values );
    T sum = T(0);
    for ( const auto& value : values )
    {
        sum += ( value - m ) * ( value - m );
    }

    const auto n = values.size();
    if ( mean ) *mean = m;
    return sum / ( n - 1 );
}

/*===========================================================================*/
/**
 *  @brief  Returns population variance calculated by using two-pass algorithm.
 *  @param  values [in] array
 *  @param  mean [in/out] mean value
 *  @return population variance
 */
/*===========================================================================*/
template <typename T>
T TwoPassVarP( const kvs::ValueArray<T>& values, T* mean = nullptr )
{
    KVS_ASSERT( values.size() != 0 );
    const auto n = values.size();
    if ( n == 1 ) { if ( mean ) *mean = values[0]; return T(0); }
    return TwoPassVar( values, mean ) * ( n - 1 ) / n;
}

/*===========================================================================*/
/**
 *  @brief  Returns compensated variance for the roundoff error.
 *  @param  values [in] array
 *  @param  mean [in/out] mean value
 *  @return compensated variance
 */
/*===========================================================================*/
template <typename T>
T CompensatedVar( const kvs::ValueArray<T>& values, T* mean = nullptr )
{
    KVS_ASSERT( values.size() - 1 != 0 );

    const auto m = Mean<T>( values );
    T sum = T(0), sum2 = T(0);
    for ( const auto& value : values )
    {
        sum += value - m;
        sum2 += ( value - m ) * ( value - m );
    }

    const auto n = values.size();
    if ( mean ) *mean = m;
    return ( sum2 - ( sum * sum ) / n ) / ( n - 1 );
}

/*===========================================================================*/
/**
 *  @brief  Returns compensated variance of population for the roundoff error.
 *  @param  values [in] array
 *  @param  mean [in/out] mean value
 *  @return compensated population variance
 */
/*===========================================================================*/
template <typename T>
T CompensatedVarP( const kvs::ValueArray<T>& values, T* mean = nullptr )
{
    KVS_ASSERT( values.size() != 0 );
    const auto n = values.size();
    if ( n == 1 ) { if ( mean ) *mean = values[0]; return T(0); }
    return CompensatedVar( values, mean ) * ( n - 1 ) / n;
}

/*===========================================================================*/
/**
 *  @brief  Returns variance calculated by using Welford's online algorithm.
 *  @param  values [in] array
 *  @param  mean [in/out] mean value
 *  @return variance
 */
/*===========================================================================*/
template <typename T>
T OnlineVar( const kvs::ValueArray<T>& values, T* mean = nullptr )
{
    KVS_ASSERT( values.size() - 1 != 0 );

    const size_t n = values.size();
    T m = T(0), m2 = T(0);
    size_t i = 1;
    for ( const auto& value : values )
    {
        const T delta = value - m;
        m += delta / i++;
        m2 += delta * ( value - m );
    }

    if ( mean ) *mean = m;
    return m2 / ( n - 1 );
}

/*===========================================================================*/
/**
 *  @brief Variance calculation class with incremental updating.
 */
/*===========================================================================*/
class IncrementalVar
{
private:
    double m_mean = 0.0; ///< mean
    double m_m2 = 0.0; ///< squared distance from the mean
    size_t m_count = 0; ///< counter for adding values

public:
    IncrementalVar() = default;
    double value() const { return m_m2 / ( m_count - 1 ); }
    double mean() const { return m_mean; }
    size_t count() const { return m_count; }
    template <typename T>
    void add( const T v )
    {
        m_count++;
        const auto delta = static_cast<double>( v ) - m_mean;
        m_mean += delta / m_count;
        m_m2 += delta * ( v - m_mean );
    }
};

/*===========================================================================*/
/**
 *  @brief  Returns population variance calculated by using Welford's online algorithm.
 *  @param  values [in] array
 *  @param  mean [in/out] mean value
 *  @return population variance
 */
/*===========================================================================*/
template <typename T>
T OnlineVarP( const kvs::ValueArray<T>& values, T* mean = nullptr )
{
    KVS_ASSERT( values.size() != 0 );
    const auto n = values.size();
    if ( n == 1 ) { if ( mean ) *mean = values[0]; return T(0); }
    return OnlineVar( values, mean ) * ( n - 1 ) / n;
}

/*===========================================================================*/
/**
 *  @brief  Returns covariance of the given arrays.
 *  @param  values1 [in] array1
 *  @param  values2 [in] array2
 *  @param  mean1 [in/out] mean value1
 *  @param  mean2 [in/out] mean value2
 *  @return covariance
 */
/*===========================================================================*/
template <typename T>
T Cov(
    const kvs::ValueArray<T>& values1,
    const kvs::ValueArray<T>& values2,
    T* mean1 = nullptr,
    T* mean2 = nullptr )
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

    if ( mean1 ) { *mean1 = sum1 / n; }
    if ( mean2 ) { *mean2 = sum2 / n; }
    return ( sum12 - ( sum1 * sum2 ) / n ) / ( n - 1 );
}

/*===========================================================================*/
/**
 *  @brief  Returns population covariance the given arrays.
 *  @param  values1 [in] array1
 *  @param  values2 [in] array2
 *  @param  mean1 [in/out] mean value1
 *  @param  mean2 [in/out] mean value2
 *  @return population covariance
 */
/*===========================================================================*/
template <typename T>
T CovP(
    const kvs::ValueArray<T>& values1,
    const kvs::ValueArray<T>& values2,
    T* mean1 = nullptr,
    T* mean2 = nullptr )
{
    KVS_ASSERT( values1.size() != 0 );
    KVS_ASSERT( values1.size() == values2.size() );
    const size_t n = values1.size();
    return ( n == 1 ) ? T(0) : Cov( values1, values2, mean1, mean2 ) * ( n - 1 ) / n;
}

/*===========================================================================*/
/**
 *  @brief  Returns covariance of the given arrays with shifting data.
 *  @param  values1 [in] array1
 *  @param  values2 [in] array2
 *  @param  mean1 [in/out] mean value1
 *  @param  mean2 [in/out] mean value2
 *  @return covariance
 */
/*===========================================================================*/
template <typename T>
T ShiftedCov(
    const kvs::ValueArray<T>& values1,
    const kvs::ValueArray<T>& values2,
    T* mean1 = nullptr,
    T* mean2 = nullptr )
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

    if ( mean1 ) { *mean1 = sum1 / n + k1; }
    if ( mean2 ) { *mean2 = sum2 / n + k2; }
    return ( sum12 - ( sum1 * sum2 ) / n ) / ( n - 1 );
}

/*===========================================================================*/
/**
 *  @brief  Returns population covariance of the given arrays with shifting data.
 *  @param  values1 [in] array1
 *  @param  values2 [in] array2
 *  @param  mean1 [in/out] mean value1
 *  @param  mean2 [in/out] mean value2
 *  @return population covariance
 */
/*===========================================================================*/
template <typename T>
T ShiftedCovP(
    const kvs::ValueArray<T>& values1,
    const kvs::ValueArray<T>& values2,
    T* mean1 = nullptr,
    T* mean2 = nullptr )
{
    KVS_ASSERT( values1.size() != 0 );
    KVS_ASSERT( values1.size() == values2.size() );
    const size_t n = values1.size();
    return ( n == 1 ) ? T(0) : ShiftedCov( values1, values2, mean1, mean2 ) * ( n - 1 ) / n;
}

/*===========================================================================*/
/**
 *  @brief  Returns covariance calculated by using two-pass algorithm.
 *  @param  values1 [in] array1
 *  @param  values2 [in] array2
 *  @param  mean1 [in/out] mean value1
 *  @param  mean2 [in/out] mean value2
 *  @return covariance
 */
/*===========================================================================*/
template <typename T>
T TwoPassCov(
    const kvs::ValueArray<T>& values1,
    const kvs::ValueArray<T>& values2,
    T* mean1 = nullptr,
    T* mean2 = nullptr )
{
    KVS_ASSERT( values1.size() - 1 != 0 );
    KVS_ASSERT( values1.size() == values2.size() );

    const size_t n = values1.size();
    const T m1 = Sum( values1 ) / n;
    const T m2 = Sum( values2 ) / n;
    T cov = T(0);
    for ( size_t i = 0; i < n; i++ )
    {
        cov += ( values1[i] - m1 ) * ( values2[i] - m2 ) / ( n - 1 );
    }

    if ( mean1 ) { *mean1 = m1; }
    if ( mean2 ) { *mean2 = m2; }
    return cov;
}

/*===========================================================================*/
/**
 *  @brief  Returns population covariance calculated by using two-pass algorithm.
 *  @param  values1 [in] array1
 *  @param  values2 [in] array2
 *  @param  mean1 [in/out] mean value1
 *  @param  mean2 [in/out] mean value2
 *  @return population covariance
 */
/*===========================================================================*/
template <typename T>
T TwoPassCovP(
    const kvs::ValueArray<T>& values1,
    const kvs::ValueArray<T>& values2,
    T* mean1 = nullptr,
    T* mean2 = nullptr )
{
    KVS_ASSERT( values1.size() != 0 );
    KVS_ASSERT( values1.size() == values2.size() );
    const size_t n = values1.size();
    return ( n == 1 ) ? T(0) : TwoPassCov( values1, values2, mean1, mean2 ) * ( n - 1 ) / n;
}

/*===========================================================================*/
/**
 *  @brief  Returns covariance calculated by using Welford's online algorithm.
 *  @param  values1 [in] array1
 *  @param  values2 [in] array2
 *  @param  mean1 [in/out] mean value1
 *  @param  mean2 [in/out] mean value2
 *  @return covariance
 */
/*===========================================================================*/
template <typename T>
T OnlineCov(
    const kvs::ValueArray<T>& values1,
    const kvs::ValueArray<T>& values2,
    T* mean1 = nullptr,
    T* mean2 = nullptr )
{
    KVS_ASSERT( values1.size() - 1 != 0 );
    KVS_ASSERT( values1.size() == values2.size() );

    const size_t n = values1.size();
    T m1 = T(0);
    T m2 = T(0);
    T cov = T(0);
    for ( size_t i = 0; i < n; i++ )
    {
        const T delta1 = ( values1[i] - m1 ) / ( i + 1 );
        const T delta2 = ( values2[i] - m2 ) / ( i + 1 );
        m1 += delta1;
        m2 += delta2;
        cov += i * delta1 * delta2 - cov / ( i + 1 );
    }

    if ( mean1 ) { *mean1 = m1; }
    if ( mean2 ) { *mean2 = m2; }
    return cov * n / ( n - 1 );
}

/*===========================================================================*/
/**
 *  @brief  Returns population covariance calculated by using Welford's online algorithm.
 *  @param  values1 [in] array1
 *  @param  values2 [in] array2
 *  @param  mean1 [in/out] mean value1
 *  @param  mean2 [in/out] mean value2
 *  @return population covariance
 */
/*===========================================================================*/
template <typename T>
T OnlineCovP(
    const kvs::ValueArray<T>& values1,
    const kvs::ValueArray<T>& values2,
    T* mean1 = nullptr,
    T* mean2 = nullptr )
{
    KVS_ASSERT( values1.size() != 0 );
    KVS_ASSERT( values1.size() == values2.size() );
    const size_t n = values1.size();
    return ( n == 1 ) ? T(0) : OnlineCov( values1, values2, mean1, mean2 ) * ( n - 1 ) / n;
}

/*===========================================================================*/
/**
 *  @brief Covariance calculation class with incremental updating.
 */
/*===========================================================================*/
class IncrementalCoV
{
private:
    double m_value = 0.0; ///< covariance
    double m_mean1 = 0.0; ///< mean
    double m_mean2 = 0.0; ///< mean
    size_t m_count = 0; ///< counter for adding values

public:
    IncrementalCoV() = default;
    double value() const { return m_value * m_count / static_cast<double>( m_count - 1 ); }
    double mean1() const { return m_mean1; }
    double mean2() const { return m_mean2; }
    size_t count() const { return m_count; }
    template <typename T>
    void add( const T v1, const T v2 )
    {
        m_count++;
        const auto delta1 = ( static_cast<double>( v1 ) - m_mean1 ) / m_count;
        const auto delta2 = ( static_cast<double>( v2 ) - m_mean2 ) / m_count;
        m_mean1 += delta1;
        m_mean2 += delta2;
        m_value += ( m_count - 1 ) * delta1 * delta2 - m_value / m_count;
    }
};

/*===========================================================================*/
/**
 *  @brief  Returns standard deviation of the given array.
 *  @param  values [in] array
 *  @param  mean [in/out] mean value
 *  @return standard deviation
 */
/*===========================================================================*/
template <typename T>
T StdDev( const kvs::ValueArray<T>& values, T* mean = nullptr )
{
    return T( std::sqrt( Var( values, mean ) ) );
}

/*===========================================================================*/
/**
 *  @brief  Returns standard deviation of the given array using population variance.
 *  @param  values [in] array
 *  @param  mean [in/out] mean value
 *  @return standard deviation
 */
/*===========================================================================*/
template <typename T>
T StdDevP( const kvs::ValueArray<T>& values, T* mean = nullptr )
{
    return T( std::sqrt( VarP( values, mean ) ) );
}

/*===========================================================================*/
/**
 *  @brief  Returns standard deviation of the given array using the specified variance calculation function.
 *  @param  values [in] array
 *  @param  VarF [in] variance calculation function
 *  @param  mean [in/out] mean value
 *  @return standard deviation
 */
/*===========================================================================*/
template <typename T, typename F>
T StdDev( const kvs::ValueArray<T>& values, F VarF, T* mean = nullptr )
{
    return T( std::sqrt( VarF( values, mean ) ) );
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
    auto mean = T(0);
    auto sdev = StdDev( values, OnlineVar<T>, &mean );
    for ( auto& value : values ) { value = ( value - mean ) / sdev; }
}

} // end of namespace Stat

} // end of namespace kvs
