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
    this->interpolationFunctions( BaseClass::localPoint() );
    this->differentialFunctions( BaseClass::localPoint() );
}

/*===========================================================================*/
/**
 *  @brief  Destroys the QuadraticTetrahedralCell class.
 */
/*===========================================================================*/
QuadraticTetrahedralCell::~QuadraticTetrahedralCell()
{
}

/*==========================================================================*/
/**
 *  @brief  Calculates the interpolation functions in the local coordinate.
 *  @return point [in] point in the local coordinate
 */
/*==========================================================================*/
const kvs::Real32* QuadraticTetrahedralCell::interpolationFunctions( const kvs::Vec3& point ) const
{
    const float x = point[0];
    const float y = point[1];
    const float z = point[2];
    const float w = 1 - x - y - z;

    const float xy = x * y;
    const float yz = y * z;
    const float zx = z * x;

    kvs::Real32* N = BaseClass::interpolationFunctions();
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

    return N;
}

/*==========================================================================*/
/**
 *  @brief  Calculates the differential functions in the local coordinate.
 *  @return point [in] point in the local coordinate
 */
/*==========================================================================*/
const kvs::Real32* QuadraticTetrahedralCell::differentialFunctions( const kvs::Vec3& point ) const
{
    const float x = point.x();
    const float y = point.y();
    const float z = point.z();
    const float w = 1 - x - y - z;

    const size_t nnodes = BaseClass::numberOfCellNodes();
    kvs::Real32* dN = BaseClass::differentialFunctions();
    kvs::Real32* dNdx = dN;
    kvs::Real32* dNdy = dNdx + nnodes;
    kvs::Real32* dNdz = dNdy + nnodes;

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

    return dN;
}

/*===========================================================================*/
/**
 *  @brief  Returns the sampled point randomly.
 *  @return coordinate value of the sampled point
 */
/*===========================================================================*/
const kvs::Vec3 QuadraticTetrahedralCell::randomSampling() const
{
    // Generate a point in the local coordinate.
    const float s = BaseClass::randomNumber();
    const float t = BaseClass::randomNumber();
    const float u = BaseClass::randomNumber();

    kvs::Vec3 point;
    if ( s + t + u <= 1.0f )
    {
        point[0] = s;
        point[1] = t;
        point[2] = u;
    }
    else if ( s - t + u >= 1.0f )
    {
        // Revise the point.
        point[0] = -u + 1.0f;
        point[1] = -s + 1.0f;
        point[2] =  t;
    }
    else if ( s + t - u >= 1.0f )
    {
        // Revise the point.
        point[0] = -s + 1.0f;
        point[1] = -t + 1.0f;
        point[2] =  u;
    }
    else if ( -s + t + u >= 1.0f )
    {
        // Revise the point.
        point[0] = -u + 1.0f;
        point[1] =  s;
        point[2] = -t + 1.0f;
    }
    else
    {
        // Revise the point.
        point[0] =   0.5f * s - 0.5f * t - 0.5f * u + 0.5f;
        point[1] = - 0.5f * s + 0.5f * t - 0.5f * u + 0.5f;
        point[2] = - 0.5f * s - 0.5f * t + 0.5f * u + 0.5f;
    }

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
const kvs::Real32 QuadraticTetrahedralCell::volume() const
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
