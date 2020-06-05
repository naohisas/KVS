/*****************************************************************************/
/**
 *  @file   ResizeEventListener.cpp
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#include "ResizeEventListener.h"
#include <kvs/ResizeEvent>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new ResizeEventListener class.
 */
/*===========================================================================*/
ResizeEventListener::ResizeEventListener()
{
    kvs::EventListener::setEventType( kvs::EventBase::ResizeEvent );
}

/*===========================================================================*/
/**
 *  @brief  Destructs the ResizeEventListener class.
 */
/*===========================================================================*/
ResizeEventListener::~ResizeEventListener()
{
}

/*===========================================================================*/
/**
 *  @brief  Executes the event function.
 *  @param  event [in] pointer to the event
 */
/*===========================================================================*/
void ResizeEventListener::onEvent( kvs::EventBase* event )
{
    this->update(
        static_cast<kvs::ResizeEvent*>(event)->width(),
        static_cast<kvs::ResizeEvent*>(event)->height() );
}

} // end of namespace kvs
