/*****************************************************************************/
/**
 *  @file   PaintEventListener.cpp
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#include "PaintEventListener.h"
#include <kvs/IgnoreUnusedVariable>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new PaintEventListener class.
 */
/*===========================================================================*/
PaintEventListener::PaintEventListener()
{
    kvs::EventListener::setEventType( kvs::EventBase::PaintEvent );
}

/*===========================================================================*/
/**
 *  @brief  Destructs the PaintEventListener class.
 */
/*===========================================================================*/
PaintEventListener::~PaintEventListener()
{
}

/*===========================================================================*/
/**
 *  @brief  Executes the event function.
 *  @param  event [in] pointer to the event
 */
/*===========================================================================*/
void PaintEventListener::onEvent( kvs::EventBase* event )
{
    kvs::IgnoreUnusedVariable( event );
    this->update();
}

} // end of namespace kvs
