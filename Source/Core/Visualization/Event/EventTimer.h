/*****************************************************************************/
/**
 *  @file   EventTimer.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#pragma once


namespace kvs
{

class TimeEvent;
class EventListener;

/*===========================================================================*/
/**
 *  @brief  Event timer class
 */
/*===========================================================================*/
class EventTimer
{
private:
    int m_id = 0; ///< timer ID
    int m_interval = -1; ///< interval time in millisecond
    bool m_is_stopped = true; ///< check flag whether the time is stopped
    kvs::TimeEvent* m_time_event = nullptr;
    kvs::EventListener* m_event_listener = nullptr;

public:
    EventTimer( kvs::EventListener* listener );
    virtual ~EventTimer();

    bool isStopped() const { return m_is_stopped; }

    virtual void stop() { if ( !m_is_stopped ) { m_is_stopped = true; } }
    virtual void start() { this->start( m_interval ); }
    virtual void start( int msec ) { this->setInterval( msec ); }
    virtual void nortify() {}

protected:
    int id() const { return m_id; }
    int interval() const { return m_interval; }
    kvs::EventListener* eventListener() { return m_event_listener; }
    kvs::TimeEvent* timeEvent() { return m_time_event; }
    void setID( int id ) { m_id = id; }
    void setInterval( int interval ) { m_interval = interval; }
    void setStopped( bool stopped ) { m_is_stopped = stopped; }
};

} // end of namespace kvs
