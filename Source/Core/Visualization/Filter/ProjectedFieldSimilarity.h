/*****************************************************************************/
/**
 *  @file   ProjectedFieldSimilarity.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  A. Fofonov, L. Linsen, Projected Field Similarity for Comparative Visualization
 *  of Multi-Run Multi-Field Time-Varying Spatial Data, Computer Graphics Forum,
 *  Volume 38, Issue 1, pp.286-299, 2019.
 */
/*****************************************************************************/
#pragma once
#include <kvs/Module>
#include <kvs/FilterBase>
#include <kvs/TableObject>
#include <kvs/Matrix>
#include <kvs/Deprecated>
#include <kvs/StructuredVolumeObjectList>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  ProjectedFieldSimilarity class.
 */
/*===========================================================================*/
class ProjectedFieldSimilarity : public kvs::FilterBase, public kvs::TableObject
{
    kvsModule( kvs::ProjectedFieldSimilarity, Filter );
    kvsModuleBaseClass( kvs::FilterBase );
    kvsModuleSuperClass( kvs::TableObject );

private:
    kvs::Matrix<kvs::Real32> m_dissimilarity_matrix{}; ///< dissimilarity (distance) matrix
    bool m_has_min_max_values = false; ///< a flag whether includes min/max values or not
    kvs::Real32 m_min_value = 0.0f; ///< min. value of a set of volume objects
    kvs::Real32 m_max_value = 0.0f; ///< max. value of a set of volume objects

public:
    ProjectedFieldSimilarity() = default;
    ProjectedFieldSimilarity( const kvs::ObjectBase* object ) { this->exec( object ); }
    ProjectedFieldSimilarity(
        const kvs::ObjectBase* object,
        const kvs::Real32 min_value,
        const kvs::Real32 max_value ):
        m_has_min_max_values( true ),
        m_min_value( min_value ),
        m_max_value( max_value )
    {
        this->exec( object );
    }

    void setMinMaxValues( const kvs::Real32 min_value, const kvs::Real32 max_value )
    {
        m_has_min_max_values = true;
        m_min_value = min_value;
        m_max_value = max_value;
    }

    bool hasMinMaxValues() const { return m_has_min_max_values; }
    kvs::Real32 minValue() const { return m_min_value; }
    kvs::Real32 maxValue() const { return m_max_value; }
    kvs::Matrix<kvs::Real32> dissimilarityMatrix() const { return m_dissimilarity_matrix; }

    SuperClass* exec( const kvs::ObjectBase* object );

public:
    KVS_DEPRECATED( kvs::Matrix<kvs::Real32> similarity() const )
    {
        return this->dissimilarityMatrix();
    }
};

} // end of namespace kvs
