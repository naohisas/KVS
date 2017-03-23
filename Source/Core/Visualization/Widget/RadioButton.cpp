/*****************************************************************************/
/**
 *  @file   RadioButton.cpp
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
#include "RadioButton.h"
#include <kvs/OpenGL>
#include <kvs/RGBColor>
#include <kvs/String>
#include <kvs/IgnoreUnusedVariable>
#include <kvs/ScreenBase>
#include <kvs/EventBase>
#include <kvs/MouseEvent>
#include "RadioButtonGroup.h"


// Default parameters.
namespace { namespace Default
{
const int TextMargin = 10;
const int CircleMargin = 0;
const int CircleWidth = 15;
const int CircleHeight = 15;
const kvs::RGBColor CircleColor = kvs::RGBColor( 200, 200, 200 );
} }

// Instance counter.
static int InstanceCounter = 0;


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new RadioButton class.
 *  @param  screen [in] pointer to the parent screen
 */
/*===========================================================================*/
RadioButton::RadioButton( kvs::ScreenBase* screen ):
    kvs::WidgetBase( screen ),
    m_group( NULL )
{
    BaseClass::setEventType(
        kvs::EventBase::PaintEvent |
        kvs::EventBase::ResizeEvent |
        kvs::EventBase::MousePressEvent |
        kvs::EventBase::MouseReleaseEvent );

    BaseClass::setMargin( ::Default::CircleMargin );
    this->setCaption( "RadioButton " + kvs::String::ToString( ::InstanceCounter++ ) );
    this->setState( false );
}

/*===========================================================================*/
/**
 *  @brief  Draws the box.
 */
/*===========================================================================*/
void RadioButton::draw_box()
{
    BaseClass::renderEngine().beginFrame( screen()->width(), screen()->height() );

    const int dy = BaseClass::textEngine().height() - ::Default::CircleHeight;
    const GLfloat x0 = static_cast<GLfloat>( BaseClass::x0() + BaseClass::margin() );
    const GLfloat y0 = static_cast<GLfloat>( BaseClass::y0() + BaseClass::margin() + dy );

    const float cx = x0 + ::Default::CircleWidth * 0.5f;
    const float cy = y0 + ::Default::CircleHeight * 0.5f;
    const kvs::Vec2 center( cx, cy );
    const float radius = ::Default::CircleWidth * 0.5f;

    BaseClass::renderEngine().beginPath();

    const kvs::RGBColor color = m_state ? kvs::RGBColor( 60, 150, 250 ) : ::Default::CircleColor;
    BaseClass::renderEngine().circle( center, radius );
    BaseClass::renderEngine().setFillColor( color );
    BaseClass::renderEngine().fill();

    const float stroke_width = 1.0f;
    const kvs::RGBAColor stroke_color( 0, 0, 0, 0.8f );
    BaseClass::renderEngine().setStrokeWidth( stroke_width );
    BaseClass::renderEngine().setStrokeColor( stroke_color );
    BaseClass::renderEngine().stroke();

    const float w = ::Default::CircleWidth;
    const float h = ::Default::CircleHeight;
    NVGpaint bg = BaseClass::renderEngine().boxGradient( x0 + 1.5f, y0 + 1.5f, w, h, 8, 3, nvgRGBA( 0, 0, 0, 32 ), nvgRGBA( 0, 0, 0, 128 ) );

    BaseClass::renderEngine().setFillPaint( bg );
    BaseClass::renderEngine().fill();

    BaseClass::renderEngine().endFrame();
}

/*===========================================================================*/
/**
 *  @brief  Draw the check mark.
 */
