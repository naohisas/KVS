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

namespace glut
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new Rectangle class.
 */
/*===========================================================================*/
Rectangle::Rectangle( void ):
    m_x( 0 ),
    m_y( 0 ),
    m_width( 0 ),
    m_height( 0 ),
    m_is_active( false )
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
    m_height( height ),
    m_is_active( false )
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
    m_height( height ),
    m_is_active( false )
{
}

/*===========================================================================*/
/**
 *  @brief  Destructs the Rectangle class.
 */
/*===========================================================================*/
Rectangle::~Rectangle( void )
{
}

/*===========================================================================*/
/**
 *  @brief  Returns the x value of the rectangle position.
 *  @return x value of the rectangle position
 */
/*===========================================================================*/
int Rectangle::x( void ) const
{
    return( m_x );
}

/*===========================================================================*/
/**
 *  @brief  Returns the y value of the rectangle position.
 *  @return y value of the rectangle position
 */
/*===========================================================================*/
int Rectangle::y( void ) const
{
    return( m_y );
}

/*===========================================================================*/
/**
 *  @brief  Returns the rectangle width.
 *  @return rectangle width
 */
/*===========================================================================*/
int Rectangle::width( void ) const
{
    return( m_width );
}

/*===========================================================================*/
/**
 *  @brief  Returns the rectangle height.
 *  @return rectangle height
 */
/*===========================================================================*/
int Rectangle::height( void ) const
{
    return( m_height );
}

/*===========================================================================*/
/**
 *  @brief  Returns the x value of the bottom-left of the rectangle.
 *  @return x value of the upper-left
 */
/*===========================================================================*/
int Rectangle::x0( void ) const
{
    return( m_x );
}

/*===========================================================================*/
/**
 *  @brief  Returns the y value of the bottom-left of the rectangle.
 *  @return y value of the upper-left
 */
/*===========================================================================*/
int Rectangle::y0( void ) const
{
    return( m_y );
}

/*===========================================================================*/
/**
 *  @brief  Returns the x value of the top-right of the rectangle.
 *  @return x value of the lower-right
 */
/*===========================================================================*/
int Rectangle::x1( void ) const
{
    return( m_x + m_width );
}

/*===========================================================================*/
/**
 *  @brief  Returns the y value of the top-right of the rectangle.
 *  @return y value of the lower-right
 */
/*===========================================================================*/
int Rectangle::y1( void ) const
{
    return( m_y + m_height );
}

/*===========================================================================*/
/**
 *  @brief  Checks whether the rectangle is active or not.
 *  @return true, if the rectangle is active
 */
/*===========================================================================*/
bool Rectangle::isActive( void ) const
{
    return( m_is_active );
}

/*===========================================================================*/
/**
 *  @brief  Sets x value of the rectangle position on the window coordinate.
 *  @param  x [in] x value of the rectangle position
 */
/*===========================================================================*/
void Rectangle::setX( const int x )
{
    m_x = x;
}

/*===========================================================================*/
/**
 *  @brief  Sets y value of the rectangle position on the window coordinate.
 *  @param  y [in] y value of the rectangle position
 */
/*===========================================================================*/
void Rectangle::setY( const int y )
{
    m_y = y;
}

/*===========================================================================*/
/**
 *  @brief  Sets a rectangle width.
 *  @param  width [in] rectangle width
 */
/*===========================================================================*/
void Rectangle::setWidth( const int width )
{
    m_width = width;
}

/*===========================================================================*/
/**
 *  @brief  Sets a rectangle height.
 *  @param  height [in] rectangle height
 */
/*===========================================================================*/
void Rectangle::setHeight( const int height )
{
    m_height = height;
}

/*===========================================================================*/
/**
 *  @brief  Sets a rectangle position.
 *  @param  x [in] x coordinate of the rectangle position
 *  @param  y [in] y coordinate of the rectangle position
 */
/*===========================================================================*/
void Rectangle::setPosition( const int x, const int y )
{
    this->setX( x );
    this->setY( y );
}

/*===========================================================================*/
/**
 *  @brief  Sets a rectangle size.
 *  @param  width [in] rectangle width
 *  @param  height [in] rectangle height
 */
/*===========================================================================*/
void Rectangle::setSize( const int width, const int height )
{
    this->setWidth( width );
    this->setHeight( height );
}

/*===========================================================================*/
/**
 *  @brief  Sets a rectangle geometry.
 *  @param  x [in] x coordinate of the rectangle position
 *  @param  y [in] y coordinate of the rectangle position
 *  @param  width [in] rectangle width
 *  @param  height [in] rectangle height
 */
/*===========================================================================*/
void Rectangle::setGeometry( const int x, const int y, const int width, const int height )
{
    this->setPosition( x, y );
    this->setSize( width, height );
}

/*===========================================================================*/
/**
 *  @brief  Activates the rectangle.
 */
/*===========================================================================*/
void Rectangle::activate( void )
{
    m_is_active = true;
}

/*===========================================================================*/
/**
 *  @brief  Deactivates the rectangle.
 */
/*===========================================================================*/
void Rectangle::deactivate( void )
{
    m_is_active = false;
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
bool Rectangle::contains(
    const int x,
    const int y,
    const bool proper )
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

} // end of namespace glut

} // end of namespace kvs
