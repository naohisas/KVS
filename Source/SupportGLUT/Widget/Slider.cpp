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
 *  $Id: Slider.cpp 1319 2012-10-02 10:32:10Z naohisa.sakamoto@gmail.com $
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
const size_t        CursorWidth = 10;
const size_t        CursorHeight = 15;
const kvs::RGBColor SliderColor = kvs::RGBColor( 200, 200, 200 );
const kvs::RGBColor SliderEdgeColor = kvs::RGBColor( 230, 230, 230 );
} }

// Instance counter.
static int InstanceCounter = 0;


namespace kvs
{

namespace glut
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new Slider class.
 *  @param  screen [in] pointer to the parent screen
 */
/*===========================================================================*/
Slider::Slider( kvs::ScreenBase* screen ):
    kvs::glut::WidgetBase( screen ),
    m_change_value( false ),
    m_show_range_value( true )
{
    BaseClass::setEventType(
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

    m_clicked_slider_color = BaseClass::darkenedColor( color, 0.95f );
    m_upper_edge_color = ::Default::SliderEdgeColor;
    m_lower_edge_color = BaseClass::darkenedColor( color, 0.6f );
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
    kvs::OpenGL::SetLineWidth( 1 );
    kvs::OpenGL::Begin( GL_LINES );
    {
        // Upper line (black).
        kvs::OpenGL::Color( kvs::RGBColor::Black() );
        kvs::OpenGL::Vertex( x,         y - 1 );
        kvs::OpenGL::Vertex( x + width, y - 1 );

        // Middle line (gray).
        kvs::OpenGL::Color( kvs::RGBColor( 100, 100, 100 ) );
        kvs::OpenGL::Vertex( x,         y );
        kvs::OpenGL::Vertex( x + width, y );

        // Lower line (light gray).
        kvs::OpenGL::Color( kvs::RGBColor( 230, 230, 230 ) );
        kvs::OpenGL::Vertex( x,         y + 1 );
        kvs::OpenGL::Vertex( x + width, y + 1 );
    }
    kvs::OpenGL::End();
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
    const int p = static_cast<int>( width * ( m_value - m_min_value ) / ( m_max_value - m_min_value ) + 0.5 );
    const int x0 = static_cast<int>( x + p - ::Default::CursorWidth * 0.5 + 0.5 );
    const int x1 = static_cast<int>( x + p + ::Default::CursorWidth * 0.5 + 0.5 );
    const int y0 = static_cast<int>( y - ::Default::CursorHeight * 0.5 + 0.5 );
    const int y1 = static_cast<int>( y + ::Default::CursorHeight * 0.5 + 0.5 );

    // Body.
    {
        kvs::OpenGL::Begin( GL_POLYGON );
        {
            kvs::OpenGL::Color( m_slider_color );
            kvs::OpenGL::Vertex( x0, y0 );
            kvs::OpenGL::Vertex( x1, y0 );
            kvs::OpenGL::Vertex( x1, y1 );
            kvs::OpenGL::Vertex( x0, y1 );
        }
        kvs::OpenGL::End();
    }

    // Lower edge.
    {
        kvs::OpenGL::Begin( GL_LINE_STRIP );
        {
            kvs::OpenGL::Color( m_lower_edge_color );
            kvs::OpenGL::Vertex( x0, y1 );
            kvs::OpenGL::Vertex( x1, y1 );
            kvs::OpenGL::Vertex( x1, y0 );
        }
        kvs::OpenGL::End();
    }

    // Upper edge.
    {
        kvs::OpenGL::Begin( GL_LINE_STRIP );
        {
            kvs::OpenGL::Color( m_upper_edge_color );
            kvs::OpenGL::Vertex( x1, y0 );
            kvs::OpenGL::Vertex( x0, y0 );
            kvs::OpenGL::Vertex( x0, y1 );
        }
        kvs::OpenGL::End();
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
    const int bar_x = BaseClass::x0() + BaseClass::margin();
    const int bar_y = BaseClass::y0() + BaseClass::margin() + BaseClass::characterHeight() + ::Default::SliderHeight / 2;
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
    const int bar_x = BaseClass::x0() + BaseClass::margin();
    const int bar_y = BaseClass::y0() + BaseClass::margin() + BaseClass::characterHeight() + ::Default::SliderHeight / 2;
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
    const size_t width = m_caption.size() * BaseClass::characterWidth() + BaseClass::margin() * 2;
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
    return ::Default::SliderHeight + ( BaseClass::characterHeight() + BaseClass::margin() ) * 2;
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

    BaseClass::render2D().setViewport( kvs::OpenGL::Viewport() );
    BaseClass::render2D().begin();
    BaseClass::drawBackground();

    const int character_width  = BaseClass::characterWidth();
    const int character_height = BaseClass::characterHeight();

    // Draw the caption.
    {
        const int x = BaseClass::x0() + m_margin;
        const int y = BaseClass::y0() + m_margin;
        BaseClass::drawText( x, y + character_height, m_caption );
    }

    // Draw the slider bar and cursor.
    {
        const int x = BaseClass::x0() + m_margin;
        const int y = BaseClass::y0() + m_margin + character_height + ::Default::SliderHeight / 2;
        const int width = BaseClass::width() - m_margin * 2;
        this->draw_slider_bar( x, y, width );
        this->draw_cursor( x, y, width );
    }

    // Draw the values.
    if ( m_show_range_value )
    {
        {
            std::string min_value = kvs::String::ToString( m_min_value );
            const int x = BaseClass::x0() + m_margin;
            const int y = BaseClass::y0() + m_margin + character_height + ::Default::SliderHeight;
            BaseClass::drawText( x, y + character_height, min_value );
        }

        {
            std::string max_value = kvs::String::ToString( m_max_value );
            const int x = BaseClass::x1() - m_margin - max_value.size() * character_width;
            const int y = BaseClass::y0() + m_margin + character_height + ::Default::SliderHeight;
            BaseClass::drawText( x, y + character_height, max_value );
        }
    }

    BaseClass::render2D().end();
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
        BaseClass::screen()->disable();
        BaseClass::activate();
        this->sliderPressed();
        BaseClass::swapColor( m_slider_color, m_clicked_slider_color );
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
        this->sliderReleased();
        BaseClass::swapColor( m_slider_color, m_clicked_slider_color );

        if ( m_change_value )
        {
            this->valueChanged();
            m_change_value = false;
        }

        BaseClass::deactivate();
        BaseClass::screen()->redraw();
    }
}

} // end of namespace glut

} // end of namespace kvs
