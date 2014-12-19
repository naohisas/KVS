/*****************************************************************************/
/**
 *  @file   ObjectCoordinate.cpp
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
#include "ObjectCoordinate.h"
#include "WorldCoordinate.h"
#include <kvs/ObjectBase>
#include <kvs/Assert>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new ObjectCoordinate class.
 *  @param  position [in] position in object coordinates
 *  @param  object [in] pointer to an object defines object coordinates
 */
/*===========================================================================*/
ObjectCoordinate::ObjectCoordinate( const kvs::Vec3& position, const kvs::ObjectBase* object ):
    m_position( position ),
    m_object( object )
{
}

/*===========================================================================*/
/**
 *  @brief  Transforms object coordinates to camera coordinates.
 *  @return world coordinates
 */
/*===========================================================================*/
const WorldCoordinate ObjectCoordinate::toWorldCoordinate() const
{
    KVS_ASSERT( m_object != NULL );

    const kvs::Vec3 position = m_object->xform().transform( m_position );
    return WorldCoordinate( position );
}

} // end of namespace kvs
