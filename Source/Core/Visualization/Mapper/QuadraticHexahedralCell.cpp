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
    this->updateInterpolationFunctions( BaseClass::localPoint() );
    this->updateDifferentialFunctions( BaseClass::localPoint() );
}

/*==========================================================================*/
/**
 *  @brief  Calculates the interpolation functions in the local coordinate.
 *  @return local [in] point in the local coordinate
 */
/*==========================================================================*/
void QuadraticHexahedralCell::updateInterpolationFunctions( const kvs::Vec3& local ) const
{
    KVS_ASSERT( BaseClass::containsLocalPoint( local ) );

    const float p = local.x();
    const float q = local.y();
    const float r = local.z();
    const float pq = p * q;
    const float qr = q * r;
    const float rp = r * p;
    const float pqr = pq * r;

    kvs::Real32* N = BaseClass::interpolationFunctions();
    N[ 0] = ( 1 - p )*( 1 - q )*r*( - 1 - 2*p - 2*q + 2*r );
    N[ 1] = p*( 1 - q )*r*( - 3 + 2*p - 2*q + 2*r );
    N[ 2] = pqr*( - 5 + 2*p + 2*q + 2*r );
    N[ 3] = ( 1 - p )*qr*( - 3 - 2*p + 2*q + 2*r );
    N[ 4] = ( 1 - p )*( 1 - q )*( 1 - r )*( 1 - 2*p - 2*q - 2*r );
    N[ 5] = p*( 1 - q )*( 1 - r )*( - 1 + 2*p - 2*q - 2*r );
    N[ 6] = pq*( 1 - r )*( - 3 + 2*p + 2*q - 2*r );
    N[ 7] = ( 1 - p )*q*( 1 - r )*( - 1 - 2*p + 2*q - 2*r );
    N[ 8] = 4*rp*( 1 - p )*( 1 - q );
    N[ 9] = 4*pqr*( 1 - q );
    N[10] = 4*pqr*( 1 - p );
    N[11] = ( 1 - p )*4*qr*( 1 - q );
    N[12] = 4*p*( 1 - p )*( 1 - q )*( 1 - r );
    N[13] = 4*pq*( 1 - q )*( 1 - r );
    N[14] = 4*pq*( 1 - p )*( 1 - r );
    N[15] = ( 1 - p )*4*q*( 1 - q )*( 1 - r );
    N[16] = ( 1 - p )*( 1 - q )*4*r*( 1 - r );
    N[17] = ( 1 - q )*4*rp*( 1 - r );
    N[18] = 4*pqr*( 1 - r );
    N[19] = ( 1 - p )*4*qr*( 1 - r );
}

/*===========================================================================*/
/**
 *  @brief  Calculates the differential functions in the local coordinate.
 *  @return local [in] point in the local coordinate
 */
