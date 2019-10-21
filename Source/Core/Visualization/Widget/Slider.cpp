/*****************************************************************************/
/**
 *  @file   Slider.cpp
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
#include "Slider.h"
#include <kvs/OpenGL>
#include <kvs/String>
#include <kvs/IgnoreUnusedVariable>
#include <kvs/ScreenBase>
#include <kvs/EventBase>
#include <kvs/MouseEvent>


// Default parameters.
namespace { namespace Default
{
const float         Value = 0.5f;
const float         MinValue = 0.0f;
const float         MaxValue = 1.0f;
const size_t        SliderMargin = 10;
const size_t        SliderWidth = 150;
const size_t        SliderHeight = 30;
const kvs::RGBColor SliderColor = kvs::RGBColor( 200, 200, 200 );
const size_t        CursorWidth = 15;
const size_t        CursorHeight = 15;
const kvs::RGBColor CursorColor = kvs::RGBColor::White();
} }

// Instance counter.
static int InstanceCounter = 0;


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new Slider class.
 *  @param  screen [in] pointer to the parent screen
 */
/*===========================================================================*/
Slider::Slider( kvs::ScreenBase* screen ):
    kvs::WidgetBase( screen ),
    m_change_value( false ),
    m_show_range_value( true ),
    m_pushed( false )
{
    BaseClass::addEventType(
        kvs::EventBase::PaintEvent |
        kvs::EventBase::ResizeEvent |
        kvs::EventBase::MousePressEvent |
        kvs::EventBase::MouseMoveEvent |
        kvs::EventBase::MouseReleaseEvent );

    BaseClass::setMargin( ::Default::SliderMargin );
    this->setCaption( "Slider " + kvs::String::ToString( ::InstanceCounter++ ) );
    this->setValue( ::Default::Value );
    this->setRange( ::Default::MinValue, ::Default::MaxValue );
    this->setSliderColor( ::Default::SliderColor );
    this->setCursorColor( ::Default::CursorColor );
}

/*===========================================================================*/
/**
 *  @brief  Sets value range.
 *  @param  min_value [in] minimum value
 *  @param  max_value [in] maximum value
 */
/*===========================================================================*/
void Slider::setRange( const float min_value, const float max_value )
{
    m_min_value = min_value;
    m_max_value = max_value;

    // Clamp the current value.
    m_value = kvs::Math::Clamp( m_value, m_min_value, m_max_value );
}

/*===========================================================================*/
/**
 *  @brief  Sets a slider color.
 *  @param  color [in] slider color
 */
/*===========================================================================*/
void Slider::setSliderColor( const kvs::RGBColor& color )
{
    m_slider_color = color;
}

/*===========================================================================*/
/**
 *  @brief  Draws the slider bar.
 *  @param  x [in] slider position x
 *  @param  y [in] slider position y
 *  @param  width [in] slider width
 */
/*===========================================================================*/
void Slider::draw_slider_bar( const int x, const int y, const int width )
{
    kvs::NanoVG* engine = BaseClass::painter().device()->renderEngine();

    const float w = width;
    const float h = 6.0f;
    const float w0 = width * ( m_value - m_min_value ) / ( m_max_value - m_min_value );

    engine->beginPath();
    engine->roundedRect( x, y, w, h, 2 );

    engine->setStrokeWidth( 1.0f );
    engine->setStrokeColor( kvs::RGBColor::Black() );
    engine->stroke();

    engine->setFillColor( m_slider_color );
    engine->fill();

    const NVGcolor c032 = nvgRGBA( 0, 0, 0, 32 );
    const NVGcolor c128 = nvgRGBA( 0, 0, 0, 128 );
    const NVGcolor c064 = nvgRGBA( 0, 0, 0, 64 );
    NVGpaint bg = engine->boxGradient( x, y + 1.0f, w, h, 2, 2, c032, c128 );
    engine->setFillPaint( bg );
    engine->fill();

    if ( true )
    {
        engine->beginPath();
        engine->roundedRect( x, y, w0, h, 2 );
        engine->setFillColor( kvs::RGBColor( 60, 150, 250 ) );
        engine->fill();

        bg = engine->boxGradient( x, y + 4.0f, w0, h, 2, 3, c064, c032 );
        engine->setFillPaint( bg );
        engine->fill();
    }
}

