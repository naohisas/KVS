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


// Default parameters.
namespace { namespace Default
{
const kvs::RGBAColor BackgroundColor = kvs::RGBAColor( 200, 200, 200, 0.0f );
const kvs::RGBAColor BackgroundBorderColor = kvs::RGBAColor( 0, 0, 0, 1.0f );
const float BackgroundBorderWidth = 0.0f;
} }


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new WidgetBase class.
 *  @param  screen [in] pointer to the screen
 */
/*===========================================================================*/
WidgetBase::WidgetBase( kvs::ScreenBase* screen ):
    m_parent( nullptr ),
    m_screen( screen ),
    m_painter(),
    m_margin( 0 ),
    m_visible( false ),
    m_active( false ),
    m_anchor( Fixed ),
    m_corner( TopLeft )
{
    addEventType( kvs::EventBase::InitializeEvent );

    if ( screen ) screen->eventHandler()->attach( this );

    // Set default parameters.
    this->setBackgroundColor( ::Default::BackgroundColor );
    this->setBackgroundBorderColor( ::Default::BackgroundBorderColor );
    this->setBackgroundBorderWidth( ::Default::BackgroundBorderWidth );
}

/*===========================================================================*/
/**
 *  @brief  Destructs the WidgetBase class.
 */
/*===========================================================================*/
WidgetBase::~WidgetBase()
{
}

/*===========================================================================*/
/**
 *  @brief  Sets a background opacity value.
 *  @param  opacity [in] opacity value
 */
/*===========================================================================*/
void WidgetBase::setBackgroundOpacity( const float opacity )
{
    const kvs::UInt8 r = m_background_color.r();
    const kvs::UInt8 g = m_background_color.g();
    const kvs::UInt8 b = m_background_color.b();
    m_background_color = kvs::RGBAColor( r, g, b, opacity );
}

/*===========================================================================*/
/**
 *  @brief  Sets a background border opacity value.
 *  @param  opacity [in] background opacity value
 */
/*===========================================================================*/
void WidgetBase::setBackgroundBorderOpacity( const float opacity )
{
    const kvs::UInt8 r = m_background_border_color.r();
    const kvs::UInt8 g = m_background_border_color.g();
    const kvs::UInt8 b = m_background_border_color.b();
    m_background_border_color = kvs::RGBAColor( r, g, b, opacity );
}

void WidgetBase::anchorToTop( const kvs::WidgetBase* parent )
{
    switch ( parent->anchor() )
    {
    case Anchor::TopRight:
    case Anchor::BottomRight:
    case Anchor::RightCenter:
        this->anchorBottomRightToTopRight( parent );
        break;
    default:
        this->anchorBottomLeftToTopLeft( parent );
        break;
    }
}

void WidgetBase::anchorToBottom( const kvs::WidgetBase* parent )
{
    switch ( parent->anchor() )
    {
    case Anchor::TopRight:
    case Anchor::BottomRight:
    case Anchor::RightCenter:
        this->anchorTopRightToBottomRight( parent );
        break;
    default:
        this->anchorTopLeftToBottomLeft( parent );
        break;
    }
}

void WidgetBase::anchorToLeft( const kvs::WidgetBase* parent )
{
    switch ( parent->anchor() )
    {
    case Anchor::BottomLeft:
    case Anchor::BottomCenter:
    case Anchor::BottomRight:
        this->anchorBottomRightToBottomLeft( parent );
        break;
    default:
        this->anchorTopRightToTopLeft( parent );
        break;
    }
}

void WidgetBase::anchorToRight( const kvs::WidgetBase* parent )
{
    switch ( parent->anchor() )
    {
    case Anchor::BottomLeft:
    case Anchor::BottomCenter:
    case Anchor::BottomRight:
        this->anchorBottomLeftToBottomRight( parent );
        break;
    default:
        this->anchorTopLeftToTopRight( parent );
        break;
    }
}

/*===========================================================================*/
/**
 *  @brief  Shows the screen.
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
 *  @brief  Hides the screen.
 */
/*===========================================================================*/
void WidgetBase::hide()
{
    m_visible = false;
}

