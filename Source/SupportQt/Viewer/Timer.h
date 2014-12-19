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
 *  $Id: Timer.h 634 2010-10-13 07:04:05Z naohisa.sakamoto $
 */
/*****************************************************************************/
#ifndef KVS__QT__TIMER_H_INCLUDE
#define KVS__QT__TIMER_H_INCLUDE

#include <kvs/qt/Qt>
#include <kvs/EventListener>
#include <kvs/EventHandler>
#include <kvs/TimeEvent>
#include <kvs/ClassName>


namespace kvs
{

namespace qt
{

/*===========================================================================*/
/**
 *  @brief  Timer class.
 */
/*===========================================================================*/
class Timer : public QObject
{
    kvsClassName( kvs::qt::Timer );

protected:

    int                 m_id;             ///< timer ID
    int                 m_interval;       ///< interval time in millisecond
    bool                m_is_stopped;     ///< checl flag whether the time is stopped
    kvs::TimeEvent*     m_time_event;     ///< time event
    kvs::EventListener* m_event_listener; ///< event listener
    kvs::EventHandler*  m_event_handler;  ///< event handler

public:

    Timer( int msec = 0 );

    Timer( kvs::EventListener* listener, int msec = 0 );

    Timer( kvs::EventHandler* handler, int msec = 0 );

    virtual ~Timer( void );

    void start( int msec );

    void start( void );

    void stop( void );

    void setInterval( int msec );

    void setEventListener( kvs::EventListener* listener );

    void setEventHandler( kvs::EventHandler* handler );

    const bool isStopped( void ) const;

    const int interval( void ) const;

protected:

    virtual void timerEvent( QTimerEvent* event );
};

} // end of namespace qt

} // end of namespace kvs

#endif // KVS__QT__TIMER_H_INCLUDE
