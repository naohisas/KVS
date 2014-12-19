/*****************************************************************************/
/**
 *  @file   KMeansClustering.cpp
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
/*****************************************************************************/
#include "KMeansClustering.h"
#include <kvs/KMeans>
#include <kvs/FastKMeans>
#include <kvs/AdaptiveKMeans>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new KMeansClustering class.
 */
/*===========================================================================*/
KMeansClustering::KMeansClustering():
    m_clustering_method( FastKMeans ),
    m_seeding_method( SmartSeeding ),
    m_seed( 0 ),
    m_nclusters( 0 ),
    m_max_iterations( 100 ),
    m_tolerance( 1.e-6 ),
    m_cluster_centers( NULL )
{
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new KMeansClustering class.
 *  @param  table [in] pointer to the table object
 */
/*===========================================================================*/
KMeansClustering::KMeansClustering( const kvs::TableObject* table ):
    m_clustering_method( FastKMeans ),
    m_seeding_method( SmartSeeding ),
    m_seed( 0 ),
    m_nclusters( 0 ),
    m_max_iterations( 100 ),
    m_tolerance( 1.e-6 ),
    m_cluster_centers( NULL )
{
    this->exec( table );
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new KMeansClustering class.
 *  @param  table [in] pointer to the table object
 *  @param  nclusters [in] number of clusters (max. number of clusters for AdaptiveKMeans)
 *  @param  clustering_method [in] clustering method (SimpleKMeans, FastKMeans, or AdaptiveKMeans)
 *  @param  seeding_method [in] seeding method (RandomSeeding or SmartSeeding)
 */
/*===========================================================================*/
KMeansClustering::KMeansClustering(
    const kvs::TableObject* table,
    const size_t nclusters,
    const ClusteringMethod clustering_method,
    const SeedingMethod seeding_method ):
    m_clustering_method( clustering_method ),
    m_seeding_method( seeding_method ),
    m_seed( 0 ),
    m_nclusters( nclusters ),
    m_max_iterations( 100 ),
    m_tolerance( 1.e-6 ),
    m_cluster_centers( NULL )
{
    this->exec( table );
}

/*===========================================================================*/
/**
 *  @brief  Destroys the KMeansClustering class.
 */
/*===========================================================================*/
KMeansClustering::~KMeansClustering()
{
    if ( m_cluster_centers ) delete [] m_cluster_centers;
}

/*===========================================================================*/
/**
 *  @brief  Executes clustering process.
 *  @param  object [in] pointer to the table object
 *  @return pointer to the clustered table object
 */
/*===========================================================================*/
KMeansClustering::SuperClass* KMeansClustering::exec( const kvs::ObjectBase* object )
{
    if ( !object )
    {
        BaseClass::setSuccess( false );
        kvsMessageError("Input object is NULL.");
        return NULL;
    }

    const kvs::TableObject* table = kvs::TableObject::DownCast( object );
    if ( !table )
    {
        BaseClass::setSuccess( false );
        kvsMessageError("Input object is not supported.");
        return NULL;
    }

    if ( m_nclusters == 0 )
    {
        this->adaptive_kmeans( table );
    }
    else
    {
        switch ( m_clustering_method )
        {
        case SimpleKMeans: this->simple_kmeans( table ); break;
        case FastKMeans: this->fast_kmeans( table ); break;
        case AdaptiveKMeans: this->adaptive_kmeans( table ); break;
        default: break;
        }
    }

    return this;
}

/*===========================================================================*/
/**
 *  @brief  Executes simple k-means clustering
 *  @param  object [in] pointer to the table object
 */
/*===========================================================================*/
void KMeansClustering::simple_kmeans( const kvs::TableObject* object )
{
    kvs::KMeans kmeans;
    kmeans.setSeedingMethod( kvs::KMeans::SeedingMethod( m_seeding_method ) );
    kmeans.setSeed( m_seed );
    kmeans.setNumberOfClusters( m_nclusters );
    kmeans.setMaxIterations( m_max_iterations );
    kmeans.setTolerance( m_tolerance );
    kmeans.setInputTableData( object->table() );
    kmeans.run();

    this->setTable( object->table(), object->labels() );
    this->addColumn( kvs::AnyValueArray( kmeans.clusterIDs() ), "cluster ID" );

    if ( m_cluster_centers ) delete [] m_cluster_centers;
    m_cluster_centers = new kvs::ValueArray<kvs::Real32> [ m_nclusters ];
    for ( size_t i = 0; i < m_nclusters; i++ )
    {
        m_cluster_centers[i] = kmeans.clusterCenter(i);
    }
}

/*===========================================================================*/
/**
 *  @brief  Executes fast k-means clustering
 *  @param  object [in] pointer to the table object
 */
/*===========================================================================*/
void KMeansClustering::fast_kmeans( const kvs::TableObject* object )
{
    kvs::FastKMeans kmeans;
    kmeans.setSeedingMethod( kvs::FastKMeans::SeedingMethod( m_seeding_method ) );
    kmeans.setSeed( m_seed );
    kmeans.setNumberOfClusters( m_nclusters );
    kmeans.setMaxIterations( m_max_iterations );
    kmeans.setTolerance( m_tolerance );
    kmeans.setInputTableData( object->table() );
    kmeans.run();

    this->setTable( object->table(), object->labels() );
    this->addColumn( kvs::AnyValueArray( kmeans.clusterIDs() ), "cluster ID" );

    if ( m_cluster_centers ) delete [] m_cluster_centers;
    m_cluster_centers = new kvs::ValueArray<kvs::Real32> [ m_nclusters ];
    for ( size_t i = 0; i < m_nclusters; i++ )
    {
        m_cluster_centers[i] = kmeans.clusterCenter(i);
    }
}

/*===========================================================================*/
/**
 *  @brief  Executes adaptive k-means clustering
 *  @param  object [in] pointer to the table object
 */
/*===========================================================================*/
void KMeansClustering::adaptive_kmeans( const kvs::TableObject* object )
{
    // If 'm_nclusters' is zero, max. number of iterations is set to 100,
    // otherwise 'm_nclusters' is assumed as max. number of iterations.
    const size_t max_nclusters = m_nclusters > 0 ? m_nclusters : 100;

    kvs::AdaptiveKMeans kmeans;
    kmeans.setMaxNumberOfClusters( max_nclusters );
    kmeans.setMaxIterations( m_max_iterations );
    kmeans.setTolerance( m_tolerance );
    kmeans.setInputTableData( object->table() );
    kmeans.run();

    this->setTable( object->table(), object->labels() );
    this->addColumn( kvs::AnyValueArray( kmeans.clusterIDs() ), "cluster ID" );

    // 'm_nclusters' is set to the estimated number of clusters after
    // the clustering process.
    m_nclusters = kmeans.numberOfClusters();

    if ( m_cluster_centers ) delete [] m_cluster_centers;
    m_cluster_centers = new kvs::ValueArray<kvs::Real32> [ m_nclusters ];
    for ( size_t i = 0; i < m_nclusters; i++ )
    {
        m_cluster_centers[i] = kmeans.clusterCenter(i);
    }
}

} // end of namespace kvs
