/*****************************************************************************/
/**
 *  @file   FastKMeans.h
 *  @author Zhang Chi, Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 * References:
 * [1] Greg Hamerly, Making k-means even faster, In proceedings of the 2010 SIAM
 *     international conference on data mining (SDM 2010), April 2010.
 * [2] D. Arthur and S. Vassilvitskii, k-means++ : The Advantages of Careful
 *     Seeding, in Proceedings of the eighteenth annual ACM-SIAM symposium on
 *     Discrete algorithms, 2007, pp. 1027-1035.
 */
/*****************************************************************************/
#pragma once
#include <kvs/MersenneTwister>
#include <kvs/ValueArray>
#include <kvs/AnyValueTable>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Fast K-means clustering class.
 */
/*===========================================================================*/
class FastKMeans
{
public:
    enum SeedingMethod
    {
        RandomSeeding,
        SmartSeeding
    };

private:
    kvs::MersenneTwister m_random{}; ///< random number generator
    SeedingMethod m_seeding_method = SmartSeeding; ///< seeding method
    size_t m_nclusters = 1; ///< number of clusters
    size_t m_max_iterations = 100; ///< maximum number of interations
    float m_tolerance = 1.e-6; ///< tolerance of distance
    kvs::AnyValueTable m_input_table{}; ///< input table data
    kvs::ValueArray<kvs::UInt32> m_cluster_ids{}; ///< cluster IDs
    kvs::ValueArray<kvs::Real32>* m_cluster_centers = nullptr; ///< cluster centers

public:
    FastKMeans() = default;
    virtual ~FastKMeans() { if ( m_cluster_centers ) { delete [] m_cluster_centers; } }

    void setSeedingMethod( SeedingMethod seeding_method ) { m_seeding_method = seeding_method; }
    void setSeed( const size_t seed ) { m_random.setSeed( seed ); }
    void setNumberOfClusters( const size_t nclusters ) { m_nclusters = nclusters; }
    void setMaxIterations( const size_t max_iterations ) { m_max_iterations = max_iterations; }
    void setTolerance( const float tolerance ) { m_tolerance = tolerance; }
    void setInputTableData( const kvs::AnyValueTable& table ) { m_input_table = table; }

    SeedingMethod seedingMethod() const { return m_seeding_method; }
    size_t numberOfClusters() const { return m_nclusters; }
    size_t maxIterations() const { return m_max_iterations; }
    float tolerance() const { return m_tolerance; }

    void run();
    const kvs::ValueArray<kvs::UInt32>& clusterIDs() const { return m_cluster_ids; }
    const kvs::ValueArray<kvs::Real32>& clusterCenter( const size_t index ) const { return m_cluster_centers[ index ]; }
};

} // end of namespace kvs