/*===========================================================================*/
/**
 *  @brief  Draws the slide cursor.
 *  @param  x [in] slider cursor position x
 *  @param  y [in] slider cursor position y
 *  @param  width [in] slider width
 */
/*===========================================================================*/
void Slider::draw_cursor( const int x, const int y, const int width )
{
    kvs::NanoVG* engine = BaseClass::painter().device()->renderEngine();

    const int p = static_cast<int>( width * ( m_value - m_min_value ) / ( m_max_value - m_min_value ) + 0.5 );
    const int x0 = static_cast<int>( x + p - ::Default::CursorWidth * 0.5 + 0.5 );
    const int x1 = static_cast<int>( x + p + ::Default::CursorWidth * 0.5 + 0.5 );
    const int y0 = static_cast<int>( y - ::Default::CursorHeight * 0.5 + 0.5 );
    const int y1 = static_cast<int>( y + ::Default::CursorHeight * 0.5 + 0.5 );

    const kvs::Vec2 center( ( x0 + x1 ) * 0.5f, ( y0 + y1 ) * 0.5f + 3.0f );
    const float radius = ( y1 - y0 ) * 0.5f;

    // Shadow
    {
        const float shadow_size = 3.0f;
        const kvs::RGBColor shadow_color = kvs::RGBColor::Black();
        const kvs::RGBAColor color0( shadow_color, 0.25f );
        const kvs::RGBAColor color1( shadow_color, 0.0f );
        NVGpaint bg = engine->radialGradient( center, radius - shadow_size, radius + shadow_size, color0, color1 );
        engine->beginPath();
        engine->circle( center, radius + shadow_size );
        engine->setFillPaint( bg );
        engine->fill();
    }

    // Knob
    {
        // Outer circle
        engine->beginPath();
        engine->circle( center, radius );
        engine->setFillColor( m_slider_color );
        engine->fill();

        const float stroke_width = 1.0f;
        const kvs::RGBAColor stroke_color( 0, 0, 0, 0.8f );
        engine->setStrokeWidth( stroke_width );
        engine->setStrokeColor( stroke_color );
        engine->stroke();

        const kvs::Vec2 start( x0, y0 );
        const kvs::Vec2 end( x1, y1 );
        const kvs::RGBAColor color0( 0, 0, 0, m_pushed ? 0.2f : 0.01f );
        const kvs::RGBAColor color1( 0, 0, 0, m_pushed ? 0.6f : 0.4f );
        NVGpaint bg = engine->linearGradient( start, end, color0, color1 );
        engine->setFillPaint( bg );
        engine->fill();

        // Inner circle
        engine->beginPath();
        engine->circle( center, radius / 2.0f );

        engine->setFillColor( m_cursor_color );
        engine->fill();

        const kvs::RGBAColor color2( 0, 0, 0, 0.8f );
        const kvs::RGBAColor color3( 0, 0, 0, 0.2f );
        bg = engine->linearGradient( start, end, color2, color3 );
        engine->setStrokePaint( bg );
        engine->stroke();
        engine->fill();
    }
}

/*===========================================================================*/
/**
 *  @brief  Test whether the mouse cursor is on the slider bar or not.
 *  @param  x [in] mouse cursor position x
 *  @param  y [in] mouse cursor position y
 *  @param  proper [in] whether the edges are checked or not
 *  @return true, if the mouse cursor is on the slider bar
 */
/*===========================================================================*/
bool Slider::is_in_slider( const int x, const int y, const bool proper )
{
    BaseClass::painter().begin( BaseClass::screen() );
    const kvs::FontMetrics metrics = BaseClass::painter().fontMetrics();
    const int character_height = metrics.height();
    BaseClass::painter().end();

    const int bar_x = BaseClass::x0() + BaseClass::margin();
    const int bar_y = BaseClass::y0() + BaseClass::margin() + character_height + ::Default::SliderHeight / 2;
    const int bar_width = BaseClass::width() - BaseClass::margin() * 2;

    const int x0 = bar_x;
    const int x1 = bar_x + bar_width;
    const int y0 = bar_y - 3;
    const int y1 = bar_y + 3;

    if ( proper )
    {
        return( ( x0 < x ) && ( x < x1 ) && ( y0 < y ) && ( y < y1 ) );
    }
    else
    {
        return( ( x0 <= x ) && ( x <= x1 ) && ( y0 <= y ) && ( y <= y1 ) );
    }
}

