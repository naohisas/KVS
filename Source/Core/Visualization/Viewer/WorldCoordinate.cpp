/*****************************************************************************/
/**
 *  @file   WorldCoordinate.cpp
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
#include "WorldCoordinate.h"
#include "CameraCoordinate.h"
#include "ObjectCoordinate.h"
#include <kvs/Camera>
#include <kvs/ObjectBase>
#include <kvs/Xform>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new WorldCoordinate class.
 *  @param  position [in] position in world coordinates
 */
/*===========================================================================*/
WorldCoordinate::WorldCoordinate( const kvs::Vec3& position ):
    m_position( position )
{
}

/*===========================================================================*/
/**
 *  @brief  Transforms world coordinates to camera coordinates.
 *  @param  camera [in] pointer to a camera defines camera coordiantes
 *  @return transformed position in camera coordinates
 */
/*===========================================================================*/
const CameraCoordinate WorldCoordinate::toCameraCoordinate( const kvs::Camera* camera ) const
{
    const kvs::Vec3 position = kvs::Xform( camera->viewingMatrix() ).transform( m_position );
    return CameraCoordinate( position, camera );
}

/*===========================================================================*/
/**
 *  @brief  Transforms the world coordinates to the object coordinates.
 *  @param  object [in] pointer to an object defines object coordiantes
 *  @return transformed position in object coordinates
 */
/*===========================================================================*/
const ObjectCoordinate WorldCoordinate::toObjectCoordinate( const kvs::ObjectBase* object ) const
{
    const kvs::Vec3 position = object->xform().inverse().transform( m_position );
    return ObjectCoordinate( position, object );
}

} // end of namespace kvs
