/****************************************************************************/
/**
 *  @file MouseEvent.cpp
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: MouseEvent.cpp 1373 2012-11-30 03:26:51Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#include "MouseEvent.h"
#include <kvs/MouseButton>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new MouseEvent class.
 */
/*===========================================================================*/
MouseEvent::MouseEvent():
    m_button( kvs::MouseButton::Left ),
    m_state( kvs::MouseButton::Up ),
    m_x( 0 ),
    m_y( 0 ),
    m_modifiers( 0 ),
    m_action( kvs::MouseButton::NoAction )
{
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new MouseEvent class.
 *  @param  event [in] mouse event
 */
/*===========================================================================*/
MouseEvent::MouseEvent( const MouseEvent& event ):
    m_button( event.m_button ),
    m_state( event.m_state ),
    m_x( event.m_x ),
    m_y( event.m_y ),
    m_modifiers( event.m_modifiers ),
    m_action( event.m_action )
{
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new MouseEvent class.
 *  @param  button [in] button code
 *  @param  state [in] mouse button state
 *  @param  x [in] mouse cursor position of x
 *  @param  y [in] mouse cursor position of y
 *  @param  modifiers [in] modifier key
 */
/*===========================================================================*/
MouseEvent::MouseEvent(
    int button,
    int state,
    int x,
    int y,
    int modifiers ):
    m_button( button ),
    m_state( state ),
    m_x( x ),
    m_y( y ),
    m_modifiers( modifiers ),
    m_action( kvs::MouseButton::NoAction )
{
}

/*===========================================================================*/
/**
 *  @brief  Destroys the MouseEvent class.
 */
/*===========================================================================*/
MouseEvent::~MouseEvent()
{
}

/*===========================================================================*/
/**
 *  @brief  Returns the button code.
 *  @return button code
 */
/*===========================================================================*/
int MouseEvent::button() const
{
    return m_button;
}

/*===========================================================================*/
/**
 *  @brief  Returns the mouse button state.
 *  @return mouse button state
 */
/*===========================================================================*/
int MouseEvent::state() const
{
    return m_state;
}

/*===========================================================================*/
/**
 *  @brief  Returns the mouse cursor position of x.
 *  @return mouse cursor position of x
 */
/*===========================================================================*/
int MouseEvent::x() const
{
    return m_x;
}

/*===========================================================================*/
/**
 *  @brief  Returns the mouse cursor position of y.
 *  @return mouse cursor position of y
 */
/*===========================================================================*/
int MouseEvent::y() const
{
    return m_y;
}

/*===========================================================================*/
/**
 *  @brief  Returns the modifier key.
 *  @return modifier key
 */
/*===========================================================================*/
int MouseEvent::modifiers() const
{
    return m_modifiers;
}

/*===========================================================================*/
/**
 *  @brief  Returns the mouse action code.
 *  @return mouse action code
 */
/*===========================================================================*/
int MouseEvent::action() const
{
    return m_action;
}

/*===========================================================================*/
/**
 *  @brief  Returns the mouse event type.
 *  @return mouse event type
 */
/*===========================================================================*/
int MouseEvent::type() const
{
    return
        m_action == kvs::MouseButton::Pressed ? EventBase::MousePressEvent :
        m_action == kvs::MouseButton::Moved ? EventBase::MouseMoveEvent :
        m_action == kvs::MouseButton::Released ? EventBase::MouseReleaseEvent :
        m_action == kvs::MouseButton::DoubleClicked ? EventBase::MouseDoubleClickEvent :
        m_action == kvs::MouseButton::Wheeled ? EventBase::WheelEvent : 0;
}

/*===========================================================================*/
/**
 *  @brief  Sets a mouse button code.
 *  @param  button [in] mouse button code
 */
/*===========================================================================*/
void MouseEvent::setButton( int button )
{
    m_button = button;
}

/*===========================================================================*/
/**
 *  @brief  Sets a mouse button state
 *  @param  state [in] mouse button state
 */
/*===========================================================================*/
void MouseEvent::setState( int state )
{
    m_state = state;
}

/*===========================================================================*/
/**
 *  @brief  Sets a mouse cursor position.
 *  @param  x [in] mouse cursor position of x
 *  @param  y [in] mouse cursor position of y
 */
/*===========================================================================*/
void MouseEvent::setPosition( int x, int y )
{
    m_x = x;
    m_y = y;
}

/*===========================================================================*/
/**
 *  @brief  Sets a modifier key
 *  @param  modifiers [in] modifier key
 */
/*===========================================================================*/
void MouseEvent::setModifiers( int modifiers )
{
    m_modifiers = modifiers;
}

/*===========================================================================*/
/**
 *  @brief  Sets a mouse action.
 *  @param  action [in] mouse action
 */
/*===========================================================================*/
void MouseEvent::setAction( int action )
{
    m_action = action;
}

} // end of namespace kvs
