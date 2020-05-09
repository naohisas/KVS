/*****************************************************************************/
/**
 *  @file   KMeans.cpp
 *  @author Naohisa Sakamoto
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
 * [1] D. Arthur and S. Vassilvitskii, k-means++ : The Advantages of Careful
 *     Seeding, in Proceedings of the eighteenth annual ACM-SIAM symposium on
 *     Discrete algorithms, 2007, pp. 1027-1035.
 */
/*****************************************************************************/
#include "KMeans.h"
#include <kvs/Value>
#include <kvs/Message>
#include <kvs/Math>


namespace
{

kvs::Real32 GetEuclideanDistance(
    const kvs::AnyValueTable& table,
    const kvs::UInt32 row_index,
    const kvs::ValueArray<kvs::Real32>& center )
{
    kvs::Real32 distance = 0.0;
    for ( size_t i = 0; i < table.columnSize(); i++ )
    {
        const kvs::Real32 x0 = center[i];
        const kvs::Real32 x1 = table.column(i).at<kvs::Real32>( row_index );
        distance += ( x1 - x0 ) * ( x1 - x0 );
    }

    return distance;
}

kvs::Real32 GetEuclideanDistance(
    const kvs::AnyValueTable& table,
    const kvs::ValueArray<kvs::Real32>& center_old,
    const kvs::ValueArray<kvs::Real32>& center_new )
{
    kvs::Real32 distance = 0.0;
    for ( size_t i = 0; i < table.columnSize(); i++ )
    {
        const kvs::Real32 x0 = center_old[i];
        const kvs::Real32 x1 = center_new[i];
        distance += ( x1 - x0 ) * ( x1 - x0 );
    }

    return distance;
}
}

namespace
{

/*===========================================================================*/
/**
 *  @brief  Calculates the cluster centroid.
 *  @param  table [in] table data
 *  @param  cluster_id [in] target cluster ID
 *  @param  ids [in] cluster ID array
 *  @param  center [out] cluster centroid
 */
/*===========================================================================*/
void CalculateCenter(
    const kvs::AnyValueTable& table,
    const kvs::UInt32 cluster_id,
    const kvs::ValueArray<kvs::UInt32>& ids,
    kvs::ValueArray<kvs::Real32>* center )
{
    const size_t nrows = table.column(0).size();
    const size_t ncolumns = table.columnSize();

    for ( size_t j = 0; j < ncolumns; j++ ) { center->at(j) = 0.0; }

    size_t counter = 0;
    for ( size_t j = 0; j < nrows; j++ )
    {
        if ( ids[j] == cluster_id )
        {
            for ( size_t k = 0; k < ncolumns; k++ )
            {
                center->at(k) += table.column(k).at<kvs::Real32>(j);
            }
            counter++;
        }
    }
    if ( counter != 0 )
    {
        for ( size_t k = 0; k < ncolumns; k++ ) { center->at(k) /= counter; }
    }
}

/*===========================================================================*/
/**
 *  @brief  Initialize centers of clusters with random seeding method.
 *  @param  table [in] pointer to table
 *  @param  nclusters [in] number of clusters
 *  @param  ids [i]
 *  @param  centers [in/out] pointer to center array
 */
/*===========================================================================*/
void InitializeCentersWithRandomSeeding(
    const kvs::AnyValueTable& table,
    const size_t nclusters,
    const kvs::ValueArray<kvs::UInt32>& ids,
    kvs::ValueArray<kvs::Real32>* centers )
{
    for ( size_t i = 0; i < nclusters; i++ )
    {
        CalculateCenter( table, i, ids, &centers[i] );
    }
}

/*===========================================================================*/
/**
 *  @brief  Initialize centers of clusters with k-means++.
 *  @param  table [in] table data
 *  @param  nclusters [in] number of clusters
 *  @param  ids [i]
 *  @param  centers [in/out] pointer to center array
 */
/*===========================================================================*/
void InitializeCentersWithSmartSeeding(
    const kvs::AnyValueTable& table,
    const size_t nclusters,
    const kvs::ValueArray<kvs::UInt32>& ids,
    kvs::ValueArray<kvs::Real32>* centers )
{
    const size_t nrows = table.column(0).size();
    const size_t ncolumns = table.columnSize();

    CalculateCenter( table, 0, ids, &(centers[0]) );

    for ( size_t i = 1; i < nclusters; i++ )
    {
        kvs::Real32 S = 0.0;
        kvs::Real32* D = new kvs::Real32 [ nrows ];
        for ( size_t j = 0; j < nrows; j++ )
        {
            kvs::Real32 distance = kvs::Value<kvs::Real32>::Max();
            for ( size_t k = 0; k < nclusters; k++ )
            {
                const kvs::Real32 d = ::GetEuclideanDistance( table, j, centers[k] );
                if ( d < distance ) { distance = d; }
            }
            D[j] = distance;
            S += distance * distance;
        }

        size_t index = 0;
        kvs::Real32 P = 0.0;
        for ( size_t j = 0; j < nrows; j++ )
        {
            const kvs::Real32 DD = D[j] * D[j];
            if ( P < kvs::Math::Max( P, DD / S ) )
            {
                P = DD / S;
                index = j;
            }
        }

        for ( size_t j = 0; j < ncolumns; j++ )
        {
            centers[i].at(j) = table.column(j).at<kvs::Real32>(index);
        }
        delete [] D;
    }
}

}


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new KMeans class.
 */
