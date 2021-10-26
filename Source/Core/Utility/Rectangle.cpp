/*****************************************************************************/
/**
 *  @file   Rectangle.cpp
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#include "Rectangle.h"


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new Rectangle class.
 *  @param  width [in] width
 *  @param  height [in] height
 */
/*===========================================================================*/
Rectangle::Rectangle( const int width, const int height ):
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
 *  @brief  Constructs a new Rectangle class.
 *  @param  p0 [in] top-left point
 *  @param  p1 [in] bottom-right point
 */
/*===========================================================================*/
Rectangle::Rectangle( const kvs::Vec2i& p0, const kvs::Vec2i& p1 ):
    m_x( p0.x() ),
    m_y( p0.y() ),
    m_width( p1.x() - p0.x() ),
    m_height( p1.y() - p0.y() )
{
}

/*===========================================================================*/
/**
 *  @brief  Check whether the given point is inside the rectangle or not.
 *  @param  x [in] x position in the pixel coordinate
 *  @param  y [in] y position in the pixel coordinate
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
