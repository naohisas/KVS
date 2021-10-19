/*****************************************************************************/
/**
 *  @file   MultiDimensionalScaling.cpp
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#include "MultiDimensionalScaling.h"
#include <kvs/Matrix>
#include <kvs/EigenDecomposition>
#include <cmath>


namespace
{

/*===========================================================================*/
/**
 *  @brief  Squre each element of the matrix.
 *  @param  m [in] input matrix
 */
/*===========================================================================*/
template <typename T>
inline void ToSquare( kvs::Matrix<T>& m )
{
    const size_t ncolumns = m.columnSize();
    const size_t nrows = m.rowSize();
    for ( size_t i = 0; i < ncolumns; i++ )
    {
        for ( size_t j = 0; j < nrows; j++ )
        {
            m[i][j] = m[i][j] * m[i][j];
        }
    }
}

}

namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Returns euclidean distance between the i-th and j-th rows of the given data table.
 *  @param  data [in] data table
 *  @param  i [in] index of the row values1
 *  @param  j [in] index of the row values2
 */
/*===========================================================================*/
template <typename T>
T MultiDimensionalScaling<T>::Euclidean(
    const kvs::ValueTable<T>& data,
    const size_t i,
    const size_t j )
{
    kvs::ValueTable<T>& d = const_cast<kvs::ValueTable<T>& >( data );
    auto rowi = d.beginRow(i);
    auto rowj = d.beginRow(j);
    auto endi = d.endRow(i);
    T dist = T(0);
    while ( rowi != endi )
    {
        const T diff = *rowi - *rowj;
        dist += diff * diff;
        ++rowi;
        ++rowj;
    }
    return static_cast<T>( std::sqrt( dist ) );
}

/*===========================================================================*/
/**
 *  @brief  Returns manhattan distance between the i-th and j-th rows of the given data table.
 *  @param  data [in] data table
 *  @param  i [in] index of the row values1
 *  @param  j [in] index of the row values2
 */
/*===========================================================================*/
template <typename T>
T MultiDimensionalScaling<T>::Manhattan(
    const kvs::ValueTable<T>& data,
    const size_t i,
    const size_t j )
{
    kvs::ValueTable<T>& d = const_cast<kvs::ValueTable<T>& >( data );
    auto rowi = d.beginRow(i);
    auto rowj = d.beginRow(j);
    auto endi = d.endRow(i);
    T dist = T(0);
    while ( rowi != endi )
    {
        const T diff = *rowi - *rowj;
        dist += std::abs( diff );
        ++rowi;
        ++rowj;
    }
    return dist;
}

/*===========================================================================*/
/**
 *  @brief  Returns distance matrix for the given data table.
 *  @param  data [in] data table
 *  @param  distance [in] distance calculation function
 */
/*===========================================================================*/
template <typename T>
kvs::Matrix<T> MultiDimensionalScaling<T>::DistanceMatrix(
    const kvs::ValueTable<T>& data,
    Distance distance )
{
    // Distance matrix
    const size_t N = data.rowSize();
    kvs::Matrix<T> D( N, N );
    for ( size_t i = 0; i < N; i++ )
    {
        for ( size_t j = i + 1; j < N; j++ )
        {
            D[i][j] = distance( data, i, j );
            D[j][i] = D[i][j];
        }
    }
    return D;
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new MDS class.
 *  @param  data [in] input data table
 *  @param  ncomponents [in] number of components
 */
/*===========================================================================*/
template <typename T>
MultiDimensionalScaling<T>::MultiDimensionalScaling(
    const kvs::ValueTable<T>& data,
    const size_t ncomponents ):
    m_ncomponents( ncomponents )
{
    this->fit( data );
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new MDS class.
 *  @param  data [in] input data table
 *  @param  distance [in] dinstance function
 *  @param  ncomponents [in] number of components
 */
/*===========================================================================*/
template <typename T>
MultiDimensionalScaling<T>::MultiDimensionalScaling(
    const kvs::ValueTable<T>& data,
    Distance distance,
    const size_t ncomponents ):
    m_distance( distance ),
    m_ncomponents( ncomponents )
{
    this->fit( data );
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new MDS class.
 *  @param  matrix [in] distance matrix
 *  @param  ncomponents [in] number of components
 */
/*===========================================================================*/
template <typename T>
MultiDimensionalScaling<T>::MultiDimensionalScaling(
    const kvs::Matrix<T>& matrix,
    const size_t ncomponents ):
    m_ncomponents( ncomponents )
{
    this->fit( matrix );
}

/*===========================================================================*/
/**
 *  @brief  Calculate the position of the embedded points.
 *  @param  data [in] input data table
 */
/*===========================================================================*/
template <typename T>
void MultiDimensionalScaling<T>::fit( const kvs::ValueTable<T>& data )
{
    this->fit( DistanceMatrix( data, m_distance ) );
}

/*===========================================================================*/
/**
 *  @brief  Calculate the position of the embedded points.
 *  @param  matrix [in] distance matrix
 */
/*===========================================================================*/
template <typename T>
void MultiDimensionalScaling<T>::fit( const kvs::Matrix<T>& matrix )
{
    const size_t n = matrix.rowSize(); // number of points
    const size_t k = m_ncomponents; // dimension of the points in the embedded space

    // Inner product matrix (P) can be solved by Young-Householder transformation.
    const auto& D = matrix;
    const auto I = kvs::Matrix<T>::Identity( n, n );
    const auto O = kvs::Matrix<T>::Ones( n, n );

    kvs::Matrix<T> DD( D ); ::ToSquare( DD );
    const auto H = I - ( 1.0f / n ) * O; // centering matrix
    const auto P = -0.5f * H * DD * H; // inner product matrix

    // Eigen decomposition.
    using Eigen = kvs::EigenDecomposition<T>;
    Eigen eigen( P, Eigen::Symmetric );
    const kvs::Vector<T>& eval = eigen.eigenValues();
    const kvs::Matrix<T>& evec = eigen.eigenVectors();

    // Embed the points by using the top-k eigen vectors
    const kvs::Vector<T> diag( k, eval.data() );
    const kvs::Matrix<T> A = kvs::Matrix<T>::Diagonal( diag );
    kvs::Matrix<T> Q( k, n );
    for ( size_t i = 0; i < k; i++ ) { Q[i] = evec[i].normalized(); }
    m_embedded_points = A * Q;
}

/*===========================================================================*/
/**
 *  @brief  Calculate and returns the position of the embedded points.
 *  @param  data [in] input data table
 */
/*===========================================================================*/
template <typename T>
kvs::ValueTable<T> MultiDimensionalScaling<T>::transform( const kvs::ValueTable<T>& data )
{
    return this->transform( DistanceMatrix( data, m_distance ) );
}

/*===========================================================================*/
/**
 *  @brief  Calculate and returns the position of the embedded points.
 *  @param  matrix [in] distance matrix
 */
/*===========================================================================*/
template <typename T>
kvs::ValueTable<T> MultiDimensionalScaling<T>::transform( const kvs::Matrix<T>& matrix )
{
    if ( m_embedded_points.rowSize() == 0 ) { this->fit( matrix ); }
    kvs::ValueTable<T> t( m_embedded_points.columnSize(), m_embedded_points.rowSize() );
    std::copy( m_embedded_points.begin(), m_embedded_points.end(), t.begin() );
    return t;
}

template class MultiDimensionalScaling<float>;
template class MultiDimensionalScaling<double>;

} // end of namespace kvs
