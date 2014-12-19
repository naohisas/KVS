/*****************************************************************************/
/**
 *  @file   MouseReleaseEventListener.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: MouseReleaseEventListener.cpp 1325 2012-10-04 10:34:52Z naohisa.sakamoto@gmail.com $
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
