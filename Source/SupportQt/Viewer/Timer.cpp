/*****************************************************************************/
/**
 *  @file   Timer.cpp
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#include "Timer.h"
#include <kvs/DebugNew>
#include <kvs/IgnoreUnusedVariable>
#include <kvs/qt/Qt>


namespace kvs
{

namespace qt
{

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
        m_id = QObject::startTimer( msec );
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
        QObject::killTimer( m_id );
    }
}

/*===========================================================================*/
/**
 *  @brief  Timer event.
 */
/*===========================================================================*/
void Timer::timerEvent( QTimerEvent* event )
{
    kvs::IgnoreUnusedVariable( event );

    if ( !m_is_stopped )
    {
        if ( m_event_listener ) m_event_listener->onEvent( m_time_event );
        else if ( m_event_handler ) m_event_handler->notify( m_time_event );
    }
    else
    {
        this->stop();
    }
}

} // end of namesapce qt

} // end of namespace kvs
