/*****************************************************************************/
/**
 *  @file   KeyRepeatEventListener.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#pragma once
#include <kvs/EventListener>
#include <kvs/KeyEvent>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  KeyRepeatEventListener class.
 */
/*===========================================================================*/
class KeyRepeatEventListener : public kvs::EventListener
{
public:
    KeyRepeatEventListener();
    KeyRepeatEventListener( KeyRepeatEvent e ) { this->update( e ); }
    virtual ~KeyRepeatEventListener();

    void update( KeyRepeatEvent e ) { keyRepeatEvent( e ); }
    virtual void update( kvs::KeyEvent* event ) { keyRepeatEvent( event ); }

private:
    void onEvent( kvs::EventBase* event );
};

} // end of namespace kvs
