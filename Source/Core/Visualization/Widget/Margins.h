/*****************************************************************************/
/**
 *  @file   Margins.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#pragma once
#include <kvs/Math>
#include <kvs/Rectangle>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Margin class.
 */
/*===========================================================================*/
class Margins
{
private:
    int m_left; ///< left margin
    int m_top; ///< top margin
    int m_right; ///< right margin
    int m_bottom; ///< bottom margin

public:
    Margins():
        m_left(0),
        m_top(0),
        m_right(0),
        m_bottom(0) {}

    Margins( const int margin ):
        m_left( margin ),
        m_top( margin ),
        m_right( margin ),
        m_bottom( margin ) {}

    Margins( const int left, const int top, const int right, const int bottom ):
        m_left( left ),
        m_top( top ),
        m_right( right ),
        m_bottom( bottom ) {}

    void setLeft( const int left ) { m_left = left; }
    void setTop( const int top ) { m_top = top; }
    void setRight( const int right ) { m_right = right; }
    void setBottom( const int bottom ) { m_bottom = bottom; }

    int left() const { return m_left; }
    int top() const { return m_top; }
    int right() const { return m_right; }
    int bottom() const { return m_bottom; }

    kvs::Rectangle content( const int width, const int height ) const;

    friend bool operator == ( const Margins& lhs, const Margins& rhs );
    friend bool operator != ( const Margins& lhs, const Margins& rhs );
};

/*===========================================================================*/
/**
 *  @brief  Returns content rectangle.
 *  @param  width [in] width of outer rectangle
 *  @param  height [in] height of outer rectangle
 *  @return content rectangle
 */
/*===========================================================================*/
inline kvs::Rectangle Margins::content( const int width, const int height ) const
{
    return kvs::Rectangle( m_left, m_top, width - m_left - m_right, height - m_top - m_bottom );
}

inline bool operator == ( const Margins& lhs, const Margins& rhs )
{
    return
        kvs::Math::Equal( lhs.left(), rhs.left() ) &&
        kvs::Math::Equal( lhs.top(), rhs.top() ) &&
        kvs::Math::Equal( lhs.right(), rhs.right() ) &&
        kvs::Math::Equal( lhs.bottom(), rhs.bottom() );
}

inline bool operator != ( const Margins& lhs, const Margins& rhs )
{
    return !operator==( lhs, rhs );
}

} // end of namespace kvs
