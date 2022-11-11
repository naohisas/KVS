/*****************************************************************************/
/**
 *  @file   TargetChangeEvent.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#pragma once
#include <kvs/KeyPressEventListener>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Target change event class.
 */
/*===========================================================================*/
class TargetChangeEvent : public kvs::KeyPressEventListener
{
private:
    int m_object_key = kvs::Key::o; ///< key for controlling object
    int m_light_key = kvs::Key::l; ///< key for controlling light
    int m_camera_key = kvs::Key::c; ///< key for controlling camera

public:
    TargetChangeEvent() = default;

    void setObjectKey( const int object_key ) { m_object_key = object_key; }
    void setLightKey( const int light_key ) { m_light_key = light_key; }
    void setCameraKey( const int camera_key ) { m_camera_key = camera_key; }
    void update( kvs::KeyEvent* event );
};

} // end of namespace kvs
