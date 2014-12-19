/*****************************************************************************/
/**
 *  @file   ColorPalette.cpp
 *  @author Naohisa Sakamoto
 *  @brief  
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: ColorPalette.cpp 1524 2013-04-11 01:46:06Z naohisa.sakamoto $
 */
/*****************************************************************************/
#include "ColorPalette.h"
#include <kvs/HSVColor>
#include <kvs/String>
#include <kvs/EventBase>
#include <kvs/MouseEvent>
#include <kvs/IgnoreUnusedVariable>
#include <kvs/ScreenBase>
#include <kvs/OpenGL>
#include <cstdio>

// Default parameters.
namespace { namespace Default
{
const size_t Margin = 10;
const size_t Width = 350;
const size_t Height = 150;
const int HPaletteWidth = 10;
const int ColorBoxSize = 50;
const kvs::RGBColor RectColor = kvs::RGBColor( 255, 255, 255 );
const kvs::RGBColor RectEdgeColor = kvs::RGBColor( 230, 230, 230 );
} }

// Instance counter.
static int InstanceCounter = 0;


namespace
{

/*===========================================================================*/
/**
 *  @brief  Draws a rectangle as lines.
 *  @param  rect [in] rectangle
 *  @param  width [in] line width
 *  @param  upper_edge_color [in] upper edge color
 *  @param  lower_edge_color [in] lower edge color
 */
/*===========================================================================*/
void DrawRectangle(
    const kvs::glut::Rectangle rect,
    const float width,
    const kvs::RGBColor& upper_edge_color,
    const kvs::RGBColor& lower_edge_color )
{
    GLfloat x0 = static_cast<GLfloat>( rect.x0() );
    GLfloat y0 = static_cast<GLfloat>( rect.y0() );
    GLfloat x1 = static_cast<GLfloat>( rect.x1() );
    GLfloat y1 = static_cast<GLfloat>( rect.y1() );

    glLineWidth( width );
    glBegin( GL_LINES );
    {
        glColor3ub( upper_edge_color.r(), upper_edge_color.g(), upper_edge_color.b() );
        glVertex2f( x0, y0 ); glVertex2f( x1, y0 ); // top
        glVertex2f( x0, y0 ); glVertex2f( x0, y1 ); // left

        glColor3ub( lower_edge_color.r(), lower_edge_color.g(), lower_edge_color.b() );
        glVertex2f( x1, y1 ); glVertex2f( x0, y1 ); // bottom
        glVertex2f( x1, y0 ); glVertex2f( x1, y1 ); // right
    }
    glEnd();
}

/*===========================================================================*/
/**
 *  @brief  Interpolates color values.
 *  @param  ratio [in] ratio [0-1]
 *  @param  c0 [in] color value 0
 *  @param  c1 [in] color value 1
 *  @return interpolated color value
 */
/*===========================================================================*/
const kvs::RGBColor GetInterpolatedColor( const float ratio, const kvs::RGBColor& c0, const kvs::RGBColor& c1 )
{
    const kvs::UInt8 r = static_cast<kvs::UInt8>( ratio * c0.r() + ( 1.0f - ratio ) * c1.r() + 0.5f );
    const kvs::UInt8 g = static_cast<kvs::UInt8>( ratio * c0.g() + ( 1.0f - ratio ) * c1.g() + 0.5f );
    const kvs::UInt8 b = static_cast<kvs::UInt8>( ratio * c0.b() + ( 1.0f - ratio ) * c1.b() + 0.5f );

    return( kvs::RGBColor( r, g, b ) );
}

} // end of namespace


