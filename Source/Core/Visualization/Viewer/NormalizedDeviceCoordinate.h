/*****************************************************************************/
/**
 *  @file   NormalizedDeviceCoordinate.h
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
#ifndef KVS__NORMALIZED_DEVICE_COORDINATE_H_INCLUDE
#define KVS__NORMALIZED_DEVICE_COORDINATE_H_INCLUDE

#include <kvs/Vector3>


namespace kvs
{

class Camera;
class WindowCoordinate;
class CameraCoordinate;

/*===========================================================================*/
/**
 *  @brief  Normalized device coordinate class.
 */
/*===========================================================================*/
class NormalizedDeviceCoordinate
{
private:

    kvs::Vec3 m_position; ///< position in normalized device coordinates

public:

    NormalizedDeviceCoordinate( const kvs::Vec3& position );

    const kvs::Vec3& position() const { return m_position; }
    const WindowCoordinate toWindowCoordinate( const int x, const int y, const size_t width, const size_t height ) const;
    const CameraCoordinate toCameraCoordinate( const kvs::Camera* camera ) const;
};

} // end of namespace kvs

#endif // KVS__NORMALIZED_DEVICE_COORDINATE_H_INCLUDE
