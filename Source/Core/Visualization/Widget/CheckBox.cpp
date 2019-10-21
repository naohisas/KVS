/*****************************************************************************/
/**
 *  @file   CheckBox.cpp
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
#include "CheckBox.h"
#include <kvs/OpenGL>
#include <kvs/String>
#include <kvs/RGBColor>
#include <kvs/IgnoreUnusedVariable>
#include <kvs/ScreenBase>
#include <kvs/EventBase>
#include <kvs/MouseEvent>


// Constant variables
namespace
{
const int TextMargin = 10; // indenting space for the text
const int BoxWidth = 15;
const int BoxHeight = 15;
const kvs::RGBColor BoxColor = kvs::RGBColor( 200, 200, 200 );
}

// Instance counter.
static int InstanceCounter = 0;


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new CheckBox class.
 *  @param  screen [in] pointer to the parent screen
 */
/*===========================================================================*/
CheckBox::CheckBox( kvs::ScreenBase* screen ):
    kvs::WidgetBase( screen ),
    m_group( NULL )
{
    BaseClass::addEventType(
        kvs::EventBase::PaintEvent |
        kvs::EventBase::ResizeEvent |
        kvs::EventBase::MousePressEvent |
        kvs::EventBase::MouseReleaseEvent );

    this->setCaption( "CheckBox " + kvs::String::ToString( ::InstanceCounter++ ) );
    this->setState( false );
}

/*===========================================================================*/
/**
 *  @brief  Draws the box.
 */
/*===========================================================================*/
void CheckBox::draw_box()
{
    kvs::NanoVG* engine = BaseClass::painter().device()->renderEngine();
    engine->beginFrame( screen()->width(), screen()->height(), screen()->devicePixelRatio() );

    const int dy = BaseClass::painter().fontMetrics().height() - ::BoxHeight;
    const GLfloat x0 = static_cast<GLfloat>( BaseClass::x0() + BaseClass::margin() );
    const GLfloat x1 = static_cast<GLfloat>( x0 + ::BoxWidth );
    const GLfloat y0 = static_cast<GLfloat>( BaseClass::y0() + BaseClass::margin() + dy );
    const GLfloat y1 = static_cast<GLfloat>( y0 + ::BoxHeight );
    const float w = x1 - x0;
    const float h = y1 - y0;

    const kvs::RGBColor color = m_state ? kvs::RGBColor( 60, 150, 250 ) : ::BoxColor;
    engine->beginPath();
    engine->roundedRect( x0, y0, w, h, 3 );
    engine->setFillColor( color );

    engine->setStrokeWidth( 1.0f );
    engine->setStrokeColor( kvs::RGBColor::Black() );

    engine->stroke();
    engine->fill();

    const float x = x0 + 1.5f;
    const float y = y0 + 1.5f;
    const NVGcolor c0 = nvgRGBA( 0, 0, 0, 32 );
    const NVGcolor c1 = nvgRGBA( 0, 0, 0, 128 );
    NVGpaint bg = engine->boxGradient( x, y, w, h, 3, 3, c0, c1 );
    engine->setFillPaint( bg );
    engine->fill();

    engine->endFrame();
}

/*===========================================================================*/
/**
 *  @brief  Draw the check mark.
 */
/*===========================================================================*/
void CheckBox::draw_mark()
{
    const int dy = BaseClass::painter().fontMetrics().height() - ::BoxHeight;
    const GLfloat x0 = static_cast<GLfloat>( BaseClass::x0() + BaseClass::margin() );
    const GLfloat y0 = static_cast<GLfloat>( BaseClass::y0() + BaseClass::margin() + dy );
    const kvs::RGBColor color = BaseClass::painter().font().color();
    BaseClass::painter().font().setColor( kvs::RGBColor::White() );
    BaseClass::painter().drawIcon( kvs::Vec2( x0 + 2, y0 + 18 ), kvs::Font::Check, 28 );
    BaseClass::painter().font().setColor( color );
}

/*===========================================================================*/
/**
 *  @brief  Returns the fitted width.
 *  @return fitted width
 */
/*===========================================================================*/
int CheckBox::adjustedWidth()
{
    BaseClass::painter().begin( BaseClass::screen() );
    const kvs::FontMetrics metrics = BaseClass::painter().fontMetrics();
    const size_t text_width = metrics.width( m_caption );
    BaseClass::painter().end();

    return ::BoxWidth + text_width + ::TextMargin + BaseClass::margin() * 2;
}

/*===========================================================================*/
/**
 *  @brief  Returns the fitted height.
 *  @return fitted height
 */
/*===========================================================================*/
int CheckBox::adjustedHeight()
{
    BaseClass::painter().begin( BaseClass::screen() );
    const kvs::FontMetrics metrics = BaseClass::painter().fontMetrics();
    const int line_height = metrics.height();
    BaseClass::painter().end();

    return line_height + BaseClass::margin() * 2;
}

/*===========================================================================*/
/**
 *  @brief  Check whether the given point is inside the check box or not.
 *  @param  x [in] x position in the window coordinate
 *  @param  y [in] y position in the window coordinate
 *  @return true if the given point is inside the check box
 */
/*===========================================================================*/
bool CheckBox::contains( int x, int y )
{
    BaseClass::painter().begin( BaseClass::screen() );
    const kvs::FontMetrics metrics = BaseClass::painter().fontMetrics();
    const int dy = metrics.height() - ::BoxHeight;
    BaseClass::painter().end();

    const GLfloat x0 = static_cast<GLfloat>( BaseClass::x0() + BaseClass::margin() );
    const GLfloat x1 = static_cast<GLfloat>( x0 + ::BoxWidth );
    const GLfloat y0 = static_cast<GLfloat>( BaseClass::y0() + BaseClass::margin() + dy );
    const GLfloat y1 = static_cast<GLfloat>( y0 + ::BoxHeight );

    return ( x0 <= x ) && ( x <= x1 ) && ( y0 <= y ) && ( y <= y1 );
}

/*===========================================================================*/
/**
 *  @brief  Paint event.
 */
/*===========================================================================*/
void CheckBox::paintEvent()
{
    this->screenUpdated();

    if ( !BaseClass::isShown() ) return;

    BaseClass::painter().begin( BaseClass::screen() );
    BaseClass::drawBackground();

    this->draw_box();
    if ( this->state() ) { this->draw_mark(); }

    const int x = BaseClass::x0() + BaseClass::margin() + ::BoxWidth + ::TextMargin;
    const int y = BaseClass::y0() + BaseClass::margin();
    const int height = BaseClass::painter().fontMetrics().height();
    const kvs::Vec2 p( x, y + height );
    BaseClass::painter().drawText( p, m_caption );

    BaseClass::painter().end();
}

/*===========================================================================*/
/**
 *  @brief  Resize event.
 *  @param  width [in] resized screen width
 *  @param  height [in] resized screen height
 */
/*===========================================================================*/
void CheckBox::resizeEvent( int width, int height )
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
void CheckBox::mousePressEvent( kvs::MouseEvent* event )
{
    if ( !BaseClass::isShown() ) return;

    if ( this->contains( event->x(), event->y() ) )
    {
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
void CheckBox::mouseReleaseEvent( kvs::MouseEvent* event )
{
    if ( !BaseClass::isShown() ) return;

    if ( BaseClass::isActive() )
    {
        if ( this->contains( event->x(), event->y() ) )
        {
            m_state = !m_state;
            this->stateChanged();
        }

        this->released();
        BaseClass::deactivate();
        BaseClass::screen()->redraw();
    }
}

} // end of namespace kvs
