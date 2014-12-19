/*****************************************************************************/
/**
 *  @file   ResizeEventListener.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: ResizeEventListener.cpp 1325 2012-10-04 10:34:52Z naohisa.sakamoto@gmail.com $
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
