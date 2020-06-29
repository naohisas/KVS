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
    KeyReleaseEventListener( KeyReleaseEventFunc func ):
        kvs::EventListener( kvs::EventBase::KeyReleaseEvent ) { this->update( func ); }
    virtual ~KeyReleaseEventListener() {}

    void update( KeyReleaseEventFunc func ) { keyReleaseEvent( func ); }
    virtual void update( kvs::KeyEvent* event ) { keyReleaseEvent( event ); }

private:
    void onEvent( kvs::EventBase* event ) { this->update( static_cast<kvs::KeyEvent*>(event) ); }
};

} // end of namespace kvs