/*===========================================================================*/
/**
 *  @brief  Test whether the mouse cursor is on the slider cursor or not.
 *  @param  x [in] mouse cursor position x
 *  @param  y [in] mouse cursor position y
 *  @param  proper [in] whether the edges are checked or not
 *  @return true, if the mouse cursor is on the slider cursor
 */
/*===========================================================================*/
bool Slider::is_in_cursor( const int x, const int y, const bool proper )
{
    BaseClass::painter().begin( BaseClass::screen() );
    const kvs::FontMetrics metrics = BaseClass::painter().fontMetrics();
    const int character_height = metrics.height();
    BaseClass::painter().end();

    const int bar_x = BaseClass::x0() + BaseClass::margin();
    const int bar_y = BaseClass::y0() + BaseClass::margin() + character_height + ::Default::SliderHeight / 2;
    const int bar_width = BaseClass::width() - BaseClass::margin() * 2;

    const int p = static_cast<int>( bar_width * ( m_value - m_min_value ) / ( m_max_value - m_min_value ) + 0.5f );
    const int x0 = static_cast<int>( bar_x + p - ::Default::CursorWidth * 0.5 + 0.5 );
    const int x1 = static_cast<int>( bar_x + p + ::Default::CursorWidth * 0.5 + 0.5 );
    const int y0 = static_cast<int>( bar_y - ::Default::CursorHeight * 0.5 + 0.5 );
    const int y1 = static_cast<int>( bar_y + ::Default::CursorHeight * 0.5 + 0.5 );

    if ( proper )
    {
        return( ( x0 < x ) && ( x < x1 ) && ( y0 < y ) && ( y < y1 ) );
    }
    else
    {
        return( ( x0 <= x ) && ( x <= x1 ) && ( y0 <= y ) && ( y <= y1 ) );
    }
}

/*===========================================================================*/
/**
 *  @brief  Returns the current specifying value.
 *  @param  x [in] mouse cursor position x
 *  @return specifying value
 */
/*===========================================================================*/
float Slider::get_value( const int x )
{
    const float bar_x = static_cast<float>( BaseClass::x() + BaseClass::margin() );
    const float bar_width = static_cast<float>( BaseClass::width() - BaseClass::margin() * 2 );
    return m_min_value + ( x - bar_x ) / bar_width * ( m_max_value - m_min_value );
}

/*===========================================================================*/
/**
 *  @brief  Returns the fitted width.
 *  @return fitted width
 */
/*===========================================================================*/
int Slider::adjustedWidth()
{
    BaseClass::painter().begin( BaseClass::screen() );
    const kvs::FontMetrics metrics = BaseClass::painter().fontMetrics();
    const size_t text_width = metrics.width( m_caption );
    const size_t width = text_width + BaseClass::margin() * 2;
    BaseClass::painter().end();

    return kvs::Math::Max( width, ::Default::SliderWidth );
}

/*===========================================================================*/
/**
 *  @brief  Returns the fitted height.
 *  @return fitted height
 */
/*===========================================================================*/
int Slider::adjustedHeight()
{
    BaseClass::painter().begin( BaseClass::screen() );
    const kvs::FontMetrics metrics = BaseClass::painter().fontMetrics();
    const size_t text_height = metrics.height();
    BaseClass::painter().end();

    return ::Default::SliderHeight + ( text_height + BaseClass::margin() ) * 2;
}

/*===========================================================================*/
/**
 *  @brief  Paint event.
 */
