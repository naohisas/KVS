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
 *  $Id: PushButton.cpp 1319 2012-10-02 10:32:10Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#include "PushButton.h"
#include <kvs/OpenGL>
#include <kvs/RGBColor>
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
const kvs::RGBColor ButtonEdgeColor = kvs::RGBColor( 230, 230, 230 );
} }

// Instance counter.
static int InstanceCounter = 0;


namespace kvs
{

namespace glut
{

/*===========================================================================*/
/**
 *  @brief  Constructors a new PushButton class.
 *  @param  screen [in] pointer to the parent screen
 */
/*===========================================================================*/
PushButton::PushButton( kvs::ScreenBase* screen ):
    kvs::glut::WidgetBase( screen )
{
    BaseClass::setEventType(
        kvs::EventBase::PaintEvent |
        kvs::EventBase::ResizeEvent |
        kvs::EventBase::MousePressEvent |
        kvs::EventBase::MouseReleaseEvent );

    BaseClass::setMargin( ::Default::ButtonMargin );
    this->setCaption( "PushButton " + kvs::String::ToString( ::InstanceCounter++ ) );
    this->setTextMargin( ::Default::TextMargin );
    this->setButtonColor( ::Default::ButtonColor );
}

/*===========================================================================*/
/**
 *  @brief  Sets a button color.
 *  @param  color [in] button color
 */
/*===========================================================================*/
void PushButton::setButtonColor( const kvs::RGBColor& color )
{
    m_button_color = color;
    m_clicked_button_color = BaseClass::darkenedColor( color, 0.95f );
    m_upper_edge_color = ::Default::ButtonEdgeColor;
    m_lower_edge_color = BaseClass::darkenedColor( color, 0.6f );
}

/*===========================================================================*/
/**
 *  @brief  Returns the fitted width.
 *  @return fitted width
 */
/*===========================================================================*/
int PushButton::adjustedWidth()
{
    return m_caption.size() * BaseClass::characterWidth() + ( m_text_margin + m_margin ) * 2;
}

/*===========================================================================*/
/**
 *  @brief  Returns the fitted height.
 *  @return fitted height
 */
/*===========================================================================*/
int PushButton::adjustedHeight()
{
    return BaseClass::characterHeight() + ( m_text_margin + m_margin ) * 2;
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
    return static_cast<int>( x0 + ( x1 - x0 - m_caption.size() * BaseClass::characterWidth() ) * 0.5f + 0.5f );
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
    return static_cast<int>( y0 + ( y1 - y0 - BaseClass::characterHeight() ) * 0.5f + 0.5f );
}

/*===========================================================================*/
/**
 *  @brief  Draws the button.
 */
/*===========================================================================*/
void PushButton::draw_button()
{
    const GLfloat x0 = static_cast<GLfloat>( BaseClass::x0() + BaseClass::margin() );
    const GLfloat x1 = static_cast<GLfloat>( BaseClass::x1() - BaseClass::margin() );
    const GLfloat y0 = static_cast<GLfloat>( BaseClass::y0() + BaseClass::margin() );
    const GLfloat y1 = static_cast<GLfloat>( BaseClass::y1() - BaseClass::margin() );

    kvs::OpenGL::SetLineWidth( 1 );

    // Button.
    {
        kvs::OpenGL::Begin( GL_POLYGON );
        kvs::OpenGL::Color( m_button_color );
        kvs::OpenGL::Vertex( x0, y1 );
        kvs::OpenGL::Vertex( x0, y0 );
        kvs::OpenGL::Vertex( x1, y0 );
        kvs::OpenGL::Vertex( x1, y1 );
        kvs::OpenGL::End();
    }

    // Lower edge.
    {
        kvs::OpenGL::Begin( GL_LINE_STRIP );
        kvs::OpenGL::Color( m_lower_edge_color );
        kvs::OpenGL::Vertex( x0, y1 );
        kvs::OpenGL::Vertex( x1, y1 );
        kvs::OpenGL::Vertex( x1, y0 );
        kvs::OpenGL::End();
    }

    // Upper edge.
    {
        kvs::OpenGL::Begin( GL_LINE_STRIP );
        kvs::OpenGL::Color( m_upper_edge_color );
        kvs::OpenGL::Vertex( x1, y0 );
        kvs::OpenGL::Vertex( x0, y0 );
        kvs::OpenGL::Vertex( x0, y1 );
        kvs::OpenGL::End();
    }
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

    BaseClass::render2D().setViewport( kvs::OpenGL::Viewport() );
    BaseClass::render2D().begin();
    BaseClass::drawBackground();

    this->draw_button();

    const int x = this->get_aligned_x();
    const int y = this->get_aligned_y();
    BaseClass::drawText( x, y + BaseClass::characterHeight(), m_caption );

    BaseClass::render2D().end();
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
        BaseClass::screen()->disable();
        BaseClass::activate();
        this->pressed();
        BaseClass::swapColor( m_button_color, m_clicked_button_color );
        BaseClass::swapColor( m_upper_edge_color, m_lower_edge_color );
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
        this->released();
        BaseClass::swapColor( m_button_color, m_clicked_button_color );
        BaseClass::swapColor( m_upper_edge_color, m_lower_edge_color );
        BaseClass::deactivate();
        BaseClass::screen()->redraw();
    }
}

} // end of namespace glut

} // end of namespace kvs
