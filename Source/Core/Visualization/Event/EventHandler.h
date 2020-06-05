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
private:
    std::vector<kvs::EventListener*> m_listeners; ///< list of the event listener

public:
    EventHandler() {}
    virtual ~EventHandler() {}

    void attach( kvs::EventListener* listener );
    void detach( const kvs::EventListener* listener );
    void detach( const std::string& name );
    void notify( kvs::EventBase* event );
    void clear();
};

} // end of namespace kvs
