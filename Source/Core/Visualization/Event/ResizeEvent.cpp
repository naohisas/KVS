/*****************************************************************************/
/**
 *  @file   ResizeEvent.cpp
 *  @author Naohisa Sakamoto
 *  @brief  
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: ResizeEvent.cpp 1373 2012-11-30 03:26:51Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#include "ResizeEvent.h"


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new ResizeEvent class.
 */
/*===========================================================================*/
ResizeEvent::ResizeEvent()
{
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new ResizeEvent class.
 *  @param  event [in] resize event
 */
/*===========================================================================*/
ResizeEvent::ResizeEvent( const ResizeEvent& event ):
    m_width( event.width() ),
    m_height( event.height() )
{
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new ResizeEvent class.
 *  @param  width [in] window width
 *  @param  height [in] window height
 */
/*===========================================================================*/
ResizeEvent::ResizeEvent( int width, int height ):
    m_width( width ),
    m_height( height )
{
}

/*===========================================================================*/
/**
 *  @brief  Destructs the ResizeEvent class.
 */
/*===========================================================================*/
ResizeEvent::~ResizeEvent()
{
}

/*===========================================================================*/
/**
 *  @brief  Returns the window width.
 *  @return window width
 */
/*===========================================================================*/
int ResizeEvent::width() const
{
    return m_width;
}

/*===========================================================================*/
/**
 *  @brief  Returns the window height.
 *  @return window height
 */
/*===========================================================================*/
int ResizeEvent::height() const
{
    return m_height;
}

/*===========================================================================*/
/**
 *  @brief  Returns the event type.
 *  @return event type
 */
/*===========================================================================*/
int ResizeEvent::type() const
{
    return kvs::EventBase::ResizeEvent;
}

/*===========================================================================*/
/**
 *  @brief  Sets window width and height.
 *  @param  width [in] window width
 *  @param  height [in] window height
 */
/*===========================================================================*/
void ResizeEvent::setSize( int width, int height )
{
    m_width = width;
    m_height = height;
}

} // end of namespace kvs
