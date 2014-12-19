/*****************************************************************************/
/**
 *  @file   HexahedralCell.cpp
 *  @author Takuma Kawamura, Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: QuadraticHexahedralCell.cpp 1792 2014-07-31 04:50:42Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#include "QuadraticHexahedralCell.h"


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new QuadraticHexahedralCell class.
 *  @param  volume [in] pointer to the unstructured volume object
 */
/*===========================================================================*/
QuadraticHexahedralCell::QuadraticHexahedralCell(
    const kvs::UnstructuredVolumeObject* volume ):
    kvs::CellBase( volume )
{
    // Set the initial interpolation functions and differential functions.
    this->interpolationFunctions( BaseClass::localPoint() );
    this->differentialFunctions( BaseClass::localPoint() );
}

/*===========================================================================*/
/**
 *  @brief  Destroys the QuadraticHexahedralCell class.
 */
/*===========================================================================*/
QuadraticHexahedralCell::~QuadraticHexahedralCell()
{
}

/*==========================================================================*/
/**
 *  @brief  Calculates the interpolation functions in the local coordinate.
 *  @return point [in] point in the local coordinate
 */
/*==========================================================================*/
const kvs::Real32* QuadraticHexahedralCell::interpolationFunctions( const kvs::Vec3& point ) const
{
    const float x = point.x();
    const float y = point.y();
    const float z = point.z();
    const float xy = x * y;
    const float yz = y * z;
    const float zx = z * x;
    const float xyz = xy * z;

    kvs::Real32* N = BaseClass::interpolationFunctions();
    N[ 0] = ( 1 - x )*( 1 - y )*z*( - 1 - 2*x - 2*y + 2*z );
    N[ 1] = x*( 1 - y )*z*( - 3 + 2*x - 2*y + 2*z );
    N[ 2] = xyz*( - 5 + 2*x + 2*y + 2*z );
    N[ 3] = ( 1 - x )*yz*( - 3 - 2*x + 2*y + 2*z );
    N[ 4] = ( 1 - x )*( 1 - y )*( 1 - z )*( 1 - 2*x - 2*y - 2*z );
    N[ 5] = x*( 1 - y )*( 1 - z )*( - 1 + 2*x - 2*y - 2*z );
    N[ 6] = xy*( 1 - z )*( - 3 + 2*x + 2*y - 2*z );
    N[ 7] = ( 1 - x )*y*( 1 - z )*( - 1 - 2*x + 2*y - 2*z );
    N[ 8] = 4*zx*( 1 - x )*( 1 - y );
    N[ 9] = 4*xyz*( 1 - y );
    N[10] = 4*xyz*( 1 - x );
    N[11] = ( 1 - x )*4*yz*( 1 - y );
    N[12] = 4*x*( 1 - x )*( 1 - y )*( 1 - z );
    N[13] = 4*xy*( 1 - y )*( 1 - z );
    N[14] = 4*xy*( 1 - x )*( 1 - z );
    N[15] = ( 1 - x )*4*y*( 1 - y )*( 1 - z );
    N[16] = ( 1 - x )*( 1 - y )*4*z*( 1 - z );
    N[17] = ( 1 - y )*4*zx*( 1 - z );
    N[18] = 4*xyz*( 1 - z );
    N[19] = ( 1 - x )*4*yz*( 1 - z );

    return N;
}

/*===========================================================================*/
/**
 *  @brief  Calculates the differential functions in the local coordinate.
 */
