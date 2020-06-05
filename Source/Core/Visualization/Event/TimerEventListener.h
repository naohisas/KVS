/*****************************************************************************/
/**
 *  @file   TimerEventListener.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#pragma once
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
