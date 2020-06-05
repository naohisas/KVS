/*****************************************************************************/
/**
 *  @file   KeyRepeatEventListener.cpp
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#include "KeyRepeatEventListener.h"


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new KeyRepeatEventListener class.
 */
/*===========================================================================*/
KeyRepeatEventListener::KeyRepeatEventListener()
{
    kvs::EventListener::setEventType( kvs::EventBase::KeyRepeatEvent );
}

/*===========================================================================*/
/**
 *  @brief  Destructs the KeyRepeatEventListener class.
 */
/*===========================================================================*/
KeyRepeatEventListener::~KeyRepeatEventListener()
{
}

/*===========================================================================*/
/**
 *  @brief  Executes the event function.
 *  @param  event [in] pointer to the event
 */
/*===========================================================================*/
void KeyRepeatEventListener::onEvent( kvs::EventBase* event )
{
    this->update( static_cast<kvs::KeyEvent*>(event) );
}

} // end of namespace kvs
