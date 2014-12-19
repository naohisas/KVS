/*****************************************************************************/
/**
 *  @file   BasicQuadraticTetrahedralCell.h
 *  @author Naoya Maeda
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: BasicQuadraticTetrahedralCell.h 1773 2014-05-14 08:11:38Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#pragma once
#include "BasicCellCommon.h"


namespace kvs
{

namespace cell
{

template <typename T>
class BasicQuadraticTetrahedralCell : 
    public detail::BasicUnstructuredCellImplementBase<T>, 
    public detail::TetrahedricCellFunction<T>, 
    public detail::MixInCellFunction<BasicQuadraticTetrahedralCell<T>, T>
{
    friend struct detail::CellGenerator<BasicQuadraticTetrahedralCell<T> >;

public:
    enum { NumberOfNodes = 10 };
    typedef T ValueType;

private:
    // used by CellGenerator
    explicit BasicQuadraticTetrahedralCell( const kvs::VolumeObjectBase* volume ) :
        detail::BasicUnstructuredCellImplementBase<T>( volume )
    {
        KVS_ASSERT( volume->volumeType() == kvs::VolumeObjectBase::Unstructured );
        KVS_ASSERT( kvs::UnstructuredVolumeObject::DownCast( volume )->cellType() == kvs::UnstructuredVolumeObject::QuadraticTetrahedra );
    }

public:
    void updateInterpolationFactors( const kvs::Vector3<T>& local )
    {
        KVS_ASSERT( this->includesLocal( local ) );

        const T x = local.x();
        const T y = local.y();
        const T z = local.z();
        const T w = 1 - x - y - z;

        T* N = this->interpolation_factors();

        N[0] = w * (2 * w - 1); // (0, 0, 0)
        N[1] = x * (2 * x - 1); // (1, 0, 0)
        N[2] = z * (2 * z - 1); // (0, 0, 1)
        N[3] = y * (2 * y - 1); // (0, 1, 0)
        N[4] = 4 * x * w; // (1/2,   0,   0)
        N[5] = 4 * z * w; // (  0,   0, 1/2)
        N[6] = 4 * y * w; // (  0, 1/2,   0)
        N[7] = 4 * z * x; // (1/2,   0, 1/2)
        N[8] = 4 * y * z; // (  0, 1/2, 1/2)
        N[9] = 4 * x * y; // (1/2, 1/2,   0)
    }

    void updateDifferentialFactors( const kvs::Vector3<T>& local )
    {
        KVS_ASSERT( this->includesLocal( local ) );

        const T x = local.x();
        const T y = local.y();
        const T z = local.z();
        const T w = 1 - x - y - z;

        const int nnodes = NumberOfNodes;
        T* dNdx = this->differential_factors();
        T* dNdy = dNdx + nnodes;
        T* dNdz = dNdy + nnodes;

        // dNdx
        dNdx[0] = -4 * w + 1;
        dNdx[1] =  4 * x - 1;
        dNdx[2] =  0;
        dNdx[3] =  0;
        dNdx[4] =  4 * (w - x);
        dNdx[5] = -4 * z;
        dNdx[6] = -4 * y;
        dNdx[7] =  4 * z;
        dNdx[8] =  0;
        dNdx[9] =  4 * y;

        // dNdy
        dNdy[0] = -4 * w + 1;
        dNdy[1] =  0;
        dNdy[2] =  0;
        dNdy[3] =  4 * y - 1;
        dNdy[4] = -4 * x;
        dNdy[5] = -4 * z;
        dNdy[6] =  4 * (w - y);
        dNdy[7] =  0;
        dNdy[8] =  4 * z;
        dNdy[9] =  4 * x;

        // dNdz
        dNdz[0] = -4 * w + 1;
        dNdz[1] =  0;
        dNdz[2] =  4 * z - 1;
        dNdz[3] =  0;
        dNdz[4] = -4 * x;
        dNdz[5] =  4 * (w - z);
        dNdz[6] = -4 * y;
        dNdz[7] =  4 * x;
        dNdz[8] =  4 * y;
        dNdz[9] =  0;
    }

    T volume()
    {
        const T a = (T)(0.585410196624969);
        const T b = (T)(0.138196601125011);
        kvs::Vector3<T> v[4];
        v[0] = kvs::Vector3<T>( a, b, b );
        v[1] = kvs::Vector3<T>( b, a, b );
        v[2] = kvs::Vector3<T>( b, b, a );
        v[3] = kvs::Vector3<T>( b, b, b );

        T sum_metric = 0;
        for ( int i = 0 ; i < 4; ++i )
        {
            this->updateDifferentialFactors( v[i] );
            sum_metric += this->jacobianMatrix().determinant();
        }

        return Math::Abs( sum_metric ) / ( 6 * 4 );

        //const kvs::Vector3<T> v0(  0,  0,  0);
        //const kvs::Vector3<T> v1(  1,  0,  0);
        //const kvs::Vector3<T> v2(  0,  0,  1);
        //const kvs::Vector3<T> v3(  0,  1,  0);
        //const kvs::Vector3<T> v4(0.5,  0,  0);
        //const kvs::Vector3<T> v5(  0,  0,0.5);
        //const kvs::Vector3<T> v6(  0,0.5,  0);
        //const kvs::Vector3<T> v7(0.5,  0,0.5);
        //const kvs::Vector3<T> v8(  0,0.5,0.5);
        //const kvs::Vector3<T> v9(0.5,0.5,  0);

        //const kvs::Vector3<T> c[8] = {
        //    (v0 + v4 + v5 + v6) * 0.25,
        //    (v4 + v1 + v7 + v9) * 0.25,
        //    (v5 + v7 + v2 + v8) * 0.25,
        //    (v6 + v9 + v8 + v3) * 0.25,
        //    (v4 + v7 + v5 + v6) * 0.25,
        //    (v4 + v9 + v7 + v6) * 0.25,
        //    (v8 + v6 + v5 + v7) * 0.25,
        //    (v8 + v7 + v9 + v6) * 0.25 };

        //T sum_metric = 0;
        //for ( int i = 0 ; i < 8; ++i )
        //{
        //    this->updateDifferentialFactors( v[i] );
        //    sum_metric += this->jacobianMatrix().determinant();
        //}

        //return Math::Abs( sum_metric ) / ( 6 * 8 );
    }
};

} // end of namespace cell

} // end of namespace kvs
