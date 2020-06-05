/*****************************************************************************/
/**
 *  @file   InitializeEvent.cpp
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#include "InitializeEvent.h"


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new InitializeEvent class.
 */
/*===========================================================================*/
InitializeEvent::InitializeEvent()
{
}

/*===========================================================================*/
/**
 *  @brief  Destroys the InitializeEvent class.
 */
/*===========================================================================*/
InitializeEvent::~InitializeEvent()
{
}

/*===========================================================================*/
/**
 *  @brief  Returns event type.
 *  @return event type
 */
/*===========================================================================*/
int InitializeEvent::type() const
{
    return EventBase::InitializeEvent;
}

} // end of namespace kvs
