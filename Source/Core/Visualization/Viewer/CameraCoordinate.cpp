/*****************************************************************************/
/**
 *  @file   CameraCoordinate.cpp
 *  @author Naohisa Sakamoto
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
const kvs::NormalizedDeviceCoordinate CameraCoordinate::toNormalizedDeviceCoordinate() const
{
    const auto p = kvs::Xform( m_camera->projectionMatrix() ).project( m_position );
    return kvs::NormalizedDeviceCoordinate( p );
}

/*===========================================================================*/
/**
 *  @brief  Transforms camera coordinate to world coordinates.
 *  @return world coordinates
 */
/*===========================================================================*/
const kvs::WorldCoordinate CameraCoordinate::toWorldCoordinate() const
{
    return kvs::WorldCoordinate( m_camera->xform().transform( m_position ) );
}

} // end of namespace kvs