/*===========================================================================*/
void RadioButton::draw_mark()
{
    const int dy = BaseClass::textEngine().height() - ::Default::CircleHeight;
    const GLfloat x0 = static_cast<GLfloat>( BaseClass::x0() + BaseClass::margin() );
    const GLfloat y0 = static_cast<GLfloat>( BaseClass::y0() + BaseClass::margin() + dy );

    const float cx = x0 + ::Default::CircleWidth * 0.5f;
    const float cy = y0 + ::Default::CircleHeight * 0.5f;

    const kvs::Vec2 center( cx, cy );
    const float radius = ::Default::CircleWidth * 0.5f / 2.5f;

    BaseClass::renderEngine().beginPath();

    BaseClass::renderEngine().circle( center, radius );
    BaseClass::renderEngine().setFillColor( kvs::RGBColor::White() );
    BaseClass::renderEngine().fill();

    BaseClass::renderEngine().endFrame();
}

/*===========================================================================*/
/**
 *  @brief  Returns the fitted width.
 *  @return fitted width
 */
/*===========================================================================*/
int RadioButton::adjustedWidth()
{
    return BaseClass::textEngine().width( m_caption ) + ::Default::TextMargin + BaseClass::margin() * 2;
}

/*===========================================================================*/
/**
 *  @brief  Returns the fitted height.
 *  @return fitted height
 */
/*===========================================================================*/
int RadioButton::adjustedHeight()
{
    return BaseClass::textEngine().height() + BaseClass::margin() * 2;
}

/*===========================================================================*/
/**
 *  @brief  Check whether the given point is inside the check box or not.
 *  @param  x [in] x position in the window coordinate
 *  @param  y [in] y position in the window coordinate
 *  @return true if the given point is inside the check box
 */
/*===========================================================================*/
bool RadioButton::contains( int x, int y )
{
    const int dy = BaseClass::textEngine().height() - ::Default::CircleHeight;
    const GLfloat x0 = static_cast<GLfloat>( BaseClass::x0() + BaseClass::margin() );
    const GLfloat x1 = static_cast<GLfloat>( x0 + ::Default::CircleWidth );
    const GLfloat y0 = static_cast<GLfloat>( BaseClass::y0() + BaseClass::margin() + dy );
    const GLfloat y1 = static_cast<GLfloat>( y0 + ::Default::CircleHeight );

    return ( x0 <= x ) && ( x <= x1 ) && ( y0 <= y ) && ( y <= y1 );
}

/*===========================================================================*/
/**
 *  @brief  Paint event.
 */
/*===========================================================================*/
void RadioButton::paintEvent()
{
    this->screenUpdated();

    if ( !BaseClass::isShown() ) return;

    BaseClass::render2D().setViewport( kvs::OpenGL::Viewport() );
    BaseClass::render2D().begin();
    BaseClass::drawBackground();

    this->draw_box();
    if ( this->state() ) this->draw_mark();

    const int x = BaseClass::x0() + BaseClass::margin() + ::Default::CircleWidth + ::Default::TextMargin;
    const int y = BaseClass::y0() + BaseClass::margin();
    BaseClass::textEngine().draw( kvs::Vec2( x, y + BaseClass::textEngine().height() ), m_caption, BaseClass::screen() );

    BaseClass::render2D().end();
}

/*===========================================================================*/
/**
 *  @brief  Resize event.
 *  @param  width [in] resized screen width
 *  @param  height [in] resized screen height
 */
/*===========================================================================*/
void RadioButton::resizeEvent( int width, int height )
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
void RadioButton::mousePressEvent( kvs::MouseEvent* event )
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
void RadioButton::mouseReleaseEvent( kvs::MouseEvent* event )
{
    if ( !BaseClass::isShown() ) return;

    if ( BaseClass::isActive() )
    {
        if ( this->contains( event->x(), event->y() ) )
        {
            if ( !m_state )
            {
                m_state = true;
                this->stateChanged();
            }

            if ( m_group )
            {
                std::list<kvs::RadioButton*>::const_iterator button = m_group->radioButtons().begin();
                std::list<kvs::RadioButton*>::const_iterator last = m_group->radioButtons().end();
                while ( button != last )
                {
                    if ( *button != this )
                    {
                        (*button)->setState( false );
                        (*button)->stateChanged();
                    }
                    button++;
                }
            }
        }

        this->released();
        BaseClass::deactivate();
        BaseClass::screen()->redraw();
    }
}

} // end of namespace kvs
