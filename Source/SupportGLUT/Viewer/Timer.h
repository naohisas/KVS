/*****************************************************************************/
/**
 *  @file   Timer.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: Timer.h 1719 2014-03-12 06:42:32Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#pragma once
#include <kvs/EventListener>
#include <kvs/EventHandler>
#include <kvs/TimeEvent>


namespace kvs
{

namespace glut
{

/*===========================================================================*/
/**
 *  @brief  Timer class.
 */
/*===========================================================================*/
class Timer
{
protected:
    int m_id; ///< timer ID
    int m_interval; ///< interval time in millisecond
    bool m_is_stopped; ///< checl flag whether the time is stopped
    kvs::TimeEvent* m_time_event; ///< time event
    kvs::EventListener* m_event_listener; ///< event listener
    kvs::EventHandler* m_event_handler; ///< event handler

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
    friend void TimerEventFunction( int id );
};

} // end of namespace glut

} // end of namespace kvs
