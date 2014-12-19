/*****************************************************************************/
/**
 *  @file   IdleEventListener.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: IdleEventListener.cpp 1325 2012-10-04 10:34:52Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#include "IdleEventListener.h"
#include <kvs/IgnoreUnusedVariable>
#include <kvs/Binary>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new IdleEventListener class.
 */
/*===========================================================================*/
IdleEventListener::IdleEventListener()
{
    kvs::EventListener::setEventType( kvsBinary12(1111,1111,1111) );
}

/*===========================================================================*/
/**
 *  @brief  Destructs the IdleEventListener class.
 */
/*===========================================================================*/
IdleEventListener::~IdleEventListener()
{
}

/*===========================================================================*/
/**
 *  @brief  Executes the event function.
 *  @param  event [in] pointer to the event
 */
/*===========================================================================*/
void IdleEventListener::onEvent( kvs::EventBase* event )
{
    kvs::IgnoreUnusedVariable( event );

    this->update();
}

} // end of namespace kvs
