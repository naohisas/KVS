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
 *  $Id: RadioButton.cpp 1319 2012-10-02 10:32:10Z naohisa.sakamoto@gmail.com $
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
const int           TextMargin = 10;
const int           CircleMargin = 0;
const int           CircleWidth = 10;
const int           CircleHeight = 10;
const kvs::RGBColor CircleColor = kvs::RGBColor( 255, 255, 255 );
const kvs::RGBColor CircleEdgeColor = kvs::RGBColor( 230, 230, 230 );
const float         CircleVertices[14] = {
     6.00000f,  0.00000f,
     3.74094f,  4.69099f,
    -1.33513f,  5.84957f,
    -5.40581f,  2.60330f,
    -5.40581f, -2.60330f,
    -1.33513f, -5.84957f,
     3.74094f, -4.69099f
};
const float         MarkVertices[14] = {
     2.00000f,  0.00000f,
     1.24698f,  1.56366f,
    -0.44504f,  1.94986f,
    -1.80194f,  0.86777f,
    -1.80194f, -0.86777f,
    -0.44504f, -1.94986f,
     1.24698f, -1.56366f
};
} }

// Instance counter.
static int InstanceCounter = 0;

namespace kvs
{

namespace glut
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new RadioButton class.
 *  @param  screen [in] pointer to the parent screen
 */
/*===========================================================================*/
RadioButton::RadioButton( kvs::ScreenBase* screen ):
    kvs::glut::WidgetBase( screen ),
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

    m_upper_edge_color = BaseClass::get_darkened_color( ::Default::CircleColor, 0.6f );
    m_lower_edge_color = ::Default::CircleEdgeColor;
}

/*===========================================================================*/
/**
 *  @brief  Returns the caption.
 *  @return caption string
 */
/*===========================================================================*/
const std::string& RadioButton::caption( void ) const
{
    return( m_caption );
}

/*===========================================================================*/
/**
 *  @brief  Returns the check status.
 *  @return true if the box is checked
 */
/*===========================================================================*/
bool RadioButton::state( void ) const
{
    return( m_state );
}

/*===========================================================================*/
/**
 *  @brief  Sets a caption string.
 *  @param  caption [in] caption string
 */
/*===========================================================================*/
void RadioButton::setCaption( const std::string caption )
{
    m_caption = caption;
}

/*===========================================================================*/
/**
 *  @brief  Sets a check status.
 *  @param  state [in] check status
 */
/*===========================================================================*/
void RadioButton::setState( const bool state )
{
    m_state = state;
}

/*===========================================================================*/
/**
 *  @brief  Draws the box.
 */
/*===========================================================================*/
void RadioButton::draw_box( void )
{
    const int dy = BaseClass::characterHeight() - ::Default::CircleHeight;
    const GLfloat x0 = static_cast<GLfloat>( BaseClass::x0() + BaseClass::margin() );
    const GLfloat y0 = static_cast<GLfloat>( BaseClass::y0() + BaseClass::margin() + dy );

    const float cx = x0 + ::Default::CircleWidth * 0.5f;
    const float cy = y0 + ::Default::CircleHeight * 0.5f;

    glLineWidth( 1 );

    // Circle.
    {
        const GLubyte r = static_cast<GLubyte>( ::Default::CircleColor.r() );
        const GLubyte g = static_cast<GLubyte>( ::Default::CircleColor.g() );
        const GLubyte b = static_cast<GLubyte>( ::Default::CircleColor.b() );
        glColor3ub( r, g, b );
        glBegin( GL_POLYGON );
        glVertex2f( ::Default::CircleVertices[ 0] + cx, ::Default::CircleVertices[ 1] + cy );
        glVertex2f( ::Default::CircleVertices[ 2] + cx, ::Default::CircleVertices[ 3] + cy );
        glVertex2f( ::Default::CircleVertices[ 4] + cx, ::Default::CircleVertices[ 5] + cy );
        glVertex2f( ::Default::CircleVertices[ 6] + cx, ::Default::CircleVertices[ 7] + cy );
        glVertex2f( ::Default::CircleVertices[ 8] + cx, ::Default::CircleVertices[ 9] + cy );
        glVertex2f( ::Default::CircleVertices[10] + cx, ::Default::CircleVertices[11] + cy );
        glVertex2f( ::Default::CircleVertices[12] + cx, ::Default::CircleVertices[13] + cy );
        glEnd();
    }

    // Lower edge.
    {
        const GLubyte r = static_cast<GLubyte>( m_lower_edge_color.r() );
        const GLubyte g = static_cast<GLubyte>( m_lower_edge_color.g() );
        const GLubyte b = static_cast<GLubyte>( m_lower_edge_color.b() );
        glColor3ub( r, g, b );
        glBegin( GL_LINE_STRIP );
        glVertex2f( ::Default::CircleVertices[12] + cx, ::Default::CircleVertices[13] + cy );
        glVertex2f( ::Default::CircleVertices[ 0] + cx, ::Default::CircleVertices[ 1] + cy );
        glVertex2f( ::Default::CircleVertices[ 2] + cx, ::Default::CircleVertices[ 3] + cy );
        glVertex2f( ::Default::CircleVertices[ 4] + cx, ::Default::CircleVertices[ 5] + cy );
        glVertex2f( ::Default::CircleVertices[ 6] + cx, ::Default::CircleVertices[ 7] + cy );
        glEnd();
    }

    // Upper edge.
    {
        const GLubyte r = static_cast<GLubyte>( m_upper_edge_color.r() );
        const GLubyte g = static_cast<GLubyte>( m_upper_edge_color.g() );
        const GLubyte b = static_cast<GLubyte>( m_upper_edge_color.b() );
        glColor3ub( r, g, b );
        glBegin( GL_LINE_STRIP );
        glVertex2f( ::Default::CircleVertices[ 6] + cx, ::Default::CircleVertices[ 7] + cy );
        glVertex2f( ::Default::CircleVertices[ 8] + cx, ::Default::CircleVertices[ 9] + cy );
        glVertex2f( ::Default::CircleVertices[10] + cx, ::Default::CircleVertices[11] + cy );
        glVertex2f( ::Default::CircleVertices[12] + cx, ::Default::CircleVertices[13] + cy );
        glEnd();
    }
}

