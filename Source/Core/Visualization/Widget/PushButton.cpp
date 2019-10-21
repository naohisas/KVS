/*****************************************************************************/
/**
 *  @file   PushButton.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id$
 */
/*****************************************************************************/
#include "PushButton.h"
#include <kvs/OpenGL>
#include <kvs/RGBColor>
#include <kvs/RGBAColor>
#include <kvs/String>
#include <kvs/IgnoreUnusedVariable>
#include <kvs/EventBase>
#include <kvs/MouseEvent>


// Default parameters.
namespace { namespace Default
{
const int           TextMargin = 10;
const int           ButtonMargin = 0;
const kvs::RGBColor ButtonColor = kvs::RGBColor( 200, 200, 200 );
} }

// Instance counter.
static int InstanceCounter = 0;


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Constructors a new PushButton class.
 *  @param  screen [in] pointer to the parent screen
 */
/*===========================================================================*/
PushButton::PushButton( kvs::ScreenBase* screen ):
    kvs::WidgetBase( screen ),
    m_pushed( false ),
    m_grad_top_color( kvs::RGBColor::White() ),
    m_grad_bottom_color( kvs::RGBColor::Black() ),
    m_border_light_color( kvs::RGBColor( 250, 250, 250 ) ),
    m_border_dark_color( kvs::RGBColor( 0, 0, 0 ) )
{
    BaseClass::addEventType(
        kvs::EventBase::PaintEvent |
        kvs::EventBase::ResizeEvent |
        kvs::EventBase::MousePressEvent |
        kvs::EventBase::MouseReleaseEvent );

    BaseClass::setMargin( ::Default::ButtonMargin );
    this->setCaption( "PushButton " + kvs::String::ToString( ::InstanceCounter++ ) );
    this->setTextMargin( ::Default::TextMargin );
    this->setButtonColor( ::Default::ButtonColor );

    BaseClass::painter().font().setStyleToBold();
    BaseClass::painter().font().setColor( kvs::RGBColor::White() );
}

/*===========================================================================*/
/**
 *  @brief  Returns the fitted width.
 *  @return fitted width
 */
/*===========================================================================*/
int PushButton::adjustedWidth()
{
    BaseClass::painter().begin( BaseClass::screen() );
    const kvs::FontMetrics metrics = BaseClass::painter().fontMetrics();
    const size_t text_width = metrics.width( m_caption );
    BaseClass::painter().end();
    return text_width + ( m_text_margin + BaseClass::margin() ) * 2;
}

/*===========================================================================*/
/**
 *  @brief  Returns the fitted height.
 *  @return fitted height
 */
/*===========================================================================*/
int PushButton::adjustedHeight()
{
    BaseClass::painter().begin( BaseClass::screen() );
    const kvs::FontMetrics metrics = BaseClass::painter().fontMetrics();
    const size_t character_height = metrics.height();
    BaseClass::painter().end();
    return character_height + ( m_text_margin + BaseClass::margin() ) * 2;
}

/*===========================================================================*/
/**
 *  @brief  Returns the centering position x.
 *  @return x coordinate
 */
/*===========================================================================*/
int PushButton::get_aligned_x()
{
    // Centering the caption along the x axis.
    const GLfloat x0 = static_cast<GLfloat>( BaseClass::x0() + BaseClass::margin() );
    const GLfloat x1 = static_cast<GLfloat>( BaseClass::x1() - BaseClass::margin() );
    const size_t text_width = BaseClass::painter().fontMetrics().width( m_caption );
    return static_cast<int>( x0 + ( x1 - x0 - text_width ) * 0.5f + 0.5f );
}

/*===========================================================================*/
/**
 *  @brief  Returns the centering position y.
 *  @return y coordinate
 */
/*===========================================================================*/
int PushButton::get_aligned_y()
{
    // Centering the caption along the y axis.
    const GLfloat y0 = static_cast<GLfloat>( BaseClass::y0() + BaseClass::margin() );
    const GLfloat y1 = static_cast<GLfloat>( BaseClass::y1() - BaseClass::margin() );
    const size_t text_height = BaseClass::painter().fontMetrics().height();
    return static_cast<int>( y0 + ( y1 - y0 - text_height ) * 0.5f + 0.5f );
}

/*===========================================================================*/
/**
 *  @brief  Draws the button.
 */
