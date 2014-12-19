/*****************************************************************************/
/**
 *  @file   WheelEvent.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: WheelEvent.cpp 1373 2012-11-30 03:26:51Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#include "WheelEvent.h"


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new WheelEvent class.
 */
/*===========================================================================*/
WheelEvent::WheelEvent()
{
}

/*===========================================================================*/
/**
 *  @brief  Destructs the WheelEvent class.
 */
/*===========================================================================*/
WheelEvent::~WheelEvent()
{
}

/*===========================================================================*/
/**
 *  @brief  Returns the x coordinate value of mouse cursor position.
 *  @return x coordinate value
 */
/*===========================================================================*/
int WheelEvent::x() const
{
    return m_x;
}

/*===========================================================================*/
/**
 *  @brief  Returns the y coordinate value of mouse cursor position.
 *  @return y coordinate value
 */
/*===========================================================================*/
int WheelEvent::y() const
{
    return m_y;
}

/*===========================================================================*/
/**
 *  @brief  Returns the scroll direction.
 *  @return scroll direction
 */
/*===========================================================================*/
int WheelEvent::direction() const
{
    return m_direction;
}

/*===========================================================================*/
/**
 *  @brief  Returns the event type.
 *  @return event type
 */
/*===========================================================================*/
int WheelEvent::type() const
{
    return kvs::EventBase::WheelEvent;
}

/*===========================================================================*/
/**
 *  @brief  Sets a mouse cursor position
 *  @param  x [in] x coordinate value
 *  @param  y [in] y coordinate value
 */
/*===========================================================================*/
void WheelEvent::setPosition( int x, int y )
{
    m_x = x;
    m_y = y;
}

/*===========================================================================*/
/**
 *  @brief  Sets a scroll direction
 *  @param  direction [in] scroll direction
 */
/*===========================================================================*/
void WheelEvent::setDirection( int direction )
{
    m_direction = direction;
}

} // end of namespace kvs
