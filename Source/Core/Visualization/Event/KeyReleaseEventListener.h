/*****************************************************************************/
/**
 *  @file   KeyReleaseEventListener.h
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
 *  @brief  KeyReleaseEventListener class.
 */
/*===========================================================================*/
class KeyReleaseEventListener : public kvs::EventListener
{
public:
    KeyReleaseEventListener(): kvs::EventListener( kvs::EventBase::KeyReleaseEvent ) {}
    KeyReleaseEventListener( KeyReleaseEvent e ) { this->update( e ); }
    virtual ~KeyReleaseEventListener() {}

    void update( KeyReleaseEvent e ) { keyReleaseEvent( e ); }
    virtual void update( kvs::KeyEvent* event ) { keyReleaseEvent( event ); }

private:
    void onEvent( kvs::EventBase* event ) { this->update( static_cast<kvs::KeyEvent*>(event) ); }
};

} // end of namespace kvs
