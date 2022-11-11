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
    TimerEventListener( const int msec = -1 ): kvs::EventListener( kvs::EventBase::TimerEvent, msec ) {}
    TimerEventListener( TimerEventFunc func, const int msec = -1 ):
        kvs::EventListener( kvs::EventBase::TimerEvent, msec ) { this->update( func ); }
    virtual ~TimerEventListener() = default;

    void update( TimerEventFunc func ) { timerEvent( func, kvs::EventListener::timerInterval() ); }
    virtual void update( kvs::TimeEvent* event ) { timerEvent( event ); }

private:
    void onEvent( kvs::EventBase* event ) { this->update( static_cast<kvs::TimeEvent*>(event) ); }
};

} // end of namespace kvs
