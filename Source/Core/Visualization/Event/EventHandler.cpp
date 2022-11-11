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
 *  @brief  Returns the event listener specified by the name.
 *  @param  name [in] event listener name
 *  @return pointer to the event listener
 */
/*===========================================================================*/
kvs::EventListener* EventHandler::listener( const std::string& name )
{
    auto it = std::find_if(
        m_listeners.begin(),
        m_listeners.end(),
        [&] ( kvs::EventListener* l ) { return l->name() == name; } );
    if ( it == m_listeners.end() ) { return nullptr; }
    return *it;
}

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
    m_listeners.shrink_to_fit();
}

} // end of namespace kvs
