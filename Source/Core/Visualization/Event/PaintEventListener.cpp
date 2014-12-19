/*****************************************************************************/
/**
 *  @file   PaintEventListener.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: PaintEventListener.cpp 1325 2012-10-04 10:34:52Z naohisa.sakamoto@gmail.com $
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
