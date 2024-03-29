/*****************************************************************************/
/**
 *  @file   ObjectCoordinate.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#pragma once

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
    kvs::Vec3 m_position{}; ///< position in object coordinates
    const kvs::ObjectBase* m_object = nullptr; ///< object defines object coordinates

public:
    ObjectCoordinate( const kvs::Vec3& position, const kvs::ObjectBase* object );

    const kvs::Vec3& position() const { return m_position; }
    const WorldCoordinate toWorldCoordinate() const;
};

} // end of namespace kvs