namespace kvs
{

namespace glut
{

ColorPalette::ColorPalette( kvs::ScreenBase* screen ):
    kvs::glut::WidgetBase( screen ),
    m_H_indicator( -1 ),
    m_S_indicator( -1 ),
    m_V_indicator( -1 )
{
    BaseClass::setEventType(
        kvs::EventBase::PaintEvent |
        kvs::EventBase::ResizeEvent |
        kvs::EventBase::MousePressEvent |
        kvs::EventBase::MouseMoveEvent |
        kvs::EventBase::MouseReleaseEvent );

    BaseClass::setMargin( ::Default::Margin );
    this->setCaption( "Color palette " + kvs::String::ToString( ::InstanceCounter++ ) );

    m_upper_edge_color = BaseClass::get_darkened_color( ::Default::RectColor, 0.6f );
    m_lower_edge_color = ::Default::RectEdgeColor;
}

ColorPalette::~ColorPalette( void )
{
}

const std::string& ColorPalette::caption( void ) const
{
    return( m_caption );
}

const kvs::RGBColor ColorPalette::color( void ) const
{
    const float h = this->get_H_value();
    const kvs::RGBColor c0( kvs::HSVColor( h, 1, 1 ) ); // top-right
    const kvs::RGBColor c1( kvs::HSVColor( h, 0, 1 ) ); // top-left
    const kvs::RGBColor c2( kvs::HSVColor( h, 0, 0 ) ); // bottom-left
    const kvs::RGBColor c3( kvs::HSVColor( h, 1, 0 ) ); // bottom-right

    const float v = this->get_V_value();
    const kvs::RGBColor v0( ::GetInterpolatedColor( v, c1, c2 ) );
    const kvs::RGBColor v1( ::GetInterpolatedColor( v, c0, c3 ) );

    const float s = this->get_S_value();
    return( ::GetInterpolatedColor( s, v1, v0 ) );
}

void ColorPalette::setCaption( const std::string& caption )
{
    m_caption = caption;
}

void ColorPalette::paintEvent( void )
{
    this->screenUpdated();

    if ( !BaseClass::isShown() ) return;

    BaseClass::begin_draw();
    BaseClass::draw_background();

    // Draw the caption.
    {
        const int x = BaseClass::x0() + BaseClass::margin();
        const int y = BaseClass::y0() + BaseClass::margin();
        BaseClass::draw_text( x, y + BaseClass::characterHeight(), m_caption );
    }

    // Selected color box.
    {
        const int x = BaseClass::x1() - BaseClass::margin() - ::Default::ColorBoxSize;
        const int y = BaseClass::y0() + BaseClass::margin() + BaseClass::characterHeight() + 5;
        const int width = ::Default::ColorBoxSize;
        const int height = ::Default::ColorBoxSize;
        m_selected_color_box.setGeometry( x, y, width, height );
    }

    // H palette.
    {
        const int x = m_selected_color_box.x0() - ::Default::HPaletteWidth - 10;
        const int y = BaseClass::y0() + BaseClass::margin() + BaseClass::characterHeight() + 5;
        const int width = ::Default::HPaletteWidth;
        const int height = BaseClass::height() - BaseClass::margin() * 2 - BaseClass::characterHeight();
        m_H_palette.setGeometry( x, y, width, height );

        // Initialize the H indicator.
        if ( m_H_indicator < 0 ) m_H_indicator = m_H_palette.y0() + m_H_palette.height() / 2;
    }

    // SV palette
    {
        const int x = BaseClass::x0() + BaseClass::margin();;
        const int y = BaseClass::y0() + BaseClass::margin() + BaseClass::characterHeight() + 5;
        const int width = m_H_palette.x0() - BaseClass::x0() - BaseClass::margin() * 2;
        const int height = BaseClass::height() - BaseClass::margin() * 2 - BaseClass::characterHeight();
        m_SV_palette.setGeometry( x, y, width, height );

        // Initialize the S and V indicator.
        if ( m_S_indicator < 0 ) m_S_indicator = m_SV_palette.x0() + m_SV_palette.width() / 2;
        if ( m_V_indicator < 0 ) m_V_indicator = m_SV_palette.y0() + m_SV_palette.height() / 2;
    }

    this->draw_SV_palette();
    this->draw_H_palette();
    this->draw_selected_color_box();
    this->draw_selected_color_value();

    BaseClass::end_draw();
}

void ColorPalette::resizeEvent( int width, int height )
{
    kvs::IgnoreUnusedVariable( width );
    kvs::IgnoreUnusedVariable( height );

    this->screenResized();
}

void ColorPalette::mousePressEvent( kvs::MouseEvent* event )
{
    if ( !BaseClass::isShown() ) return;

    if ( BaseClass::contains( event->x(), event->y() ) )
    {
        BaseClass::screen()->disable();
        BaseClass::activate();

        if ( m_SV_palette.contains( event->x(), event->y(), true ) )
        {
            m_SV_palette.activate();
            m_S_indicator = kvs::Math::Clamp( event->x(), m_SV_palette.x0(), m_SV_palette.x1() );
            m_V_indicator = kvs::Math::Clamp( event->y(), m_SV_palette.y0(), m_SV_palette.y1() );
        }

        if ( m_H_palette.contains( event->x(), event->y(), true ) )
        {
            m_H_palette.activate();
            m_H_indicator = kvs::Math::Clamp( event->y(), m_H_palette.y0(), m_H_palette.y1() );
        }

        BaseClass::screen()->redraw();
    }
}

void ColorPalette::mouseMoveEvent( kvs::MouseEvent* event )
{
    if ( !BaseClass::isShown() ) return;

    if ( BaseClass::isActive() )
    {
        if ( m_SV_palette.isActive() )
        {
            m_S_indicator = kvs::Math::Clamp( event->x(), m_SV_palette.x0(), m_SV_palette.x1() );
            m_V_indicator = kvs::Math::Clamp( event->y(), m_SV_palette.y0(), m_SV_palette.y1() );
        }

        if ( m_H_palette.isActive() )
        {
            m_H_indicator = kvs::Math::Clamp( event->y(), m_H_palette.y0(), m_H_palette.y1() );
        }

        BaseClass::screen()->redraw();
    }
}

void ColorPalette::mouseReleaseEvent( kvs::MouseEvent* event )
{
    kvs::IgnoreUnusedVariable( event );

    if ( !BaseClass::isShown() ) return;

    if ( BaseClass::isActive() )
    {
        if ( m_SV_palette.isActive() ) m_SV_palette.deactivate();
        if ( m_H_palette.isActive() ) m_H_palette.deactivate();

        BaseClass::deactivate();
        BaseClass::screen()->redraw();
    }
}

void ColorPalette::draw_SV_palette( void )
{
    const int x0 = m_SV_palette.x0();
    const int y0 = m_SV_palette.y0();
    const int x1 = m_SV_palette.x1();
    const int y1 = m_SV_palette.y1();

    const float h = this->get_H_value();
    const kvs::RGBColor c0( kvs::HSVColor( h, 1, 1 ) ); // top-right
    const kvs::RGBColor c1( kvs::HSVColor( h, 0, 1 ) ); // top-left
    const kvs::RGBColor c2( kvs::HSVColor( h, 0, 0 ) ); // bottom-left
    const kvs::RGBColor c3( kvs::HSVColor( h, 1, 0 ) ); // bottom-right

    // Draw SV palette.
    glBegin( GL_QUADS );
    glColor3ub( c0.r(), c0.g(), c0.b() ); glVertex2i( x1, y0 );
    glColor3ub( c1.r(), c1.g(), c1.b() ); glVertex2i( x0, y0 );
    glColor3ub( c2.r(), c2.g(), c2.b() ); glVertex2i( x0, y1 );
    glColor3ub( c3.r(), c3.g(), c3.b() ); glVertex2i( x1, y1 );
    glEnd();

    // Draw the cross lines.
    {
        const int margin = 4;
        const float color = 1.0f;
        const int cursor_x0 = kvs::Math::Max( m_S_indicator - margin, m_SV_palette.x0() );
        const int cursor_y0 = kvs::Math::Max( m_V_indicator - margin, m_SV_palette.y0() );
        const int cursor_x1 = kvs::Math::Min( m_S_indicator + margin, m_SV_palette.x1() );
        const int cursor_y1 = kvs::Math::Min( m_V_indicator + margin, m_SV_palette.y1() );

        glLineWidth( 1 );
        glBegin( GL_LINES );
        glColor3f( color, color, color );
        glVertex2i( cursor_x0, cursor_y0 ); glVertex2i( cursor_x1, cursor_y0 );     // top
        glVertex2i( cursor_x0, cursor_y1 ); glVertex2i( cursor_x1, cursor_y1 );     // bottom
        glVertex2i( cursor_x0, cursor_y0 ); glVertex2i( cursor_x0, cursor_y1 + 1 ); // left
        glVertex2i( cursor_x1, cursor_y0 ); glVertex2i( cursor_x1, cursor_y1 + 1 ); // right
        glEnd();

        glLineWidth( 1 );
        glBegin( GL_LINES );
        glColor3f( color, color, color );
        // Horizontal lines.
        glVertex2i( x0, m_V_indicator ); glVertex2i( cursor_x0, m_V_indicator );
        glVertex2i( cursor_x1, m_V_indicator ); glVertex2i( x1, m_V_indicator );
        // Vertical lines.
        glVertex2i( m_S_indicator, y0 ); glVertex2i( m_S_indicator, cursor_y0 );
        glVertex2i( m_S_indicator, cursor_y1 ); glVertex2i( m_S_indicator, y1 );
        glEnd();
    }

    // Draw border.
    ::DrawRectangle( m_SV_palette, 1, m_upper_edge_color, m_lower_edge_color );
}

void ColorPalette::draw_H_palette( void )
{
    const int x0 = m_H_palette.x0();
    const int y0 = m_H_palette.y0();
    const int x1 = m_H_palette.x1();
    const int y1 = m_H_palette.y1();

    const int stride = ( y1 - y0 ) / 6;
    const int dx = m_H_palette.width() / 2;

    // Draw H palette.
    glLineWidth( static_cast<GLfloat>( m_H_palette.width() ) );
    glBegin( GL_LINE_STRIP );
    glColor3ub( 255,   0,   0 ); glVertex2i( x0 + dx, y1 );
    glColor3ub( 255, 255,   0 ); glVertex2i( x0 + dx, y0 + stride * 5 );
    glColor3ub(   0, 255,   0 ); glVertex2i( x0 + dx, y0 + stride * 4 );
    glColor3ub(   0, 255, 255 ); glVertex2i( x0 + dx, y0 + stride * 3 );
    glColor3ub(   0,   0, 255 ); glVertex2i( x0 + dx, y0 + stride * 2 );
    glColor3ub( 255,   0, 255 ); glVertex2i( x0 + dx, y0 + stride * 1 );
    glColor3ub( 255,   0,   0 ); glVertex2i( x0 + dx, y0 );
    glEnd();

    // Draw the line
    const float color = 1.0f;
    glLineWidth( 1 );
    glBegin( GL_LINES );
    glColor3f( color, color, color );
    glVertex2i( x0, m_H_indicator );
    glVertex2i( x1, m_H_indicator );
    glEnd();

    // Draw border.
    ::DrawRectangle( m_H_palette, 1, m_upper_edge_color, m_lower_edge_color );
}

void ColorPalette::draw_selected_color_box( void )
{
    const int x0 = m_selected_color_box.x0();
    const int y0 = m_selected_color_box.y0();
    const int x1 = m_selected_color_box.x1();
    const int y1 = m_selected_color_box.y1();

    const kvs::RGBColor current_color = this->color();

    glBegin( GL_QUADS );
    glColor3ub( current_color.r(), current_color.g(), current_color.b() );
    glVertex2i( x1, y1 );
    glVertex2i( x0, y1 );
    glVertex2i( x0, y0 );
    glVertex2i( x1, y0 );
    glEnd();

    // Draw border.
    ::DrawRectangle( m_selected_color_box, 1, m_upper_edge_color, m_lower_edge_color );
}

void ColorPalette::draw_selected_color_value( void )
{
    const kvs::RGBColor current_color = this->color();
    const int r = static_cast<int>( current_color.r() );
    const int g = static_cast<int>( current_color.g() );
    const int b = static_cast<int>( current_color.b() );
    const float h = this->get_H_value();
    const float s = this->get_S_value();
    const float v = this->get_V_value();

    char R[10]; sprintf( R, "R: %3d", r );
    char G[10]; sprintf( G, "G: %3d", g );
    char B[10]; sprintf( B, "B: %3d", b );

    char H[10]; sprintf( H, "H: %3d", static_cast<int>( h * 255.0f + 0.5f ) );
    char S[10]; sprintf( S, "S: %3d", static_cast<int>( s * 255.0f + 0.5f ) );
    char V[10]; sprintf( V, "V: %3d", static_cast<int>( v * 255.0f + 0.5f ) );

    int x = m_selected_color_box.x0();
    int y = m_selected_color_box.y1() + 10;
    BaseClass::draw_text( x, y += BaseClass::characterHeight(), R );
    BaseClass::draw_text( x, y += BaseClass::characterHeight(), G );
    BaseClass::draw_text( x, y += BaseClass::characterHeight(), B );
    BaseClass::draw_text( x, y += BaseClass::characterHeight(), "" );
    BaseClass::draw_text( x, y += BaseClass::characterHeight(), H );
    BaseClass::draw_text( x, y += BaseClass::characterHeight(), S );
    BaseClass::draw_text( x, y += BaseClass::characterHeight(), V );
}

int ColorPalette::get_fitted_width( void )
{
    const size_t width = m_caption.size() * BaseClass::characterWidth() + BaseClass::margin() * 2;
    return( kvs::Math::Max( width, ::Default::Width ) );
}

int ColorPalette::get_fitted_height( void )
{
    return( ::Default::Height + BaseClass::characterHeight() + BaseClass::margin() * 2 );
}

float ColorPalette::get_H_value( void ) const
{
    return( static_cast<float>( m_H_palette.y1() - m_H_indicator ) / m_H_palette.height() );
}

float ColorPalette::get_S_value( void ) const
{
    return( static_cast<float>( m_S_indicator - m_SV_palette.x0() ) / m_SV_palette.width() );
}

float ColorPalette::get_V_value( void ) const
{
    return( static_cast<float>( m_SV_palette.y1() - m_V_indicator ) / m_SV_palette.height() );
}

} // end of namespace glut

} // end of namespace kvs
