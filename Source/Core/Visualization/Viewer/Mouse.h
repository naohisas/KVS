/****************************************************************************/
/**
 *  @file   Mouse.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: Mouse.h 1713 2014-02-04 05:46:24Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#pragma once

#include <kvs/Trackball>
#include <kvs/Timer>
#include <kvs/Vector2>
#include <kvs/Deprecated>


namespace kvs
{

/*==========================================================================*/
/**
 *  Mouse class
 */
/*==========================================================================*/
class Mouse
{
public:
    enum OperationMode
    {
        Rotation = 0, ///< rotation mode
        Translation = 1, ///< translation mode
        Scaling = 2  ///< scaling mode
    };

private:
    kvs::Trackball m_trackball; ///< trackball
    OperationMode m_operation_mode; ///< operation mode
    kvs::Vec2i m_old; ///< old mouse position on the window coordinate (org: upper-left)
    kvs::Vec2i m_new; ///< new mouse position on the window coordinate (org: upper-left)
    kvs::Vec2i m_start; ///< position at start of rotation
    kvs::Vec2i m_stop; ///< position at stop of rotation
    kvs::Timer m_timer; ///< auto check timer
    bool m_enable_auto_updating; ///< flag for auto-updating of the transformation
    bool m_is_updating; ///< if true, the transformation is now updating
    bool m_is_damping; ///< slow flag (true: if slow mode)

public:
    Mouse();
    virtual ~Mouse();

    void press( const int x, const int y );
    void move( const int x, const int y );
    void wheel( const float value );
    void release( const int x, const int y );
    bool idle();
    void reset();

    void setOperationMode( const OperationMode mode ) { m_operation_mode = mode; }
    OperationMode operationMode() const { return m_operation_mode; }

    kvs::Trackball& trackball() { return m_trackball; }

    void setEnabledAutoUpdating( const bool enable ) { m_enable_auto_updating = enable; }
    void enableAutoUpdating() { this->setEnabledAutoUpdating( true ); }
    void disableAutoUpdating() { this->setEnabledAutoUpdating( false ); }
    bool isEnabledAutoUpdating() const { return m_enable_auto_updating; }
    bool isUpdating() { return m_is_updating; }

    KVS_DEPRECATED( void attachCamera( kvs::Camera* camera ) ) { m_trackball.attachCamera( camera ); }
    KVS_DEPRECATED( void setRotationCenter( const kvs::Vec2& center ) ) { m_trackball.setRotationCenter( center ); }
    KVS_DEPRECATED( const kvs::Vec3& scaling() const ) { return m_trackball.scaling(); }
    KVS_DEPRECATED( const kvs::Vec3& translation() const ) { return m_trackball.translation(); }
    KVS_DEPRECATED( const kvs::Quaternion& rotation() const ) { return m_trackball.rotation(); }
};

} // end of namespace kvs
