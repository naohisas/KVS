/*****************************************************************************/
/**
 *  @file   MouseDoubleClickEventListener.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: MouseDoubleClickEventListener.cpp 1325 2012-10-04 10:34:52Z naohisa.sakamoto@gmail.com $
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
