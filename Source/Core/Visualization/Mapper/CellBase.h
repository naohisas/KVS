/****************************************************************************/
/**
 *  @file   CellBase.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: CellBase.h 1792 2014-07-31 04:50:42Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#ifndef KVS__CELL_BASE_H_INCLUDE
#define KVS__CELL_BASE_H_INCLUDE

#include <kvs/DebugNew>
#include <kvs/Type>
#include <kvs/Vector4>
#include <kvs/Matrix44>
#include <kvs/UnstructuredVolumeObject>
#include <kvs/IgnoreUnusedVariable>
#include <kvs/Message>
#include <cstring>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  CellBase class.
 */
/*===========================================================================*/
class CellBase
{
protected:

    size_t m_nnodes; ///< number of nodes
    kvs::Vec3* m_vertices; ///< coordinates of the nodes
    kvs::Real32* m_scalars; ///< scalar values of the nodes
    kvs::Real32* m_interpolation_functions; ///< interpolation functions
    kvs::Real32* m_differential_functions; ///< differential functions
    mutable kvs::Vec3 m_global_point; ///< sampling point in the global coordinate
    mutable kvs::Vec3 m_local_point;  ///< sampling point in the local coordinate
    const kvs::UnstructuredVolumeObject* m_reference_volume; ///< reference unstructured volume

public:

    CellBase( const kvs::UnstructuredVolumeObject* volume );
    virtual ~CellBase();

    virtual const kvs::Real32* interpolationFunctions( const kvs::Vec3& local ) const = 0;
    virtual const kvs::Real32* differentialFunctions( const kvs::Vec3& local ) const = 0;
    virtual void bindCell( const kvs::UInt32 index );
    virtual void setGlobalPoint( const kvs::Vec3& global ) const;
    virtual void setLocalPoint( const kvs::Vec3& local ) const;
    virtual const kvs::Vec3 transformGlobalToLocal( const kvs::Vec3& global ) const;
    virtual const kvs::Vec3 transformLocalToGlobal( const kvs::Vec3& local ) const;

    virtual const kvs::Vec3 randomSampling() const;
    virtual const kvs::Real32 volume() const;
    virtual const kvs::Real32 averagedScalar() const;
    virtual const kvs::Real32 scalar() const;
    virtual const kvs::Vec3 gradient() const;

    size_t numberOfCellNodes() const { return m_nnodes; }
    kvs::Real32* const interpolationFunctions() const { return m_interpolation_functions; }
    kvs::Real32* const differentialFunctions() const { return m_differential_functions; }
    const kvs::Vec3* vertices() const { return m_vertices; }
    const kvs::Real32* scalars() const { return m_scalars; }
    const kvs::Vec3 globalPoint() const { return m_global_point; }
    const kvs::Vec3 localPoint() const { return m_local_point; }
    const kvs::Mat3 JacobiMatrix() const;
    const kvs::Real32 randomNumber() const;
};

} // end of namespace kvs

#endif // KVS__CELL_BASE_H_INCLUDE
