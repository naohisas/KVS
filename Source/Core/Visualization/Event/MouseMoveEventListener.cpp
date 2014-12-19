/*****************************************************************************/
/**
 *  @file   MouseMoveEventListener.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: MouseMoveEventListener.cpp 1325 2012-10-04 10:34:52Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#include "MouseMoveEventListener.h"


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new MouseMoveEventListener class.
 */
/*===========================================================================*/
MouseMoveEventListener::MouseMoveEventListener()
{
    kvs::EventListener::setEventType( kvs::EventBase::MouseMoveEvent );
}

/*===========================================================================*/
/**
 *  @brief  Destructs the MouseMoveEventListener class.
 */
/*===========================================================================*/
MouseMoveEventListener::~MouseMoveEventListener()
{
}

/*===========================================================================*/
/**
 *  @brief  Executes the event function.
 *  @param  event [in] pointer to the event
 */
/*===========================================================================*/
void MouseMoveEventListener::onEvent( kvs::EventBase* event )
{
    this->update( static_cast<kvs::MouseEvent*>(event) );
}

} // end of namespace kvs
