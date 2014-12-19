/*****************************************************************************/
/**
 *  @file   Timer.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: Timer.cpp 631 2010-10-10 02:15:35Z naohisa.sakamoto $
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
    m_event_handler( 0 )
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
    m_event_handler( 0 )
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
    m_event_handler( handler )
{
}

/*===========================================================================*/
/**
 *  @brief  Destructs the Timer class.
 */
/*===========================================================================*/
Timer::~Timer( void )
{
    if ( m_time_event ) delete m_time_event;
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
        m_id = QObject::startTimer( msec );
    }
}

/*===========================================================================*/
/**
 *  @brief  Starts the timer.
 */
/*===========================================================================*/
void Timer::start( void )
{
    this->start( m_interval );
}

/*===========================================================================*/
/**
 *  @brief  Stops the timer.
 */
/*===========================================================================*/
void Timer::stop( void )
{
    if ( !m_is_stopped )
    {
        m_is_stopped = true;
        QObject::killTimer( m_id );
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
const bool Timer::isStopped( void ) const
{
    return( m_is_stopped );
}

/*===========================================================================*/
/**
 *  @brief  Returns the interval time.
 *  @return interval time
 */
/*===========================================================================*/
const int Timer::interval( void ) const
{
    return( m_interval );
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
