/****************************************************************************/
/**
 *  @file   GridBase.h
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
/****************************************************************************/
#ifndef KVS__GRID_BASE_H_INCLUDE
#define KVS__GRID_BASE_H_INCLUDE

#include <kvs/Type>
#include <kvs/Vector3>
#include <kvs/StructuredVolumeObject>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  GridBase class.
 */
/*===========================================================================*/
class GridBase
{
private:

    size_t m_nnodes; ///< number of nodes
    size_t m_veclen; ///< vector length
    kvs::Vec3ui m_base_index; ///< base index
    kvs::Real32* m_values; ///< values
    kvs::Real32* m_interpolation_functions;
    kvs::Real32* m_differential_functions;
    kvs::Vec3 m_local_point; ///< local point
    const kvs::StructuredVolumeObject* m_reference_volume;

public:

    GridBase( const kvs::StructuredVolumeObject* volume );
    virtual ~GridBase();

    virtual int findGrid( const kvs::Vec3& global ) const = 0;
    virtual kvs::Vec3 globalToLocal( const kvs::Vec3& global ) const = 0;

    void bind( const kvs::Vec3ui& base_index );
    void bind( const kvs::UInt32 grid_index );
    void bind( const kvs::Vec3& global );
    void setLocalPoint( const kvs::Vec3& local );
    void updateInterpolationFunctions( const kvs::Vec3& local );
    void updateDifferentialFunctions( const kvs::Vec3& local );
    kvs::Real32 scalar() const;
    kvs::Vec3 vector() const;
    kvs::Vec3 gradientVector() const;

    size_t veclen() const { return m_veclen; }
    size_t numberOfCellNodes() const { return m_nnodes; }
    kvs::Real32* interpolationFunctions() const { return m_interpolation_functions; }
    kvs::Real32* differentialFunctions() const { return m_differential_functions; }
    const kvs::Real32* values() const { return m_values; }
    kvs::Real32 value( const size_t index ) const { return m_values[ index ]; }
    const kvs::Vec3ui baseIndex() const { return m_base_index; }
    const kvs::Vec3 localPoint() const { return m_local_point; }
    const kvs::StructuredVolumeObject* referenceVolume() const { return m_reference_volume; }

protected:

    kvs::UInt32 gridIndexOf( const kvs::Vec3ui& base_index ) const;
    kvs::Vec3ui baseIndexOf( const kvs::UInt32 grid_index ) const;
    kvs::Real32 interpolateValue( const kvs::Real32* values, const kvs::Real32* weights ) const;
};

} // end of namespace kvs

#endif // KVS__GRID_BASE_H_INCLUDE
