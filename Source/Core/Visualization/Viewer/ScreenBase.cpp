/****************************************************************************/
/**
 *  @file   ScreenBase.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: ScreenBase.cpp 1570 2013-05-09 08:32:56Z naohisa.sakamoto@gmail.com $
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
    m_x( 0 ),
    m_y( 0 ),
    m_width( 512 ),
    m_height( 512 ),
    m_title("")
{
    m_event_handler = new kvs::EventHandler();
}

/*===========================================================================*/
/**
 *  @brief  Destructs the ScreenBase class.
 */
/*===========================================================================*/
ScreenBase::~ScreenBase()
{
    delete m_event_handler;
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
    if ( name != "" ) event->setName( event->name() );
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

/*
void ScreenBase::create(){}
void ScreenBase::show(){}
void ScreenBase::showFullScreen(){}
void ScreenBase::showNormal(){}
void ScreenBase::hide(){}
void ScreenBase::popUp(){}
void ScreenBase::pushDown(){}
void ScreenBase::redraw(){}
void ScreenBase::resize( int, int ){}
bool ScreenBase::isFullScreen() const { return false; }
void ScreenBase::enable(){}
void ScreenBase::disable(){}
void ScreenBase::reset(){}
*/

} // end of namespace kvs
