/*****************************************************************************/
/**
 *  @file   InitializeEventListener.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: InitializeEventListener.cpp 1325 2012-10-04 10:34:52Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#include "InitializeEventListener.h"
#include <kvs/IgnoreUnusedVariable>
#include <kvs/Binary>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new InitializeEventListener class.
 */
/*===========================================================================*/
InitializeEventListener::InitializeEventListener()
{
    kvs::EventListener::setEventType( kvs::EventBase::InitializeEvent );
}

/*===========================================================================*/
/**
 *  @brief  Destructs the InitializeEventListener class.
 */
/*===========================================================================*/
InitializeEventListener::~InitializeEventListener()
{
}

/*===========================================================================*/
/**
 *  @brief  Executes the event function.
 *  @param  event [in] pointer to the event
 */
/*===========================================================================*/
void InitializeEventListener::onEvent( kvs::EventBase* event )
{
    kvs::IgnoreUnusedVariable( event );
    this->update();
}

} // end of namespace kvs
