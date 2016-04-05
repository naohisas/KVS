/*****************************************************************************/
/**
 *  @file   WorldCoordinate.h
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
#include <kvs/Matrix44>


namespace kvs
{

class Camera;
class ObjectBase;
class CameraCoordinate;
class ObjectCoordinate;

/*===========================================================================*/
/**
 *  @brief  World coordinate class.
 */
/*===========================================================================*/
class WorldCoordinate
{
private:

    kvs::Vec3 m_position; ///< position in world coordinates

public:

    WorldCoordinate( const kvs::Vec3& position );

    const kvs::Vec3& position() const { return m_position; }
    const CameraCoordinate toCameraCoordinate( const kvs::Camera* camera ) const;
    const ObjectCoordinate toObjectCoordinate( const kvs::ObjectBase* object ) const;
};

} // end of namespace kvs
