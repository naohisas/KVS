/****************************************************************************/
/**
 *  @file   QuadraticTetrahedralCell.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: QuadraticTetrahedralCell.cpp 1818 2014-10-09 02:51:44Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#include "QuadraticTetrahedralCell.h"


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new QuadraticTetrahedralCell class.
 *  @param  volume [in] pointer to the unstructured volume object
 */
/*===========================================================================*/
QuadraticTetrahedralCell::QuadraticTetrahedralCell(
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
void QuadraticTetrahedralCell::updateInterpolationFunctions( const kvs::Vec3& local ) const
{
    KVS_ASSERT( BaseClass::containsLocalPoint( local ) );

    const float p = local.x();
    const float q = local.y();
    const float r = local.z();
    const float w = 1 - p - q - r;

    kvs::Real32* N = BaseClass::interpolationFunctions();
    N[0] = w * (2 * w - 1); // (0, 0, 0)
    N[1] = p * (2 * p - 1); // (1, 0, 0)
    N[2] = r * (2 * r - 1); // (0, 0, 1)
    N[3] = q * (2 * q - 1); // (0, 1, 0)
    N[4] = 4 * p * w; // (1/2,   0,   0)
    N[5] = 4 * r * w; // (  0,   0, 1/2)
    N[6] = 4 * q * w; // (  0, 1/2,   0)
    N[7] = 4 * r * p; // (1/2,   0, 1/2)
    N[8] = 4 * q * r; // (  0, 1/2, 1/2)
    N[9] = 4 * p * q; // (1/2, 1/2,   0)
}

/*==========================================================================*/
/**
 *  @brief  Calculates the differential functions in the local coordinate.
 *  @return local [in] point in the local coordinate
 */
/*==========================================================================*/
void QuadraticTetrahedralCell::updateDifferentialFunctions( const kvs::Vec3& local ) const
{
    KVS_ASSERT( BaseClass::containsLocalPoint( local ) );

    const float p = local.x();
    const float q = local.y();
    const float r = local.z();
    const float w = 1 - p - q - r;

    const size_t nnodes = BaseClass::numberOfCellNodes();
    kvs::Real32* dN = BaseClass::differentialFunctions();
    kvs::Real32* dNdp = dN;
    kvs::Real32* dNdq = dNdp + nnodes;
    kvs::Real32* dNdr = dNdq + nnodes;

    // dNdp
    dNdp[0] = -4 * w + 1;
    dNdp[1] =  4 * p - 1;
    dNdp[2] =  0;
    dNdp[3] =  0;
    dNdp[4] =  4 * (w - p);
    dNdp[5] = -4 * r;
    dNdp[6] = -4 * q;
    dNdp[7] =  4 * r;
    dNdp[8] =  0;
    dNdp[9] =  4 * q;

    // dNdq
    dNdq[0] = -4 * w + 1;
    dNdq[1] =  0;
    dNdq[2] =  0;
    dNdq[3] =  4 * q - 1;
    dNdq[4] = -4 * p;
    dNdq[5] = -4 * r;
    dNdq[6] =  4 * (w - q);
    dNdq[7] =  0;
    dNdq[8] =  4 * r;
    dNdq[9] =  4 * p;

    // dNdr
    dNdr[0] = -4 * w + 1;
    dNdr[1] =  0;
    dNdr[2] =  4 * r - 1;
    dNdr[3] =  0;
    dNdr[4] = -4 * p;
    dNdr[5] =  4 * (w - r);
    dNdr[6] = -4 * q;
    dNdr[7] =  4 * p;
    dNdr[8] =  4 * q;
    dNdr[9] =  0;
}

/*===========================================================================*/
/**
 *  @brief  Returns the sampled point randomly.
 *  @return coordinate value of the sampled point
 */
/*===========================================================================*/
kvs::Vec3 QuadraticTetrahedralCell::randomSampling() const
{
    // Generate a point in the local coordinate.
    const float p = BaseClass::randomNumber();
    const float q = BaseClass::randomNumber();
    const float r = BaseClass::randomNumber();

    kvs::Vec3 local;
    if ( p + q + r <= 1.0f )
    {
        local[0] = p;
        local[1] = q;
        local[2] = r;
    }
    else if ( p - q + r >= 1.0f )
    {
        // Revise the point.
        local[0] = -r + 1.0f;
        local[1] = -p + 1.0f;
        local[2] =  q;
    }
    else if ( p + q - r >= 1.0f )
    {
        // Revise the point.
        local[0] = -p + 1.0f;
        local[1] = -q + 1.0f;
        local[2] =  r;
    }
    else if ( -p + q + r >= 1.0f )
    {
        // Revise the point.
        local[0] = -r + 1.0f;
        local[1] =  p;
        local[2] = -q + 1.0f;
    }
    else
    {
        // Revise the point.
        local[0] =   0.5f * p - 0.5f * q - 0.5f * r + 0.5f;
        local[1] = - 0.5f * p + 0.5f * q - 0.5f * r + 0.5f;
        local[2] = - 0.5f * p - 0.5f * q + 0.5f * r + 0.5f;
    }

    return BaseClass::localToGlobal( local );
}

/*===========================================================================*/
/**
 *  @brief  Returns the volume of the cell.
 *  @return volume of the cell
 */
/*===========================================================================*/
kvs::Real32 QuadraticTetrahedralCell::volume() const
{
    const kvs::Vec3 v0(   0,  0,  0 );
    const kvs::Vec3 v1(   1,  0,  0 );
    const kvs::Vec3 v2(   0,  0,  1 );
    const kvs::Vec3 v3(   0,  1,  0 );
    const kvs::Vec3 v4( 0.5,  0,  0 );
    const kvs::Vec3 v5(   0,  0,0.5 );
    const kvs::Vec3 v6(   0,0.5,  0 );
    const kvs::Vec3 v7( 0.5,  0,0.5 );
    const kvs::Vec3 v8(   0,0.5,0.5 );
    const kvs::Vec3 v9( 0.5,0.5,  0 );

    const kvs::Vec3 c[8] = {
        ( v0 + v4 + v5 + v6 ) * 0.25,
        ( v4 + v1 + v7 + v9 ) * 0.25,
        ( v5 + v7 + v2 + v8 ) * 0.25,
        ( v6 + v9 + v8 + v3 ) * 0.25,
        ( v4 + v7 + v5 + v6 ) * 0.25,
        ( v4 + v9 + v7 + v6 ) * 0.25,
        ( v8 + v6 + v5 + v7 ) * 0.25,
        ( v8 + v7 + v9 + v6 ) * 0.25 };

    float sum_metric = 0;
    for( size_t i = 0 ; i < 8 ; i++ )
    {
        BaseClass::setLocalPoint( c[i] );
        const kvs::Mat3 J = BaseClass::JacobiMatrix();
        const float metric_element = J.determinant();

        sum_metric += kvs::Math::Abs<float>( metric_element );
    }

    return sum_metric / ( 6.0f * 8.0f );
}

} // end of namespace kvs
