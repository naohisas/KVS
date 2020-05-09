/*****************************************************************************/
/**
 *  @file   AdaptiveKMeans.cpp
 *  @author Zhang Chi, Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id$
 */
/*----------------------------------------------------------------------------
 *
 * References:
 * [1] C. Sugar and J. Gareth, Finding the number of clusters in a data set:
 *     An information theoretic approach, Journal of the American Statistical
 *     Association, vol. 98, pp. 750-763, 2003.
 */
/*****************************************************************************/
#include "AdaptiveKMeans.h"
#include <kvs/FastKMeans>
#include <kvs/Matrix>
#include <cmath>


namespace
{

/*===========================================================================*/
/**
 *  @brief  Returns Mahalanobis distance.
 *  @param  x [in] multivariant vector
 *  @param  u [in] mean values
 *  @return Mahalanobis distance
 */
/*===========================================================================*/
kvs::Real32 GetMahalanobisDistance(
    const kvs::ValueArray<kvs::Real32>& x,
    const kvs::ValueArray<kvs::Real32>& u )
{
    const size_t dim = x.size();
    kvs::Matrix<kvs::Real32> d( dim, 1 );
    kvs::Matrix<kvs::Real32> dt( 1, dim );
    for ( size_t i = 0; i < dim; i++ ) { d[i][0] = dt[0][i] = x[i] - u[i]; }

    // Covariance matrix (identity matrix)
    kvs::Matrix<kvs::Real32> S( dim, dim );
    S.setIdentity();

    // In this function, the Mahalanobis distance reduces to the Euclidean
    // distance since the covariance matrix is the identity matrix.
    return ( dt * S * d )[0][0];
}

/*===========================================================================*/
/**
 *  @brief  Returns row array.
 *  @param  table [in] table data
 *  @param  row_index [in] index of the row
 *  @return row array
 */
/*===========================================================================*/
kvs::ValueArray<kvs::Real32> GetRowArray(
    const kvs::AnyValueTable& table,
    const size_t row_index )
{
    const size_t ncolumns = table.columnSize();
    kvs::ValueArray<kvs::Real32> row( ncolumns );
    for ( size_t i = 0; i < ncolumns; i++ )
    {
        row[i] = table.column(i).at<kvs::Real32>( row_index );
    }

    return row;
}

}

namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new AdaptiveKMeans class.
 */
/*===========================================================================*/
AdaptiveKMeans::AdaptiveKMeans():
    m_nclusters( 0 ),
    m_max_iterations( 100 ),
    m_tolerance( float( 1.e-6 ) ),
    m_max_nclusters( 10 ),
    m_cluster_centers( NULL )
{
}

/*===========================================================================*/
/**
 *  @brief  Destroys the AdaptiveKMeans class.
 */
/*===========================================================================*/
AdaptiveKMeans::~AdaptiveKMeans()
{
    if ( m_cluster_centers ) delete [] m_cluster_centers;
}

/*===========================================================================*/
/**
 *  @brief  Executes adaptive k-means clustering.
 */
/*===========================================================================*/
void AdaptiveKMeans::run()
{
    if ( m_input_table.empty() )
    {
        kvsMessageError("Input table data is not assigned.");
        return;
    }

    const size_t ncolumns = m_input_table.columnSize();
    const size_t nrows = m_input_table.column(0).size();
    for ( size_t i = 1; i < m_input_table.columnSize(); i++ )
    {
        if ( nrows != m_input_table.column(i).size() )
        {
            kvsMessageError("The number of rows is different between each column.");
            return;
        }
    }

    const size_t K = m_max_nclusters; // number of clusters
    const size_t p = ncolumns; // p-dimension
    const kvs::Real32 Y = p * 0.5f; // transformation power

    size_t nclusters = 1; // number of clusters (best k)
    kvs::Real32 Jmax = 0.0f; // maximum jump
    kvs::ValueArray<kvs::UInt32> IDs; // cluster IDs with the best k
    kvs::ValueArray<kvs::Real32>* centers = NULL; // cluster centers with the best k
    kvs::ValueArray<kvs::Real32> distortion( K + 1 ); distortion[0] = 0.0f; // distortion
    for ( size_t k = 1; k < K + 1; k++ )
    {
        // k-means clustering.
        kvs::FastKMeans kmeans;
        kmeans.setSeedingMethod( kvs::FastKMeans::SmartSeeding );
        kmeans.setNumberOfClusters( k );
        kmeans.setMaxIterations( m_max_iterations );
        kmeans.setTolerance( m_tolerance );
        kmeans.setInputTableData( m_input_table );
        kmeans.run();

        // Calculate the distortions (averaged Mahalanobis distance per dimension).
        distortion[k] = 0.0f;
        for ( size_t i = 0; i < nrows; i++ )
        {
            kvs::ValueArray<kvs::Real32> cx = kmeans.clusterCenter(0);
            kvs::ValueArray<kvs::Real32> x = ::GetRowArray( m_input_table, i );
            kvs::Real32 distance = ::GetMahalanobisDistance( x, cx );
            for ( size_t j = 1; j < k; j++ )
            {
                cx = kmeans.clusterCenter(j);
                distance = kvs::Math::Min( distance, ::GetMahalanobisDistance( x, cx ) );
            }
            distortion[k] += distance;
        }
        distortion[k] = ( 1.0f / p ) * ( ( 1.0f / nrows ) * distortion[k] );

        // Calculate jump in transformed distortion.
        kvs::Real32 Jk = std::pow( distortion[k], -Y ) - std::pow( distortion[k-1], -Y );
        if ( Jk > Jmax )
        {
            // Update the number of clusters and store the clustering result (IDs).
            // Finally, the number of clusters can be calculated as the largest jump (argmax J[k]).
            nclusters = k;
            Jmax = Jk;
            IDs = kmeans.clusterIDs();

            if ( centers ) delete [] centers;
            centers = new kvs::ValueArray<kvs::Real32> [ nclusters ];
            for ( size_t i = 0; i < nclusters; i++ )
            {
                centers[i] = kmeans.clusterCenter(i);
            }
        }
    }

    m_nclusters = nclusters;
    m_cluster_ids = IDs;
    m_cluster_centers = centers;
    m_distortions = distortion;
}

/*===========================================================================*/
/**
 *  @brief  Returns jump vector.
 *  @return jump vector
 */
/*===========================================================================*/
const kvs::ValueArray<kvs::Real32> AdaptiveKMeans::jumps() const
{
    if ( m_distortions.size() != m_max_nclusters + 1 ) return kvs::ValueArray<kvs::Real32>();

    const kvs::ValueArray<kvs::Real32>& d = m_distortions;
    const size_t p = m_input_table.columnSize() - 1;
    const kvs::Real32 Y = p * 0.5f;
    kvs::ValueArray<kvs::Real32> J( m_max_nclusters + 1 ); J[0] = 0.0f;
    for ( size_t k = 1; k < m_max_nclusters + 1; k++ )
    {
        J[k] = std::pow( d[k], -Y ) - std::pow( d[k-1], -Y );
    }

    return J;
}

} // end of namespace kvs
