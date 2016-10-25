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
#include <kvs/Xorshift128>
#include <kvs/UnstructuredVolumeObject>
#include <kvs/IgnoreUnusedVariable>
#include <kvs/Message>
#include <kvs/Deprecated>
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
private:
    size_t m_nnodes; ///< number of nodes
    size_t m_veclen; ///< vector length
    kvs::Vec3* m_coords; ///< coordinates of the nodes
    kvs::Real32* m_values; ///< scalar values of the nodes
    kvs::Real32* m_interpolation_functions; ///< interpolation functions
    kvs::Real32* m_differential_functions; ///< differential functions
    mutable kvs::Vec3 m_local_point;  ///< sampling point in the local coordinate
    const kvs::UnstructuredVolumeObject* m_reference_volume; ///< reference unstructured volume
    mutable kvs::Xorshift128 m_rand; ///< random number generator

public:

    CellBase( const kvs::UnstructuredVolumeObject* volume );
    virtual ~CellBase();

    virtual void updateInterpolationFunctions( const kvs::Vec3& local ) const = 0;
    virtual void updateDifferentialFunctions( const kvs::Vec3& local ) const = 0;
    virtual void bindCell( const kvs::UInt32 index );
    virtual void setLocalPoint( const kvs::Vec3& local ) const;
    virtual bool containsLocalPoint( const kvs::Vec3& local ) const;
    virtual kvs::Vec3 globalPoint() const;
    virtual kvs::Vec3 globalToLocal( const kvs::Vec3& global ) const;
    virtual kvs::Vec3 localToGlobal( const kvs::Vec3& local ) const;
    virtual kvs::Vec3 randomSampling() const;
    virtual kvs::Real32 volume() const;
    virtual kvs::Vec3 localCenter() const;

    void setSeed( const kvs::UInt32 seed ) { m_rand.setSeed( seed ); }
    size_t veclen() const { return m_veclen; }
    size_t numberOfCellNodes() const { return m_nnodes; }
    kvs::Real32* interpolationFunctions() const { return m_interpolation_functions; }
    kvs::Real32* differentialFunctions() const { return m_differential_functions; }
    const kvs::Vec3* coords() const { return m_coords; }
    const kvs::Real32* values() const { return m_values; }
    const kvs::Vec3& coord( const size_t index ) const { return m_coords[ index ]; }
    kvs::Real32 value( const size_t index ) const { return m_values[ index ]; }
    kvs::Vec3 localPoint() const { return m_local_point; }
    const kvs::UnstructuredVolumeObject* referenceVolume() const { return m_reference_volume; }
    kvs::Mat3 JacobiMatrix() const;
    kvs::Vec3 center() const;
    kvs::Real32 scalar() const;
    kvs::Vec3 vector() const;
    kvs::Vec3 gradientVector() const;
    kvs::Mat3 gradientTensor() const;
    bool contains( const kvs::Vec3& global ) const;

protected:

    kvs::Real32 randomNumber() const;
    bool containsInBounds( const kvs::Vec3& global ) const;
    kvs::Real32 interpolateValue( const kvs::Real32* values, const kvs::Real32* weights, const size_t nnodes ) const;
    kvs::Vec3 interpolateCoord( const kvs::Vec3* coords, const kvs::Real32* weights, const size_t nnodes ) const;

public:
    KVS_DEPRECATED( const kvs::Vec3* vertices() const ) { return this->coords(); }
    KVS_DEPRECATED( const kvs::Real32* scalars() const ) { return this->values(); }
    KVS_DEPRECATED( const kvs::Vec3 gradient() const ) { return this->gradientVector(); }
    KVS_DEPRECATED( virtual const kvs::Real32* interpolationFunctions( const kvs::Vec3& local ) const )
    {
        this->updateInterpolationFunctions( local );
        return this->interpolationFunctions();
    }
    KVS_DEPRECATED( virtual const kvs::Real32* differentialFunctions( const kvs::Vec3& local ) const )
    {
        this->updateDifferentialFunctions( local );
        return this->differentialFunctions();
    }
    KVS_DEPRECATED( virtual const kvs::Vec3 transformGlobalToLocal( const kvs::Vec3& global ) const )
    {
        return this->globalToLocal( global );
    }
    KVS_DEPRECATED( virtual const kvs::Vec3 transformLocalToGlobal( const kvs::Vec3& local ) const )
    {
        return this->localToGlobal( local );
    }
    KVS_DEPRECATED( kvs::Real32 averagedScalar() const )
    {
        const size_t nnodes = this->numberOfCellNodes();;
        const kvs::Real32* S = this->values();
        kvs::Real32 Sa = 0; for ( size_t i = 0; i < nnodes; i++ ) { Sa += S[i]; }
        return Sa / nnodes;
    }
};

} // end of namespace kvs

#endif // KVS__CELL_BASE_H_INCLUDE
