/*****************************************************************************/
/**
 *  @file   BasicTetrahedralCell.h
 *  @author Naoya Maeda
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: BasicTetrahedralCell.h 1773 2014-05-14 08:11:38Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#pragma once
#include "BasicCellCommon.h"


namespace kvs
{

namespace cell
{

template <typename T>
class BasicTetrahedralCell : 
    public detail::BasicUnstructuredCellImplementBase<T>, 
    public detail::TetrahedricCellFunction<T>, 
    public detail::MixInCellFunction<BasicTetrahedralCell<T>, T>
{
    friend struct detail::CellGenerator<BasicTetrahedralCell<T> >;

public:
    enum { NumberOfNodes = 4 };
    typedef T ValueType;

private:
    // used by CellGenerator
    explicit BasicTetrahedralCell( const kvs::VolumeObjectBase* volume ) :
        detail::BasicUnstructuredCellImplementBase<T>( volume )
    {
        KVS_ASSERT( volume->volumeType() == kvs::VolumeObjectBase::Unstructured );
        KVS_ASSERT( kvs::UnstructuredVolumeObject::DownCast( volume )->cellType() == kvs::UnstructuredVolumeObject::Tetrahedra );
    }

public:
    void updateInterpolationFactors( const kvs::Vector3<T>& local )
    {
        KVS_ASSERT( this->includesLocal( local ) );

        T* N = this->interpolation_factors();
        N[0] = local.x();
        N[1] = local.y();
        N[2] = local.z();
        N[3] = 1 - N[0] - N[1] - N[2];
    }

    void updateDifferentialFactors( const kvs::Vector3<T>& local )
    {
        KVS_ASSERT( this->includesLocal( local ) );
        (void)local;// do nothing.
    }

    T volume()
    {
        return kvs::Math::Abs( this->jacobianMatrix().determinant() ) / 6;
    }

public:
    // hiding BasicUnstructuredCellImplementBase.gradient
    const kvs::Vector3<T> gradient()
    {
        //     [ds/dX]   [dx/dX,dy/dX,dz/dX]   [ds/dx]            [ds/dx]
        // g = [ds/dY] = [dx/dY,dy/dY,dz/dY] * [ds/dy] = J^-1^t * [ds/dy]
        //     [ds/dZ]   [dx/dZ,dy/dZ,dz/dZ]   [ds/dz]            [ds/dz]
        //
        //   = ( [ds/dx ds/dy ds/dz] * J^-1 )^t
        //
        // s = (s0-s3) * x + (s1-s3) * y + (s2-s3) * z + s3
        // ds/dx = s0-s3, ds/dy = s1-s3, ds/dz = s2-s3
        KVS_ASSERT( this->valueDimension() == 1 );

        const T* s = this->values();
        const kvs::Vector3<T> g( s[0] - s[3], s[1] - s[3], s[2] - s[3] );
        return g * this->jacobianMatrix().inverted();
    }

    // hiding BasicUnstructuredCellImplementBase.jacobianMatrix
    const kvs::Matrix33<T> jacobianMatrix()
    {
        //     [dX/dx,dX/dy,dX/dz]   [v0x-v3x,v1x-v3x,v2z-v3x]
        // J = [dY/dx,dY/dy,dY/dz] = [v0y-v3y,v1y-v3y,v2y-v3y]
        //     [dZ/dx,dZ/dy,dZ/dz]   [v0z-v3z,v1z-v3z,v2z-v3z]

        const T* v = this->coords();
        const kvs::Vector3<T> V3( v + 9 );
        const kvs::Vector3<T> V30 = kvs::Vector3<T>( v + 0 ) - V3;
        const kvs::Vector3<T> V31 = kvs::Vector3<T>( v + 3 ) - V3;
        const kvs::Vector3<T> V32 = kvs::Vector3<T>( v + 6 ) - V3;
        return kvs::Matrix33<T>( V30[0], V31[0], V32[0],
                                 V30[1], V31[1], V32[1],
                                 V30[2], V31[2], V32[2] );
    }

    // hiding MixInCellFunction.globalToLocal
    const kvs::Vector3<T> globalToLocal( const kvs::Vector3<T>& global )
    {
        // X = (v0x-v3x) * x + (v1x-v3x) * y + (v2x-v3z) * z + v3x
        // Y = (v0y-v3y) * x + (v1y-v3y) * y + (v2y-v3z) * z + v3y
        // Z = (v0z-v3z) * x + (v1z-v3z) * y + (v2z-v3z) * z + v3z

        // [X]   [                 ]   [x]
        // [Y] = [v0-v3,v1-v3,v2-v3] * [y] + v3
        // [Z]   [                 ]   [z]
        
        // [x]   [                 ]^-1 ([X]   )
        // [y] = [v0-v3,v1-v3,v2-v3]  * ([Y]-v3)
        // [z]   [                 ]    ([Z]   )
        const T* v = this->coords();
        const kvs::Vector3<T> V3( v + 9 );
        return this->jacobianMatrix().inverted() * ( global - V3 );
    }
};

} // end of namespace cell

} // end of namespace kvs
