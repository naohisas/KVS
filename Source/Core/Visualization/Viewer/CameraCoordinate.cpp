/*****************************************************************************/
/**
 *  @file   CameraCoordinate.cpp
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
#include "CameraCoordinate.h"
#include "NormalizedDeviceCoordinate.h"
#include "WorldCoordinate.h"
#include <kvs/Camera>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new CameraCoordinate class.
 *  @param  position [in] position in camera coordinates
 *  @param  camera [in] pointer to a camera defines camera coordinates
 */
/*===========================================================================*/
CameraCoordinate::CameraCoordinate( const kvs::Vec3& position, const kvs::Camera* camera ):
    m_position( position ),
    m_camera( camera )
{
}

/*===========================================================================*/
/**
 *  @brief  Transforms camera coordinates to normalized device coordinates.
 *  @return normalized device coordinates
 */
/*===========================================================================*/
const NormalizedDeviceCoordinate CameraCoordinate::toNormalizedDeviceCoordinate() const
{
    const kvs::Vec3 position = kvs::Xform( m_camera->projectionMatrix() ).project( m_position );
    return NormalizedDeviceCoordinate( position );
}

/*===========================================================================*/
/**
 *  @brief  Transforms camera coordinate to world coordinates.
 *  @return world coordinates
 */
/*===========================================================================*/
const WorldCoordinate CameraCoordinate::toWorldCoordinate() const
{
    return WorldCoordinate( m_camera->xform().transform( m_position ) );
}

} // end of namespace kvs
