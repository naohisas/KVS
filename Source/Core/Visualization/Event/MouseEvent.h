/****************************************************************************/
/**
 *  @file   MouseEvent.h
 *  @author Naohisa Sakamoto
 */
/****************************************************************************/
#pragma once
#include <kvs/EventBase>


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
    int m_button; ///< mouse button
    int m_state; ///< mouse button state
    int m_x; ///< mouse cursol position x
    int m_y; ///< mouse cursol position y
    int m_modifiers; ///< modifier keys
    int m_action; ///< mouse action

public:
    MouseEvent();
    MouseEvent( const MouseEvent& event );
    MouseEvent( int button, int state, int x, int y, int modifiers = 0 );
    virtual ~MouseEvent();

    int button() const;
    int state() const;
    int x() const;
    int y() const;
    int modifiers() const;
    int action() const;
    int type() const;

    void setButton( int button );
    void setState( int state );
    void setPosition( int x, int y );
    void setModifiers( int modifiers );
    void setAction( int action );
};

} // end of namespace kvs
