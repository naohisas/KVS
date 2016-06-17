/****************************************************************************/
/**
 *  @file   TetrahedralCell.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: TetrahedralCell.cpp 1792 2014-07-31 04:50:42Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#include "TetrahedralCell.h"


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new TetrahedralCell class.
 *  @param  volume [in] pointer to the unstructured volume object
 */
/*===========================================================================*/
TetrahedralCell::TetrahedralCell(
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
void TetrahedralCell::updateInterpolationFunctions( const kvs::Vec3& local ) const
{
    KVS_ASSERT( this->containsLocalPoint( local ) );

    const float p = local.x();
    const float q = local.y();
    const float r = local.z();

    kvs::Real32* N = BaseClass::interpolationFunctions();
    N[0] = p;
    N[1] = q;
    N[2] = r;
    N[3] = 1.0f - p - q - r;
}

/*==========================================================================*/
/**
 *  @brief  Calculates the differential functions in the local coordinate.
 *  @return local [in] point in the local coordinate
 */
/*==========================================================================*/
void TetrahedralCell::updateDifferentialFunctions( const kvs::Vec3& local ) const
{
    KVS_ASSERT( this->containsLocalPoint( local ) );
    kvs::IgnoreUnusedVariable( local );

    const size_t nnodes = BaseClass::numberOfCellNodes();
    kvs::Real32* dN = BaseClass::differentialFunctions();
    kvs::Real32* dNdp = dN;
    kvs::Real32* dNdq = dN + nnodes;
    kvs::Real32* dNdr = dN + nnodes + nnodes;

    dNdp[0] =   1.0f;
    dNdp[1] =   0.0f;
    dNdp[2] =   0.0f;
    dNdp[3] =  -1.0f;

    dNdq[0] =   0.0f;
    dNdq[1] =   1.0f;
    dNdq[2] =   0.0f;
    dNdq[3] =  -1.0f;

    dNdr[0] =   0.0f;
    dNdr[1] =   0.0f;
    dNdr[2] =   1.0f;
    dNdr[3] =  -1.0f;
}

/*===========================================================================*/
/**
 *  @brief  True if the tetrahedra cell contains a point defined in the local coordinate.
 *  @param  local [in] local point
 *  @return true if the tetrahedra cell contains the local point
 */
/*===========================================================================*/
bool TetrahedralCell::containsLocalPoint( const kvs::Vec3& local ) const
{
    if ( local.x() < 0 || 1 < local.x() ) { return false; }
    if ( local.y() < 0 || 1 < local.y() ) { return false; }
    if ( local.z() < 0 || 1 < local.z() ) { return false; }
    if ( local.x() + local.y() + local.z() > 1 ) { return false; }
    return true;
}

/*===========================================================================*/
/**
 *  @brief  Returns the sampled point randomly in the cell.
 *  @return coordinate value of the sampled point
 */
/*===========================================================================*/
kvs::Vec3 TetrahedralCell::randomSampling() const
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

    return this->localToGlobal( local );
}

/*===========================================================================*/
/**
 *  @brief  Returns the volume of the cell.
 *  @return volume of the cell
 */
/*===========================================================================*/
kvs::Real32 TetrahedralCell::volume() const
{
    const kvs::Vec3 v01( BaseClass::coord(1) - BaseClass::coord(0) );
    const kvs::Vec3 v02( BaseClass::coord(2) - BaseClass::coord(0) );
    const kvs::Vec3 v03( BaseClass::coord(3) - BaseClass::coord(0) );

    return kvs::Math::Abs( ( v01.cross( v02 ) ).dot( v03 ) ) * 0.166666f;
}

/*===========================================================================*/
/**
 *  @brief  Returns a center of the cell in the local coordinate.
 *  @return center of the cell in the local coordinate
 */
/*===========================================================================*/
kvs::Vec3 TetrahedralCell::localCenter() const
{
    return kvs::Vec3( 0.25f, 0.25f, 0.25f );
}

/*===========================================================================*/
/**
 *  @brief  Returns the global point for the local point in the class
 *  @return global point
 */
/*===========================================================================*/
kvs::Vec3 TetrahedralCell::globalPoint() const
{
    const kvs::Vec3 v3( BaseClass::coord(3) );
    const kvs::Vec3 v03( BaseClass::coord(0) - v3 );
    const kvs::Vec3 v13( BaseClass::coord(1) - v3 );
    const kvs::Vec3 v23( BaseClass::coord(2) - v3 );

    const kvs::Mat3 M(
        v03.x(), v13.x(), v23.x(),
        v03.y(), v13.y(), v23.y(),
        v03.z(), v13.z(), v23.z() );

    return M * BaseClass::localPoint() + v3;
}

/*===========================================================================*/
/**
 *  @brief  Transforms the global to the local coordinate.
 *  @param  global [in] point in the global coordinate
 */
/*===========================================================================*/
kvs::Vec3 TetrahedralCell::globalToLocal( const kvs::Vec3& global ) const
{
    const kvs::Vec3 v3( BaseClass::coord(3) );
    const kvs::Vec3 v03( BaseClass::coord(0) - v3 );
    const kvs::Vec3 v13( BaseClass::coord(1) - v3 );
    const kvs::Vec3 v23( BaseClass::coord(2) - v3 );

    const kvs::Mat3 M(
        v03.x(), v13.x(), v23.x(),
        v03.y(), v13.y(), v23.y(),
        v03.z(), v13.z(), v23.z() );

    return M.inverted() * ( global - v3 );
}

/*===========================================================================*/
/**
 *  @brief  Transforms the local to the global coordinate.
 *  @param  local [in] point in the local coordinate
 */
/*===========================================================================*/
kvs::Vec3 TetrahedralCell::localToGlobal( const kvs::Vec3& local ) const
{
    this->setLocalPoint( local );
    return this->globalPoint();
}

} // end of namespace kvs
