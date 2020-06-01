#include "Timer.h"
#include "../GLFW.h"


namespace kvs
{

namespace glfw
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new Timer class.
 *  @param  msec [in] interval time in milliseconds
 */
/*===========================================================================*/
Timer::Timer( int msec ):
    m_id( 0 ),
    m_interval( msec ),
    m_is_stopped( true ),
    m_time_event( new kvs::TimeEvent ),
    m_event_listener( 0 ),
    m_event_handler( 0 ),
    m_now_time( 0 ),
    m_last_time( 0 ),
    m_elapse_time( 0 )
{
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new Timer class.
 *  @param  listener [in] pointer to a event listener
 *  @param  msec [in] interval time in milliseconds
 */
/*===========================================================================*/
Timer::Timer( kvs::EventListener* listener, int msec ):
    m_id( 0 ),
    m_interval( msec ),
    m_is_stopped( true ),
    m_time_event( new kvs::TimeEvent ),
    m_event_listener( listener ),
    m_event_handler( 0 ),
    m_now_time( 0 ),
    m_last_time( 0 ),
    m_elapse_time( 0 )
{
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new Timer class.
 *  @param  handler [in] pointer to a event handler
 *  @param  msec [in] interval time in milliseconds
 */
/*===========================================================================*/
Timer::Timer( kvs::EventHandler* handler, int msec ):
    m_id( 0 ),
    m_interval( msec ),
    m_is_stopped( true ),
    m_time_event( new kvs::TimeEvent ),
    m_event_listener( 0 ),
    m_event_handler( handler ),
    m_now_time( 0 ),
    m_last_time( 0 ),
    m_elapse_time( 0 )
{
}

/*===========================================================================*/
/**
 *  @brief  Destructs the Timer class.
 */
/*===========================================================================*/
Timer::~Timer()
{
    if ( m_time_event ) { delete m_time_event; }
}

/*===========================================================================*/
/**
 *  @brief  Starts the timer.
 *  @param  msec [in] interval time in milliseconds
 */
/*===========================================================================*/
void Timer::start( int msec )
{
    if ( m_is_stopped )
    {
        m_interval = msec;
        m_is_stopped = false;

        m_now_time = glfwGetTime();
        m_last_time = m_now_time;
        m_elapse_time = 0.0;
    }
}

/*===========================================================================*/
/**
 *  @brief  Starts the timer.
 */
/*===========================================================================*/
void Timer::start()
{
    this->start( m_interval );
}

/*===========================================================================*/
/**
 *  @brief  Stops the timer.
 */
/*===========================================================================*/
void Timer::stop()
{
    if ( !m_is_stopped )
    {
        m_is_stopped = true;
    }
}

/*===========================================================================*/
/**
 *  @brief  Sets a interval time.
 *  @param  msec [in] interval time in milliseconds
 */
/*===========================================================================*/
void Timer::setInterval( int msec )
{
    m_interval = msec;
}

/*===========================================================================*/
/**
 *  @brief  Sets an event listener.
 *  @param  listener [in] pointer to the event listener
 */
/*===========================================================================*/
void Timer::setEventListener( kvs::EventListener* listener )
{
    m_event_listener = listener;
}

/*===========================================================================*/
/**
 *  @brief  Sets an event handler.
 *  @param  handler [in] pointer to the event handler
 */
/*===========================================================================*/
void Timer::setEventHandler( kvs::EventHandler* handler )
{
    m_event_handler = handler;
}

/*===========================================================================*/
/**
 *  @brief  Checks whether the timer is stopped or not.
 *  @return true, if the timer is stopped
 */
/*===========================================================================*/
bool Timer::isStopped() const
{
    return m_is_stopped;
}

/*===========================================================================*/
/**
 *  @brief  Returns the interval time.
 *  @return interval time
 */
/*===========================================================================*/
int Timer::interval() const
{
    return m_interval;
}

/*===========================================================================*/
/**
 *  @brief  Timer event.
 */
/*===========================================================================*/
void Timer::timerEvent()
{
    if ( !m_is_stopped )
    {
        m_now_time = glfwGetTime();
        m_elapse_time += ( m_now_time - m_last_time );
        m_last_time = m_now_time;

        if ( m_elapse_time * 1000.0 > m_interval )
        {
            if ( m_event_listener ) m_event_listener->onEvent( m_time_event );
            else if ( m_event_handler ) m_event_handler->notify( m_time_event );

            m_elapse_time = 0.0;
        }
    }
    else
    {
        this->stop();
    }
}

} // end of namespace glfw

} // end of namespace kvs
