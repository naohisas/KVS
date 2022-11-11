/****************************************************************************/
/**
 *  @file   KeyEvent.h
 *  @author Naohisa Sakamoto
 */
/****************************************************************************/
#pragma once
#include <kvs/EventBase>
#include <kvs/Key>


namespace kvs
{

/*==========================================================================*/
/**
 *  Key event class.
 */
/*==========================================================================*/
class KeyEvent : public kvs::EventBase
{
private:
    int m_key = 0; ///< key code
    int m_x = 0; ///< mouse cursol position x
    int m_y = 0; ///< mouse cursol position y
    int m_modifiers = 0; ///< modifier keys
    int m_action = kvs::Key::NoAction; ///< key action (pressed, released, or repeated)

public:
    KeyEvent() = default;
    KeyEvent( const KeyEvent& e ):
        m_key( e.m_key ),
        m_x( e.m_x ),
        m_y( e.m_y ),
        m_modifiers( e.m_modifiers ),
        m_action( e.m_action ) {}
    KeyEvent( int key, int x, int y ):
        m_key( key ),
        m_x( x ),
        m_y( y ) {}
    virtual ~KeyEvent() = default;

    int key() const { return m_key; }
    int x() const { return m_x; }
    int y() const { return m_y; }
    int modifiers() const { return m_modifiers; }
    int action() const { return m_action; }

    void setKey( int key ) { m_key = key; }
    void setPosition( int x, int y ) { m_x = x; m_y = y; }
    void setModifiers( int modifiers ) { m_modifiers = modifiers; }
    void setAction( int action ) { m_action = action; }

    int type() const
    {
        return
            m_action == kvs::Key::Pressed ? EventBase::KeyPressEvent :
            m_action == kvs::Key::Repeated ? EventBase::KeyRepeatEvent :
            m_action == kvs::Key::Released ? EventBase::KeyReleaseEvent : 0;
    }
};

} // end of namespace kvs
