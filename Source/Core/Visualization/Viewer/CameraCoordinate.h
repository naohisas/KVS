/*****************************************************************************/
/**
 *  @file   CameraCoordinate.h
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
#pragma once

#include <kvs/Vector3>


namespace kvs
{

class Camera;
class NormalizedDeviceCoordinate;
class WorldCoordinate;

/*===========================================================================*/
/**
 *  @brief  Camera coordinate class.
 */
/*===========================================================================*/
class CameraCoordinate
{
private:

    kvs::Vec3 m_position; ///< position in camera coordinates
    const kvs::Camera* m_camera; ///< camera defines camera coordinates

public:

    CameraCoordinate( const kvs::Vec3& position, const kvs::Camera* camera );

    const kvs::Vec3& position() const { return m_position; }
    const NormalizedDeviceCoordinate toNormalizedDeviceCoordinate() const;
    const WorldCoordinate toWorldCoordinate() const;
};

} // end of namespace kvs
