/*****************************************************************************/
/**
 *  @file   KeyPressEventListener.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: KeyPressEventListener.h 1325 2012-10-04 10:34:52Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#ifndef KVS__KEY_PRESS_EVENT_LISTENER_H_INCLUDE
#define KVS__KEY_PRESS_EVENT_LISTENER_H_INCLUDE

#include <kvs/EventListener>
#include <kvs/KeyEvent>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  KeyPressEventListener class.
 */
/*===========================================================================*/
class KeyPressEventListener : public kvs::EventListener
{
public:

    KeyPressEventListener();
    virtual ~KeyPressEventListener();

    virtual void update( kvs::KeyEvent* event ) = 0;

private:

    void onEvent( kvs::EventBase* event );
};

} // end of namespace kvs

#endif // KVS__KEY_PRESS_EVENT_LISTENER_H_INCLUDE
