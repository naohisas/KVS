/*****************************************************************************/
/**
 *  @file   PaintEvent.cpp
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
#include "PaintEvent.h"

namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new PaintEvent class.
 */
/*===========================================================================*/
PaintEvent::PaintEvent()
{
}

/*===========================================================================*/
/**
 *  @brief  Destroys the PaintEvent class.
 */
/*===========================================================================*/
PaintEvent::~PaintEvent()
{
}

/*===========================================================================*/
/**
 *  @brief  Returns the event type.
 *  @return event type
 */
/*===========================================================================*/
int PaintEvent::type() const
{
    return EventBase::PaintEvent;
}

} // end of namespace kvs
