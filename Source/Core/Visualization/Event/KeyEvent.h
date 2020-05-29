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

public:
    KeyEvent();
    KeyEvent( const KeyEvent& event );
    KeyEvent( int key, int x, int y );
    virtual ~KeyEvent();

    int key() const;
    int x() const;
    int y() const;
    int modifiers() const;
    int type() const;

    void setKey( int key );
    void setPosition( int x, int y );
    void setModifiers( int modifiers );
};

} // end of namespace kvs
