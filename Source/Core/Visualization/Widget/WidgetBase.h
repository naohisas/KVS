/*****************************************************************************/
/**
 *  @file   WidgetBase.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#pragma once
#include <kvs/ClassName>
#include <kvs/RGBAColor>
#include <kvs/EventListener>
#include <kvs/Rectangle>
#include <kvs/Painter>
#include <kvs/Deprecated>


namespace kvs
{

class ScreenBase;

/*===========================================================================*/
/**
 *  @brief  Base class of the widget.
 */
/*===========================================================================*/
class WidgetBase : public kvs::Rectangle, public kvs::EventListener
{
public:
    enum Anchor
    {
        Fixed, // fixed at the specified position (not anchored)
        TopLeft,
        TopCenter,
        TopRight,
        BottomLeft,
        BottomCenter,
        BottomRight,
        LeftCenter,
        Center,
        RightCenter
    };

private:
    const WidgetBase* m_parent; ///< pointer to the parent widget
    kvs::ScreenBase* m_screen; ///< pointer to the screen
    kvs::Painter m_painter; ///< painter

    int m_margin; ///< margin
    kvs::RGBAColor m_background_color; ///< background color
    kvs::RGBAColor m_background_border_color; ///< background border color
    float m_background_border_width; ///< background border width
    bool m_visible; ///< visible status flag
    bool m_active; ///< active status flag
    Anchor m_anchor; ///< anchor point on the screen or the parent widget
    Anchor m_corner; ///< corner point of this widget pinned to the screen or the parent widget

public:
    WidgetBase( kvs::ScreenBase* screen );
    virtual ~WidgetBase();

public:
    const kvs::Font& font() const { return m_painter.font(); }
    int margin() const { return m_margin; }
    const kvs::RGBAColor& backgroundColor() const { return m_background_color; }
    const kvs::RGBAColor& backgroundBorderColor() const { return m_background_border_color; }
    float backgroundBorderWidth() const { return m_background_border_width; }
    bool isVisible() const { return m_visible; }
    bool isActive() const { return m_active; }
    Anchor anchor() const { return m_anchor; }

    void setFont( const kvs::Font& font ) { m_painter.setFont( font ); }
    void setMargin( const int margin ) { m_margin = margin; }
    void setBackgroundColor( const kvs::RGBAColor& color ) { m_background_color = color; }
    void setBackgroundBorderColor( const kvs::RGBAColor& color ) { m_background_border_color = color; }
    void setBackgroundBorderWidth( const float width ) { m_background_border_width = width; }
    void setBackgroundOpacity( const float opacity );
    void setBackgroundBorderOpacity( const float opacity );
    void setVisible( const bool visible = true ) { m_visible = visible; }
    void setActive( const bool active = true ) { m_active = active; }

    // Anchoring to the screen
    void anchorTo( const Anchor anchor ) { m_anchor = anchor; }
    void anchorToTopLeft() { this->anchorTo( TopLeft ); }
    void anchorToTopCenter() { this->anchorTo( TopCenter ); }
    void anchorToTopRight() { this->anchorTo( TopRight ); }
    void anchorToBottomLeft() { this->anchorTo( BottomLeft ); }
    void anchorToBottomCenter() { this->anchorTo( BottomCenter ); }
    void anchorToBottomRight() { this->anchorTo( BottomRight ); }
    void anchorToLeftCenter() { this->anchorTo( LeftCenter ); }
    void anchorToCenter() { this->anchorTo( Center ); }
    void anchorToRightCenter() { this->anchorTo( RightCenter ); }

    // Anchoring to the parent widget
    void anchorTo( const kvs::WidgetBase* parent, const Anchor anchor, const Anchor corner ) { m_parent = parent; m_anchor = anchor; m_corner = corner; }
    void anchorToTopLeft( const kvs::WidgetBase* parent, const Anchor corner ) { this->anchorTo( parent, TopLeft, corner ); }
    void anchorToTopRight( const kvs::WidgetBase* parent, const Anchor corner ) { this->anchorTo( parent, TopRight, corner ); }
    void anchorToBottomLeft( const kvs::WidgetBase* parent, const Anchor corner ) { this->anchorTo( parent, BottomLeft, corner ); }
    void anchorToBottomRight( const kvs::WidgetBase* parent, const Anchor corner ) { this->anchorTo( parent, BottomRight, corner ); }
    void anchorTopLeftTo( const kvs::WidgetBase* parent, const Anchor anchor ) { this->anchorTo( parent, anchor, TopLeft ); }
    void anchorTopLeftToTopRight( const kvs::WidgetBase* parent ) { this->anchorToTopRight( parent, TopLeft ); }
    void anchorTopLeftToBottomRight( const kvs::WidgetBase* parent ) { this->anchorToBottomRight( parent, TopLeft ); }
    void anchorTopLeftToBottomLeft( const kvs::WidgetBase* parent ) { this->anchorToBottomLeft( parent, TopLeft ); }
    void anchorTopRightTo( const kvs::WidgetBase* parent, const Anchor anchor ) { this->anchorTo( parent, anchor, TopRight ); }
    void anchorTopRightToTopLeft( const kvs::WidgetBase* parent ) { this->anchorToTopLeft( parent, TopRight ); }
    void anchorTopRightToBottomLeft( const kvs::WidgetBase* parent ) { this->anchorToBottomLeft( parent, TopRight ); }
    void anchorTopRightToBottomRight( const kvs::WidgetBase* parent ) { this->anchorToBottomRight( parent, TopRight ); }
    void anchorBottomLeftTo( const kvs::WidgetBase* parent, const Anchor anchor ) { this->anchorTo( parent, anchor, BottomLeft ); }
    void anchorBottomLeftToTopLeft( const kvs::WidgetBase* parent ) { this->anchorToTopLeft( parent, BottomLeft ); }
    void anchorBottomLeftToTopRight( const kvs::WidgetBase* parent ) { this->anchorToTopRight( parent, BottomLeft ); }
    void anchorBottomLeftToBottomRight( const kvs::WidgetBase* parent ) { this->anchorToBottomRight( parent, BottomLeft ); }
    void anchorBottomRightTo( const kvs::WidgetBase* parent, const Anchor anchor ) { this->anchorTo( parent, anchor, BottomRight ); }
    void anchorBottomRightToTopLeft( const kvs::WidgetBase* parent ) { this->anchorToTopLeft( parent, BottomRight ); }
    void anchorBottomRightToTopRight( const kvs::WidgetBase* parent ) { this->anchorToTopRight( parent, BottomRight ); }
    void anchorBottomRightToBottomLeft( const kvs::WidgetBase* parent ) { this->anchorToBottomLeft( parent, BottomRight ); }
    void anchorToTop( const kvs::WidgetBase* parent );
    void anchorToBottom( const kvs::WidgetBase* parent );
    void anchorToLeft( const kvs::WidgetBase* parent );
    void anchorToRight( const kvs::WidgetBase* parent );

    void show();
    void hide();

protected:
    kvs::ScreenBase* screen() { return m_screen; }
    kvs::Painter& painter() { return m_painter; }
    kvs::Vec2i anchorPosition();
    virtual void drawBackground();
    virtual int adjustedWidth() { return 0; }
    virtual int adjustedHeight() { return 0; }
    virtual void initializeEvent();

public:
    KVS_DEPRECATED( bool isShown() const ) { return this->isVisible(); }
    KVS_DEPRECATED( void activate() ) { this->setActive( true ); }
    KVS_DEPRECATED( void deactivate() ) { this->setActive( false ); }
};

} // end of namespace kvs
