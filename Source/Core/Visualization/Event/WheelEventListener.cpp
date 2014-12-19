/*****************************************************************************/
/**
 *  @file   WheelEventListener.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: WheelEventListener.cpp 1325 2012-10-04 10:34:52Z naohisa.sakamoto@gmail.com $
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