/*===========================================================================*/
void QuadraticHexahedralCell::updateDifferentialFunctions( const kvs::Vec3& local ) const
{
    KVS_ASSERT( BaseClass::containsLocalPoint( local ) );

    const float p = local.x();
    const float q = local.y();
    const float r = local.z();

    const size_t nnodes = BaseClass::numberOfCellNodes();
    kvs::Real32* dN = BaseClass::differentialFunctions();
    kvs::Real32* dNdp = dN;
    kvs::Real32* dNdq = dNdp + nnodes;
    kvs::Real32* dNdr = dNdq + nnodes;

    dNdp[ 0] =  -(1-q)*r*(2*r-2*q-2*p-1)-2*(1-p)*(1-q)*r;
    dNdp[ 1] =  (1-q)*r*(2*r-2*q+2*p-3)+2*p*(1-q)*r;
    dNdp[ 2] =  q*r*(2*r+2*q+2*p-5)+2*p*q*r;
    dNdp[ 3] =  -q*r*(2*r+2*q-2*p-3)-2*(1-p)*q*r;
    dNdp[ 4] =  -(1-q)*(-2*r-2*q-2*p+1)*(1-r)-2*(1-p)*(1-q)*(1-r);
    dNdp[ 5] =  (1-q)*(-2*r-2*q+2*p-1)*(1-r)+2*p*(1-q)*(1-r);
    dNdp[ 6] =  q*(-2*r+2*q+2*p-3)*(1-r)+2*p*q*(1-r);
    dNdp[ 7] =  -q*(-2*r+2*q-2*p-1)*(1-r)-2*(1-p)*q*(1-r);
    dNdp[ 8] =  4*(1-p)*(1-q)*r-4*p*(1-q)*r;
    dNdp[ 9] =  4*(1-q)*q*r;
    dNdp[10] =  4*(1-p)*q*r-4*p*q*r;
    dNdp[11] =  -4*(1-q)*q*r;
    dNdp[12] =  4*(1-p)*(1-q)*(1-r)-4*p*(1-q)*(1-r);
    dNdp[13] =  4*(1-q)*q*(1-r);
    dNdp[14] =  4*(1-p)*q*(1-r)-4*p*q*(1-r);
    dNdp[15] =  -4*(1-q)*q*(1-r);
    dNdp[16] =  -4*(1-q)*(1-r)*r;
    dNdp[17] =  4*(1-q)*(1-r)*r;
    dNdp[18] =  4*q*(1-r)*r;
    dNdp[19] =  -4*q*(1-r)*r;

    dNdq[ 0] =  -(1-p)*r*(2*r-2*q-2*p-1)-2*(1-p)*(1-q)*r;
    dNdq[ 1] =  -p*r*(2*r-2*q+2*p-3)-2*p*(1-q)*r;
    dNdq[ 2] =  p*r*(2*r+2*q+2*p-5)+2*p*q*r;
    dNdq[ 3] =  (1-p)*r*(2*r+2*q-2*p-3)+2*(1-p)*q*r;
    dNdq[ 4] =  -(1-p)*(-2*r-2*q-2*p+1)*(1-r)-2*(1-p)*(1-q)*(1-r);
    dNdq[ 5] =  -p*(-2*r-2*q+2*p-1)*(1-r)-2*p*(1-q)*(1-r);
    dNdq[ 6] =  p*(-2*r+2*q+2*p-3)*(1-r)+2*p*q*(1-r);
    dNdq[ 7] =  (1-p)*(-2*r+2*q-2*p-1)*(1-r)+2*(1-p)*q*(1-r);
    dNdq[ 8] =  -4*(1-p)*p*r;
    dNdq[ 9] =  4*p*(1-q)*r-4*p*q*r;
    dNdq[10] =  4*(1-p)*p*r;
    dNdq[11] =  4*(1-p)*(1-q)*r-4*(1-p)*q*r;
    dNdq[12] =  -4*(1-p)*p*(1-r);
    dNdq[13] =  4*p*(1-q)*(1-r)-4*p*q*(1-r);
    dNdq[14] =  4*(1-p)*p*(1-r);
    dNdq[15] =  4*(1-p)*(1-q)*(1-r)-4*(1-p)*q*(1-r);
    dNdq[16] =  -4*(1-p)*(1-r)*r;
    dNdq[17] =  -4*p*(1-r)*r;
    dNdq[18] =  4*p*(1-r)*r;
    dNdq[19] =  4*(1-p)*(1-r)*r;

    dNdr[ 0] =  (1-p)*(1-q)*(2*r-2*q-2*p-1)+2*(1-p)*(1-q)*r;
    dNdr[ 1] =  p*(1-q)*(2*r-2*q+2*p-3)+2*p*(1-q)*r;
    dNdr[ 2] =  p*q*(2*r+2*q+2*p-5)+2*p*q*r;
    dNdr[ 3] =  (1-p)*q*(2*r+2*q-2*p-3)+2*(1-p)*q*r;
    dNdr[ 4] =  -2*(1-p)*(1-q)*(1-r)-(1-p)*(1-q)*(-2*r-2*q-2*p+1);
    dNdr[ 5] =  -2*p*(1-q)*(1-r)-p*(1-q)*(-2*r-2*q+2*p-1);
    dNdr[ 6] =  -2*p*q*(1-r)-p*q*(-2*r+2*q+2*p-3);
    dNdr[ 7] =  -2*(1-p)*q*(1-r)-(1-p)*q*(-2*r+2*q-2*p-1);
    dNdr[ 8] =  4*(1-p)*p*(1-q);
    dNdr[ 9] =  4*p*(1-q)*q;
    dNdr[10] =  4*(1-p)*p*q;
    dNdr[11] =  4*(1-p)*(1-q)*q;
    dNdr[12] =  -4*(1-p)*p*(1-q);
    dNdr[13] =  -4*p*(1-q)*q;
    dNdr[14] =  -4*(1-p)*p*q;
    dNdr[15] =  -4*(1-p)*(1-q)*q;
    dNdr[16] =  4*(1-p)*(1-q)*(1-r)-4*(1-p)*(1-q)*r;
    dNdr[17] =  4*p*(1-q)*(1-r)-4*p*(1-q)*r;
    dNdr[18] =  4*p*q*(1-r)-4*p*q*r;
    dNdr[19] =  4*(1-p)*q*(1-r)-4*(1-p)*q*r;
}

/*===========================================================================*/
/**
 *  @brief  Returns the volume of the cell.
 *  @return volume of the cell
 */
/*===========================================================================*/
kvs::Real32 QuadraticHexahedralCell::volume() const
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
