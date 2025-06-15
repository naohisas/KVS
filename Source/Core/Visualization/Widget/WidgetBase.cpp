/*****************************************************************************/
/**
 *  @file   WidgetBase.cpp
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#include "WidgetBase.h"
#include <kvs/OpenGL>
#include <kvs/HSVColor>
#include <kvs/RGBColor>
#include <kvs/EventBase>
#include <kvs/EventHandler>
#include <kvs/ScreenBase>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new WidgetBase class.
 *  @param  screen [in] pointer to the screen
 */
/*===========================================================================*/
WidgetBase::WidgetBase( kvs::ScreenBase* screen ):
    m_screen( screen )
{
    addEventType( kvs::EventBase::InitializeEvent );
    if ( screen ) screen->eventHandler()->attach( this );
}

/*===========================================================================*/
/**
 *  @brief  Sets a background opacity value.
 *  @param  opacity [in] opacity value
 */
/*===========================================================================*/
void WidgetBase::setBackgroundOpacity( const float opacity )
{
    m_background_color = { m_background_color, opacity };
}

/*===========================================================================*/
/**
 *  @brief  Sets a background border opacity value.
 *  @param  opacity [in] background opacity value
 */
/*===========================================================================*/
void WidgetBase::setBackgroundBorderOpacity( const float opacity )
{
    m_background_border_color = { m_background_border_color, opacity };
}

/*===========================================================================*/
/**
 *  @brief  Anchors to top of the parent widget.
 *  @param  parent [in] pointer to the parent widget
 */
/*===========================================================================*/
void WidgetBase::anchorToTop( const kvs::WidgetBase* parent )
{
    switch ( parent->anchor() )
    {
    case Anchor::TopRight:
    case Anchor::BottomRight:
    case Anchor::RightCenter:
        //
        //        +----------+
        //        |   this   |
        //   +----+----------+
        //   |    parent     |
        //   +---------------+
        //
        this->anchorBottomRightToTopRight( parent );
        break;
    default:
        //
        //   +----------+
        //   |   this   |
        //   +----------+----+
        //   |    parent     |
        //   +---------------+
        //
        this->anchorBottomLeftToTopLeft( parent );
        break;
    }
}

/*===========================================================================*/
/**
 *  @brief  Anchors to bottom of the parent widget.
 *  @param  parent [in] pointer to the parent widget
 */
/*===========================================================================*/
void WidgetBase::anchorToBottom( const kvs::WidgetBase* parent )
{
    switch ( parent->anchor() )
    {
    case Anchor::TopRight:
    case Anchor::BottomRight:
    case Anchor::RightCenter:
        //
        //   +---------------+
        //   |    parent     |
        //   +----+----------+
        //        |   this   |
        //        +----------+
        //
        this->anchorTopRightToBottomRight( parent );
        break;
    default:
        //
        //   +---------------+
        //   |    parent     |
        //   +----------+----+
        //   |   this   |
        //   +----------+
        //
        this->anchorTopLeftToBottomLeft( parent );
        break;
    }
}

/*===========================================================================*/
/**
 *  @brief  Anchors to left of the parent widget.
 *  @param  parent [in] pointer to the parent widget
 */
/*===========================================================================*/
void WidgetBase::anchorToLeft( const kvs::WidgetBase* parent )
{
    switch ( parent->anchor() )
    {
    case Anchor::BottomLeft:
    case Anchor::BottomCenter:
    case Anchor::BottomRight:
        //
        //            +--------------+
        //            |              |
        //   +--------+    parent    |
        //   |  this  |              |
        //   +--------+--------------+
        //
        this->anchorBottomRightToBottomLeft( parent );
        break;
    default:
        //
        //   +------- +--------------+
        //   |  this  |              |
        //   +--------+    parent    |
        //            |              |
        //            +--------------+
        //
        this->anchorTopRightToTopLeft( parent );
        break;
    }
}

/*===========================================================================*/
/**
 *  @brief  Anchors to right of the parent widget.
 *  @param  parent [in] pointer to the parent widget
 */
/*===========================================================================*/
void WidgetBase::anchorToRight( const kvs::WidgetBase* parent )
{
    switch ( parent->anchor() )
    {
    case Anchor::BottomLeft:
    case Anchor::BottomCenter:
    case Anchor::BottomRight:
        //
        //   +--------------+
        //   |              |
        //   |    parent    +--------+
        //   |              |  this  |
        //   +--------------+--------+
        //
        this->anchorBottomLeftToBottomRight( parent );
        break;
    default:
        //
        //   +--------------+--------+
        //   |              |  this  |
        //   |    parent    +--------+
        //   |              |
        //   +--------------+
        //
        this->anchorTopLeftToTopRight( parent );
        break;
    }
}

/*===========================================================================*/
/**
 *  @brief  Shows the widget.
 */
/*===========================================================================*/
void WidgetBase::show()
{
    if ( Rectangle::width() == 0 ) Rectangle::setWidth( this->adjustedWidth() );
    if ( Rectangle::height() == 0 ) Rectangle::setHeight( this->adjustedHeight() );

    m_visible = true;
}

/*===========================================================================*/
/**
 *  @brief  Hides the widget.
 */
/*===========================================================================*/
void WidgetBase::hide()
{
    m_visible = false;
}

