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
    this->interpolationFunctions( BaseClass::localPoint() );
    this->differentialFunctions( BaseClass::localPoint() );
}

/*===========================================================================*/
/**
 *  @brief  Destructs the unstructured volume object.
 */
/*===========================================================================*/
PyramidalCell::~PyramidalCell()
{
}

/*==========================================================================*/
/**
 *  @brief  Calculates the interpolation functions in the local coordinate.
 *  @return point [in] point in the local coordinate
 */
/*==========================================================================*/
const kvs::Real32* PyramidalCell::interpolationFunctions( const kvs::Vec3& point ) const
{
    const float z = point.z();
    float x;
    float y;
    if ( 1 - z != 0.0 )
    {
        x = point.x() / ( 1 - z ) * 2;
        y = point.y() / ( 1 - z ) * 2;
    }
    else
    {
        x = point.x() * 2;
        y = point.y() * 2;
    }

    const float xy = x * y;

    kvs::Real32* N = BaseClass::interpolationFunctions();
    N[0] = z;
    N[1] = 0.25f * ( 1 - z ) * ( 1 - x - y + xy );
    N[2] = 0.25f * ( 1 - z ) * ( 1 + x - y - xy );
    N[3] = 0.25f * ( 1 - z ) * ( 1 + x + y + xy );
    N[4] = 0.25f * ( 1 - z ) * ( 1 - x + y - xy );

    return N;
}

/*==========================================================================*/
/**
 *  @brief  Calculates the differential functions in the local coordinate.
 *  @return point [in] point in the local coordinate
 */
/*==========================================================================*/
const kvs::Real32* PyramidalCell::differentialFunctions( const kvs::Vec3& point ) const
{
    const float z = point.z();
    float x;
    float y;
    if ( 1 - z != 0.0 )
    {
        x = point.x() / ( 1 - z ) * 2;
        y = point.y() / ( 1 - z ) * 2;
    }
    else
    {
        x = point.x() * 2;
        y = point.y() * 2;
    }

    const float xy = x * y;

    const size_t nnodes = BaseClass::numberOfCellNodes();
    kvs::Real32* dN = BaseClass::differentialFunctions();
    kvs::Real32* dNdx = dN;
    kvs::Real32* dNdy = dNdx + nnodes;
    kvs::Real32* dNdz = dNdy + nnodes;

    dNdx[0] = 0.0f;
    dNdx[1] = 0.25f * ( - 1 + y );
    dNdx[2] = 0.25f * ( 1 - y );
    dNdx[3] = 0.25f * ( 1 + y );
    dNdx[4] = 0.25f * ( - 1 - y );

    dNdy[0] = 0.0f;
    dNdy[1] = 0.25f * ( - 1 + x );
    dNdy[2] = 0.25f * ( - 1 - x );
    dNdy[3] = 0.25f * ( 1 + x );
    dNdy[4] = 0.25f * ( 1 - x );

    dNdz[0] = 1.0f;
    dNdz[1] = 0.25f * ( - 1 - xy );
    dNdz[2] = 0.25f * ( - 1 + xy );
    dNdz[3] = 0.25f * ( - 1 - xy );
    dNdz[4] = 0.25f * ( - 1 + xy );

    return dN;
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

    kvs::Vec3 v12( BaseClass::m_vertices[2] - BaseClass::m_vertices[1] );
    kvs::Vec3 v14( BaseClass::m_vertices[4] - BaseClass::m_vertices[1] );
    kvs::Vec3 v10( BaseClass::m_vertices[0] - BaseClass::m_vertices[1] );

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
 *  @brief  Returns a volume of the cell.
 *  @return volume of the cell
 */
/*===========================================================================*/
const kvs::Real32 PyramidalCell::volume() const
{
    return float( m_pyramid.x() * m_pyramid.y() * m_pyramid.z() / 3);
}

/*===========================================================================*/
/**
 *  @brief  Returns a global point in the cell randomly.
 *  @return global point
 */
/*===========================================================================*/
const kvs::Vec3 PyramidalCell::randomSampling() const
{
    // Generate a point in the local coordinate.
    const float s = BaseClass::randomNumber();
    const float t = BaseClass::randomNumber();
    const float u = BaseClass::randomNumber();

    kvs::Vec3 point;
    if ( kvs::Math::Abs( s - 0.5f ) <= kvs::Math::Abs( t - 0.5f ) &&
         kvs::Math::Abs( t - 0.5f ) > kvs::Math::Abs( u - 0.5f ) )
    {
        point.x() = u - 0.5f;
        point.y() = s - 0.5f;
        point.z() = ( 0.5f - kvs::Math::Abs( t - 0.5f ) ) * 2.0f;
    }
    else if ( kvs::Math::Abs( u - 0.5f ) <= kvs::Math::Abs( s - 0.5f ) &&
              kvs::Math::Abs( t - 0.5f ) < kvs::Math::Abs( s - 0.5f ) )
    {
        point.x() = t - 0.5f;
        point.y() = u - 0.5f;
        point.z() = ( 0.5f - kvs::Math::Abs( s - 0.5f ) ) * 2.0f;
    }
    else
    {
        point.x() = s - 0.5f;
        point.y() = t - 0.5f;
        point.z() = ( 0.5f - kvs::Math::Abs( u - 0.5 ) ) * 2.0f;
    }

    this->setLocalPoint( point );
    BaseClass::m_global_point = this->transformLocalToGlobal( point );

    return BaseClass::m_global_point;
}

} // end of namespace kvs
