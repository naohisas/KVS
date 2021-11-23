/*****************************************************************************/
/**
 *  @file   AdaptiveKMeans.h
 *  @author Zhang Chi, Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 * References:
 * [1] C. Sugar and J. Gareth, Finding the number of clusters in a data set :
 *     An information theoretic approach, Journal of the American Statistical
 *     Association, vol. 98, pp. 750-763, 2003.
 */
/*****************************************************************************/
#pragma once
#include <kvs/ValueArray>
#include <kvs/AnyValueTable>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Adaptive k-means clustering class.
 */
/*===========================================================================*/
class AdaptiveKMeans
{
private:
    size_t m_nclusters = 0; ///< number of clusters
    size_t m_max_iterations = 100; ///< maximum number of interations
    float m_tolerance = 1.e-6; ///< tolerance of distance
    size_t m_max_nclusters = 10; ///< maximum number of clusters for finding the best k
    kvs::AnyValueTable m_input_table{}; ///< input table data
    kvs::ValueArray<kvs::UInt32> m_cluster_ids{}; ///< cluster IDs
    kvs::ValueArray<kvs::Real32>* m_cluster_centers = nullptr; ///< cluster centers
    kvs::ValueArray<kvs::Real32> m_distortions{}; ///< distortions for finding the best k

public:
    AdaptiveKMeans() = default;
    virtual ~AdaptiveKMeans() { if ( m_cluster_centers ) { delete [] m_cluster_centers; } }

    void setMaxNumberOfClusters( const size_t max_nclusters ) { m_max_nclusters = max_nclusters; }
    void setMaxIterations( const size_t max_iterations ) { m_max_iterations = max_iterations; }
    void setTolerance( const float tolerance ) { m_tolerance = tolerance; }
    void setInputTableData( const kvs::AnyValueTable& table ) { m_input_table = table; }

    size_t numberOfClusters() const { return m_nclusters; }
    size_t maxNumberOfClusters() const { return m_max_nclusters; }
    size_t maxIterations() const { return m_max_iterations; }
    float tolerance() const { return m_tolerance; }

    void run();
    const kvs::ValueArray<kvs::UInt32>& clusterIDs() const { return m_cluster_ids; }
    const kvs::ValueArray<kvs::Real32>& clusterCenter( const size_t index ) const { return m_cluster_centers[ index ]; }
    const kvs::ValueArray<kvs::Real32>& distortions() const { return m_distortions; }
    const kvs::ValueArray<kvs::Real32> jumps() const;
};

} // end of namespace kvs
