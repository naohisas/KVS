/*****************************************************************************/
/**
 *  @file   MouseReleaseEventListener.cpp
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#include "MouseReleaseEventListener.h"


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new MouseReleaseEventListener class.
 */
/*===========================================================================*/
MouseReleaseEventListener::MouseReleaseEventListener()
{
    kvs::EventListener::setEventType( kvs::EventBase::MouseReleaseEvent );
}

/*===========================================================================*/
/**
 *  @brief  Destructs the MouseReleaseEventListener class.
 */
/*===========================================================================*/
MouseReleaseEventListener::~MouseReleaseEventListener()
{
}

/*===========================================================================*/
/**
 *  @brief  Executes the event function.
 *  @param  event [in] pointer to the event
 */
/*===========================================================================*/
void MouseReleaseEventListener::onEvent( kvs::EventBase* event )
{
    this->update( static_cast<kvs::MouseEvent*>(event) );
}

} // end of namespace kvs
