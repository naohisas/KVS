/*****************************************************************************/
/**
 *  @file   KeyPressEventListener.h
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
 *  @brief  KeyPressEventListener class.
 */
/*===========================================================================*/
class KeyPressEventListener : public kvs::EventListener
{
public:
    KeyPressEventListener(): kvs::EventListener( kvs::EventBase::KeyPressEvent ) {}
    KeyPressEventListener( KeyPressEventFunc func ) { this->update( func ); }
    virtual ~KeyPressEventListener() {}

    void update( KeyPressEventFunc func ) { keyPressEvent( func ); }
    virtual void update( kvs::KeyEvent* event ) { keyPressEvent( event ); }

private:
    void onEvent( kvs::EventBase* event ) { this->update( static_cast<kvs::KeyEvent*>(event) ); }
};

} // end of namespace kvs
