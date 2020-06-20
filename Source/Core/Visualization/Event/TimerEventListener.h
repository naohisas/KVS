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
    TimerEventListener(): kvs::EventListener( kvs::EventBase::TimerEvent ) {}
    TimerEventListener( TimerEventFunc func ) { this->update( func ); }
    virtual ~TimerEventListener() {}

    void update( TimerEventFunc func ) { timerEvent( func ); }
    virtual void update( kvs::TimeEvent* event ) { timerEvent( event ); }

private:
    void onEvent( kvs::EventBase* event ) { this->update( static_cast<kvs::TimeEvent*>(event) ); }
};

} // end of namespace kvs
