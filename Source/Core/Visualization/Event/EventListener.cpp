/*****************************************************************************/
/**
 *  @file   EventListener.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: EventListener.cpp 1574 2013-05-21 10:28:12Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#include "EventListener.h"
#include <kvs/EventBase>
#include <kvs/MouseEvent>
#include <kvs/KeyEvent>
#include <kvs/ResizeEvent>
#include <kvs/TimeEvent>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new EventListener class.
 */
/*===========================================================================*/
EventListener::EventListener():
    m_event_type( 0 ),
    m_name( "" ),
    m_screen( NULL ),
    m_scene( NULL )
{
}

/*===========================================================================*/
/**
 *  @brief  Destructs the EventListener class.
 */
/*===========================================================================*/
EventListener::~EventListener()
{
}

/*===========================================================================*/
/**
 *  @brief  Executes the event function.
 *  @param  event [in] pointer to the event
 */
/*===========================================================================*/
void EventListener::onEvent( kvs::EventBase* event )
{
    switch ( event->type() )
    {
    case kvs::EventBase::InitializeEvent: this->initializeEvent(); break;
    case kvs::EventBase::PaintEvent: this->paintEvent(); break;
    case kvs::EventBase::MousePressEvent:
    {
        kvs::MouseEvent* e = static_cast<kvs::MouseEvent*>( event );
        this->mousePressEvent( e );
        break;
    }
    case kvs::EventBase::MouseMoveEvent:
    {
        kvs::MouseEvent* e = static_cast<kvs::MouseEvent*>( event );
        this->mouseMoveEvent( e );
        break;
    }
    case kvs::EventBase::MouseReleaseEvent:
    {
        kvs::MouseEvent* e = static_cast<kvs::MouseEvent*>( event );
        this->mouseReleaseEvent( e );
        break;
    }
    case kvs::EventBase::MouseDoubleClickEvent:
    {
        kvs::MouseEvent* e = static_cast<kvs::MouseEvent*>( event );
        this->mouseDoubleClickEvent( e );
        break;
    }
    case kvs::EventBase::ResizeEvent:
    {
        kvs::ResizeEvent* e = static_cast<kvs::ResizeEvent*>( event );
        this->resizeEvent( e->width(), e->height() );
        break;
    }
    case kvs::EventBase::WheelEvent:
    {
        kvs::KeyEvent* e = static_cast<kvs::KeyEvent*>( event );
        this->keyPressEvent( e );
        break;
    }
    case kvs::EventBase::KeyPressEvent:
    {
        kvs::KeyEvent* e = static_cast<kvs::KeyEvent*>( event );
        this->keyPressEvent( e );
        break;
    }
    case kvs::EventBase::TimerEvent:
    {
        kvs::TimeEvent* e = static_cast<kvs::TimeEvent*>( event );
        this->timerEvent( e );
        break;
    }
    default: break;
    }
}

} // end of namespace kvs
