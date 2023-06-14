/*****************************************************************************/
/**
 *  @file   WindowCoordinate.h
 *  @author Naohisa Sakamoto
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
    kvs::Vec3 m_position{}; ///< position in window coordinates
    int m_x = 0; ///< x coordinate value of left corner of the viewport
    int m_y = 0; ///< y coordinate value of left corner of the viewport
    size_t m_width = 0; ///< width of the viewport
    size_t m_height = 0; ///< height of the viewport
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
