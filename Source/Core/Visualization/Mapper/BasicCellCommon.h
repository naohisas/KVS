/*****************************************************************************/
/**
 *  @file   BasicCellCommon.h
 *  @author Naoya Maeda
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: BasicCellCommon.h 1767 2014-05-12 06:24:08Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#pragma once
#include <kvs/Type>
#include <kvs/Vector3>
#include <kvs/Matrix33>
#include <kvs/StructuredVolumeObject>
#include <kvs/UnstructuredVolumeObject>


namespace kvs
{

namespace cell
{

template <typename T>
class BasicTetrahedralCell;
template <typename T>
class BasicHexahedralCell;
template <typename T>
class BasicQuadraticTetrahedralCell;
template <typename T>
class BasicQuadraticHexahedralCell;
template <typename T>
class BasicPyramidalCell;
template <typename T>
class BasicCubicCell;


namespace detail
{

template <typename T>
inline bool InUnitCube( const kvs::Vector3<T>& v )
{
    return v.x() >= 0 && v.x() <= 1 && v.y() >= 0 && v.y() <= 1 && v.z() >= 0 && v.z() <= 1;
}

template <typename T>
struct TetrahedricCellFunction
{
    static const kvs::Vector3<T> centerOfLocal()
    {
        return kvs::Vector3<T>( 0.25, 0.25, 0.25 );
    }

    static void adjustPoint( kvs::Vector3<T>& point )
    {
        KVS_ASSERT( InUnitCube( point ) );
        T x = point.x();
        T y = point.y();
        T z = point.z();

        if ( x + y > 1 )
        {
            x = 1 - x;
            y = 1 - y;
        }
        if ( y + z > 1 )
        {
            T tmp = z;
            z = 1 - x - y;
            y = 1 - tmp;
        }
        else if ( x + y + z > 1 )
        {
            T tmp = z;
            z = x + y + z - 1;
            x = 1 - y - tmp;
        }
        point.set( x, y, z );
    }

    static bool includesLocal( const kvs::Vector3<T>& local )
    {
        const T x = local.x();
        const T y = local.y();
        const T z = local.z();
        const T w = 1 - x - y - z;
        return x >= 0 && y >= 0 && z >= 0 && w >= 0;
    }
};

template <typename T>
struct HexahedricCellFunction
{
    static const kvs::Vector3<T> centerOfLocal()
    {
        return kvs::Vector3<T>( 0.5, 0.5, 0.5 );
    }

    static void adjustPoint( kvs::Vector3<T>& point )
    {
        KVS_ASSERT( InUnitCube( point ) );
    }

    static bool includesLocal( const kvs::Vector3<T>& local )
    {
        return InUnitCube( local );
    }
};

template <typename CellType, typename T>
struct MixInCellFunction
{
public:
    const kvs::Vector3<T> globalToLocal( const kvs::Vector3<T>& global )
    {
        CellType* cell = static_cast<CellType*>( this );
        const kvs::Vector3<T> g = global;

        // Calculate the coordinate of 'global' in the local coordinate
        // by using Newton-Raphson method.
        const double TinyValue = 1.0e-6;
        const int MaxLoop = 100;
        kvs::Vector3<T> x = cell->centerOfLocal();
        for ( int i = 0; i < MaxLoop; i++ )
        {
            cell->setLocalPoint( x );
            const kvs::Vector3<T> X = cell->globalPoint();
            const kvs::Vector3<T> dX = g - X;
            const kvs::Vector3<T> dx = cell->jacobianMatrix().inverted() * dX;
            if ( dx.length() < TinyValue ) break; // Converged.
            x += dx;
        }
        return x;
    }

    void setLocalPoint( const kvs::Vector3<T>& local )
    {
        CellType* cell = static_cast<CellType*>( this );
        cell->updateInterpolationFactors( local );
        cell->updateDifferentialFactors( local );
    }

    bool includes( const kvs::Vector3<T>& global )
    {
        CellType* cell = static_cast<CellType*>( this );
        return cell->includesLocal( cell->globalToLocal( global ) );
    }
};

template <typename CellType>
struct CellGenerator;

template <typename T>
class BasicCellImplementBase
{
    friend struct CellGenerator<BasicTetrahedralCell<T> >;
    friend struct CellGenerator<BasicHexahedralCell<T> >;
    friend struct CellGenerator<BasicQuadraticTetrahedralCell<T> >;
    friend struct CellGenerator<BasicQuadraticHexahedralCell<T> >;
    friend struct CellGenerator<BasicPyramidalCell<T> >;
    friend struct CellGenerator<BasicCubicCell<T> >;

private:
    int m_nnodes;
    int m_value_dimension;

    // initialized by CellGenerator
    T* m_coords;
    T* m_values;
    T* m_interpolation_factors;
    T* m_differential_factors;

protected:
    explicit BasicCellImplementBase( const kvs::VolumeObjectBase* object )
    {
        m_nnodes = 8; // in the case of structured volume object
        if ( object->volumeType() == kvs::VolumeObjectBase::Unstructured )
        {
            typedef kvs::UnstructuredVolumeObject Volume;
            const Volume* volume = Volume::DownCast( object );
//            m_nnodes = (int)volume->cellType(); // ok?
            m_nnodes = volume->numberOfCellNodes();
        }
        m_value_dimension = object->veclen();
    }

public:
    int numberOfNodes() const
    {
        return m_nnodes;
    }

    int valueDimension() const
    {
        return m_value_dimension;
    }

    T value( int index ) const
    {
        KVS_ASSERT( index >= 0 );
        KVS_ASSERT( index < m_value_dimension );
        return this->interpolate_value( m_values + m_nnodes * index, m_interpolation_factors, m_nnodes );
    }

    T scalarValue() const
    {
        KVS_ASSERT( m_value_dimension == 1 );
        return this->value( 0 );
    }

    void vectorValue( T* output ) const
    {
        for ( int i = 0; i < m_value_dimension; ++i )
        {
            *(output++) = this->value( i );
        }
    }

    T nodeValue( int node_index, int value_index ) const
    {
        KVS_ASSERT( value_index >= 0 );
        KVS_ASSERT( value_index < m_value_dimension );
        KVS_ASSERT( node_index >= 0 );
        KVS_ASSERT( node_index < m_nnodes );
        return m_values[ node_index * m_value_dimension + value_index ];
    }

    T nodeScalarValue( int node_index ) const
    {
        return this->nodeValue( node_index, 0 );
    }

    void nodeVectorValue( int node_index, T* output ) const
    {
        for ( int i = 0; i < m_value_dimension; ++i )
        {
            *(output++) = this->nodeValue( node_index, i );
        }
    }

protected:
    T* coords()
    {
        return m_coords;
    }

    T* values()
    {
        return m_values;
    }

    T* interpolation_factors()
    {
        return m_interpolation_factors;
    }

    T* differential_factors()
    {
        return m_differential_factors;
    }

    static T interpolate_value( const T* values, const T* weights, const int nnodes )
    {
        T value = 0;
        for ( int i = 0; i < nnodes; ++i )
        {
            value += *values * *weights;
            ++values; ++weights;
        }
        return value;
    }

    static kvs::Vector3<T> interpolate_coord( const T* coords, const T* weights, const int nnodes )
    {
        kvs::Vector3<T> coord = kvs::Vector3<T>::Zero();
        for ( int i = 0; i < nnodes; ++i )
        {
            coord += kvs::Vector3<T>( coords ) * *weights;
            ++weights; coords += 3;
        }
        return coord;
    }
};

template <typename T>
class BasicUnstructuredCellImplementBase : public BasicCellImplementBase<T>
{
private:
    typedef void (*BinderFunction)( BasicUnstructuredCellImplementBase<T>*, kvs::UInt32 );
    BinderFunction m_binder;

    const float* m_volume_coords;
    const void* m_volume_values;
    const kvs::UInt32* m_volume_connections;

protected:
    BasicUnstructuredCellImplementBase( const kvs::VolumeObjectBase* volume ) :
        BasicCellImplementBase<T>( volume )
    {
        KVS_ASSERT( volume->volumeType() == kvs::VolumeObjectBase::Unstructured );
        const kvs::UnstructuredVolumeObject* uns_volume = static_cast<const kvs::UnstructuredVolumeObject*>( volume );
        m_volume_coords = uns_volume->coords().data();
        m_volume_values = uns_volume->values().data();
        m_volume_connections = uns_volume->connections().data();
        m_binder = NULL;

        if ( uns_volume->veclen() == 1 )
        {
            switch ( volume->values().typeID() )
            {
            case kvs::Type::TypeInt8:   m_binder = bind_unstructured_scalar_cell<kvs::Int8  >; break;
            case kvs::Type::TypeInt16:  m_binder = bind_unstructured_scalar_cell<kvs::Int16 >; break;
            case kvs::Type::TypeInt32:  m_binder = bind_unstructured_scalar_cell<kvs::Int32 >; break;
            case kvs::Type::TypeInt64:  m_binder = bind_unstructured_scalar_cell<kvs::Int64 >; break;
            case kvs::Type::TypeUInt8:  m_binder = bind_unstructured_scalar_cell<kvs::UInt8 >; break;
            case kvs::Type::TypeUInt16: m_binder = bind_unstructured_scalar_cell<kvs::UInt16>; break;
            case kvs::Type::TypeUInt32: m_binder = bind_unstructured_scalar_cell<kvs::UInt32>; break;
            case kvs::Type::TypeUInt64: m_binder = bind_unstructured_scalar_cell<kvs::UInt64>; break;
            case kvs::Type::TypeReal32: m_binder = bind_unstructured_scalar_cell<kvs::Real32>; break;
            case kvs::Type::TypeReal64: m_binder = bind_unstructured_scalar_cell<kvs::Real64>; break;
            default:
                KVS_ASSERT( false );
                break;
            }
        }
        else
        {
            switch ( uns_volume->values().typeID() )
            {
            case kvs::Type::TypeInt8:   m_binder = bind_unstructured_cell<kvs::Int8  >; break;
            case kvs::Type::TypeInt16:  m_binder = bind_unstructured_cell<kvs::Int16 >; break;
            case kvs::Type::TypeInt32:  m_binder = bind_unstructured_cell<kvs::Int32 >; break;
            case kvs::Type::TypeInt64:  m_binder = bind_unstructured_cell<kvs::Int64 >; break;
            case kvs::Type::TypeUInt8:  m_binder = bind_unstructured_cell<kvs::UInt8 >; break;
            case kvs::Type::TypeUInt16: m_binder = bind_unstructured_cell<kvs::UInt16>; break;
            case kvs::Type::TypeUInt32: m_binder = bind_unstructured_cell<kvs::UInt32>; break;
            case kvs::Type::TypeUInt64: m_binder = bind_unstructured_cell<kvs::UInt64>; break;
            case kvs::Type::TypeReal32: m_binder = bind_unstructured_cell<kvs::Real32>; break;
            case kvs::Type::TypeReal64: m_binder = bind_unstructured_cell<kvs::Real64>; break;
            default:
                KVS_ASSERT( false );
                break;
            }
        }
    }

public:
    void bindCell( kvs::UInt32 index )
    {
        m_binder( this, index );
    }

    const kvs::Vector3<T> globalPoint()
    {
        const int nnodes = this->numberOfNodes();
        const T* N = this->interpolation_factors();
        return this->interpolate_coord( this->coords(), N, nnodes );
    }

    const kvs::Vector3<T> gradient()
    {
        KVS_ASSERT( this->valueDimension() == 1 );

        // Calculate a gradient vector in the local coordinate.
        const int nnodes = this->numberOfNodes();
        const T* dNdx = this->differential_factors();
        const T* dNdy = dNdx + nnodes;
        const T* dNdz = dNdy + nnodes;
        const T* s = this->values();

        const T dsdx = this->interpolate_value( s, dNdx, nnodes );
        const T dsdy = this->interpolate_value( s, dNdy, nnodes );
        const T dsdz = this->interpolate_value( s, dNdz, nnodes );
        const Vector3<T> g( dsdx, dsdy, dsdz );

        // Calculate a gradient vector in the global coordinate.
        T determinant = 0;
        const Matrix33<T> J_inv = this->jacobianMatrix().inverted( &determinant );
        return kvs::Math::IsZero( determinant ) ? kvs::Vector3<T>::Zero() : J_inv * g;
    }

    const kvs::Matrix33<T> jacobianMatrix()
    {
        const int nnodes = this->numberOfNodes();
        const T* dNdx = this->differential_factors();
        const T* dNdy = dNdx + nnodes;
        const T* dNdz = dNdy + nnodes;

        const kvs::Vector3<T> dx = this->interpolate_coord( this->coords(), dNdx, nnodes );
        const kvs::Vector3<T> dy = this->interpolate_coord( this->coords(), dNdy, nnodes );
        const kvs::Vector3<T> dz = this->interpolate_coord( this->coords(), dNdz, nnodes );
        return Matrix33<T>( dx[0], dy[0], dz[0], 
                            dx[1], dy[1], dz[1], 
                            dx[2], dy[2], dz[2] );
    }

private:
    template <typename U>
    static void bind_unstructured_cell( BasicUnstructuredCellImplementBase<T>* cell, kvs::UInt32 index )
    {
        const int nnodes = cell->numberOfNodes();
        const int dim = cell->valueDimension();
        const kvs::UInt32* connections = cell->m_volume_connections + index * nnodes;
        const float* coords = cell->m_volume_coords;
        const void* values = cell->m_volume_values;
        T* cell_coords = cell->coords();
        T* cell_values = cell->values();

        for ( int i = 0; i < nnodes; ++i )
        {
            const kvs::UInt32 node_index = *(connections++);

            cell_coords[0] = coords[ 3 * node_index + 0 ];
            cell_coords[1] = coords[ 3 * node_index + 1 ];
            cell_coords[2] = coords[ 3 * node_index + 2 ];
            cell_coords += 3;

            const U* in_values = static_cast<const U*>( values ) + node_index * dim;
            for ( int d = 0; d < dim; ++d )
            {
                cell_values[ nnodes * d ] = static_cast<T>( *(in_values++) );
            }
            ++cell_values;
        }
    }

    template <typename U>
    static void bind_unstructured_scalar_cell( BasicUnstructuredCellImplementBase<T>* cell, kvs::UInt32 index )
    {
        KVS_ASSERT( cell->valueDimension() == 1 );

        const int nnodes = cell->numberOfNodes();
        const kvs::UInt32* connections = cell->m_volume_connections + index * nnodes;
        const float* coords = cell->m_volume_coords;
        const void* values = cell->m_volume_values;
        T* cell_coords = cell->coords();
        T* cell_values = cell->values();

        for ( int i = 0; i < nnodes; ++i )
        {
            const kvs::UInt32 node_index = *(connections++);

            cell_coords[0] = coords[ 3 * node_index + 0 ];
            cell_coords[1] = coords[ 3 * node_index + 1 ];
            cell_coords[2] = coords[ 3 * node_index + 2 ];
            cell_coords += 3;

            *cell_values = static_cast<T>( static_cast<const U*>( values )[ node_index ] );
            ++cell_values;
        }
    }
};

} // end of namespace detail

} // end of namespace cell

} // end of namespace kvs
