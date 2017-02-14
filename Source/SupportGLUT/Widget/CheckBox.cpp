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
 *  $Id: CheckBox.cpp 1319 2012-10-02 10:32:10Z naohisa.sakamoto@gmail.com $
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


// Default parameters.
namespace { namespace Default
{
const int           TextMargin = 10;
const int           BoxMargin = 0;
const int           BoxWidth = 10;
const int           BoxHeight = 10;
const kvs::RGBColor BoxColor = kvs::RGBColor( 255, 255, 255 );
const kvs::RGBColor BoxEdgeColor = kvs::RGBColor( 230, 230, 230 );
} }

// Instance counter.
static int InstanceCounter = 0;

namespace kvs
{

namespace glut
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new CheckBox class.
 *  @param  screen [in] pointer to the parent screen
 */
/*===========================================================================*/
CheckBox::CheckBox( kvs::ScreenBase* screen ):
    kvs::glut::WidgetBase( screen ),
    m_group( NULL )
{
    BaseClass::setEventType(
        kvs::EventBase::PaintEvent |
        kvs::EventBase::ResizeEvent |
        kvs::EventBase::MousePressEvent |
        kvs::EventBase::MouseReleaseEvent );

    BaseClass::setMargin( ::Default::BoxMargin );
    this->setCaption( "CheckBox " + kvs::String::ToString( ::InstanceCounter++ ) );
    this->setState( false );

    m_upper_edge_color = BaseClass::get_darkened_color( ::Default::BoxColor, 0.6f );
    m_lower_edge_color = ::Default::BoxEdgeColor;
}

/*===========================================================================*/
/**
 *  @brief  Returns the caption.
 *  @return caption string
 */
/*===========================================================================*/
const std::string& CheckBox::caption( void ) const
{
    return( m_caption );
}

/*===========================================================================*/
/**
 *  @brief  Returns the check status.
 *  @return true if the box is checked
 */
/*===========================================================================*/
bool CheckBox::state( void ) const
{
    return( m_state );
}

/*===========================================================================*/
/**
 *  @brief  Sets a caption string.
 *  @param  caption [in] caption string
 */
/*===========================================================================*/
void CheckBox::setCaption( const std::string caption )
{
    m_caption = caption;
}

/*===========================================================================*/
/**
 *  @brief  Sets a check status.
 *  @param  state [in] check status
 */
/*===========================================================================*/
void CheckBox::setState( const bool state )
{
    m_state = state;
}

/*===========================================================================*/
/**
 *  @brief  Draws the box.
 */
/*===========================================================================*/
void CheckBox::draw_box( void )
{
    const int dy = BaseClass::characterHeight() - ::Default::BoxHeight;
    const GLfloat x0 = static_cast<GLfloat>( BaseClass::x0() + BaseClass::margin() );
    const GLfloat x1 = static_cast<GLfloat>( x0 + ::Default::BoxWidth );
    const GLfloat y0 = static_cast<GLfloat>( BaseClass::y0() + BaseClass::margin() + dy );
    const GLfloat y1 = static_cast<GLfloat>( y0 + ::Default::BoxHeight );

    glLineWidth( 1 );

    {
        const GLubyte r = static_cast<GLubyte>( ::Default::BoxColor.r() );
        const GLubyte g = static_cast<GLubyte>( ::Default::BoxColor.g() );
        const GLubyte b = static_cast<GLubyte>( ::Default::BoxColor.b() );
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
 *  @brief  Draw the check mark.
 */
/*===========================================================================*/
void CheckBox::draw_mark( void )
{
    const int dy = BaseClass::characterHeight() - ::Default::BoxHeight;
    const GLfloat x0 = static_cast<GLfloat>( BaseClass::x0() + BaseClass::margin() );
    const GLfloat y0 = static_cast<GLfloat>( BaseClass::y0() + BaseClass::margin() + dy );

    glEnable( GL_LINE_SMOOTH );
    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    glHint( GL_LINE_SMOOTH_HINT, GL_NICEST );

    glLineWidth( 1.5 );

    glColor3ub( 0, 0, 0 );
    glBegin( GL_LINE_STRIP );
    glVertex2f( x0 + 2, y0 + 4 );
    glVertex2f( x0 + 4, y0 + 8 );
    glVertex2f( x0 + 7, y0 + 3 );
    glEnd();
}

/*===========================================================================*/
/**
 *  @brief  Returns the fitted width.
 *  @return fitted width
 */
/*===========================================================================*/
int CheckBox::get_fitted_width( void )
{
    return( m_caption.size() * BaseClass::characterWidth() + ::Default::TextMargin + BaseClass::margin() * 2 );
}

/*===========================================================================*/
/**
 *  @brief  Returns the fitted height.
 *  @return fitted height
 */
/*===========================================================================*/
int CheckBox::get_fitted_height( void )
{
    return( BaseClass::characterHeight() + BaseClass::margin() * 2 );
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
    const int dy = BaseClass::characterHeight() - ::Default::BoxHeight;
    const GLfloat x0 = static_cast<GLfloat>( BaseClass::x0() + BaseClass::margin() );
    const GLfloat x1 = static_cast<GLfloat>( x0 + ::Default::BoxWidth );
    const GLfloat y0 = static_cast<GLfloat>( BaseClass::y0() + BaseClass::margin() + dy );
    const GLfloat y1 = static_cast<GLfloat>( y0 + ::Default::BoxHeight );

    return( ( x0 <= x ) && ( x <= x1 ) && ( y0 <= y ) && ( y <= y1 ) );
}

/*===========================================================================*/
/**
 *  @brief  Attaches the check box group.
 *  @param  group [in] pointer to the check box group
 */
/*===========================================================================*/
void CheckBox::attach_group( kvs::glut::CheckBoxGroup* group )
{
    m_group = group;
}

/*===========================================================================*/
/**
 *  @brief  Detaches the check box group.
 */
/*===========================================================================*/
void CheckBox::detach_group( void )
{
    m_group = NULL;
}

/*===========================================================================*/
/**
 *  @brief  Paint event.
 */
/*===========================================================================*/
void CheckBox::paintEvent( void )
{
    this->screenUpdated();

    if ( !BaseClass::isShown() ) return;

    BaseClass::render2D().setViewport( kvs::OpenGL::Viewport() );
    BaseClass::render2D().begin();
    BaseClass::draw_background();

    this->draw_box();
    if ( this->state() ) this->draw_mark();

    const int x = BaseClass::x0() + BaseClass::margin() + ::Default::BoxWidth + ::Default::TextMargin;
    const int y = BaseClass::y0() + BaseClass::margin();
    BaseClass::draw_text( x, y + BaseClass::characterHeight(), m_caption );

    BaseClass::render2D().end();
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

} // end of namespace glut

} // end of namespace kvs
