/*****************************************************************************/
/**
 *  @file   KeyPressEventListener.cpp
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#include "KeyPressEventListener.h"


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new KeyPressEventListener class.
 */
/*===========================================================================*/
KeyPressEventListener::KeyPressEventListener()
{
    kvs::EventListener::setEventType( kvs::EventBase::KeyPressEvent );
}

/*===========================================================================*/
/**
 *  @brief  Destructs the KeyPressEventListener class.
 */
/*===========================================================================*/
KeyPressEventListener::~KeyPressEventListener()
{
}

/*===========================================================================*/
/**
 *  @brief  Executes the event function.
 *  @param  event [in] pointer to the event
 */
/*===========================================================================*/
void KeyPressEventListener::onEvent( kvs::EventBase* event )
{
    this->update( static_cast<kvs::KeyEvent*>(event) );
}

} // end of namespace kvs
