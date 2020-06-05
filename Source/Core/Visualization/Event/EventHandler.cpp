/*****************************************************************************/
/**
 *  @file   EventHandler.cpp
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#include "EventHandler.h"
#include <algorithm>
#include <kvs/EventBase>
#include <kvs/EventListener>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Attches an event listener.
 *  @param  listener [in] pointer to event listener
 */
/*===========================================================================*/
void EventHandler::attach( kvs::EventListener* listener )
{
    m_listeners.push_back( listener );
}

/*===========================================================================*/
/**
 *  @brief  Detaches the event listener.
 *  @param  listener [in] pointer to the event listener
 */
/*===========================================================================*/
void EventHandler::detach( const kvs::EventListener* listener )
{
    /*
//    std::vector<kvs::EventListener*>::iterator p;
    auto p = std::find( m_listeners.begin(), m_listeners.end(), listener );
    if ( p != m_listeners.end() ) { m_listeners.erase( p ); }
    */
    auto result = std::remove( m_listeners.begin(), m_listeners.end(), listener );
    m_listeners.erase( result, m_listeners.end() );
}

/*===========================================================================*/
/**
 *  @brief  Detaches the event listener.
 *  @param  name [in] event name
 */
/*===========================================================================*/
void EventHandler::detach( const std::string& name )
{
    /*
    std::vector<kvs::EventListener*>::iterator listener = m_listeners.begin();
    std::vector<kvs::EventListener*>::iterator end = m_listeners.end();
    while ( listener != end )
    {
        if ( (*listener)->name() == name ) break;
        ++listener;
    }

    if ( listener != end ) { m_listeners.erase( listener ); }
    */
    auto result = std::remove_if(
        m_listeners.begin(), m_listeners.end(),
        [&]( kvs::EventListener* l ) { return l->name() == name; } );
    m_listeners.erase( result, m_listeners.end() );
}

/*===========================================================================*/
/**
 *  @brief  Notifies the event to the registered event listeners.
 *  @param  event [in] pointer to notified event
 */
/*===========================================================================*/
void EventHandler::notify( kvs::EventBase* event )
{
    /*
    std::vector<kvs::EventListener*>::iterator listener = m_listeners.begin();
    std::vector<kvs::EventListener*>::iterator end = m_listeners.end();
    while ( listener != end )
    {
        if ( (*listener)->eventType() & event->type() )
        {
            (*listener)->onEvent( event );
        }
        ++listener;
    }
    */
    for ( auto& l : m_listeners )
    {
        if ( l->eventType() & event->type() )
        {
            l->onEvent( event );
        }
    }
}

/*===========================================================================*/
/**
 *  @brief  Clears the registered event listeners.
 */
/*===========================================================================*/
void EventHandler::clear()
{
    m_listeners.clear();
}

} // end of namespace kvs
