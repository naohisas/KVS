/*****************************************************************************/
/**
 *  @file   InitializeEvent.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id$
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
