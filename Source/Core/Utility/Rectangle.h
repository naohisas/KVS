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
#ifndef KVS__RECTANGLE_H_INCLUDE
#define KVS__RECTANGLE_H_INCLUDE

#include <kvs/Vector2>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Rectangle class.
 */
/*===========================================================================*/
class Rectangle
{
private:
    int m_x; ///< x position of widget on the window coordinate (org: upper-left)
    int m_y; ///< y position of widet on the window coordinate (org: upper-left)
    int m_width; ///< widget width
    int m_height; ///< widget height

public:
    Rectangle();
    Rectangle( const int width, const int height );
    Rectangle( const int x, const int y, const int width, const int height );
    virtual ~Rectangle();

    int x() const { return m_x; }
    int y() const { return m_y; }
    int width() const { return m_width; }
    int height() const { return m_height; }
    int left() const { return this->x(); }
    int right() const { return this->x() + this->width(); }
    int top() const { return this->y(); }
    int bottom() const { return this->y() + this->height(); }
    int x0() const { return this->left(); }
    int y0() const { return this->top(); }
    int x1() const { return this->right(); }
    int y1() const { return this->bottom(); }
    kvs::Vec2i topLeft() const { return kvs::Vec2i( this->left(), this->top() ); }
    kvs::Vec2i topRight() const { return kvs::Vec2i( this->right(), this->top() ); }
    kvs::Vec2i bottomLeft() const { return kvs::Vec2i( this->left(), this->bottom() ); }
    kvs::Vec2i bottomRight() const { return kvs::Vec2i( this->right(), this->bottom() ); }

    void setX( const int x ) { m_x = x; }
    void setY( const int y ) { m_y = y; }
    void setWidth( const int width ) { m_width = width; }
    void setHeight( const int height ) { m_height = height; }
    void setPosition( const int x, const int y ) { m_x = x; m_y = y; }
    void setSize( const int width, const int height ) { m_width = width; m_height = height; }
    void setGeometry( const int x, const int y, const int width, const int height )
    {
        this->setPosition( x, y );
        this->setSize( width, height );
    }

    bool contains( const int x, const int y, const bool proper = false ) const;
};

} // end of namespace kvs

#endif // KVS__RECTANGLE_H_INCLUDE
