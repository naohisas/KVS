/****************************************************************************/
/**
 *  @file   Trackball.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: Trackball.h 1713 2014-02-04 05:46:24Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#pragma once

#include <kvs/Vector2>
#include <kvs/Vector3>
#include <kvs/Quaternion>


namespace kvs
{

class Camera;

/*==========================================================================*/
/**
 *  Trackball class
 */
/*==========================================================================*/
class Trackball
{
private:
    float m_size; ///< trackball size
    float m_depth; ///< how near from center
    float m_scaling_factor; ///< scaling factor
    kvs::Vec2 m_rotation_center; ///< center of rotation in the devise coordinate system
    kvs::Vec3 m_scaling; ///< current scaling value
    kvs::Vec3 m_translation; ///< current translation vector
    kvs::Quaternion m_rotation; ///< current rotation quaternion
    kvs::Camera* m_ref_camera; ///< pointer to camera (reference only)

public:
    Trackball();
    virtual ~Trackball();

    void setSize( const float size ) { m_size = size; }
    void setDepth( const float depth ) { m_depth = depth; }
    void setScalingFactor( const float factor ) { m_scaling_factor = factor; }
    void setRotationCenter( const kvs::Vec2& center ) { m_rotation_center = center; }
    void setScaling( const kvs::Vec3& scaling ) { m_scaling = scaling; }
    void setTranslation( const kvs::Vec3& translation ) { m_translation = translation; }
    void setRotation( const kvs::Quaternion& rotation ) { m_rotation = rotation; }

    float size() const { return m_size; }
    float depth() const { return m_depth; }
    float scalingFactor() const { return m_scaling_factor; }
    const kvs::Vec2& rotationCenter() const { return m_rotation_center; }
    const kvs::Vec3& scaling() const { return m_scaling; }
    const kvs::Vec3& translation() const { return m_translation; }
    const kvs::Quaternion& rotation() const { return m_rotation; }

    void attachCamera( kvs::Camera* camera ) { m_ref_camera = camera; }
    void resetRotationCenter() { m_rotation_center = kvs::Vec2::Zero(); }

    void scale( const kvs::Vec2i& start, const kvs::Vec2i& end );
    void rotate( const kvs::Vec2i& start, const kvs::Vec2i& end );
    void translate( const kvs::Vec2i& start, const kvs::Vec2i& end );
    void reset();

private:
    float depth_on_sphere( const kvs::Vec2& pos ) const;
    kvs::Vec2 get_norm_position( const kvs::Vec2i& pos ) const;
};

} // end of namespace kvs