/*===========================================================================*/
/**
 *  @brief  Returns anchor position of the widget.
 *  @return anchor position
 */
/*===========================================================================*/
kvs::Vec2i WidgetBase::anchorPosition()
{
    const int aw = this->width();
    const int ah = this->height();
    if ( m_parent )
    {
        const int px0 = m_parent->x0();
        const int py0 = m_parent->y0();
        const int px1 = m_parent->x1();
        const int py1 = m_parent->y1();
        switch ( m_anchor )
        {
        case Anchor::TopLeft:
        {
            switch ( m_corner )
            {
            case Anchor::BottomLeft:  return { px0, py0 - ah };
            case Anchor::BottomRight: return { px0 - aw, py0 - ah };
            case Anchor::TopRight:    return { px0 - aw, py0 };
            default: break;
            }
            break;
        }
        case Anchor::TopRight:
        {
            switch ( m_corner )
            {
            case Anchor::BottomLeft:  return { px1, py0 - ah };
            case Anchor::BottomRight: return { px1 - aw, py0 - ah };
            case Anchor::TopLeft:     return { px1, py0 };
            default: break;
            }
            break;
        }
        case Anchor::BottomLeft:
        {
            switch ( m_corner )
            {
            case Anchor::BottomRight: return { px0 - aw, py1 - ah };
            case Anchor::TopRight:    return { px0 - aw, py1 };
            case Anchor::TopLeft:     return { px0, py1 };
            default: break;
            }
            break;
        }
        case Anchor::BottomRight:
        {
            switch ( m_corner )
            {
            case Anchor::BottomLeft: return { px1, py1 - ah };
            case Anchor::TopLeft:    return { px1, py1 };
            case Anchor::TopRight:   return { px1 - aw, py1 };
            default: break;
            }
            break;
        }
        default: break;
        }
    }
    else
    {
        const int w = m_screen->width();
        const int h = m_screen->height();
        switch ( m_anchor )
        {
        case Anchor::TopLeft:      return { 0, 0 };
        case Anchor::TopCenter:    return { ( w - aw ) / 2, 0 };
        case Anchor::TopRight:     return { w - aw, 0 };
        case Anchor::BottomLeft:   return { 0, h - ah };
        case Anchor::BottomCenter: return { ( w - aw ) / 2, h - ah };
        case Anchor::BottomRight:  return { w - aw, h - ah };
        case Anchor::LeftCenter:   return { 0, ( h - ah ) / 2 };
        case Anchor::Center:       return { ( w - aw ) / 2, ( h - ah ) / 2 };
        case Anchor::RightCenter:  return { w - aw, ( h - ah ) / 2 };
        default: break;
        }
    }

    return topLeft();
}

/*==========================================================================*/
/**
 *  @brief  Draws the background.
 */
/*==========================================================================*/
void WidgetBase::drawBackground()
{
    kvs::OpenGL::WithPushedAttrib attrib( GL_ALL_ATTRIB_BITS );
    attrib.disable( GL_TEXTURE_1D );
    attrib.disable( GL_TEXTURE_2D );
    attrib.disable( GL_TEXTURE_3D );
    attrib.enable( GL_BLEND );

    kvs::OpenGL::SetBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

    const auto dpr = m_screen->devicePixelRatio();
    const auto x0 = this->x0() * dpr;
    const auto x1 = this->x1() * dpr;
    const auto y0 = this->y0() * dpr;
    const auto y1 = this->y1() * dpr;
    if ( m_background_color.a() > 0.0f )
    {
        // Draw background.
        kvs::OpenGL::Begin( GL_POLYGON );
        {
            kvs::OpenGL::Color( m_background_color );
            kvs::OpenGL::Vertex( kvs::Vec2( x0, y1 ) ); // bottom-left
            kvs::OpenGL::Vertex( kvs::Vec2( x0, y0 ) ); // top-left
            kvs::OpenGL::Vertex( kvs::Vec2( x1, y0 ) ); // top-right
            kvs::OpenGL::Vertex( kvs::Vec2( x1, y1 ) ); // bottom-right
        }
        kvs::OpenGL::End();
    }

    if ( m_background_border_width > 0.0f && m_background_border_color.a() > 0.0f )
    {
        // Draw outline of the background.
        kvs::OpenGL::SetLineWidth( m_background_border_width );
        kvs::OpenGL::Begin( GL_POLYGON );
        {
            kvs::OpenGL::Color( m_background_border_color );
            kvs::OpenGL::Vertex( kvs::Vec2( x0, y1 ) ); // bottom-left
            kvs::OpenGL::Vertex( kvs::Vec2( x0, y0 ) ); // top-left
            kvs::OpenGL::Vertex( kvs::Vec2( x1, y0 ) ); // top-right
            kvs::OpenGL::Vertex( kvs::Vec2( x1, y1 ) ); // bottom-right
        }
        kvs::OpenGL::End();
    }
}

/*===========================================================================*/
/**
 *  @brief  Initialize event method.
 */
/*===========================================================================*/
void WidgetBase::initializeEvent()
{
    if ( Rectangle::width() == 0 ) Rectangle::setWidth( this->adjustedWidth() );
    if ( Rectangle::height() == 0 ) Rectangle::setHeight( this->adjustedHeight() );
    Rectangle::setPosition( this->anchorPosition() );
}

} // end of namespace kvs