kvs::Vec2i WidgetBase::anchorPosition()
{
    if ( m_parent )
    {
        const int px0 = m_parent->x0();
        const int py0 = m_parent->y0();
        const int px1 = m_parent->x1();
        const int py1 = m_parent->y1();
        const int aw = this->width();
        const int ah = this->height();
        switch ( m_anchor )
        {
        case Anchor::TopLeft:
        {
            switch ( m_corner )
            {
            case Anchor::BottomLeft: return kvs::Vec2i( px0, py0 - ah );
            case Anchor::BottomRight: return kvs::Vec2i( px0 - aw, py0 - ah );
            case Anchor::TopRight: return kvs::Vec2i( px0 - aw, py0 );
            default: break;
            }
            break;
        }
        case Anchor::TopRight:
        {
            switch ( m_corner )
            {
            case Anchor::BottomLeft: return kvs::Vec2i( px1, py0 - ah );
            case Anchor::BottomRight: return kvs::Vec2i( px1 - aw, py0 - ah );
            case Anchor::TopLeft: return kvs::Vec2i( px1, py0 );
            default: break;
            }
            break;
        }
        case Anchor::BottomLeft:
        {
            switch ( m_corner )
            {
            case Anchor::BottomRight: return kvs::Vec2i( px0 - aw, py1 - ah );
            case Anchor::TopRight: return kvs::Vec2i( px0 - aw, py1 );
            case Anchor::TopLeft: return kvs::Vec2i( px0, py1 );
            default: break;
            }
            break;
        }
        case Anchor::BottomRight:
        {
            switch ( m_corner )
            {
            case Anchor::BottomLeft: return kvs::Vec2i( px1, py1 - ah );
            case Anchor::TopLeft: return kvs::Vec2i( px1, py1 );
            case Anchor::TopRight: return kvs::Vec2i( px1 - aw, py1 );
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
        const int aw = this->width();
        const int ah = this->height();
        switch ( m_anchor )
        {
        case Anchor::TopLeft: return kvs::Vec2i( 0, 0 );
        case Anchor::TopCenter: return kvs::Vec2i( ( w - aw ) / 2, 0 );
        case Anchor::TopRight: return kvs::Vec2i( w - aw, 0 );
        case Anchor::BottomLeft: return kvs::Vec2i( 0, h - ah );
        case Anchor::BottomCenter: return kvs::Vec2i( ( w - aw ) / 2, h - ah );
        case Anchor::BottomRight: return kvs::Vec2i( w - aw, h - ah );
        case Anchor::LeftCenter: return kvs::Vec2i( 0, ( h - ah ) / 2 );
        case Anchor::Center: return kvs::Vec2i( ( w - aw ) / 2, ( h - ah ) / 2 );
        case Anchor::RightCenter: return kvs::Vec2i( w - aw, ( h - ah ) / 2 );
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

    if ( m_background_color.a() > 0.0f )
    {
        // Draw background.
        kvs::OpenGL::Begin( GL_POLYGON );
        {
            kvs::OpenGL::Color( m_background_color );
            kvs::OpenGL::Vertex( kvs::Vec2( this->x0(), this->y1() ) );
            kvs::OpenGL::Vertex( kvs::Vec2( this->x0(), this->y0() ) );
            kvs::OpenGL::Vertex( kvs::Vec2( this->x1(), this->y0() ) );
            kvs::OpenGL::Vertex( kvs::Vec2( this->x1(), this->y1() ) );
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
            kvs::OpenGL::Vertex( kvs::Vec2( this->x0(), this->y1() ) );
            kvs::OpenGL::Vertex( kvs::Vec2( this->x0(), this->y0() ) );
            kvs::OpenGL::Vertex( kvs::Vec2( this->x1(), this->y0() ) );
            kvs::OpenGL::Vertex( kvs::Vec2( this->x1(), this->y1() ) );
        }
        kvs::OpenGL::End();
    }
}

void WidgetBase::initializeEvent()
{
    if ( Rectangle::width() == 0 ) Rectangle::setWidth( this->adjustedWidth() );
    if ( Rectangle::height() == 0 ) Rectangle::setHeight( this->adjustedHeight() );
    const auto p = anchorPosition();
    Rectangle::setPosition( p.x(), p.y() );
}

} // end of namespace kvs