/*===========================================================================*/
void PushButton::draw_button()
{
    kvs::NanoVG* engine = BaseClass::painter().device()->renderEngine();
    engine->beginFrame( screen()->width(), screen()->height(), screen()->devicePixelRatio() );

    const GLfloat x0 = static_cast<GLfloat>( BaseClass::x0() + BaseClass::margin() );
    const GLfloat x1 = static_cast<GLfloat>( BaseClass::x1() - BaseClass::margin() );
    const GLfloat y0 = static_cast<GLfloat>( BaseClass::y0() + BaseClass::margin() );
    const GLfloat y1 = static_cast<GLfloat>( BaseClass::y1() - BaseClass::margin() );
    const float width = x1 - x0;
    const float height = y1 - y0;

    const float corner_radius = 4.0f;
    engine->beginPath();
    {
        const float x = x0 + 1.0f;
        const float y = y0 + 1.0f;
        const float w = width - 2.0f;
        const float h = height - 2.0f;
        const float r = corner_radius - 1.0f;
        const kvs::RGBColor color = m_pushed ? kvs::RGBColor( 60, 150, 250 ) : m_button_color;
        engine->roundedRect( x, y, w, h, r );
        engine->setFillColor( color );
        engine->fill();

        const kvs::RGBAColor top_color( m_pushed ? m_grad_bottom_color : m_grad_top_color, m_pushed ? 0.3f : 0.1f );
        const kvs::RGBAColor bottom_color( m_grad_bottom_color, 0.2f );
        const kvs::Vec2 p0( x0, y0 );
        const kvs::Vec2 p1( x0, y1 );
        NVGpaint bg = engine->linearGradient( p0, p1, top_color, bottom_color );
        engine->setFillPaint( bg );
        engine->fill();
    }

    engine->beginPath();
    {
        const float x = x0 + 0.5f;
        const float y = y0 + ( m_pushed ? 0.5f : 1.5f );
        const float w = width - 1.0f;
        const float h = height - 1.0f - ( m_pushed ? 0.0f : 1.0f );
        const float r = corner_radius;
        const kvs::RGBAColor light_color( m_border_light_color, 0.6f );
        engine->roundedRect( x, y, w, h, r );
        engine->setStrokeColor( light_color );
        engine->setStrokeWidth( 1.0f );
        engine->stroke();
    }

    {
        const float x = x0 + 0.5f;
        const float y = y0 + 0.5f;
        const float w = width - 1.0f;
        const float h = height - 2.0f;
        const float r = corner_radius;
        const kvs::RGBAColor dark_color( m_border_dark_color, 0.6f );
        engine->roundedRect( x, y, w, h, r );
        engine->setStrokeColor( dark_color );
        engine->stroke();
    }

    engine->endFrame();
}

/*===========================================================================*/
/**
 *  @brief  Paint event.
 */
/*===========================================================================*/
void PushButton::paintEvent()
{
    this->screenUpdated();

    if ( !BaseClass::isShown() ) return;

    BaseClass::painter().begin( BaseClass::screen() );
    BaseClass::drawBackground();

    this->draw_button();

    const size_t text_height = BaseClass::painter().fontMetrics().height();
    const kvs::Vec2 p( this->get_aligned_x(), this->get_aligned_y() + text_height );
    const kvs::Font font = BaseClass::painter().font();
    BaseClass::painter().font().setColor( kvs::RGBColor::Black() );
    BaseClass::painter().drawText( p, m_caption );
    BaseClass::painter().setFont( font );

    const kvs::Vec2 d( 0.5, 1 );
//    BaseClass::painter().font().setColor( kvs::RGBColor::White() );
    BaseClass::painter().drawText( p + d, m_caption );

    BaseClass::painter().end();
}

/*===========================================================================*/
/**
 *  @brief  Resize event.
 *  @param  width [in] resized screen width
 *  @param  height [in] resized screen height
 */
/*===========================================================================*/
void PushButton::resizeEvent( int width, int height )
{
    kvs::IgnoreUnusedVariable( width );
    kvs::IgnoreUnusedVariable( height );

    this->screenResized();
}

/*===========================================================================*/
/**
 *  @brief  Mouse press event.
 *  @param  event [in] pointer to the mouse event
 */
/*===========================================================================*/
void PushButton::mousePressEvent( kvs::MouseEvent* event )
{
    if ( !BaseClass::isShown() ) return;

    if ( BaseClass::contains( event->x(), event->y() ) )
    {
        m_pushed = true;
        BaseClass::screen()->disable();
        BaseClass::activate();
        this->pressed();
        BaseClass::screen()->redraw();
    }
}

/*===========================================================================*/
/**
 *  @brief  Mouse release event.
 *  @param  event [in] pointer to the mouse event
 */
/*===========================================================================*/
void PushButton::mouseReleaseEvent( kvs::MouseEvent* event )
{
    kvs::IgnoreUnusedVariable( event );

    if ( !BaseClass::isShown() ) return;

    if ( BaseClass::isActive() )
    {
        m_pushed = false;
        this->released();
        BaseClass::deactivate();
        BaseClass::screen()->redraw();
    }
}

} // end of namespace kvs