/*===========================================================================*/
/**
 *  @brief  Draw the check mark.
 */
/*===========================================================================*/
void RadioButton::draw_mark( void )
{
    const int dy = BaseClass::characterHeight() - ::Default::CircleHeight;
    const GLfloat x0 = static_cast<GLfloat>( BaseClass::x0() + BaseClass::margin() );
    const GLfloat y0 = static_cast<GLfloat>( BaseClass::y0() + BaseClass::margin() + dy );

    const float cx = x0 + ::Default::CircleWidth * 0.5f;
    const float cy = y0 + ::Default::CircleHeight * 0.5f;

    glEnable( GL_LINE_SMOOTH );
    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    glHint( GL_LINE_SMOOTH_HINT, GL_NICEST );

    glLineWidth( 1 );
    glColor3ub( 0, 0, 0 );
    glBegin( GL_LINE_STRIP );
    glVertex2f( ::Default::MarkVertices[ 0] + cx, ::Default::MarkVertices[ 1] + cy );
    glVertex2f( ::Default::MarkVertices[ 2] + cx, ::Default::MarkVertices[ 3] + cy );
    glVertex2f( ::Default::MarkVertices[ 4] + cx, ::Default::MarkVertices[ 5] + cy );
    glVertex2f( ::Default::MarkVertices[ 6] + cx, ::Default::MarkVertices[ 7] + cy );
    glVertex2f( ::Default::MarkVertices[ 8] + cx, ::Default::MarkVertices[ 9] + cy );
    glVertex2f( ::Default::MarkVertices[10] + cx, ::Default::MarkVertices[11] + cy );
    glVertex2f( ::Default::MarkVertices[12] + cx, ::Default::MarkVertices[13] + cy );
    glVertex2f( ::Default::MarkVertices[ 0] + cx, ::Default::MarkVertices[ 1] + cy );
    glEnd();

    glPointSize( 2 );
    glColor3ub( 0, 0, 0 );
    glBegin( GL_POINTS );
    glVertex2f( cx, cy );
    glEnd();
}

/*===========================================================================*/
/**
 *  @brief  Returns the fitted width.
 *  @return fitted width
 */
/*===========================================================================*/
int RadioButton::get_fitted_width( void )
{
    return( m_caption.size() * BaseClass::characterWidth() + ::Default::TextMargin + BaseClass::margin() * 2 );
}

/*===========================================================================*/
/**
 *  @brief  Returns the fitted height.
 *  @return fitted height
 */
/*===========================================================================*/
int RadioButton::get_fitted_height( void )
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
bool RadioButton::contains( int x, int y )
{
    const int dy = BaseClass::characterHeight() - ::Default::CircleHeight;
    const GLfloat x0 = static_cast<GLfloat>( BaseClass::x0() + BaseClass::margin() );
    const GLfloat x1 = static_cast<GLfloat>( x0 + ::Default::CircleWidth );
    const GLfloat y0 = static_cast<GLfloat>( BaseClass::y0() + BaseClass::margin() + dy );
    const GLfloat y1 = static_cast<GLfloat>( y0 + ::Default::CircleHeight );

    return( ( x0 <= x ) && ( x <= x1 ) && ( y0 <= y ) && ( y <= y1 ) );
}

/*===========================================================================*/
/**
 *  @brief  Attaches the radio button group.
 *  @param  group [in] pointer to the radio button group
 */
/*===========================================================================*/
void RadioButton::attach_group( kvs::glut::RadioButtonGroup* group )
{
    m_group = group;
}

/*===========================================================================*/
/**
 *  @brief  Detaches the radio button group.
 */
/*===========================================================================*/
void RadioButton::detach_group( void )
{
    m_group = NULL;
}

/*===========================================================================*/
/**
 *  @brief  Paint event.
 */
/*===========================================================================*/
void RadioButton::paintEvent( void )
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
                std::list<kvs::glut::RadioButton*>::const_iterator button = m_group->radioButtons().begin();
                std::list<kvs::glut::RadioButton*>::const_iterator last = m_group->radioButtons().end();
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

} // end of namespace glut

} // end of namespace kvs
