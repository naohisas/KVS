/*****************************************************************************/
/**
 *  @file   Rectangle.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: Rectangle.h 1319 2012-10-02 10:32:10Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#ifndef KVS__GLUT__RECTANGLE_H_INCLUDE
#define KVS__GLUT__RECTANGLE_H_INCLUDE


namespace kvs
{

namespace glut
{

/*===========================================================================*/
/**
 *  @brief  Rectangle class.
 */
/*===========================================================================*/
class Rectangle
{
protected:

    int m_x; ///< x position of widget on the window coordinate (org: upper-left)
    int m_y; ///< y position of widet on the window coordinate (org: upper-left)
    int m_width; ///< widget width
    int m_height; ///< widget height
    bool m_is_active; ///< check flag for widget activation

public:

    Rectangle( void );
    Rectangle( const int width, const int height );
    Rectangle( const int x, const int y, const int width, const int height );
    virtual ~Rectangle( void );

    int x( void ) const;
    int y( void ) const;
    int width( void ) const;
    int height( void ) const;
    int x0( void ) const;
    int y0( void ) const;
    int x1( void ) const;
    int y1( void ) const;
    bool isActive( void ) const;

    void setX( const int x );
    void setY( const int y );
    void setWidth( const int width );
    void setHeight( const int height );
    void setPosition( const int x, const int y );
    void setSize( const int width, const int height );
    void setGeometry( const int x, const int y, const int width, const int height );

    void activate( void );
    void deactivate( void );
    bool contains( const int x, const int y, const bool proper = false );
};

} // end of namespace glut

} // end of namespace kvs

#endif // KVS__GLUT__RECTANGLE_H_INCLUDE
