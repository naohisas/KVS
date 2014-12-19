/*****************************************************************************/
/**
 *  @file   TimerEventListener.h
 *  @author Naohisa Sakamoto
 *  @brief  
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: TimerEventListener.h 1325 2012-10-04 10:34:52Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#ifndef KVS__TIMER_EVENT_LISTENER_H_INCLUDE
#define KVS__TIMER_EVENT_LISTENER_H_INCLUDE

#include <kvs/EventListener>
#include <kvs/TimeEvent>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  TimerEventListener class.
 */
/*===========================================================================*/
class TimerEventListener : public kvs::EventListener
{
public:

    TimerEventListener();
    virtual ~TimerEventListener();

    virtual void update( kvs::TimeEvent* event ) = 0;

private:

    void onEvent( kvs::EventBase* event );
};

} // end of namespace kvs

#endif // KVS__TIMER_EVENT_LISTENER_H_INCLUDE
