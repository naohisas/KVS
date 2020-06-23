#pragma once


namespace kvs
{

class TimeEvent;
class EventListener;

class EventTimer
{
private:
    int m_id; ///< timer ID
    int m_interval; ///< interval time in millisecond
    bool m_is_stopped; ///< check flag whether the time is stopped
    kvs::TimeEvent* m_time_event;
    kvs::EventListener* m_event_listener;

public:
    EventTimer( kvs::EventListener* listener );
    virtual ~EventTimer();

    bool isStopped() const { return m_is_stopped; }

    virtual void stop();
    virtual void start();
    virtual void start( int msec );
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
