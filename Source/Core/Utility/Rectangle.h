/*****************************************************************************/
/**
 *  @file   Rectangle.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#pragma once
#include <kvs/Vector2>
#include <kvs/Vector4>


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
    // Rectangle: {m_x, m_y, m_width, m_height}
    //
    //   p0 = {x0, y0} = {m_x, m_y}
    //   p1 = {x1, y1} = {m_x + m_width, m_y + m_height}
    //
    //   p0-------------+
    //   |              |
    //   |              |
    //   +--------------p1

    int m_x = 0; ///< x position of widget on pixel coordinates (orgin: upper-left)
    int m_y = 0; ///< y position of widet on pixel coordinates (orgin: upper-left)
    int m_width = 0; ///< widget width
    int m_height = 0; ///< widget height

public:
    Rectangle() = default;
    Rectangle( const int width, const int height );
    Rectangle( const int x, const int y, const int width, const int height );
    Rectangle( const kvs::Vec2i& p0, const kvs::Vec2i& p1 );
    virtual ~Rectangle() = default;

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
    kvs::Vec2i p0() const { return { this->left(), this->top() }; }
    kvs::Vec2i p1() const { return { this->right(), this->bottom() }; }
    kvs::Vec2i topLeft() const { return { this->left(), this->top() }; }
    kvs::Vec2i topRight() const { return { this->right(), this->top() }; }
    kvs::Vec2i bottomLeft() const { return { this->left(), this->bottom() }; }
    kvs::Vec2i bottomRight() const { return { this->right(), this->bottom() }; }

    void setX( const int x ) { m_x = x; }
    void setY( const int y ) { m_y = y; }
    void setWidth( const int width ) { m_width = width; }
    void setHeight( const int height ) { m_height = height; }
    void setPosition( const int x, const int y ) { m_x = x; m_y = y; }
    void setPosition( const kvs::Vec2i& p ) { this->setPosition( p.x(), p.y() ); }
    void setSize( const int width, const int height ) { m_width = width; m_height = height; }
    void setSize( const kvs::Vec2i& s ) { this->setSize( s.x(), s.y() ); }
    void setGeometry( const int x, const int y, const int width, const int height )
    {
        this->setPosition( x, y );
        this->setSize( width, height );
    }
    void setGeometry( const kvs::Vec2i& p, const kvs::Vec2i& s )
    {
        this->setPosition( p );
        this->setSize( s );
    }
    void setGeometry( const kvs::Vec4i& g )
    {
        this->setPosition( g.x(), g.y() );
        this->setSize( g.z(), g.w() );
    }

    bool contains( const int x, const int y, const bool proper = false ) const;
    bool contains( const kvs::Vec2i& p, const bool proper = false ) const
    {
        return this->contains( p.x(), p.y(), proper );
    }
};

} // end of namespace kvs