/*===========================================================================*/
KMeans::KMeans():
    m_seeding_method( KMeans::SmartSeeding ),
    m_nclusters( 1 ),
    m_max_iterations( 100 ),
    m_tolerance( float(1.e-6) ),
    m_cluster_centers( NULL )
{
}

/*===========================================================================*/
/**
 *  @brief  Destroys the KMeans class.
 */
/*===========================================================================*/
KMeans::~KMeans()
{
    if ( m_cluster_centers ) delete [] m_cluster_centers;
}

/*===========================================================================*/
/**
 *  @brief  Executes K-means clustering.
 */
/*===========================================================================*/
void KMeans::run()
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

    // Allocate memory for the cluster center.
    m_cluster_centers = new kvs::ValueArray<kvs::Real32> [ m_nclusters ];
    for ( size_t i = 0; i < m_nclusters; i++ ) { m_cluster_centers[i].allocate( ncolumns ); }

    // Assign initial cluster IDs to each row of the input table randomly.
    kvs::ValueArray<kvs::UInt32> IDs( nrows );
    for ( size_t i = 0; i < nrows; i++ ) IDs[i] = kvs::UInt32( m_nclusters * m_random() );

    // Calculate the center of cluster.
    switch ( m_seeding_method )
    {
    case RandomSeeding:
        ::InitializeCentersWithRandomSeeding( m_input_table, m_nclusters, IDs, m_cluster_centers );
        break;
    case SmartSeeding:
        ::InitializeCentersWithSmartSeeding( m_input_table, m_nclusters, IDs, m_cluster_centers );
        break;
    default:
        ::InitializeCentersWithRandomSeeding( m_input_table, m_nclusters, IDs, m_cluster_centers );
        break;
    }

    // Cluster center used for convergence test.
    kvs::ValueArray<kvs::Real32> center_new( ncolumns );

    // Clustering.
    bool converged = false;
    size_t counter = 0;
    while ( !converged )
    {
        // Calculate euclidean distance between the center of cluster and the point, and update the IDs.
        for ( size_t i = 0; i < nrows; i++ )
        {
            size_t id = 0;
            kvs::Real32 distance = kvs::Value<kvs::Real32>::Max();
            for ( size_t j = 0; j < m_nclusters; j++ )
            {
                const kvs::Real32 d = ::GetEuclideanDistance( m_input_table, i, m_cluster_centers[j] );
                if ( d < distance ) { distance = d; id = j; }
            }
            IDs[i] = id;
        }

        // Convergence test.
        converged = true;
        for ( size_t i = 0; i < m_nclusters; i++ )
        {
            ::CalculateCenter( m_input_table, i, IDs, &center_new );

            const kvs::Real32 distance = ::GetEuclideanDistance( m_input_table, m_cluster_centers[i], center_new );
            if ( !( distance < m_tolerance ) )
            {
                converged = false;
                break;
            }
        }

        if ( counter++ > m_max_iterations ) break;

        // Calculate the center of cluster.
        if ( !converged )
        {
            for ( size_t i = 0; i < m_nclusters; i++ )
            {
                ::CalculateCenter( m_input_table, i, IDs, &(m_cluster_centers[i]) );
            }
        }

    } // end of while

    m_cluster_ids = IDs;
}

} // end of namespace kvs
