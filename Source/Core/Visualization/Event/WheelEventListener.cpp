/*****************************************************************************/
/**
 *  @file   WheelEventListener.cpp
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#include "WheelEventListener.h"


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new WheelEventListener class.
 */
/*===========================================================================*/
WheelEventListener::WheelEventListener()
{
    kvs::EventListener::setEventType( kvs::EventBase::WheelEvent );
}

/*===========================================================================*/
/**
 *  @brief  Destructs the WheelEventListener class.
 */
/*===========================================================================*/
WheelEventListener::~WheelEventListener()
{
}

/*===========================================================================*/
/**
 *  @brief  Executes the event function.
 *  @param  event [in] pointer to the event
 */
/*===========================================================================*/
void WheelEventListener::onEvent( kvs::EventBase* event )
{
    this->update( static_cast<kvs::WheelEvent*>(event) );
}

} // end of namespace kvs
