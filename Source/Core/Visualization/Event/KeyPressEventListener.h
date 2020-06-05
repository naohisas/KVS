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
    KeyPressEventListener();
    virtual ~KeyPressEventListener();
    virtual void update( kvs::KeyEvent* event ) = 0;

private:
    void onEvent( kvs::EventBase* event );
};

} // end of namespace kvs
