/*****************************************************************************/
/**
 *  @file   BasicCubicCell.h
 *  @author Naoya Maeda
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: BasicCubicCell.h 1773 2014-05-14 08:11:38Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#pragma once
#include "BasicCellCommon.h"


namespace kvs
{

namespace cell
{

template <typename T>
class BasicCubicCell : 
    public detail::BasicCellImplementBase<T>, 
    public detail::HexahedricCellFunction<T>, 
    public detail::MixInCellFunction<BasicCubicCell<T>, T>
{
    friend struct detail::CellGenerator<BasicCubicCell<T> >;

public:
    enum { NumberOfNodes = 8 };
    typedef T ValueType;

private:
    typedef void (*BinderFunction)( BasicCubicCell<T>*, const kvs::Vec3i& );
    BinderFunction m_binder;

    const void* m_volume_values;
    kvs::Vector3ui m_volume_resolution;
    kvs::Vector3<T> m_local_point;

private:
    // used by CellGenerator
    explicit BasicCubicCell( const kvs::VolumeObjectBase* volume ) :
        detail::BasicCellImplementBase<T>( volume )
    {
        KVS_ASSERT( volume->volumeType() == kvs::VolumeObjectBase::Structured );
        KVS_ASSERT( kvs::StructuredVolumeObject::DownCast( volume )->gridType() == kvs::StructuredVolumeObject::Uniform );
        const kvs::StructuredVolumeObject* str_volume = kvs::StructuredVolumeObject::DownCast( volume );

        m_volume_values = str_volume->values().data();
        m_volume_resolution = str_volume->resolution();
        m_binder = NULL;

        if ( str_volume->veclen() == 1 )
        {
            switch ( str_volume->values().typeID() )
            {
            case kvs::Type::TypeInt8:   m_binder = bind_cubic_scalar_cell<kvs::Int8  >; break;
            case kvs::Type::TypeInt16:  m_binder = bind_cubic_scalar_cell<kvs::Int16 >; break;
            case kvs::Type::TypeInt32:  m_binder = bind_cubic_scalar_cell<kvs::Int32 >; break;
            case kvs::Type::TypeInt64:  m_binder = bind_cubic_scalar_cell<kvs::Int64 >; break;
            case kvs::Type::TypeUInt8:  m_binder = bind_cubic_scalar_cell<kvs::UInt8 >; break;
            case kvs::Type::TypeUInt16: m_binder = bind_cubic_scalar_cell<kvs::UInt16>; break;
            case kvs::Type::TypeUInt32: m_binder = bind_cubic_scalar_cell<kvs::UInt32>; break;
            case kvs::Type::TypeUInt64: m_binder = bind_cubic_scalar_cell<kvs::UInt64>; break;
            case kvs::Type::TypeReal32: m_binder = bind_cubic_scalar_cell<kvs::Real32>; break;
            case kvs::Type::TypeReal64: m_binder = bind_cubic_scalar_cell<kvs::Real64>; break;
            default:
                KVS_ASSERT( false );
                break;
            }
        }
        else
        {
            switch ( str_volume->values().typeID() )
            {
            case kvs::Type::TypeInt8:   m_binder = bind_cubic_cell<kvs::Int8  >; break;
            case kvs::Type::TypeInt16:  m_binder = bind_cubic_cell<kvs::Int16 >; break;
            case kvs::Type::TypeInt32:  m_binder = bind_cubic_cell<kvs::Int32 >; break;
            case kvs::Type::TypeInt64:  m_binder = bind_cubic_cell<kvs::Int64 >; break;
            case kvs::Type::TypeUInt8:  m_binder = bind_cubic_cell<kvs::UInt8 >; break;
            case kvs::Type::TypeUInt16: m_binder = bind_cubic_cell<kvs::UInt16>; break;
            case kvs::Type::TypeUInt32: m_binder = bind_cubic_cell<kvs::UInt32>; break;
            case kvs::Type::TypeUInt64: m_binder = bind_cubic_cell<kvs::UInt64>; break;
            case kvs::Type::TypeReal32: m_binder = bind_cubic_cell<kvs::Real32>; break;
            case kvs::Type::TypeReal64: m_binder = bind_cubic_cell<kvs::Real64>; break;
            default:
                KVS_ASSERT( false );
                break;
            }
        }
    }

public:
    void bindCell( kvs::UInt32 index )
    {
        m_binder( this, this->index_to_index3( index ) );
    }

    void bindCellByGlobalPoint( const kvs::Vector3<T>& global )
    {
        KVS_ASSERT( global.x() >= 0 );
        KVS_ASSERT( global.y() >= 0 );
        KVS_ASSERT( global.z() >= 0 );
        KVS_ASSERT( global.x() < m_volume_resolution.x() - 1 );
        KVS_ASSERT( global.y() < m_volume_resolution.y() - 1 );
        KVS_ASSERT( global.z() < m_volume_resolution.z() - 1 );
        m_binder( this, to_vec<int>( global ) );
    }

public:
    void updateInterpolationFactors( const kvs::Vector3<T>& local )
    {
        KVS_ASSERT( this->includesLocal( local ) );
        m_local_point = local;

        const T x = local.x();
        const T y = local.y();
        const T z = local.z();
        T* N = this->interpolation_factors();

        N[0] = (1-x)*(1-y)*   z ;
        N[1] =    x *(1-y)*   z ;
        N[2] =    x *   y *   z ;
        N[3] = (1-x)*   y *   z ;
        N[4] = (1-x)*(1-y)*(1-z);
        N[5] =    x *(1-y)*(1-z);
        N[6] =    x *   y *(1-z);
        N[7] = (1-x)*   y *(1-z);
    }

    void updateDifferentialFactors( const kvs::Vector3<T>& local )
    {
        KVS_ASSERT( this->includesLocal( local ) );

        const T x = local.x();
        const T y = local.y();
        const T z = local.z();
        const T xy = x * y;
        const T yz = y * z;
        const T zx = z * x;

        const int nnodes = NumberOfNodes;
        T* dNdx = this->differential_factors();
        T* dNdy = dNdx + nnodes;
        T* dNdz = dNdy + nnodes;

        // dNdx
        dNdx[0] = - z + yz;
        dNdx[1] = z - yz;
        dNdx[2] = yz;
        dNdx[3] = - yz;
        dNdx[4] = - 1 + y + z - yz;
        dNdx[5] = 1 - y - z + yz;
        dNdx[6] = y - yz;
        dNdx[7] = - y + yz;

        // dNdy
        dNdy[0] = - z + zx;
        dNdy[1] = - zx;
        dNdy[2] = zx;
        dNdy[3] = z - zx;
        dNdy[4] = - 1 + x + z - zx;
        dNdy[5] = - x + zx;
        dNdy[6] = x - zx;
        dNdy[7] = 1 - x - z + zx;

        // dNdz
        dNdz[0] = 1 - y - x + xy;
        dNdz[1] = x - xy;
        dNdz[2] = xy;
        dNdz[3] = y - xy;
        dNdz[4] = - 1 + y + x - xy;
        dNdz[5] = - x + xy;
        dNdz[6] = - xy;
        dNdz[7] = - y + xy;
    }

    static T volume()
    {
        return 1;
    }

public:
    // hiding MixInCellFunction.globalToLocal
    const kvs::Vector3<T> globalToLocal( const kvs::Vector3<T>& global )
    {
        T* coords = this->coords();
        return global - kvs::Vector3<T>( coords[0], coords[1], coords[2] );
    }

public:
    const kvs::Vector3<T> globalPoint()
    {
        T* coords = this->coords();
        return kvs::Vector3<T>( coords[0], coords[1], coords[2] ) + m_local_point;
    }

    const kvs::Vector3<T> gradient()
    {
        KVS_ASSERT( this->valueDimension() == 1 );

        const int nnodes = NumberOfNodes;
        const T* dNdx = this->differential_factors();
        const T* dNdy = dNdx + nnodes;
        const T* dNdz = dNdy + nnodes;
        const T* s = this->values();

        const T dsdx = this->interpolate_value( s, dNdx, nnodes );
        const T dsdy = this->interpolate_value( s, dNdy, nnodes );
        const T dsdz = this->interpolate_value( s, dNdz, nnodes );
        return Vector3<T>( dsdx, dsdy, dsdz );
    }

    static const kvs::Matrix33<T> jacobianMatrix()
    {
        return kvs::Matrix33<T>::Identity();
    }

private:
    template <typename U>
    static kvs::Vector3<U> to_vec( const kvs::Vector3<T>& v )
    {
        return kvs::Vector3<U>( static_cast<U>( v.x() ), static_cast<U>( v.y() ), static_cast<U>( v.z() ) );
    }

    kvs::Vec3i index_to_index3( kvs::UInt32 index )
    {
        const int nx = this->m_volume_resolution.x();
        const int ny = this->m_volume_resolution.y();
        const int nx1 = nx - 1;
        const int nx1_ny1 = nx1 * ( ny - 1 );
        const int ix = index % nx1_ny1 % nx1;
        const int iy = index % nx1_ny1 / nx1;
        const int iz = index / nx1_ny1;
        return kvs::Vec3i( ix, iy, iz );
    }

    void setup_bind( const kvs::Vec3i& index, kvs::UInt32* data_index )
    {
        T* cell_coords = this->coords();

        // stores global point of (0, 0, 0) (local)
        cell_coords[0] = static_cast<T>( index.x() );
        cell_coords[1] = static_cast<T>( index.y() );
        cell_coords[2] = static_cast<T>( index.z() );

        const int nx = m_volume_resolution.x();
        const int ny = m_volume_resolution.y();
        const int nx_ny = nx * ny;
        const int base_index = index.x() + index.y() * nx + index.z() * nx_ny;

        data_index[0] = base_index          + nx_ny;
        data_index[1] = base_index + 1      + nx_ny;
        data_index[2] = base_index + 1 + nx + nx_ny;
        data_index[3] = base_index     + nx + nx_ny;
        data_index[4] = base_index;
        data_index[5] = base_index + 1;
        data_index[6] = base_index + 1 + nx;
        data_index[7] = base_index     + nx;
    }

    template <typename U>
    static void bind_cubic_scalar_cell( BasicCubicCell<T>* cell, const kvs::Vec3i& index )
    {
        KVS_ASSERT( cell->valueDimension() == 1 );

        kvs::UInt32 data_index[ 8 ];
        cell->setup_bind( index, data_index );

        const int nnodes = NumberOfNodes;
        const void* values = cell->m_volume_values;
        T* cell_values = cell->values();

        for ( int i = 0; i < nnodes; ++i )
        {
            const U* in_values = static_cast<const U*>( values ) + data_index[i];
            cell_values[ i ] = static_cast<T>( *in_values );
        }
    }

    template <typename U>
    static void bind_cubic_cell( BasicCubicCell<T>* cell, const kvs::Vec3i& index )
    {
        kvs::UInt32 data_index[ 8 ];
        cell->setup_bind( index, data_index );

        const int dim = cell->valueDimension();
        const int nnodes = NumberOfNodes;
        const void* values = cell->m_volume_values;
        T* cell_values = cell->values();

        for ( int i = 0; i < nnodes; ++i )
        {
            const U* in_values = static_cast<const U*>( values ) + data_index[i] * dim;
            for ( int d = 0; d < dim; ++d )
            {
                cell_values[ i + nnodes * d ] = static_cast<T>( *(in_values++) );
            }
        }
    }
};

} // end of namespace cell

} // end of namespace kvs
