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
    this->interpolationFunctions( BaseClass::localPoint() );
    this->differentialFunctions( BaseClass::localPoint() );
}

/*===========================================================================*/
/**
 *  @brief  Destroys the TetrahedralCell class.
 */
/*===========================================================================*/
TetrahedralCell::~TetrahedralCell()
{
}

/*==========================================================================*/
/**
 *  @brief  Calculates the interpolation functions in the local coordinate.
 *  @return local [in] point in the local coordinate
 */
/*==========================================================================*/
const kvs::Real32* TetrahedralCell::interpolationFunctions( const kvs::Vec3& local ) const
{
    const float x = local.x();
    const float y = local.y();
    const float z = local.z();

    kvs::Real32* N = BaseClass::interpolationFunctions();
    N[0] = x;
    N[1] = y;
    N[2] = z;
    N[3] = 1.0f - x - y - z;

    return N;
}

/*==========================================================================*/
/**
 *  @brief  Calculates the differential functions in the local coordinate.
 *  @return local [in] point in the local coordinate
 */
/*==========================================================================*/
const kvs::Real32* TetrahedralCell::differentialFunctions( const kvs::Vec3& local ) const
{
    kvs::IgnoreUnusedVariable( local );

    const size_t nnodes = BaseClass::numberOfCellNodes();
    kvs::Real32* dN = BaseClass::differentialFunctions();
    kvs::Real32* dNdx = dN;
    kvs::Real32* dNdy = dN + nnodes;
    kvs::Real32* dNdz = dN + nnodes + nnodes;

    dNdx[0] =   1.0f;
    dNdx[1] =   0.0f;
    dNdx[2] =   0.0f;
    dNdx[3] =  -1.0f;

    dNdy[0] =   0.0f;
    dNdy[1] =   1.0f;
    dNdy[2] =   0.0f;
    dNdy[3] =  -1.0f;

    dNdz[0] =   0.0f;
    dNdz[1] =   0.0f;
    dNdz[2] =   1.0f;
    dNdz[3] =  -1.0f;

    return dN;
}

/*===========================================================================*/
/**
 *  @brief  Returns the sampled point randomly in the cell.
 *  @return coordinate value of the sampled point
 */
/*===========================================================================*/
const kvs::Vec3 TetrahedralCell::randomSampling() const
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

    this->setLocalPoint( point );
    BaseClass::m_global_point = this->transformLocalToGlobal( point );

    return BaseClass::m_global_point;
}

/*===========================================================================*/
/**
 *  @brief  Returns the volume of the cell.
 *  @return volume of the cell
 */
/*===========================================================================*/
const kvs::Real32 TetrahedralCell::volume() const
{
    const kvs::Vec3 v01( BaseClass::m_vertices[1] - BaseClass::m_vertices[0] );
    const kvs::Vec3 v02( BaseClass::m_vertices[2] - BaseClass::m_vertices[0] );
    const kvs::Vec3 v03( BaseClass::m_vertices[3] - BaseClass::m_vertices[0] );

    return kvs::Math::Abs( ( v01.cross( v02 ) ).dot( v03 ) ) * 0.166666f;
}

/*===========================================================================*/
/**
 *  @brief  Transforms the global to the local coordinate.
 *  @param  global [in] point in the global coordinate
 */
/*===========================================================================*/
const kvs::Vec3 TetrahedralCell::transformGlobalToLocal( const kvs::Vec3& global ) const
{
    const kvs::Vec3 v3( BaseClass::m_vertices[3] );
    const kvs::Vec3 v03( BaseClass::m_vertices[0] - v3 );
    const kvs::Vec3 v13( BaseClass::m_vertices[1] - v3 );
    const kvs::Vec3 v23( BaseClass::m_vertices[2] - v3 );

    const kvs::Mat3 M( v03.x(), v13.x(), v23.x(),
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
const kvs::Vec3 TetrahedralCell::transformLocalToGlobal( const kvs::Vec3& local ) const
{
    const kvs::Vec3 v3( BaseClass::m_vertices[3] );
    const kvs::Vec3 v03( BaseClass::m_vertices[0] - v3 );
    const kvs::Vec3 v13( BaseClass::m_vertices[1] - v3 );
    const kvs::Vec3 v23( BaseClass::m_vertices[2] - v3 );

    const kvs::Mat3 M( v03.x(), v13.x(), v23.x(),
                       v03.y(), v13.y(), v23.y(),
                       v03.z(), v13.z(), v23.z() );

    return M * local + v3;
}

} // end of namespace kvs
