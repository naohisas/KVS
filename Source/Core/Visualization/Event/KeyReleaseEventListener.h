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
    KeyReleaseEventListener();
    virtual ~KeyReleaseEventListener();

    virtual void update( kvs::KeyEvent* event ) = 0;

private:
    void onEvent( kvs::EventBase* event );
};

} // end of namespace kvs
