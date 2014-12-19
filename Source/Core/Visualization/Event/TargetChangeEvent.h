/*****************************************************************************/
/**
 *  @file   TargetChangeEvent.h
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
#ifndef KVS__TARGET_CHANGE_EVENT_H_INCLUDE
#define KVS__TARGET_CHANGE_EVENT_H_INCLUDE

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

    int m_object_key; ///< key for controlling object
    int m_light_key; ///< key for controlling light
    int m_camera_key; ///< key for controlling camera

public:

    TargetChangeEvent();

    void setObjectKey( const int object_key ) { m_object_key = object_key; }
    void setLightKey( const int light_key ) { m_light_key = light_key; }
    void setCameraKey( const int camera_key ) { m_camera_key = camera_key; }
    void update( kvs::KeyEvent* event );
};

} // end of namespace kvs

#endif // KVS__TARGET_CHANGE_EVENT_H_INCLUDE
