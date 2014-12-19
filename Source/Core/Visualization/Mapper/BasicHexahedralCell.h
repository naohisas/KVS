/*****************************************************************************/
/**
 *  @file   BasicHexahedralCell.h
 *  @author Naoya Maeda
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: BasicHexahedralCell.h 1773 2014-05-14 08:11:38Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#pragma once
#include "BasicCellCommon.h"


namespace kvs
{

namespace cell
{

template <typename T>
class BasicHexahedralCell : 
    public detail::BasicUnstructuredCellImplementBase<T>, 
    public detail::HexahedricCellFunction<T>,
    public detail::MixInCellFunction<BasicHexahedralCell<T>, T>
{
    friend struct detail::CellGenerator<BasicHexahedralCell<T> >;

public:
    enum { NumberOfNodes = 8 };
    typedef T ValueType;

private:
    // used by CellGenerator
    explicit BasicHexahedralCell( const kvs::VolumeObjectBase* volume ) :
        detail::BasicUnstructuredCellImplementBase<T>( volume )
    {
        KVS_ASSERT( volume->volumeType() == kvs::VolumeObjectBase::Unstructured );
        KVS_ASSERT( kvs::UnstructuredVolumeObject::DownCast( volume )->cellType() == kvs::UnstructuredVolumeObject::Hexahedra );
    }

public:
    void updateInterpolationFactors( const kvs::Vector3<T>& local )
    {
        KVS_ASSERT( this->includesLocal( local ) );

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
        dNdx[4] = - 1 + y +z - yz;
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

    T volume()
    {
        KVS_ASSERT( false );
        return 0; // not implemented.
    }
};

} // end of namespace cell

} // end of namespace kvs