/*===========================================================================*/
const kvs::Real32* QuadraticHexahedralCell::differentialFunctions( const kvs::Vec3& point ) const
{
    const float x = point.x();
    const float y = point.y();
    const float z = point.z();

    const size_t nnodes = BaseClass::numberOfCellNodes();
    kvs::Real32* dN = BaseClass::differentialFunctions();
    kvs::Real32* dNdx = dN;
    kvs::Real32* dNdy = dNdx + nnodes;
    kvs::Real32* dNdz = dNdy + nnodes;

    dNdx[ 0] =  -(1-y)*z*(2*z-2*y-2*x-1)-2*(1-x)*(1-y)*z;
    dNdx[ 1] =  (1-y)*z*(2*z-2*y+2*x-3)+2*x*(1-y)*z;
    dNdx[ 2] =  y*z*(2*z+2*y+2*x-5)+2*x*y*z;
    dNdx[ 3] =  -y*z*(2*z+2*y-2*x-3)-2*(1-x)*y*z;
    dNdx[ 4] =  -(1-y)*(-2*z-2*y-2*x+1)*(1-z)-2*(1-x)*(1-y)*(1-z);
    dNdx[ 5] =  (1-y)*(-2*z-2*y+2*x-1)*(1-z)+2*x*(1-y)*(1-z);
    dNdx[ 6] =  y*(-2*z+2*y+2*x-3)*(1-z)+2*x*y*(1-z);
    dNdx[ 7] =  -y*(-2*z+2*y-2*x-1)*(1-z)-2*(1-x)*y*(1-z);
    dNdx[ 8] =  4*(1-x)*(1-y)*z-4*x*(1-y)*z;
    dNdx[ 9] =  4*(1-y)*y*z;
    dNdx[10] =  4*(1-x)*y*z-4*x*y*z;
    dNdx[11] =  -4*(1-y)*y*z;
    dNdx[12] =  4*(1-x)*(1-y)*(1-z)-4*x*(1-y)*(1-z);
    dNdx[13] =  4*(1-y)*y*(1-z);
    dNdx[14] =  4*(1-x)*y*(1-z)-4*x*y*(1-z);
    dNdx[15] =  -4*(1-y)*y*(1-z);
    dNdx[16] =  -4*(1-y)*(1-z)*z;
    dNdx[17] =  4*(1-y)*(1-z)*z;
    dNdx[18] =  4*y*(1-z)*z;
    dNdx[19] =  -4*y*(1-z)*z;

    dNdy[ 0] =  -(1-x)*z*(2*z-2*y-2*x-1)-2*(1-x)*(1-y)*z;
    dNdy[ 1] =  -x*z*(2*z-2*y+2*x-3)-2*x*(1-y)*z;
    dNdy[ 2] =  x*z*(2*z+2*y+2*x-5)+2*x*y*z;
    dNdy[ 3] =  (1-x)*z*(2*z+2*y-2*x-3)+2*(1-x)*y*z;
    dNdy[ 4] =  -(1-x)*(-2*z-2*y-2*x+1)*(1-z)-2*(1-x)*(1-y)*(1-z);
    dNdy[ 5] =  -x*(-2*z-2*y+2*x-1)*(1-z)-2*x*(1-y)*(1-z);
    dNdy[ 6] =  x*(-2*z+2*y+2*x-3)*(1-z)+2*x*y*(1-z);
    dNdy[ 7] =  (1-x)*(-2*z+2*y-2*x-1)*(1-z)+2*(1-x)*y*(1-z);
    dNdy[ 8] =  -4*(1-x)*x*z;
    dNdy[ 9] =  4*x*(1-y)*z-4*x*y*z;
    dNdy[10] =  4*(1-x)*x*z;
    dNdy[11] =  4*(1-x)*(1-y)*z-4*(1-x)*y*z;
    dNdy[12] =  -4*(1-x)*x*(1-z);
    dNdy[13] =  4*x*(1-y)*(1-z)-4*x*y*(1-z);
    dNdy[14] =  4*(1-x)*x*(1-z);
    dNdy[15] =  4*(1-x)*(1-y)*(1-z)-4*(1-x)*y*(1-z);
    dNdy[16] =  -4*(1-x)*(1-z)*z;
    dNdy[17] =  -4*x*(1-z)*z;
    dNdy[18] =  4*x*(1-z)*z;
    dNdy[19] =  4*(1-x)*(1-z)*z;

    dNdz[ 0] =  (1-x)*(1-y)*(2*z-2*y-2*x-1)+2*(1-x)*(1-y)*z;
    dNdz[ 1] =  x*(1-y)*(2*z-2*y+2*x-3)+2*x*(1-y)*z;
    dNdz[ 2] =  x*y*(2*z+2*y+2*x-5)+2*x*y*z;
    dNdz[ 3] =  (1-x)*y*(2*z+2*y-2*x-3)+2*(1-x)*y*z;
    dNdz[ 4] =  -2*(1-x)*(1-y)*(1-z)-(1-x)*(1-y)*(-2*z-2*y-2*x+1);
    dNdz[ 5] =  -2*x*(1-y)*(1-z)-x*(1-y)*(-2*z-2*y+2*x-1);
    dNdz[ 6] =  -2*x*y*(1-z)-x*y*(-2*z+2*y+2*x-3);
    dNdz[ 7] =  -2*(1-x)*y*(1-z)-(1-x)*y*(-2*z+2*y-2*x-1);
    dNdz[ 8] =  4*(1-x)*x*(1-y);
    dNdz[ 9] =  4*x*(1-y)*y;
    dNdz[10] =  4*(1-x)*x*y;
    dNdz[11] =  4*(1-x)*(1-y)*y;
    dNdz[12] =  -4*(1-x)*x*(1-y);
    dNdz[13] =  -4*x*(1-y)*y;
    dNdz[14] =  -4*(1-x)*x*y;
    dNdz[15] =  -4*(1-x)*(1-y)*y;
    dNdz[16] =  4*(1-x)*(1-y)*(1-z)-4*(1-x)*(1-y)*z;
    dNdz[17] =  4*x*(1-y)*(1-z)-4*x*(1-y)*z;
    dNdz[18] =  4*x*y*(1-z)-4*x*y*z;
    dNdz[19] =  4*(1-x)*y*(1-z)-4*(1-x)*y*z;

    return dN;
}

