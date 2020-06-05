/*****************************************************************************/
/**
 *  @file   EventListener.cpp
 *  @author Naohisa Sakamoto
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
    case kvs::EventBase::InitializeEvent:
    {
        this->initializeEvent();
        break;
    }
    case kvs::EventBase::PaintEvent:
    {
        this->paintEvent();
        break;
    }
    case kvs::EventBase::MousePressEvent:
    {
        auto* e = static_cast<kvs::MouseEvent*>( event );
        this->mousePressEvent( e );
        break;
    }
    case kvs::EventBase::MouseMoveEvent:
    {
        auto* e = static_cast<kvs::MouseEvent*>( event );
        this->mouseMoveEvent( e );
        break;
    }
    case kvs::EventBase::MouseReleaseEvent:
    {
        auto* e = static_cast<kvs::MouseEvent*>( event );
        this->mouseReleaseEvent( e );
        break;
    }
    case kvs::EventBase::MouseDoubleClickEvent:
    {
        auto* e = static_cast<kvs::MouseEvent*>( event );
        this->mouseDoubleClickEvent( e );
        break;
    }
    case kvs::EventBase::ResizeEvent:
    {
        auto* e = static_cast<kvs::ResizeEvent*>( event );
        this->resizeEvent( e->width(), e->height() );
        break;
    }
    case kvs::EventBase::WheelEvent:
    {
        auto* e = static_cast<kvs::KeyEvent*>( event );
        this->keyPressEvent( e );
        break;
    }
    case kvs::EventBase::KeyPressEvent:
    {
        auto* e = static_cast<kvs::KeyEvent*>( event );
        this->keyPressEvent( e );
        break;
    }
    case kvs::EventBase::KeyRepeatEvent:
    {
        auto* e = static_cast<kvs::KeyEvent*>( event );
        this->keyRepeatEvent( e );
        break;
    }
    case kvs::EventBase::KeyReleaseEvent:
    {
        auto* e = static_cast<kvs::KeyEvent*>( event );
        this->keyReleaseEvent( e );
        break;
    }
    case kvs::EventBase::TimerEvent:
    {
        auto* e = static_cast<kvs::TimeEvent*>( event );
        this->timerEvent( e );
        break;
    }
    default: break;
    }
}

} // end of namespace kvs
