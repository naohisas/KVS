/****************************************************************************/
/**
 *  @file   ScreenBase.cpp
 *  @author Naohisa Sakamoto
 */
/****************************************************************************/
#include "ScreenBase.h"
#include <kvs/EventHandler>
#include <kvs/EventListener>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new ScreenBase class.
 */
/*===========================================================================*/
ScreenBase::ScreenBase():
    m_event_handler( new kvs::EventHandler() ),
    m_paint_device( new kvs::PaintDevice() )
{
}

/*===========================================================================*/
/**
 *  @brief  Destructs the ScreenBase class.
 */
/*===========================================================================*/
ScreenBase::~ScreenBase()
{
    if ( m_event_handler ) { delete m_event_handler; }
    if ( m_paint_device ) { delete m_paint_device; }
}

/*===========================================================================*/
/**
 *  @brief  Sets an event to the event handler.
 *  @param  event [in] pointer to an event
 *  @param  name [in] event name
 */
/*===========================================================================*/
void ScreenBase::setEvent( kvs::EventListener* event, const std::string& name )
{
    m_event_handler->clear();
    this->addEvent( event, name );
}

/*===========================================================================*/
/**
 *  @brief  Adds an event to the event handler.
 *  @param  event [in] pointer to an event
 *  @param  name [in] event name
 */
/*===========================================================================*/
void ScreenBase::addEvent( kvs::EventListener* event, const std::string& name )
{
    event->setScreen( this );
    if ( name != "" ) { event->setName( name ); }
    m_event_handler->attach( event );
}

/*===========================================================================*/
/**
 *  @brief  Removes the event from the event handler.
 *  @param  event [in] pointer to the event
 */
/*===========================================================================*/
void ScreenBase::removeEvent( const kvs::EventListener* event )
{
    m_event_handler->detach( event );
}

/*===========================================================================*/
/**
 *  @brief  Removes the event from the event handler.
 *  @param  name [in] event name
 */
/*===========================================================================*/
void ScreenBase::removeEvent( const std::string& name )
{
    m_event_handler->detach( name );
}

} // end of namespace kvs
