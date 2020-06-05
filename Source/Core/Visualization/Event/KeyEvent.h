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
    int m_key; ///< key code
    int m_x; ///< mouse cursol position x
    int m_y; ///< mouse cursol position y
    int m_modifiers; ///< modifier keys
    int m_action; ///< key action (pressed, released, or repeated)

public:
    KeyEvent();
    KeyEvent( const KeyEvent& event );
    KeyEvent( int key, int x, int y );
    virtual ~KeyEvent();

    int key() const;
    int x() const;
    int y() const;
    int modifiers() const;
    int action() const { return m_action; }
    int type() const;

    void setKey( int key );
    void setPosition( int x, int y );
    void setModifiers( int modifiers );
    void setAction( int action ) { m_action = action; }
};

} // end of namespace kvs
