/*****************************************************************************/
/**
 *  @file   BasicQuadraticHexahedralCell.h
 *  @author Naoya Maeda
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: BasicQuadraticHexahedralCell.h 1773 2014-05-14 08:11:38Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#pragma once
#include "BasicCellCommon.h"


namespace kvs
{

namespace cell
{

template <typename T>
class BasicQuadraticHexahedralCell : 
    public detail::BasicUnstructuredCellImplementBase<T>, 
    public detail::HexahedricCellFunction<T>,
    public detail::MixInCellFunction<BasicQuadraticHexahedralCell<T>, T>
{
    friend struct detail::CellGenerator<BasicQuadraticHexahedralCell<T> >;

public:
    enum { NumberOfNodes = 20 };
    typedef T ValueType;

private:
    // used by CellGenerator
    explicit BasicQuadraticHexahedralCell( const kvs::VolumeObjectBase* volume ) :
        detail::BasicUnstructuredCellImplementBase<T>( volume )
    {
        KVS_ASSERT( volume->volumeType() == kvs::VolumeObjectBase::Unstructured );
        KVS_ASSERT( kvs::UnstructuredVolumeObject::DownCast( volume )->cellType() == kvs::UnstructuredVolumeObject::QuadraticHexahedra );
    }

public:
    void updateInterpolationFactors( const kvs::Vector3<T>& local )
    {
        KVS_ASSERT( this->includesLocal( local ) );

        const T x = local.x();
        const T y = local.y();
        const T z = local.z();
        const T xy = x * y;
        const T yz = y * z;
        const T zx = z * x;
        const T xyz = xy * z;

        T* N = this->interpolation_factors();

        N[ 0] = (1-x)*(1-y)*z*(-1-2*x-2*y+2*z);
        N[ 1] = x*(1-y)*z*(-3+2*x-2*y+2*z);
        N[ 2] = xyz*(-5+2*x+2*y+2*z);
        N[ 3] = (1-x)*yz*(-3-2*x+2*y+2*z);
        N[ 4] = (1-x)*(1-y)*(1-z)*(1-2*x-2*y-2*z);
        N[ 5] = x*(1-y)*(1-z)*(-1+2*x-2*y-2*z);
        N[ 6] = xy*(1-z)*(-3+2*x+2*y-2*z);
        N[ 7] = (1-x)*y*(1-z)*(-1-2*x+2*y-2*z);
        N[ 8] = 4*zx*(1-x)*(1-y);
        N[ 9] = 4*xyz*(1-y);
        N[10] = 4*xyz*(1-x);
        N[11] = (1-x)*4*yz*(1-y);
        N[12] = 4*x*(1-x)*(1-y)*(1-z);
        N[13] = 4*xy*(1-y)*(1-z);
        N[14] = 4*xy*(1-x)*(1-z);
        N[15] = (1-x)*4*y*(1-y)*(1-z);
        N[16] = (1-x)*(1-y)*4*z*(1-z);
        N[17] = (1-y)*4*zx*(1-z);
        N[18] = 4*xyz*(1-z);
        N[19] = (1-x)*4*yz*(1-z);
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

        // dNdx
        dNdx[ 0] = -(1-y)*z*(2*z-2*y-2*x-1)-2*(1-x)*(1-y)*z;
        dNdx[ 1] = (1-y)*z*(2*z-2*y+2*x-3)+2*x*(1-y)*z;
        dNdx[ 2] = y*z*(2*z+2*y+2*x-5)+2*x*y*z;
        dNdx[ 3] = -y*z*(2*z+2*y-2*x-3)-2*(1-x)*y*z;
        dNdx[ 4] = -(1-y)*(-2*z-2*y-2*x+1)*(1-z)-2*(1-x)*(1-y)*(1-z);
        dNdx[ 5] = (1-y)*(-2*z-2*y+2*x-1)*(1-z)+2*x*(1-y)*(1-z);
        dNdx[ 6] = y*(-2*z+2*y+2*x-3)*(1-z)+2*x*y*(1-z);
        dNdx[ 7] = -y*(-2*z+2*y-2*x-1)*(1-z)-2*(1-x)*y*(1-z);
        dNdx[ 8] = 4*(1-x)*(1-y)*z-4*x*(1-y)*z;
        dNdx[ 9] = 4*(1-y)*y*z;
        dNdx[10] = 4*(1-x)*y*z-4*x*y*z;
        dNdx[11] = -4*(1-y)*y*z;
        dNdx[12] = 4*(1-x)*(1-y)*(1-z)-4*x*(1-y)*(1-z);
        dNdx[13] = 4*(1-y)*y*(1-z);
        dNdx[14] = 4*(1-x)*y*(1-z)-4*x*y*(1-z);
        dNdx[15] = -4*(1-y)*y*(1-z);
        dNdx[16] = -4*(1-y)*(1-z)*z;
        dNdx[17] = 4*(1-y)*(1-z)*z;
        dNdx[18] = 4*y*(1-z)*z;
        dNdx[19] = -4*y*(1-z)*z;

        // dNdy
        dNdy[ 0] = -(1-x)*z*(2*z-2*y-2*x-1)-2*(1-x)*(1-y)*z;
        dNdy[ 1] = -x*z*(2*z-2*y+2*x-3)-2*x*(1-y)*z;
        dNdy[ 2] = x*z*(2*z+2*y+2*x-5)+2*x*y*z;
        dNdy[ 3] = (1-x)*z*(2*z+2*y-2*x-3)+2*(1-x)*y*z;
        dNdy[ 4] = -(1-x)*(-2*z-2*y-2*x+1)*(1-z)-2*(1-x)*(1-y)*(1-z);
        dNdy[ 5] = -x*(-2*z-2*y+2*x-1)*(1-z)-2*x*(1-y)*(1-z);
        dNdy[ 6] = x*(-2*z+2*y+2*x-3)*(1-z)+2*x*y*(1-z);
        dNdy[ 7] = (1-x)*(-2*z+2*y-2*x-1)*(1-z)+2*(1-x)*y*(1-z);
        dNdy[ 8] = -4*(1-x)*x*z;
        dNdy[ 9] = 4*x*(1-y)*z-4*x*y*z;
        dNdy[10] = 4*(1-x)*x*z;
        dNdy[11] = 4*(1-x)*(1-y)*z-4*(1-x)*y*z;
        dNdy[12] = -4*(1-x)*x*(1-z);
        dNdy[13] = 4*x*(1-y)*(1-z)-4*x*y*(1-z);
        dNdy[14] = 4*(1-x)*x*(1-z);
        dNdy[15] = 4*(1-x)*(1-y)*(1-z)-4*(1-x)*y*(1-z);
        dNdy[16] = -4*(1-x)*(1-z)*z;
        dNdy[17] = -4*x*(1-z)*z;
        dNdy[18] = 4*x*(1-z)*z;
        dNdy[19] = 4*(1-x)*(1-z)*z;

        // dNdz 
        dNdz[ 0] = (1-x)*(1-y)*(2*z-2*y-2*x-1)+2*(1-x)*(1-y)*z;
        dNdz[ 1] = x*(1-y)*(2*z-2*y+2*x-3)+2*x*(1-y)*z;
        dNdz[ 2] = x*y*(2*z+2*y+2*x-5)+2*x*y*z;
        dNdz[ 3] = (1-x)*y*(2*z+2*y-2*x-3)+2*(1-x)*y*z;
        dNdz[ 4] = -2*(1-x)*(1-y)*(1-z)-(1-x)*(1-y)*(-2*z-2*y-2*x+1);
        dNdz[ 5] = -2*x*(1-y)*(1-z)-x*(1-y)*(-2*z-2*y+2*x-1);
        dNdz[ 6] = -2*x*y*(1-z)-x*y*(-2*z+2*y+2*x-3);
        dNdz[ 7] = -2*(1-x)*y*(1-z)-(1-x)*y*(-2*z+2*y-2*x-1);
        dNdz[ 8] = 4*(1-x)*x*(1-y);
        dNdz[ 9] = 4*x*(1-y)*y;
        dNdz[10] = 4*(1-x)*x*y;
        dNdz[11] = 4*(1-x)*(1-y)*y;
        dNdz[12] = -4*(1-x)*x*(1-y);
        dNdz[13] = -4*x*(1-y)*y;
        dNdz[14] = -4*(1-x)*x*y;
        dNdz[15] = -4*(1-x)*(1-y)*y;
        dNdz[16] = 4*(1-x)*(1-y)*(1-z)-4*(1-x)*(1-y)*z;
        dNdz[17] = 4*x*(1-y)*(1-z)-4*x*(1-y)*z;
        dNdz[18] = 4*x*y*(1-z)-4*x*y*z;
        dNdz[19] = 4*(1-x)*y*(1-z)-4*(1-x)*y*z;
    }

    T volume()
    {
        const int resolution = 3;
        const T sampling_length = 1 / static_cast<T>( resolution );
        const T adjustment = sampling_length * T( 0.5 );

        kvs::Vector3<T> sampling_position = kvs::Vector3<T>::Constant( adjustment );

        T  sum_metric = 0;

        for ( int k = 0; k < resolution; ++k )
        {
            for ( int j = 0; j < resolution; ++j )
            {
                for ( int i = 0; i < resolution; ++i )
                {
                    this->updateDifferentialFactors( sampling_position );
                    sum_metric += this->jacobianMatrix().determinant();
                    sampling_position.x() += sampling_length;
                }
                sampling_position.x() = adjustment;
                sampling_position.y() += sampling_length;
            }
            sampling_position.y() = adjustment;
            sampling_position.z() += sampling_length;
        }

        const int resolution3 = resolution * resolution * resolution;
        return kvs::Math::Abs( sum_metric ) / resolution3;
    }
};

} // end of namespace cell

} // end of namespace kvs
