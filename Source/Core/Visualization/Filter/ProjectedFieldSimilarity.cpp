/*****************************************************************************/
/**
 *  @file   ProjectedFieldSimilarity.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  A. Fofonov, L. Linsen, Projected Field Similarity for Comparative Visualization
 *  of Multi-Run Multi-Field Time-Varying Spatial Data, Computer Graphics Forum,
 *  Volume 38, Issue 1, pp.286-299, 2019.
 */
/*****************************************************************************/
#include "ProjectedFieldSimilarity.h"
#include <kvs/MultiDimensionalScaling>
#include <kvs/FieldSimilarity>


namespace
{

inline kvs::Matrix<float> DistanceMatrix(
    const kvs::StructuredVolumeObjectList& volumes,
    const float min_value,
    const float max_value )
{
    const auto nvolumes = volumes.size();
    kvs::Matrix<float> d( nvolumes, nvolumes );
    #pragma omp parallel for
    for ( size_t i = 0; i < nvolumes - 1; ++i )
    {
        #pragma omp parallel for
        for ( size_t j = i + 1; j < nvolumes; ++j )
        {
            auto vi = volumes.load( i );
            auto vj = volumes.load( j );
            d[i][j] = 1.0f - kvs::FieldSimilarity( vi, vj, min_value, max_value );
            d[j][i] = d[i][j];
        }
    }
    return d;
}

inline kvs::Matrix<float> DistanceMatrix(
    const kvs::StructuredVolumeObjectList& volumes )
{
    const auto nvolumes = volumes.size();
    kvs::Matrix<float> d( nvolumes, nvolumes );
    #pragma omp parallel for
    for ( size_t i = 0; i < nvolumes - 1; ++i )
    {
        #pragma omp parallel for
        for ( size_t j = i + 1; j < nvolumes; ++j )
        {
            auto vi = volumes.load( i );
            auto vj = volumes.load( j );
            d[i][j] = 1.0f - kvs::FieldSimilarity( vi, vj );
            d[j][i] = d[i][j];
        }
    }
    return d;
}

}

namespace kvs
{

using ThisClass = ProjectedFieldSimilarity;

ThisClass::SuperClass* ThisClass::exec( const kvs::ObjectBase* object )
{
    if ( !object )
    {
        BaseClass::setSuccess( false );
        kvsMessageError("Input object is NULL.");
        return nullptr;
    }

    using ObjectList = kvs::StructuredVolumeObjectList;
    const auto* object_list = ObjectList::DownCast( object );
    if ( !object_list )
    {
        BaseClass::setSuccess( false );
        kvsMessageError("Input object is not volume a object list.");
        return nullptr;
    }

    m_dissimilarity_matrix = m_has_min_max_values ?
        ::DistanceMatrix( *object_list, m_min_value, m_max_value ):
        ::DistanceMatrix( *object_list );

    kvs::MultiDimensionalScaling<float> mds( 2 );
    SuperClass::setTable( mds.transform( m_dissimilarity_matrix ) );

    return this;
}

} // end of namespace kvs
