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
 *  @brief  Returns the caption.
 *  @return caption string
 */
/*===========================================================================*/
const std::string& PushButton::caption( void ) const
{
    return( m_caption );
}

/*===========================================================================*/
/**
 *  @brief  Sets a caption string.
 *  @param  caption [in] caption string
 */
/*===========================================================================*/
void PushButton::setCaption( const std::string caption )
{
    m_caption = caption;
}

/*===========================================================================*/
/**
 *  @brief  Sets a text margin.
 *  @param  margin [in] margin width
 */
/*===========================================================================*/
void PushButton::setTextMargin( const int margin )
{
    m_text_margin = margin;
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
    m_clicked_button_color = BaseClass::get_darkened_color( color, 0.95f );
    m_upper_edge_color = ::Default::ButtonEdgeColor;
    m_lower_edge_color = BaseClass::get_darkened_color( color, 0.6f );
}

/*===========================================================================*/
/**
 *  @brief  Returns the fitted width.
 *  @return fitted width
 */
/*===========================================================================*/
int PushButton::get_fitted_width( void )
{
    return( m_caption.size() * BaseClass::characterWidth() + ( m_text_margin + m_margin ) * 2 );
}

/*===========================================================================*/
/**
 *  @brief  Returns the fitted height.
 *  @return fitted height
 */
/*===========================================================================*/
int PushButton::get_fitted_height( void )
{
    return( BaseClass::characterHeight() + ( m_text_margin + m_margin ) * 2 );
}

/*===========================================================================*/
/**
 *  @brief  Returns the centering position x.
 *  @return x coordinate
 */
/*===========================================================================*/
int PushButton::get_aligned_x( void )
{
    // Centering the caption along the x axis.
    const GLfloat x0 = static_cast<GLfloat>( BaseClass::x0() + BaseClass::margin() );
    const GLfloat x1 = static_cast<GLfloat>( BaseClass::x1() - BaseClass::margin() );
    return( static_cast<int>( x0 + ( x1 - x0 - m_caption.size() * BaseClass::characterWidth() ) * 0.5f + 0.5f ) );
}

/*===========================================================================*/
/**
 *  @brief  Returns the centering position y.
 *  @return y coordinate
 */
/*===========================================================================*/
int PushButton::get_aligned_y( void )
{
    // Centering the caption along the y axis.
    const GLfloat y0 = static_cast<GLfloat>( BaseClass::y0() + BaseClass::margin() );
    const GLfloat y1 = static_cast<GLfloat>( BaseClass::y1() - BaseClass::margin() );
    return( static_cast<int>( y0 + ( y1 - y0 - BaseClass::characterHeight() ) * 0.5f + 0.5f ) );
}

/*===========================================================================*/
/**
 *  @brief  Draws the button.
 */
/*===========================================================================*/
void PushButton::draw_button( void )
{
    const GLfloat x0 = static_cast<GLfloat>( BaseClass::x0() + BaseClass::margin() );
    const GLfloat x1 = static_cast<GLfloat>( BaseClass::x1() - BaseClass::margin() );
    const GLfloat y0 = static_cast<GLfloat>( BaseClass::y0() + BaseClass::margin() );
    const GLfloat y1 = static_cast<GLfloat>( BaseClass::y1() - BaseClass::margin() );

    glLineWidth( 1 );

    // Button.
    {
        const GLubyte r = static_cast<GLubyte>( m_button_color.r() );
        const GLubyte g = static_cast<GLubyte>( m_button_color.g() );
        const GLubyte b = static_cast<GLubyte>( m_button_color.b() );
        glColor3ub( r, g, b );
        glBegin( GL_POLYGON );
        glVertex2f( x0, y1 );
        glVertex2f( x0, y0 );
        glVertex2f( x1, y0 );
        glVertex2f( x1, y1 );
        glEnd();
    }

    // Lower edge.
    {
        const GLubyte r = static_cast<GLubyte>( m_lower_edge_color.r() );
        const GLubyte g = static_cast<GLubyte>( m_lower_edge_color.g() );
        const GLubyte b = static_cast<GLubyte>( m_lower_edge_color.b() );
        glColor3ub( r, g, b );
        glBegin( GL_LINE_STRIP );
        glVertex2f( x0, y1 );
        glVertex2f( x1, y1 );
        glVertex2f( x1, y0 );
        glEnd();
    }

    // Upper edge.
    {
        const GLubyte r = static_cast<GLubyte>( m_upper_edge_color.r() );
        const GLubyte g = static_cast<GLubyte>( m_upper_edge_color.g() );
        const GLubyte b = static_cast<GLubyte>( m_upper_edge_color.b() );
        glColor3ub( r, g, b );
        glBegin( GL_LINE_STRIP );
        glVertex2f( x1, y0 );
        glVertex2f( x0, y0 );
        glVertex2f( x0, y1 );
        glEnd();
    }
}

/*===========================================================================*/
/**
 *  @brief  Paint event.
 */
/*===========================================================================*/
void PushButton::paintEvent( void )
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
        BaseClass::swap_color( m_button_color, m_clicked_button_color );
        BaseClass::swap_color( m_upper_edge_color, m_lower_edge_color );
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
        BaseClass::swap_color( m_button_color, m_clicked_button_color );
        BaseClass::swap_color( m_upper_edge_color, m_lower_edge_color );
        BaseClass::deactivate();
        BaseClass::screen()->redraw();
    }
}

} // end of namespace glut

} // end of namespace kvs