/*===========================================================================*/
/**
 *  @brief  Returns the sampled point randomly.
 *  @return coordinate value of the sampled point
 */
/*===========================================================================*/
const kvs::Vec3 QuadraticHexahedralCell::randomSampling() const
{
    // Generate a point in the local coordinate.
    const float s = BaseClass::randomNumber();
    const float t = BaseClass::randomNumber();
    const float u = BaseClass::randomNumber();

    const kvs::Vec3 point( s, t, u );
    BaseClass::setLocalPoint( point );
    BaseClass::m_global_point = BaseClass::transformLocalToGlobal( point );

    return BaseClass::m_global_point;
}

/*===========================================================================*/
/**
 *  @brief  Returns the volume of the cell.
 *  @return volume of the cell
 */
/*===========================================================================*/
const kvs::Real32 QuadraticHexahedralCell::volume() const
{
    const size_t resolution = 3;
    const float sampling_length = 1.0f / (float)resolution;
    const float adjustment = sampling_length * 0.5f;

    kvs::Vec3 sampling_position( -adjustment, -adjustment, -adjustment );

    float sum_metric = 0;

    for( size_t k = 0 ; k < resolution ; k++ )
    {
        sampling_position[ 2 ] +=  sampling_length;
        for( size_t j = 0 ; j < resolution ; j++ )
        {
            sampling_position[ 1 ] += sampling_length;
            for( size_t i = 0 ; i < resolution ; i++ )
            {
                sampling_position[ 0 ] += sampling_length;

                BaseClass::setLocalPoint( sampling_position );
                const kvs::Mat3 J = BaseClass::JacobiMatrix();
                const float metric_element = J.determinant();

                sum_metric += kvs::Math::Abs<float>( metric_element );
            }
            sampling_position[ 0 ] = -adjustment;
        }
        sampling_position[ 1 ] = -adjustment;
    }

    const float resolution3 = resolution * resolution * resolution;

    return sum_metric / resolution3;
}

} // end of namespace kvs
