#include "EventTimer.h"
#include "TimeEvent.h"
#include "EventListener.h"


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new event timer.
 *  @param  listener [in] pointer to the event listener
 */
/*===========================================================================*/
EventTimer::EventTimer( kvs::EventListener* listener ):
    m_time_event( new kvs::TimeEvent ),
    m_event_listener( listener )
{
}

/*===========================================================================*/
/**
 *  @brief  Destroys the event timer.
 */
/*===========================================================================*/
EventTimer::~EventTimer()
{
    if ( m_time_event ) { delete m_time_event; }
}

} // end of namespace kvs
