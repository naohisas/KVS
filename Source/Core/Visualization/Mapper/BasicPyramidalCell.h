/*****************************************************************************/
/**
 *  @file   BasicPyramidalCell.h
 *  @author Naoya Maeda
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: BasicPyramidalCell.h 1773 2014-05-14 08:11:38Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#pragma once
#include "BasicCellCommon.h"


namespace kvs
{

namespace cell
{

template <typename T>
class BasicPyramidalCell : 
    public detail::BasicUnstructuredCellImplementBase<T>, 
    public detail::MixInCellFunction<BasicPyramidalCell<T>, T>
{
    friend struct detail::CellGenerator<BasicPyramidalCell<T> >;

public:
    enum { NumberOfNodes = 5 };
    typedef T ValueType;

private:
    // used by CellGenerator
    explicit BasicPyramidalCell( const kvs::VolumeObjectBase* volume ) :
        detail::BasicUnstructuredCellImplementBase<T>( volume )
    {
        KVS_ASSERT( volume->volumeType() == kvs::VolumeObjectBase::Unstructured );
        KVS_ASSERT( kvs::UnstructuredVolumeObject::DownCast( volume )->cellType() == kvs::UnstructuredVolumeObject::Pyramid );
    }

public:
    void updateInterpolationFactors( const kvs::Vector3<T>& local )
    {
        KVS_ASSERT( this->includesLocal( local ) );
        const T x = local.x();
        const T y = local.y();
        const T z = local.z();
        T* N = this->interpolation_factors();

        const T k = x * y * z / ( 1 - z );

        N[0] = z;
        N[1] = T( 0.25 ) * ( ( 1 - x ) * ( 1 - y ) - z + k );
        N[2] = T( 0.25 ) * ( ( 1 + x ) * ( 1 - y ) - z - k );
        N[3] = T( 0.25 ) * ( ( 1 + x ) * ( 1 + y ) - z + k );
        N[4] = T( 0.25 ) * ( ( 1 - x ) * ( 1 + y ) - z - k );
    }

    void updateDifferentialFactors( const kvs::Vector3<T>& local )
    {
        KVS_ASSERT( this->includesLocal( local ) );

        const T x = local.x();
        const T y = local.y();
        const T z = local.z();

        const int nnodes = NumberOfNodes;
        T* dNdx = this->differential_factors();
        T* dNdy = dNdx + nnodes;
        T* dNdz = dNdy + nnodes;

        const T t = 1 / ( 1 - z );
        const T dkdx = y * z * t;
        const T dkdy = x * z * t;
        const T dkdz = x * y * t * t;

        // dNdx
        dNdx[0] = 0;
        dNdx[1] = T( 0.25 ) * ( - ( 1 - y ) + dkdx );
        dNdx[2] = T( 0.25 ) * (   ( 1 - y ) - dkdx );
        dNdx[3] = T( 0.25 ) * (   ( 1 + y ) + dkdx );
        dNdx[4] = T( 0.25 ) * ( - ( 1 + y ) - dkdx );

        // dNdy
        dNdy[0] = 0;
        dNdy[1] = T( 0.25 ) * ( - ( 1 - x ) + dkdy );
        dNdy[2] = T( 0.25 ) * ( - ( 1 + x ) - dkdy );
        dNdy[3] = T( 0.25 ) * (   ( 1 + x ) + dkdy );
        dNdy[4] = T( 0.25 ) * (   ( 1 - x ) - dkdy );

        // dNdz
        dNdz[0] = 1;
        dNdz[1] = T( 0.25 ) * ( - 1 + dkdz );
        dNdz[2] = T( 0.25 ) * ( - 1 - dkdz );
        dNdz[3] = T( 0.25 ) * ( - 1 + dkdz );
        dNdz[4] = T( 0.25 ) * ( - 1 - dkdz );
    }

    T volume()
    {
        KVS_ASSERT( false );
        return 0; // not implemented.
    }

public:
    static const kvs::Vector3<T> centerOfLocal()
    {
        return kvs::Vector3<T>( 0, 0, T( 0.2 ) );
    }

    void adjustPoint( kvs::Vector3<T>& point )
    {
        KVS_ASSERT( detail::InUnitCube( point ) );
        T x = point.x();
        T y = point.y();
        T z = point.z();

        if ( y <= 1 - x && z <= 1 - x )
        {
            T temp = x;
            x = y;
            y = z;
            z = temp;
        }
        else if ( z <= 1 - y && x <= 1 - y )
        {
            T temp = x;
            x = z;
            z = y;
            y = temp;
        }

        point.x() = 2 * x - ( 1 - z );
        point.y() = 2 * y - ( 1 - z );
        point.z() = z;
    }

    static bool includesLocal( const kvs::Vector3<T>& local )
    {
        const T x = local.x();
        const T y = local.y();
        const T z = local.z();
        return z >= 0 && z <= 1 && kvs::Math::Abs( x ) <= 1 - z && kvs::Math::Abs( y ) <= 1 - z;
    }
};

} // end of namespace cell

} // end of namespace kvs
