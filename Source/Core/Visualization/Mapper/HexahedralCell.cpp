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
    this->interpolationFunctions( BaseClass::localPoint() );
    this->differentialFunctions( BaseClass::localPoint() );
}

/*===========================================================================*/
/**
 *  @brief  Destroys the HexahedralCell class.
 */
/*===========================================================================*/
HexahedralCell::~HexahedralCell()
{
}

/*==========================================================================*/
/**
 *  @brief  Calculates the interpolation functions in the local coordinate.
 *  @return local [in] point in the local coordinate
 */
/*==========================================================================*/
const kvs::Real32* HexahedralCell::interpolationFunctions( const kvs::Vec3& local ) const
{
    const float x = local.x();
    const float y = local.y();
    const float z = local.z();
    const float xy = x * y;
    const float yz = y * z;
    const float zx = z * x;
    const float xyz = xy * z;

    kvs::Real32* N = BaseClass::interpolationFunctions();
    N[0] = z - zx - yz + xyz;
    N[1] = zx - xyz;
    N[2] = xyz;
    N[3] = yz - xyz;
    N[4] = 1.0f - x - y - z + xy + yz + zx - xyz;
    N[5] = x - xy - zx + xyz;
    N[6] = xy - xyz;
    N[7] = y - xy - yz + xyz;

    return N;
}

/*==========================================================================*/
/**
 *  @brief  Calculates the differential functions in the local coordinate.
 *  @return local [in] point in the local coordinate
 */
/*==========================================================================*/
const kvs::Real32* HexahedralCell::differentialFunctions( const kvs::Vec3& local ) const
{
    const float x = local.x();
    const float y = local.y();
    const float z = local.z();
    const float xy = x * y;
    const float yz = y * z;
    const float zx = z * x;

    const size_t nnodes = BaseClass::numberOfCellNodes();
    kvs::Real32* dN = BaseClass::differentialFunctions();
    kvs::Real32* dNdx = dN;
    kvs::Real32* dNdy = dN + nnodes;
    kvs::Real32* dNdz = dN + nnodes + nnodes;

    dNdx[0] =  - z + yz;
    dNdx[1] =  z - yz;
    dNdx[2] =  yz;
    dNdx[3] =  - yz;
    dNdx[4] =  - 1.0f + y +z - yz;
    dNdx[5] =  1.0f - y - z + yz;
    dNdx[6] =  y - yz;
    dNdx[7] =  - y + yz;

    dNdy[0] =  - z + zx;
    dNdy[1] =  - zx;
    dNdy[2] =  zx;
    dNdy[3] =  z - zx;
    dNdy[4] =  - 1.0f + x + z - zx;
    dNdy[5] =  - x + zx;
    dNdy[6] =  x - zx;
    dNdy[7] =  1.0f - x - z + zx;

    dNdz[0] =  1.0f - y - x + xy;
    dNdz[1] =  x - xy;
    dNdz[2] =  xy;
    dNdz[3] =  y - xy;
    dNdz[4] =  - 1.0f + y + x - xy;
    dNdz[5] =  - x + xy;
    dNdz[6] =  - xy;
    dNdz[7] =  - y + xy;

    return dN;
}

/*===========================================================================*/
/**
 *  @brief  Returns the sampled point randomly in the cell.
 *  @return coordinate value of the sampled point
 */
/*===========================================================================*/
const kvs::Vec3 HexahedralCell::randomSampling() const
{
    // Generate a point in the local coordinate.
    const float s = BaseClass::randomNumber();
    const float t = BaseClass::randomNumber();
    const float u = BaseClass::randomNumber();

    const kvs::Vec3 point( s, t, u );
    this->setLocalPoint( point );
    BaseClass::m_global_point = BaseClass::transformLocalToGlobal( point );

    return BaseClass::m_global_point;
}

/*===========================================================================*/
/**
 *  @brief  Returns the volume of the cell.
 *  @return volume of the cell
 */
/*===========================================================================*/
const kvs::Real32 HexahedralCell::volume() const
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
