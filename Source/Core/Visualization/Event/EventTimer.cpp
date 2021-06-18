#include "EventTimer.h"
#include "TimeEvent.h"
#include "EventListener.h"


namespace kvs
{

EventTimer::EventTimer( kvs::EventListener* listener ):
    m_id( 0 ),
    m_interval( -1 ),
    m_is_stopped( true ),
    m_time_event( new kvs::TimeEvent ),
    m_event_listener( listener )
{
}

EventTimer::~EventTimer()
{
    if ( m_time_event ) { delete m_time_event; }
}

void EventTimer::stop()
{
    if ( !m_is_stopped ) { m_is_stopped = true; }
}

void EventTimer::start()
{
    this->start( m_interval );
}

void EventTimer::start( int msec )
{
    this->setInterval( msec );
}

} // end of namespace kvs
