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

    m_upper_edge_color = BaseClass::darkenedColor( ::Default::CircleColor, 0.6f );
    m_lower_edge_color = ::Default::CircleEdgeColor;
}

/*===========================================================================*/
/**
 *  @brief  Draws the box.
 */
/*===========================================================================*/
void RadioButton::draw_box()
{
    const int dy = BaseClass::characterHeight() - ::Default::CircleHeight;
    const GLfloat x0 = static_cast<GLfloat>( BaseClass::x0() + BaseClass::margin() );
    const GLfloat y0 = static_cast<GLfloat>( BaseClass::y0() + BaseClass::margin() + dy );

    const float cx = x0 + ::Default::CircleWidth * 0.5f;
    const float cy = y0 + ::Default::CircleHeight * 0.5f;

    kvs::OpenGL::SetLineWidth( 1 );

    // Circle.
    {
        kvs::OpenGL::Begin( GL_POLYGON );
        kvs::OpenGL::Color( ::Default::CircleColor );
        kvs::OpenGL::Vertex( ::Default::CircleVertices[ 0] + cx, ::Default::CircleVertices[ 1] + cy );
        kvs::OpenGL::Vertex( ::Default::CircleVertices[ 2] + cx, ::Default::CircleVertices[ 3] + cy );
        kvs::OpenGL::Vertex( ::Default::CircleVertices[ 4] + cx, ::Default::CircleVertices[ 5] + cy );
        kvs::OpenGL::Vertex( ::Default::CircleVertices[ 6] + cx, ::Default::CircleVertices[ 7] + cy );
        kvs::OpenGL::Vertex( ::Default::CircleVertices[ 8] + cx, ::Default::CircleVertices[ 9] + cy );
        kvs::OpenGL::Vertex( ::Default::CircleVertices[10] + cx, ::Default::CircleVertices[11] + cy );
        kvs::OpenGL::Vertex( ::Default::CircleVertices[12] + cx, ::Default::CircleVertices[13] + cy );
        kvs::OpenGL::End();
    }

    // Lower edge.
    {
        kvs::OpenGL::Begin( GL_LINE_STRIP );
        kvs::OpenGL::Color( m_lower_edge_color );
        kvs::OpenGL::Vertex( ::Default::CircleVertices[12] + cx, ::Default::CircleVertices[13] + cy );
        kvs::OpenGL::Vertex( ::Default::CircleVertices[ 0] + cx, ::Default::CircleVertices[ 1] + cy );
        kvs::OpenGL::Vertex( ::Default::CircleVertices[ 2] + cx, ::Default::CircleVertices[ 3] + cy );
        kvs::OpenGL::Vertex( ::Default::CircleVertices[ 4] + cx, ::Default::CircleVertices[ 5] + cy );
        kvs::OpenGL::Vertex( ::Default::CircleVertices[ 6] + cx, ::Default::CircleVertices[ 7] + cy );
        kvs::OpenGL::End();
    }

    // Upper edge.
    {
        kvs::OpenGL::Begin( GL_LINE_STRIP );
        kvs::OpenGL::Color( m_upper_edge_color );
        kvs::OpenGL::Vertex( ::Default::CircleVertices[ 6] + cx, ::Default::CircleVertices[ 7] + cy );
        kvs::OpenGL::Vertex( ::Default::CircleVertices[ 8] + cx, ::Default::CircleVertices[ 9] + cy );
        kvs::OpenGL::Vertex( ::Default::CircleVertices[10] + cx, ::Default::CircleVertices[11] + cy );
        kvs::OpenGL::Vertex( ::Default::CircleVertices[12] + cx, ::Default::CircleVertices[13] + cy );
        kvs::OpenGL::End();
    }
}

/*===========================================================================*/
/**
 *  @brief  Draw the check mark.
 */
/*===========================================================================*/
void RadioButton::draw_mark()
{
    const int dy = BaseClass::characterHeight() - ::Default::CircleHeight;
    const GLfloat x0 = static_cast<GLfloat>( BaseClass::x0() + BaseClass::margin() );
    const GLfloat y0 = static_cast<GLfloat>( BaseClass::y0() + BaseClass::margin() + dy );

    const float cx = x0 + ::Default::CircleWidth * 0.5f;
    const float cy = y0 + ::Default::CircleHeight * 0.5f;

    kvs::OpenGL::Enable( GL_LINE_SMOOTH );
    kvs::OpenGL::Enable( GL_BLEND );
    kvs::OpenGL::SetBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    kvs::OpenGL::Hint( GL_LINE_SMOOTH_HINT, GL_NICEST );

    kvs::OpenGL::SetLineWidth( 1 );
    kvs::OpenGL::Color( kvs::RGBColor::Black() );
    kvs::OpenGL::Begin( GL_LINE_STRIP );
    kvs::OpenGL::Vertex( ::Default::MarkVertices[ 0] + cx, ::Default::MarkVertices[ 1] + cy );
    kvs::OpenGL::Vertex( ::Default::MarkVertices[ 2] + cx, ::Default::MarkVertices[ 3] + cy );
    kvs::OpenGL::Vertex( ::Default::MarkVertices[ 4] + cx, ::Default::MarkVertices[ 5] + cy );
    kvs::OpenGL::Vertex( ::Default::MarkVertices[ 6] + cx, ::Default::MarkVertices[ 7] + cy );
    kvs::OpenGL::Vertex( ::Default::MarkVertices[ 8] + cx, ::Default::MarkVertices[ 9] + cy );
    kvs::OpenGL::Vertex( ::Default::MarkVertices[10] + cx, ::Default::MarkVertices[11] + cy );
    kvs::OpenGL::Vertex( ::Default::MarkVertices[12] + cx, ::Default::MarkVertices[13] + cy );
    kvs::OpenGL::Vertex( ::Default::MarkVertices[ 0] + cx, ::Default::MarkVertices[ 1] + cy );
    kvs::OpenGL::End();

    kvs::OpenGL::SetPointSize( 2 );
    kvs::OpenGL::Color( kvs::RGBColor::Black() );
    kvs::OpenGL::Begin( GL_POINTS );
    kvs::OpenGL::Vertex( cx, cy );
    kvs::OpenGL::End();
}

/*===========================================================================*/
/**
 *  @brief  Returns the fitted width.
 *  @return fitted width
 */
/*===========================================================================*/
int RadioButton::adjustedWidth()
{
    return m_caption.size() * BaseClass::characterWidth() + ::Default::TextMargin + BaseClass::margin() * 2;
}

/*===========================================================================*/
/**
 *  @brief  Returns the fitted height.
 *  @return fitted height
 */
/*===========================================================================*/
int RadioButton::adjustedHeight()
{
    return BaseClass::characterHeight() + BaseClass::margin() * 2;
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
