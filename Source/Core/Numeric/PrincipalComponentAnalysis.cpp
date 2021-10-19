/*****************************************************************************/
/**
 *  @file   PrincipalComponentAnalysis.cpp
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#include "PrincipalComponentAnalysis.h"
#include <kvs/Stat>
#include <kvs/Matrix>
#include <kvs/EigenDecomposition>
#include <algorithm>


namespace
{

/*===========================================================================*/
/**
 *  @brief  Returns covariance matrix.
 *  @param  data [in] data table
 */
/*===========================================================================*/
template <typename T>
inline kvs::Matrix<T> Covariance( const kvs::ValueTable<T>& data )
{
    const size_t ncols = data.columnSize();
    kvs::Matrix<T> cov( ncols, ncols );
    for ( size_t row = 0; row < ncols; ++row )
    {
        for ( size_t col = 0; col < ncols; ++col )
        {
            cov[row][col] = kvs::Stat::OnlineCov( data[row], data[col] );
            if ( row != col ) { cov[col][row] = cov[row][col]; }
        }
    }
    return std::move( cov );
}

/*===========================================================================*/
/**
 *  @brief  Returns mean values.
 *  @param  data [in] data table
 */
/*===========================================================================*/
template <typename T>
inline kvs::Vector<T> Mean( const kvs::ValueTable<T>& data )
{
    const size_t ncols = data.columnSize();
    kvs::Vector<T> mean( ncols );
    for ( size_t i = 0; i < ncols; i++ )
    {
        mean[i] = kvs::Stat::OnlineMean( data.column(i) );
    }
    return std::move( mean );
}

/*===========================================================================*/
/**
 *  @brief  Returns deviation matrix.
 *  @param  data [in] data table
 */
/*===========================================================================*/
template <typename T>
inline kvs::Matrix<T> Deviation( const kvs::ValueTable<T>& data )
{
    auto& d = const_cast<kvs::ValueTable<T>& >( data );
    const auto mean = Mean( data );
    const size_t nrows = data.rowSize();
    const size_t ncols = data.columnSize();
    kvs::Matrix<T> matrix( nrows, ncols );
    for ( size_t i = 0; i < nrows; i++ )
    {
        matrix[i] = kvs::Vector<T>( d.beginRow(i), d.endRow(i) ) - mean;
    }
    return std::move( matrix );
}

} // end of namespace


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new PCA class.
 *  @param  data [in] data table
 *  @param  ncomponents [in] number of components
 */
/*===========================================================================*/
template <typename T>
PrincipalComponentAnalysis<T>::PrincipalComponentAnalysis(
    const kvs::ValueTable<T>& data,
    const size_t ncomponents ):
    m_ncomponents( ncomponents )
{
    this->fit( data );
}

/*===========================================================================*/
/**
 *  @brief  Calculate the position of the embedded points.
 *  @param  data [in] input data table
 */
/*===========================================================================*/
template <typename T>
void PrincipalComponentAnalysis<T>::fit( const kvs::ValueTable<T>& data )
{
    using Eigen = kvs::EigenDecomposition<T>;
    const kvs::Matrix<T> cov = ::Covariance<T>( data );
    const Eigen eigen( cov );
    const T sum = std::accumulate( eigen.eigenValues().begin(), eigen.eigenValues().end(), T(0) );

    m_ncomponents = m_ncomponents == 0 ? data.columnSize() : m_ncomponents;
    m_components.resize( m_ncomponents, cov.columnSize() );
    m_explained_variance.resize( m_ncomponents );
    m_explained_variance_ratio.resize( m_ncomponents );
    for ( size_t i = 0; i < m_ncomponents; i++ )
    {
        m_components[i] = eigen.eigenVectors()[i];
        m_explained_variance[i] = eigen.eigenValues()[i];
        m_explained_variance_ratio[i] = eigen.eigenValues()[i] / sum;
    }
}

/*===========================================================================*/
/**
 *  @brief  Calculate and returns the position of the embedded points.
 *  @param  data [in] input data table
 */
/*===========================================================================*/
template <typename T>
kvs::ValueTable<T> PrincipalComponentAnalysis<T>::transform( const kvs::ValueTable<T>& data )
{
    if ( m_components.rowSize() == 0 ) { this->fit( data ); }
    kvs::Matrix<T> m = ::Deviation<T>( data ) * m_components.transposed();
    kvs::ValueTable<T> t( m.rowSize(), m.columnSize() );
    std::copy( m.begin(), m.end(), t.beginInRowOrder() );
    return t;
}

template class PrincipalComponentAnalysis<float>;
template class PrincipalComponentAnalysis<double>;

} // end of namespace kvs
