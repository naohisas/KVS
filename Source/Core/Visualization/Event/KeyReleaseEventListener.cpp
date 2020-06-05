/*****************************************************************************/
/**
 *  @file   KeyReleaseEventListener.cpp
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#include "KeyReleaseEventListener.h"


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new KeyReleaseEventListener class.
 */
/*===========================================================================*/
KeyReleaseEventListener::KeyReleaseEventListener()
{
    kvs::EventListener::setEventType( kvs::EventBase::KeyReleaseEvent );
}

/*===========================================================================*/
/**
 *  @brief  Destructs the KeyReleaseEventListener class.
 */
/*===========================================================================*/
KeyReleaseEventListener::~KeyReleaseEventListener()
{
}

/*===========================================================================*/
/**
 *  @brief  Executes the event function.
 *  @param  event [in] pointer to the event
 */
/*===========================================================================*/
void KeyReleaseEventListener::onEvent( kvs::EventBase* event )
{
    this->update( static_cast<kvs::KeyEvent*>(event) );
}

} // end of namespace kvs
