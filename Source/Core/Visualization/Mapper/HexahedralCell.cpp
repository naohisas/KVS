/****************************************************************************/
/**
 *  @file   HexahedralCell.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: HexahedralCell.cpp 1792 2014-07-31 04:50:42Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#include "HexahedralCell.h"


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new HexahedralCell class.
 *  @param  volume [in] pointer to the unstructured volume object
 */
/*===========================================================================*/
HexahedralCell::HexahedralCell(
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
void HexahedralCell::updateInterpolationFunctions( const kvs::Vec3& local ) const
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
    N[0] = r - rp - qr + pqr;
    N[1] = rp - pqr;
    N[2] = pqr;
    N[3] = qr - pqr;
    N[4] = 1.0f - p - q - r + pq + qr + rp - pqr;
    N[5] = p - pq - rp + pqr;
    N[6] = pq - pqr;
    N[7] = q - pq - qr + pqr;
}

/*==========================================================================*/
/**
 *  @brief  Calculates the differential functions in the local coordinate.
 *  @return local [in] point in the local coordinate
 */
/*==========================================================================*/
void HexahedralCell::updateDifferentialFunctions( const kvs::Vec3& local ) const
{
    KVS_ASSERT( BaseClass::containsLocalPoint( local ) );

    const float p = local.x();
    const float q = local.y();
    const float r = local.z();
    const float pq = p * q;
    const float qr = q * r;
    const float rp = r * p;

    const size_t nnodes = BaseClass::numberOfCellNodes();
    kvs::Real32* dN = BaseClass::differentialFunctions();
    kvs::Real32* dNdp = dN;
    kvs::Real32* dNdq = dN + nnodes;
    kvs::Real32* dNdr = dN + nnodes + nnodes;

    dNdp[0] =  - r + qr;
    dNdp[1] =  r - qr;
    dNdp[2] =  qr;
    dNdp[3] =  - qr;
    dNdp[4] =  - 1.0f + q +r - qr;
    dNdp[5] =  1.0f - q - r + qr;
    dNdp[6] =  q - qr;
    dNdp[7] =  - q + qr;

    dNdq[0] =  - r + rp;
    dNdq[1] =  - rp;
    dNdq[2] =  rp;
    dNdq[3] =  r - rp;
    dNdq[4] =  - 1.0f + p + r - rp;
    dNdq[5] =  - p + rp;
    dNdq[6] =  p - rp;
    dNdq[7] =  1.0f - p - r + rp;

    dNdr[0] =  1.0f - q - p + pq;
    dNdr[1] =  p - pq;
    dNdr[2] =  pq;
    dNdr[3] =  q - pq;
    dNdr[4] =  - 1.0f + q + p - pq;
    dNdr[5] =  - p + pq;
    dNdr[6] =  - pq;
    dNdr[7] =  - q + pq;
}

/*===========================================================================*/
/**
 *  @brief  Returns the volume of the cell.
 *  @return volume of the cell
 */
/*===========================================================================*/
kvs::Real32 HexahedralCell::volume() const
{
    const size_t resolution = 3;
    const float sampling_length = 1.0f / (float)resolution;
    const float adjustment = sampling_length * 0.5f;

    kvs::Vec3 sampling_position( -adjustment, -adjustment, -adjustment );

    float sum_metric = 0;
    for ( size_t k = 0 ; k < resolution ; k++ )
    {
        sampling_position[ 2 ] +=  sampling_length;
        for( size_t j = 0 ; j < resolution ; j++ )
        {
            sampling_position[ 1 ] += sampling_length;
            for( size_t i = 0 ; i < resolution ; i++ )
            {
                sampling_position[ 0 ] += sampling_length;

                this->setLocalPoint( sampling_position );
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
