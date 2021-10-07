/*****************************************************************************/
/**
 *  @file   Timer.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#pragma once
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
    int m_id = 0; ///< timer ID
    int m_interval = 0; ///< interval time in millisecond
    bool m_is_stopped = true; ///< checl flag whether the time is stopped
    kvs::TimeEvent* m_time_event = nullptr; ///< time event
    kvs::EventListener* m_event_listener = nullptr; ///< event listener
    kvs::EventHandler* m_event_handler = nullptr; ///< event handler

public:
    Timer( int msec = 0 ):
        m_interval( msec ),
        m_time_event( new kvs::TimeEvent ) {}
    Timer( kvs::EventListener* listener, int msec = 0 ):
        m_interval( msec ),
        m_time_event( new kvs::TimeEvent ),
        m_event_listener( listener ) {}
    Timer( kvs::EventHandler* handler, int msec = 0 ):
        m_interval( msec ),
        m_time_event( new kvs::TimeEvent ),
        m_event_handler( handler ) {}
    virtual ~Timer() { if ( m_time_event ) { delete m_time_event; } }

    void start( int msec );
    void start();
    void stop();

    void setInterval( int msec ) { m_interval = msec; }
    void setEventListener( kvs::EventListener* listener ) { m_event_listener = listener; }
    void setEventHandler( kvs::EventHandler* handler ) { m_event_handler = handler; }
    bool isStopped() const { return m_is_stopped; }
    int interval() const { return m_interval; }

protected:
    virtual void timerEvent( QTimerEvent* event );
};

} // end of namespace qt

} // end of namespace kvs