/*===========================================================================*/
void Slider::paintEvent()
{
    this->screenUpdated();

    if ( !BaseClass::isShown() ) return;

    BaseClass::painter().begin( BaseClass::screen() );
    BaseClass::drawBackground();

    kvs::NanoVG* engine = BaseClass::painter().device()->renderEngine();
    engine->beginFrame( screen()->width(), screen()->height(), screen()->devicePixelRatio() );

    const float height = BaseClass::painter().fontMetrics().height();
    // Draw the caption.
    {
        const int x = BaseClass::x0() + BaseClass::margin();
        const int y = BaseClass::y0() + BaseClass::margin();
        const kvs::Vec2 p( x, y + height );
        BaseClass::painter().drawText( p, m_caption );
    }

    // Draw the slider bar and cursor.
    {
        const int x = BaseClass::x0() + BaseClass::margin();
        const int y = BaseClass::y0() + BaseClass::margin() + height + ::Default::SliderHeight / 2;
        const int width = BaseClass::width() - BaseClass::margin() * 2;
        this->draw_slider_bar( x, y, width );
        this->draw_cursor( x, y, width );
    }

    // Draw the values.
    if ( m_show_range_value )
    {
        {
            const std::string min_value = kvs::String::ToString( m_min_value );
            const int x = BaseClass::x0() + BaseClass::margin();
            const int y = BaseClass::y0() + BaseClass::margin() + height + ::Default::SliderHeight;
            const kvs::Vec2 p( x, y + height );
            BaseClass::painter().drawText( p, min_value );
        }

        {
            const std::string max_value = kvs::String::ToString( m_max_value );
            const size_t text_width = BaseClass::painter().fontMetrics().width( max_value );
            const int x = BaseClass::x1() - BaseClass::margin() - text_width;
            const int y = BaseClass::y0() + BaseClass::margin() + height + ::Default::SliderHeight;
            const kvs::Vec2 p( x, y + height );
            BaseClass::painter().drawText( p, max_value );
        }
    }

    engine->endFrame();

    BaseClass::painter().end();
}

/*===========================================================================*/
/**
 *  @brief  Resize event.
 *  @param  width [in] resized screen width
 *  @param  height [in] resized screen height
 */
/*===========================================================================*/
void Slider::resizeEvent( int width, int height )
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
void Slider::mousePressEvent( kvs::MouseEvent* event )
{
    if ( !BaseClass::isShown() ) return;

    if ( this->is_in_cursor( event->x(), event->y() ) )
    {
        m_pushed = true;
        BaseClass::screen()->disable();
        BaseClass::activate();
        this->sliderPressed();
        BaseClass::screen()->redraw();
    }
    else
    {
        if ( this->is_in_slider( event->x(), event->y() ) )
        {
            /* This process is emitted when the slider bar is clicked, and then
             * the cursor is moved to the clicked position.
             */
            m_value = this->get_value( event->x() );
            m_value = kvs::Math::Clamp( m_value, m_min_value, m_max_value );
            m_change_value = true;

            /* Since the slider cursor is moved to the clicked position,
             * not the sliderPressed function but the sliderMoved function is
             * called here.
             */
            BaseClass::screen()->disable();
            BaseClass::activate();
            this->sliderMoved();
            BaseClass::screen()->redraw();
        }
    }
}

/*===========================================================================*/
/**
 *  @brief  Mouse move event.
 *  @param  event [in] pointer to the mouse event
 */
/*===========================================================================*/
void Slider::mouseMoveEvent( kvs::MouseEvent* event )
{
    if ( !BaseClass::isShown() ) return;

    if ( BaseClass::isActive() )
    {
        // Update the value.
        m_value = this->get_value( event->x() );
        m_value = kvs::Math::Clamp( m_value, m_min_value, m_max_value );
        m_change_value = true;

        this->sliderMoved();

        BaseClass::screen()->redraw();
    }
}

/*===========================================================================*/
/**
 *  @brief  Mouse release event.
 *  @param  event [in] pointer to the mouse event
 */
/*===========================================================================*/
void Slider::mouseReleaseEvent( kvs::MouseEvent* event )
{
    kvs::IgnoreUnusedVariable( event );

    if ( !BaseClass::isShown() ) return;

    if ( BaseClass::isActive() )
    {
        m_pushed = false;
        this->sliderReleased();

        if ( m_change_value )
        {
            this->valueChanged();
            m_change_value = false;
        }

        BaseClass::deactivate();
        BaseClass::screen()->redraw();
    }
}

} // end of namespace kvs
