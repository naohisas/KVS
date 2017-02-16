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

    Rectangle();
    Rectangle( const int width, const int height );
    Rectangle( const int x, const int y, const int width, const int height );
    virtual ~Rectangle();

    int x() const { return m_x; }
    int y() const { return m_y; }
    int width() const { return m_width; }
    int height() const { return m_height; }
    int x0() const { return m_x; }
    int y0() const { return m_y; }
    int x1() const { return m_x + m_width; }
    int y1() const { return m_y + m_height; }
    bool isActive() const { return m_is_active; }

    void setX( const int x ) { m_x = x; }
    void setY( const int y ) { m_y = y; }
    void setWidth( const int width ) { m_width = width; }
    void setHeight( const int height ) { m_height = height; }
    void setPosition( const int x, const int y ) { m_x = x; m_y = y; }
    void setSize( const int width, const int height ) { m_width = width; m_height = height; }
    void setGeometry( const int x, const int y, const int width, const int height ) { this->setPosition( x, y ); this->setSize( width, height ); }

    void activate() { m_is_active = true; }
    void deactivate() { m_is_active = false; }
    bool contains( const int x, const int y, const bool proper = false );
};

} // end of namespace glut

} // end of namespace kvs

#endif // KVS__GLUT__RECTANGLE_H_INCLUDE
