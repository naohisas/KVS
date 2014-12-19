/*****************************************************************************/
/**
 *  @file   ObjectCoordinate.h
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
#ifndef KVS__OBJECT_COORDINATE_H_INCLUDE
#define KVS__OBJECT_COORDINATE_H_INCLUDE

#include <kvs/Vector3>


namespace kvs
{

class ObjectBase;
class WorldCoordinate;

/*===========================================================================*/
/**
 *  @brief  Object coordinate class.
 */
/*===========================================================================*/
class ObjectCoordinate
{
private:

    kvs::Vector3f m_position; ///< position in object coordinates
    const kvs::ObjectBase* m_object; ///< object defines object coordinates

public:

    ObjectCoordinate( const kvs::Vec3& position, const kvs::ObjectBase* object );

    const kvs::Vec3& position() const { return m_position; }
    const WorldCoordinate toWorldCoordinate() const;
};

} // end of namespace kvs

#endif // KVS__OBJECT_COORDINATE_H_INCLUDE
