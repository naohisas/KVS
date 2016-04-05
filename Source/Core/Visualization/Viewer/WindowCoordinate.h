/*****************************************************************************/
/**
 *  @file   WindowCoordinate.h
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
#include <kvs/Vector4>


namespace kvs
{

class NormalizedDeviceCoordinate;

/*===========================================================================*/
/**
 *  @brief  Window coordinate class.
 */
/*===========================================================================*/
class WindowCoordinate
{
private:

    kvs::Vec3 m_position; ///< position in window coordinates
    int m_x; ///< x coordinate value of left corner of the viewport
    int m_y; ///< y coordinate value of left corner of the viewport
    size_t m_width; ///< width of the viewport
    size_t m_height; ///< height of the viewport
    static float m_front; ///< depth value of the front clipping plane
    static float m_back; ///< depth value of the back clipping plane

public:

    static void SetDepthRange( const float front, const float back );
    static float CalculateDepth( const float nd_depth );
    static float InvertDepth( const float depth );

public:

    WindowCoordinate( const kvs::Vec3& position, const int x, const int y, const size_t width, const size_t height );
    WindowCoordinate( const kvs::Vec3& position, const kvs::Vec4i& viewport );

    const kvs::Vec3& position() const { return m_position; }
    const NormalizedDeviceCoordinate toNormalizedDeviceCoordinate() const;
};

} // end of namespace kvs
