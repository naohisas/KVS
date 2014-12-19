/*****************************************************************************/
/**
 *  @file   WheelEventListener.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: WheelEventListener.h 1325 2012-10-04 10:34:52Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#ifndef KVS__WHEEL_EVENT_LISTENER_H_INCLUDE
#define KVS__WHEEL_EVENT_LISTENER_H_INCLUDE

#include <kvs/EventListener>
#include <kvs/WheelEvent>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  WheelEventListener class.
 */
/*===========================================================================*/
class WheelEventListener : public kvs::EventListener
{
public:

    WheelEventListener();
    virtual ~WheelEventListener();

    virtual void update( kvs::WheelEvent* event ) = 0;

private:

    void onEvent( kvs::EventBase* event );
};

} // end of namespace kvs

#endif // KVS__WHEEL_EVENT_LISTENER_H_INCLUDE
