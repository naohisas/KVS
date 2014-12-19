/*****************************************************************************/
/**
 *  @file   KMeansClustering.h
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
#ifndef KVS__K_MEANS_CLUSTERING_H_INCLUDE
#define KVS__K_MEANS_CLUSTERING_H_INCLUDE

#include <kvs/FilterBase>
#include <kvs/TableObject>
#include <kvs/ValueArray>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  k-means clustering class.
 */
/*===========================================================================*/
class KMeansClustering : public kvs::FilterBase, public kvs::TableObject
{
    kvsModule( kvs::KMeansClustering, Filter );
    kvsModuleBaseClass( kvs::FilterBase );
    kvsModuleSuperClass( kvs::TableObject );

public:

    enum ClusteringMethod
    {
        SimpleKMeans,
        FastKMeans,
        AdaptiveKMeans
    };

    enum SeedingMethod
    {
        RandomSeeding,
        SmartSeeding
    };

private:

    ClusteringMethod m_clustering_method; ///< clustering method
    SeedingMethod m_seeding_method; ///< seeding method
    size_t m_seed; ///< seed point
    size_t m_nclusters; ///< number of clusters
    size_t m_max_iterations; ///< maximum number of interations
    float m_tolerance; ///< tolerance of distance
    kvs::ValueArray<kvs::Real32>* m_cluster_centers; ///< cluster centers

public:

    KMeansClustering();
    KMeansClustering( const kvs::TableObject* table );
    KMeansClustering( const kvs::TableObject* table,
                      const size_t nclusters,
                      const ClusteringMethod clustering_method = FastKMeans,
                      const SeedingMethod seeding_method = SmartSeeding );
    virtual ~KMeansClustering();

    SuperClass* exec( const kvs::ObjectBase* object );

    void setClusteringMethod( const ClusteringMethod method ) { m_clustering_method = method; }
    void setSeedingMethod( const SeedingMethod method ) { m_seeding_method = method; }
    void setSeed( const size_t seed ) { m_seed = seed; }
    void setNumberOfClusters( const size_t nclusters ) { m_nclusters = nclusters; }
    void setMaxInterations( const size_t max_iterations ) { m_max_iterations = max_iterations; }
    void setTolerance( const float tolerance ) { m_tolerance = tolerance; }

    const kvs::ValueArray<kvs::Real32>& clusterCenter( const size_t index ) { return m_cluster_centers[index]; }

private:

    void simple_kmeans( const kvs::TableObject* object );
    void fast_kmeans( const kvs::TableObject* object );
    void adaptive_kmeans( const kvs::TableObject* object );
};

} // end of namespace kvs

#endif // KVS__K_MEANS_CLUSTERING_H_INCLUDE
