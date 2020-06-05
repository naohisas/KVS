/*****************************************************************************/
/**
 *  @file   MouseDoubleClickEventListener.cpp
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#include "MouseDoubleClickEventListener.h"


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new MouseDoubleClickEventListener class.
 */
/*===========================================================================*/
MouseDoubleClickEventListener::MouseDoubleClickEventListener()
{
    kvs::EventListener::setEventType( kvs::EventBase::MouseDoubleClickEvent );
}

/*===========================================================================*/
/**
 *  @brief  Destructs the MouseDoubleClickEventListener class.
 */
/*===========================================================================*/
MouseDoubleClickEventListener::~MouseDoubleClickEventListener()
{
}

/*===========================================================================*/
/**
 *  @brief  Executes the event function.
 *  @param  event [in] pointer to the event
 */
/*===========================================================================*/
void MouseDoubleClickEventListener::onEvent( kvs::EventBase* event )
{
    this->update( static_cast<kvs::MouseEvent*>(event) );
}

} // end of namespace kvs
