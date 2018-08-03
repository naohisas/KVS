/****************************************************************************/
/**
 *  @file   PyramidalCell.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: PyramidalCell.cpp 1792 2014-07-31 04:50:42Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#include "PyramidalCell.h"


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new PyramidalCell class.
 *  @param  volume [in] pointer to the unstructured volume object
 */
/*===========================================================================*/
PyramidalCell::PyramidalCell( const kvs::UnstructuredVolumeObject* volume ):
    kvs::CellBase( volume ),
    m_pyramid( 0, 0, 0 )
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
void PyramidalCell::updateInterpolationFunctions( const kvs::Vec3& local ) const
{
    KVS_ASSERT( this->containsLocalPoint( local ) );

    float p = local.x();
    float q = local.y();
    const float r = local.z();
    if ( 1 - r != 0.0 )
    {
        p = p / ( 1 - r ) * 2;
        q = q / ( 1 - r ) * 2;
    }
    else
    {
        p = p * 2;
        q = q * 2;
    }

    const float pq = p * q;

    kvs::Real32* N = BaseClass::interpolationFunctions();
    N[0] = r;
    N[1] = 0.25f * ( 1 - r ) * ( 1 - p - q + pq );
    N[2] = 0.25f * ( 1 - r ) * ( 1 + p - q - pq );
    N[3] = 0.25f * ( 1 - r ) * ( 1 + p + q + pq );
    N[4] = 0.25f * ( 1 - r ) * ( 1 - p + q - pq );
}

/*==========================================================================*/
/**
 *  @brief  Calculates the differential functions in the local coordinate.
 *  @return local [in] point in the local coordinate
 */
/*==========================================================================*/
void PyramidalCell::updateDifferentialFunctions( const kvs::Vec3& local ) const
{
    KVS_ASSERT( this->containsLocalPoint( local ) );

    float p = local.x();
    float q = local.y();
    const float r = local.z();
    if ( 1 - r != 0.0 )
    {
        p = p / ( 1 - r ) * 2;
        q = q / ( 1 - r ) * 2;
    }
    else
    {
        p = p * 2;
        q = q * 2;
    }

    const float pq = p * q;
    const size_t nnodes = BaseClass::numberOfCellNodes();
    kvs::Real32* dN = BaseClass::differentialFunctions();
    kvs::Real32* dNdp = dN;
    kvs::Real32* dNdq = dNdp + nnodes;
    kvs::Real32* dNdr = dNdq + nnodes;

    dNdp[0] = 0.0f;
    dNdp[1] = 0.25f * ( - 1 + q );
    dNdp[2] = 0.25f * (   1 - q );
    dNdp[3] = 0.25f * (   1 + q );
    dNdp[4] = 0.25f * ( - 1 - q );

    dNdq[0] = 0.0f;
    dNdq[1] = 0.25f * ( - 1 + p );
    dNdq[2] = 0.25f * ( - 1 - p );
    dNdq[3] = 0.25f * (   1 + p );
    dNdq[4] = 0.25f * (   1 - p );

    dNdr[0] = 1.0f;
    dNdr[1] = 0.25f * ( - 1 - pq );
    dNdr[2] = 0.25f * ( - 1 + pq );
    dNdr[3] = 0.25f * ( - 1 - pq );
    dNdr[4] = 0.25f * ( - 1 + pq );
}

/*===========================================================================*/
/**
 *  @brief  Binds a cell indicated by the given index.
 *  @param  index [in] index of the cell
 */
/*===========================================================================*/
void PyramidalCell::bindCell( const kvs::UInt32 index )
{
    BaseClass::bindCell( index );

    kvs::Vec3 v12( BaseClass::coord(2) - BaseClass::coord(1) );
    kvs::Vec3 v14( BaseClass::coord(4) - BaseClass::coord(1) );
    kvs::Vec3 v10( BaseClass::coord(0) - BaseClass::coord(1) );

    m_pyramid.x() = (float)v12.length();
    m_pyramid.y() = (float)v14.length();
    kvs::Vec3 height = ( v12.cross( v14 ) ).normalized();
    height.x() = height.x() * v10.x();
    height.y() = height.y() * v10.y();
    height.z() = height.z() * v10.z();
    m_pyramid.z() = (float)height.length();
}

/*===========================================================================*/
/**
 *  @brief  True if the pyramid cell contains a point defined in the local coordinate.
 *  @param  local [in] local point
 *  @return True if the pyramid cell contains the local point
 */
/*===========================================================================*/
bool PyramidalCell::containsLocalPoint( const kvs::Vec3& local ) const
{
    if ( local.z() < 0 || 1 < local.z() ) { return false; }
    if ( kvs::Math::Abs( local.x() ) > 1 - local.z() ) { return false; }
    if ( kvs::Math::Abs( local.y() ) > 1 - local.z() ) { return false; }
    return true;
}

/*===========================================================================*/
/**
 *  @brief  Returns a volume of the cell.
 *  @return volume of the cell
 */
/*===========================================================================*/
kvs::Real32 PyramidalCell::volume() const
{
    return float( m_pyramid.x() * m_pyramid.y() * m_pyramid.z() / 3 );
}

/*===========================================================================*/
/**
 *  @brief  Returns a center of the cell in the local coordinate.
 *  @return center of the cell in the local coordinate
 */
/*===========================================================================*/
kvs::Vec3 PyramidalCell::localCenter() const
{
    return kvs::Vec3( 0.0f, 0.0f, 0.2f );
}

/*===========================================================================*/
/**
 *  @brief  Returns a global point in the cell randomly.
 *  @return global point
 */
/*===========================================================================*/
kvs::Vec3 PyramidalCell::randomSampling() const
{
    // Generate a point in the local coordinate.
    const float p = BaseClass::randomNumber();
    const float q = BaseClass::randomNumber();
    const float r = BaseClass::randomNumber();

    kvs::Vec3 local;
    if ( kvs::Math::Abs( p - 0.5f ) <= kvs::Math::Abs( q - 0.5f ) &&
         kvs::Math::Abs( q - 0.5f ) > kvs::Math::Abs( r - 0.5f ) )
    {
        local[0] = r - 0.5f;
        local[1] = p - 0.5f;
        local[2] = ( 0.5f - kvs::Math::Abs( q - 0.5f ) ) * 2.0f;
    }
    else if ( kvs::Math::Abs( r - 0.5f ) <= kvs::Math::Abs( p - 0.5f ) &&
              kvs::Math::Abs( q - 0.5f ) < kvs::Math::Abs( p - 0.5f ) )
    {
        local[0] = q - 0.5f;
        local[1] = r - 0.5f;
        local[2] = ( 0.5f - kvs::Math::Abs( p - 0.5f ) ) * 2.0f;
    }
    else
    {
        local[0] = p - 0.5f;
        local[1] = q - 0.5f;
        local[2] = ( 0.5f - kvs::Math::Abs( r - 0.5 ) ) * 2.0f;
    }

    return BaseClass::localToGlobal( local );
}

} // end of namespace kvs
