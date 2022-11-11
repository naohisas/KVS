/*****************************************************************************/
/**
 *  @file   EventHandler.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#pragma once
#include <vector>
#include <cstddef>
#include <string>


namespace kvs
{

class EventListener;
class EventBase;

/*===========================================================================*/
/**
 *  @brief  Event handler class.
 */
/*===========================================================================*/
class EventHandler
{
public:
    using EventListeners = std::vector<kvs::EventListener*>;

private:
    EventListeners m_listeners{}; ///< list of the event listener

public:
    EventHandler() = default;
    virtual ~EventHandler() = default;

    EventListeners& listeners() { return m_listeners; }
    kvs::EventListener* listener( const std::string& name );
    void attach( kvs::EventListener* listener );
    void detach( const kvs::EventListener* listener );
    void detach( const std::string& name );
    void notify( kvs::EventBase* event );
    void clear();
};

} // end of namespace kvs
