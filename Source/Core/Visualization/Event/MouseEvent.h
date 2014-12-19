/****************************************************************************/
/**
 *  @file MouseEvent.h
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: MouseEvent.h 1325 2012-10-04 10:34:52Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#ifndef KVS__MOUSE_EVENT_H_INCLUDE
#define KVS__MOUSE_EVENT_H_INCLUDE

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
protected:

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

#endif // KVS__MOUSE_EVENT_H_INCLUDE
