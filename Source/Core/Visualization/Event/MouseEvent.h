/****************************************************************************/
/**
 *  @file   MouseEvent.h
 *  @author Naohisa Sakamoto
 */
/****************************************************************************/
#pragma once
#include <kvs/EventBase>
#include <kvs/MouseButton>


namespace kvs
{

/*==========================================================================*/
/**
 *  Mouse event class.
 */
/*==========================================================================*/
class MouseEvent : public kvs::EventBase
{
private:
    int m_button = kvs::MouseButton::Left; ///< mouse button
    int m_state = kvs::MouseButton::Up; ///< mouse button state
    int m_x = 0; ///< mouse cursol position x
    int m_y = 0; ///< mouse cursol position y
    int m_modifiers = 0; ///< modifier keys
    int m_action = kvs::MouseButton::NoAction; ///< mouse action

public:
    MouseEvent() = default;
    MouseEvent( const MouseEvent& e ):
        m_button( e.m_button ),
        m_state( e.m_state ),
        m_x( e.m_x ),
        m_y( e.m_y ),
        m_modifiers( e.m_modifiers ),
        m_action( e.m_action ) {}
    MouseEvent( int button, int state, int x, int y, int modifiers = 0 ):
        m_button( button ),
        m_state( state ),
        m_x( x ),
        m_y( y ),
        m_modifiers( modifiers ),
        m_action( kvs::MouseButton::NoAction ) {}
    virtual ~MouseEvent() = default;

    int button() const { return m_button; }
    int state() const { return m_state; }
    int x() const { return m_x; }
    int y() const { return m_y; }
    int modifiers() const { return m_modifiers; }
    int action() const { return m_action; }

    void setButton( int button ) { m_button = button; }
    void setState( int state ) { m_state = state; }
    void setPosition( int x, int y ) { m_x = x; m_y = y; }
    void setModifiers( int modifiers ) { m_modifiers = modifiers; }
    void setAction( int action ) { m_action = action; }

    int type() const
    {
        return
            m_action == kvs::MouseButton::Pressed ? EventBase::MousePressEvent :
            m_action == kvs::MouseButton::Moved ? EventBase::MouseMoveEvent :
            m_action == kvs::MouseButton::Released ? EventBase::MouseReleaseEvent :
            m_action == kvs::MouseButton::DoubleClicked ? EventBase::MouseDoubleClickEvent :
            m_action == kvs::MouseButton::Wheeled ? EventBase::WheelEvent : 0;
    }
};

} // end of namespace kvs
