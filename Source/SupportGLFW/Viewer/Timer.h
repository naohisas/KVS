#pragma once
#include <kvs/EventListener>
#include <kvs/EventHandler>
#include <kvs/TimeEvent>
#include "Application.h"


namespace kvs
{

namespace glfw
{

/*===========================================================================*/
/**
 *  @brief  Timer class.
 */
/*===========================================================================*/
class Timer
{
public:
    friend class kvs::glfw::Application;

private:
    int m_id; ///< timer ID
    int m_interval; ///< interval time in millisecond
    bool m_is_stopped; ///< checl flag whether the time is stopped
    kvs::TimeEvent* m_time_event; ///< time event
    kvs::EventListener* m_event_listener; ///< event listener
    kvs::EventHandler* m_event_handler; ///< event handler

    double m_now_time;
    double m_last_time;
    double m_elapse_time;

public:
    Timer( int msec = 0 );
    Timer( kvs::EventListener* listener, int msec = 0 );
    Timer( kvs::EventHandler* handler, int msec = 0 );
    virtual ~Timer();

    void start( int msec );
    void start();
    void stop();
    void setInterval( int msec );
    void setEventListener( kvs::EventListener* listener );
    void setEventHandler( kvs::EventHandler* handler );

    bool isStopped() const;
    int interval() const;

protected:
    void timerEvent();
};

} // end of namespace glfw

} // end of namespace kvs
