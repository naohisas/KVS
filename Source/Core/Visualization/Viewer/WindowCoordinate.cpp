/*****************************************************************************/
/**
 *  @file   WindowCoordinate.cpp
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
/*****************************************************************************/
#include "WindowCoordinate.h"
#include "NormalizedDeviceCoordinate.h"


namespace kvs
{

float WindowCoordinate::m_front = 0.0f;
float WindowCoordinate::m_back = 1.0f;

/*===========================================================================*/
/**
 *  @brief  Sets depth range.
 *  @param  front [in] depth value of the front clipping plane
 *  @param  back [in] depth value of the back clipping plane
 */
/*===========================================================================*/
void WindowCoordinate::SetDepthRange( const float front, const float back )
{
    m_front = front;
    m_back = back;
}

/*===========================================================================*/
/**
 *  @brief  Calculates the depth value.
 *  @param  nd_depth [in] depth value
 *  @return depth value
 */
/*===========================================================================*/
float WindowCoordinate::CalculateDepth( const float nd_depth )
{
    return m_front + ( m_back - m_front ) * nd_depth;
}

/*===========================================================================*/
/**
 *  @brief  Inverts the depth value.
 *  @param  depth [in] depth value
 *  @return depth value
 */
/*===========================================================================*/
float WindowCoordinate::InvertDepth( const float depth )
{
    return ( depth - m_front ) / ( m_back - m_front );
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new WindowCoordinate class.
 *  @param  position [in] position in the window coordinates
 *  @param  x [in] x coordinate value of left corner of the viewport
 *  @param  y [in] y coordinate value of left corner of the viewport
 *  @param  width [in] width of the viewport
 *  @param  height [in] height of the viewport
 */
/*===========================================================================*/
WindowCoordinate::WindowCoordinate(
    const kvs::Vec3& position,
    const int x,
    const int y,
    const size_t width,
    const size_t height ):
    m_position( position ),
    m_x( x ),
    m_y( y ),
    m_width( width ),
    m_height( height )
{
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new WindowCoordinate class.
 *  @param  position [in] position in the window coordinates
 *  @param  viewport [in] viewport vector
 */
/*===========================================================================*/
WindowCoordinate::WindowCoordinate( const kvs::Vec3& position, const kvs::Vec4i& viewport ):
    m_position( position ),
    m_x( viewport[0] ),
    m_y( viewport[1] ),
    m_width( static_cast<size_t>( viewport[2] ) ),
    m_height( static_cast<size_t>( viewport[3] ) )
{
}

/*===========================================================================*/
/**
 *  @brief  Transforms the widnow coordinates to the normalized device coordinates.
 *  @return transformed position in the normalized device coordinates
 */
/*===========================================================================*/
const NormalizedDeviceCoordinate WindowCoordinate::toNormalizedDeviceCoordinate() const
{
    const float x = 2.0f * ( m_position[0] - m_x ) / m_width - 1.0f;
    const float y = 2.0f * ( m_position[1] - m_y ) / m_height - 1.0f;
    const float z = WindowCoordinate::InvertDepth( m_position[2] ) * 2.0f - 1.0f;
    const kvs::Vec3 position( x, y, z );
    return NormalizedDeviceCoordinate( position );
}

} // end of namespace kvs
