/*****************************************************************************/
/**
 *  @file   Rectangle.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: Rectangle.cpp 1319 2012-10-02 10:32:10Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#include "Rectangle.h"


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new Rectangle class.
 */
/*===========================================================================*/
Rectangle::Rectangle():
    m_x( 0 ),
    m_y( 0 ),
    m_width( 0 ),
    m_height( 0 )
{
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new Rectangle class.
 *  @param  width [in] width
 *  @param  height [in] height
 */
/*===========================================================================*/
Rectangle::Rectangle( const int width, const int height ):
    m_x( 0 ),
    m_y( 0 ),
    m_width( width ),
    m_height( height )
{
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new Rectangle class.
 *  @param  x [in] x coordinate
 *  @param  y [in] y coordinate
 *  @param  width [in] width
 *  @param  height [in] height
 */
/*===========================================================================*/
Rectangle::Rectangle( const int x, const int y, const int width, const int height ):
    m_x( x ),
    m_y( y ),
    m_width( width ),
    m_height( height )
{
}

/*===========================================================================*/
/**
 *  @brief  Destructs the Rectangle class.
 */
/*===========================================================================*/
Rectangle::~Rectangle()
{
}

/*===========================================================================*/
/**
 *  @brief  Check whether the given point is inside the rectangle or not.
 *  @param  x [in] x position in the window coordinate
 *  @param  y [in] y position in the window coordinate
 *  @param  proper [in] whether the edges are checked or not
 *  @return true if the given point is inside the width
 */
/*===========================================================================*/
bool Rectangle::contains( const int x, const int y, const bool proper ) const
{
    /*
     *   p0 ------------ *
     *    |              |  p  : ( x,  y  )
     *    |   p          |
     *    |              |  p0 : ( x0, y0 )
     *    |              |  p1 : ( x1, y1 )
     *    |              |
     *    * ------------ p1
     *
     */
    const int x0 = m_x;
    const int x1 = m_x + m_width;
    const int y0 = m_y;
    const int y1 = m_y + m_height;

    if ( proper )
    {
        return( ( x0 < x ) && ( x < x1 ) && ( y0 < y ) && ( y < y1 ) );
    }
    else
    {
        return( ( x0 <= x ) && ( x <= x1 ) && ( y0 <= y ) && ( y <= y1 ) );
    }
}

} // end of namespace kvs
