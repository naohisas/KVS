/****************************************************************************/
/**
 *  @file   Mouse.h
 *  @author Naohisa Sakamoto
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
    kvs::Trackball m_trackball{}; ///< trackball
    OperationMode m_operation_mode = Rotation; ///< operation mode
    kvs::Vec2i m_old{ 0, 0 }; ///< old mouse position on the window coordinate (org: upper-left)
    kvs::Vec2i m_new{ 0, 0 }; ///< new mouse position on the window coordinate (org: upper-left)
    kvs::Vec2i m_start{ 0, 0 }; ///< position at start of rotation
    kvs::Vec2i m_stop{ 0, 0 }; ///< position at stop of rotation
    kvs::Timer m_timer{}; ///< auto check timer
    bool m_enable_auto_updating = true; ///< flag for auto-updating of the transformation
    bool m_is_updating{ false }; ///< if true, the transformation is now updating
    bool m_is_damping{ false }; ///< slow flag (true: if slow mode)

public:
    Mouse() = default;
    virtual ~Mouse() = default;

    void press( const int x, const int y );
    void move( const int x, const int y );
    void wheel( const float value );
    void release( const int x, const int y );
    bool idle();
    void reset();

    void setTrackball( const kvs::Trackball& trackball ) { m_trackball = trackball; }
    void setOperationMode( const OperationMode mode ) { m_operation_mode = mode; }
    void setOperationModeToRotation() { setOperationMode( Rotation ); }
    void setOperationModeToTranslation() { setOperationMode( Translation ); }
    void setOperationModeToScaling() { setOperationMode( Scaling ); }

    kvs::Trackball& trackball() { return m_trackball; }
    OperationMode operationMode() const { return m_operation_mode; }

    void setEnabledAutoUpdating( const bool enable ) { m_enable_auto_updating = enable; }
    void enableAutoUpdating() { this->setEnabledAutoUpdating( true ); }
    void disableAutoUpdating() { this->setEnabledAutoUpdating( false ); }
    bool isEnabledAutoUpdating() const { return m_enable_auto_updating; }
    bool isUpdating() { return m_is_updating; }
};

} // end of namespace kvs
