/*****************************************************************************/
/**
 *  @file   NormalizedDeviceCoordinate.cpp
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
#include "NormalizedDeviceCoordinate.h"
#include "WindowCoordinate.h"
#include "CameraCoordinate.h"
#include <kvs/Camera>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new NormalizedDeviceCoordinate class.
 *  @param  position [in] position in normalized device coordinates
 */
/*===========================================================================*/
NormalizedDeviceCoordinate::NormalizedDeviceCoordinate( const kvs::Vec3& position ):
    m_position( position )
{
}

/*===========================================================================*/
/**
 *  @brief  Transforms normalized device coordinates to world coordinates.
 *  @param  x [in] x coordinate value of left corner of the viewport
 *  @param  y [in] y coordinate value of left corner of the viewport
 *  @param  width [in] width of the viewport
 *  @param  height [in] height of the viewport
 *  @return world coordinates
 */
/*===========================================================================*/
const WindowCoordinate NormalizedDeviceCoordinate::toWindowCoordinate(
    const int x,
    const int y,
    const size_t width,
    const size_t height ) const
{
    const float px = ( m_position[0] + 1.0f ) * width * 0.5f + x;
    const float py = ( m_position[1] + 1.0f ) * height * 0.5f + y;
    const float pz = WindowCoordinate::CalculateDepth( ( m_position[2] + 1.0f ) / 2.0f );
    const kvs::Vec3 position( px, py, pz );
    return WindowCoordinate( position, x, y, width, height );
}

/*===========================================================================*/
/**
 *  @brief  Transforms normalized device coordinates to world coordinates.
 *  @param  viewport [in] viewport vector
 *  @return world coordinates
 */
/*===========================================================================*/
const WindowCoordinate NormalizedDeviceCoordinate::toWindowCoordinate(
    const kvs::Vec4i& viewport ) const
{
    return this->toWindowCoordinate(
        viewport[0],
        viewport[1],
        static_cast<size_t>( viewport[2] ),
        static_cast<size_t>( viewport[3] ) );
}

/*===========================================================================*/
/**
 *  @brief  Transforms normalized device coordinates to camera coordinates.
 *  @param  camera [in] pointer to a camera defines camera coordinates
 *  @return camera coordinates
 */
/*===========================================================================*/
const CameraCoordinate NormalizedDeviceCoordinate::toCameraCoordinate( const kvs::Camera* camera ) const
{
    const kvs::Xform xform( camera->projectionMatrix() );
    const kvs::Vec3 position = xform.inverse().project( m_position );
    return CameraCoordinate( position, camera );
}

} // end of namespace kvs
