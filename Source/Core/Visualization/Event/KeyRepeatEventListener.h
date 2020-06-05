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
    virtual ~KeyRepeatEventListener();
    virtual void update( kvs::KeyEvent* event ) = 0;

private:
    void onEvent( kvs::EventBase* event );
};

} // end of namespace kvs
