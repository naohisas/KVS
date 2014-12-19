/****************************************************************************/
/**
 *  @file KeyEvent.h
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: KeyEvent.h 1325 2012-10-04 10:34:52Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#ifndef KVS__KEY_EVENT_H_INCLUDE
#define KVS__KEY_EVENT_H_INCLUDE

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
protected:

    int m_key; ///< key code
    int m_x; ///< mouse cursol position x
    int m_y; ///< mouse cursol position y

public:

    KeyEvent();
    KeyEvent( const KeyEvent& event );
    KeyEvent( int key, int x, int y );
    virtual ~KeyEvent();

    int key() const;
    int x() const;
    int y() const;
    int type() const;

    void setKey( int key );
    void setPosition( int x, int y );
};

} // end of namespace kvs

#endif // KVS_CORE_KEY_EVENT_H_INCLUDE
