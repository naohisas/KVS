/****************************************************************************/
/**
 *  @file   PrismaticCell.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id$
 */
/****************************************************************************/
#include "PrismaticCell.h"


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new PrismaticCell class.
 *  @param  volume [in] pointer to the unstructured volume object
 */
/*===========================================================================*/
PrismaticCell::PrismaticCell(
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
void PrismaticCell::updateInterpolationFunctions( const kvs::Vec3& local ) const
{
    KVS_ASSERT( this->containsLocalPoint( local ) );

    // 0 <= p,q,r <= 1, p + q <= 1
    const float p = local.x();
    const float q = local.y();
    const float r = local.z();

    kvs::Real32* N = BaseClass::interpolationFunctions();
    N[0] = ( 1 - p - q ) * r;
    N[1] = p * r;
    N[2] = q * r;
    N[3] = ( 1 - p - q ) * ( 1 - r );
    N[4] = p * ( 1 - r );
    N[5] = q * ( 1 - r );
}

/*==========================================================================*/
/**
 *  @brief  Calculates the differential functions in the local coordinate.
 *  @return local [in] point in the local coordinate
 */
/*==========================================================================*/
void PrismaticCell::updateDifferentialFunctions( const kvs::Vec3& local ) const
{
    KVS_ASSERT( this->containsLocalPoint( local ) );

    const float p = local.x();
    const float q = local.y();
    const float r = local.z();

    const size_t nnodes = BaseClass::numberOfCellNodes();
    kvs::Real32* dN = BaseClass::differentialFunctions();
    kvs::Real32* dNdp = dN;
    kvs::Real32* dNdq = dNdp + nnodes;
    kvs::Real32* dNdr = dNdq + nnodes;

    dNdp[0] = -r;
    dNdp[1] =  r;
    dNdp[2] =  0;
    dNdp[3] = -( 1 - r );
    dNdp[4] =  ( 1 - r );
    dNdp[5] =  0;

    dNdq[0] = -r;
    dNdq[1] =  0;
    dNdq[2] =  r;
    dNdq[3] = -( 1 - r );
    dNdq[4] =  0;
    dNdq[5] =  ( 1 - r );

    dNdr[0] =  ( 1 - p - q );
    dNdr[1] =  p;
    dNdr[2] =  q;
    dNdr[3] = -( 1 - p - q );
    dNdr[4] = -p;
    dNdr[5] = -q;
}

/*===========================================================================*/
/**
 *  @brief  True if the prismatic cell contains a point defined in the local coordinate.
 *  @param  local [in] local point
 *  @return true if the prismatic cell contains the local point
 */
/*===========================================================================*/
bool PrismaticCell::containsLocalPoint( const kvs::Vec3& local ) const
{
    if ( local.x() < 0 || 1 < local.x() ) { return false; }
    if ( local.y() < 0 || 1 < local.y() ) { return false; }
    if ( local.z() < 0 || 1 < local.z() ) { return false; }
    if ( local.x() + local.y() > 1 ) { return false; }
    return true;
}

/*===========================================================================*/
/**
 *  @brief  Returns the sampled point randomly in the cell.
 *  @return coordinate value of the sampled point
 */
/*===========================================================================*/
kvs::Vec3 PrismaticCell::randomSampling() const
{
    // Generate a point in the local coordinate.
    const float p = BaseClass::randomNumber();
    const float q = BaseClass::randomNumber();
    const float r = BaseClass::randomNumber();

    kvs::Vec3 local;
    if ( p + q > 1.0f )
    {
        local[0] = 1.0f - q;
        local[1] = 1.0f - p;
        local[2] = r;
    }
    else
    {
        local[0] = p;
        local[1] = q;
        local[2] = r;
    }

    return BaseClass::localToGlobal( local );
}

/*===========================================================================*/
/**
 *  @brief  Returns the volume of the cell.
 *  @return volume of the cell
 */
/*===========================================================================*/
kvs::Real32 PrismaticCell::volume() const
{
    const size_t N = 9;
    kvs::Vec3 P[ N ] = {
        kvs::Vec3( 0.3f, 0.3f, 0.2f ),
        kvs::Vec3( 0.6f, 0.3f, 0.2f ),
        kvs::Vec3( 0.3f, 0.6f, 0.2f ),
        kvs::Vec3( 0.3f, 0.3f, 0.5f ),
        kvs::Vec3( 0.6f, 0.3f, 0.5f ),
        kvs::Vec3( 0.3f, 0.6f, 0.5f ),
        kvs::Vec3( 0.3f, 0.3f, 0.8f ),
        kvs::Vec3( 0.6f, 0.3f, 0.8f ),
        kvs::Vec3( 0.3f, 0.6f, 0.8f )
    };

    float S = 0.0f;
    for ( size_t i = 0; i < N; i++ )
    {
        BaseClass::setLocalPoint( P[i] );
        const kvs::Mat3 J = BaseClass::JacobiMatrix();
        const float D = 0.5f * J.determinant();
        S += kvs::Math::Abs<float>( D );
    }

    return S / N;
}

/*===========================================================================*/
/**
 *  @brief  Returns a center of the cell in the local coordinate.
 *  @return center of the cell in the local coordinate
 */
/*===========================================================================*/
kvs::Vec3 PrismaticCell::localCenter() const
{
    return kvs::Vec3( 1.0f / 3.0f, 1.0f / 3.0f, 0.5f );
}

} // end of namespace kvs
