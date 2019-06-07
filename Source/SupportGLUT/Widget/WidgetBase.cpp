/*****************************************************************************/
/**
 *  @file   WidgetBase.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: WidgetBase.cpp 1719 2014-03-12 06:42:32Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#include "WidgetBase.h"
#include <kvs/OpenGL>
#include <kvs/glut/GLUT>
#include <kvs/HSVColor>
#include <kvs/RGBColor>
#include <kvs/EventHandler>
#include <kvs/ScreenBase>

// Default parameters.
namespace { namespace Default
{
void*                CharacterFont = GLUT_BITMAP_8_BY_13;
const int            CharacterWidth = 8;
const int            CharacterHeight = 12;
const kvs::RGBColor  TextColor = kvs::RGBColor( 0, 0, 0 );
const kvs::RGBAColor BackgroundColor = kvs::RGBAColor( 200, 200, 200, 0.0f );
const kvs::RGBAColor BackgroundBorderColor = kvs::RGBAColor( 0, 0, 0, 1.0f );
const float          BackgroundBorderWidth = 0.0f;
} }


namespace kvs
{

namespace glut
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new WidgetBase class.
 *  @param  screen [in] pointer to the screen
 */
/*===========================================================================*/
WidgetBase::WidgetBase( kvs::ScreenBase* screen ):
    m_screen( screen ),
    m_margin( 0 ),
    m_is_shown( false ),
    m_is_active( false )
{
    if ( screen ) screen->eventHandler()->attach( this );

    // Set default parameters.
    this->setTextColor( ::Default::TextColor );
    this->setBackgroundColor( ::Default::BackgroundColor );
    this->setBackgroundBorderColor( ::Default::BackgroundBorderColor );
    this->setBackgroundBorderWidth( ::Default::BackgroundBorderWidth );
    m_character_width = ::Default::CharacterWidth;
    m_character_height = ::Default::CharacterHeight;
}

/*===========================================================================*/
/**
 *  @brief  Destructs the WidgetBase class.
 */
/*===========================================================================*/
WidgetBase::~WidgetBase()
{
}

/*===========================================================================*/
/**
 *  @brief  Sets a background opacity value.
 *  @param  opacity [in] opacity value
 */
/*===========================================================================*/
void WidgetBase::setBackgroundOpacity( const float opacity )
{
    const kvs::UInt8 r = m_background_color.r();
    const kvs::UInt8 g = m_background_color.g();
    const kvs::UInt8 b = m_background_color.b();
    m_background_color = kvs::RGBAColor( r, g, b, opacity );
}

/*===========================================================================*/
/**
 *  @brief  Sets a background border opacity value.
 *  @param  opacity [in] background opacity value
 */
/*===========================================================================*/
void WidgetBase::setBackgroundBorderOpacity( const float opacity )
{
    const kvs::UInt8 r = m_background_border_color.r();
    const kvs::UInt8 g = m_background_border_color.g();
    const kvs::UInt8 b = m_background_border_color.b();
    m_background_border_color = kvs::RGBAColor( r, g, b, opacity );
}

/*===========================================================================*/
/**
 *  @brief  Shows the screen.
 */
/*===========================================================================*/
void WidgetBase::show()
{
    if ( Rectangle::width() == 0 ) Rectangle::setWidth( this->adjustedWidth() );
    if ( Rectangle::height() == 0 ) Rectangle::setHeight( this->adjustedHeight() );
    m_is_shown = true;
}

/*===========================================================================*/
/**
 *  @brief  Hides the screen.
 */
/*===========================================================================*/
void WidgetBase::hide()
{
    m_is_shown = false;
}

/*==========================================================================*/
/**
 *  @brief  Draws the background.
 */
/*==========================================================================*/
void WidgetBase::drawBackground()
{
    kvs::OpenGL::WithPushedAttrib attrib( GL_ALL_ATTRIB_BITS );
    attrib.disable( GL_TEXTURE_1D );
    attrib.disable( GL_TEXTURE_2D );
    attrib.enable( GL_BLEND );

    kvs::OpenGL::SetBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

    if ( m_background_color.a() > 0.0f )
    {
        // Draw background.
        kvs::OpenGL::Begin( GL_POLYGON );
        {
            kvs::OpenGL::Color( m_background_color );
            kvs::OpenGL::Vertex( kvs::Vec2( this->x0(), this->y1() ) );
            kvs::OpenGL::Vertex( kvs::Vec2( this->x0(), this->y0() ) );
            kvs::OpenGL::Vertex( kvs::Vec2( this->x1(), this->y0() ) );
            kvs::OpenGL::Vertex( kvs::Vec2( this->x1(), this->y1() ) );
        }
        kvs::OpenGL::End();
    }


    if ( m_background_border_width > 0.0f && m_background_border_color.a() > 0.0f )
    {
        // Draw outline of the background.
        kvs::OpenGL::SetLineWidth( m_background_border_width );
        kvs::OpenGL::Begin( GL_POLYGON );
        {
            kvs::OpenGL::Color( m_background_border_color );
            kvs::OpenGL::Vertex( kvs::Vec2( this->x0(), this->y1() ) );
            kvs::OpenGL::Vertex( kvs::Vec2( this->x0(), this->y0() ) );
            kvs::OpenGL::Vertex( kvs::Vec2( this->x1(), this->y0() ) );
            kvs::OpenGL::Vertex( kvs::Vec2( this->x1(), this->y1() ) );
        }
        kvs::OpenGL::End();
    }
}

/*===========================================================================*/
/**
 *  @brief  Draws the text.
 *  @param  x [in] text position x
 *  @param  y [in] text position y
 *  @param  text [in] text
 */
/*===========================================================================*/
void WidgetBase::drawText( const int x, const int y, const std::string& text )
{
    kvs::OpenGL::WithPushedAttrib attrib( GL_ALL_ATTRIB_BITS );
    attrib.disable( GL_TEXTURE_1D );
    attrib.disable( GL_TEXTURE_2D );
    attrib.disable( GL_BLEND );

    kvs::OpenGL::Color( m_text_color );
    kvs::OpenGL::SetRasterPos( x, y );
    char* line_head = const_cast<char*>( text.c_str() );
    for ( char* p = line_head; *p; p++ )
    {
        glutBitmapCharacter( ::Default::CharacterFont, *p );
    }
}

void WidgetBase::drawRect(
    const kvs::Rectangle& rect,
    const kvs::RGBColor& upper_color,
    const kvs::RGBColor& lower_color,
    const float width )
{
    kvs::OpenGL::SetLineWidth( width );
    kvs::OpenGL::Begin( GL_LINES );
    {
        // Top and left side lines
        kvs::OpenGL::Color( upper_color );
        kvs::OpenGL::Vertices( rect.topLeft(), rect.topRight() );
        kvs::OpenGL::Vertices( rect.topLeft(), rect.bottomLeft() );
        // Bottom and right side lines
        kvs::OpenGL::Color( lower_color );
        kvs::OpenGL::Vertices( rect.bottomLeft(), rect.bottomRight() );
        kvs::OpenGL::Vertices( rect.topRight(), rect.bottomRight() );
    }
    kvs::OpenGL::End();
}

/*===========================================================================*/
/**
 *  @brief  Swaps the colors.
 *  @param  color1 [in] color 1
 *  @param  color2 [in] color 2
 */
/*===========================================================================*/
void WidgetBase::swapColor( kvs::RGBColor& color1, kvs::RGBColor& color2 )
{
    const kvs::RGBColor temp = color1;
    color1 = color2;
    color2 = temp;
}

/*===========================================================================*/
/**
 *  @brief  Retruns the darkened color.
 *  @param  color [in] source color
 *  @param  darkness [in] coefficient value [0,1]
 *  @return darkened color
 */
/*===========================================================================*/
kvs::RGBColor WidgetBase::darkenedColor( const kvs::RGBColor& color, const float darkness )
{
    kvs::HSVColor hsv( color );
    hsv.set( hsv.h(), hsv.s(), hsv.v() * darkness );
    return kvs::RGBColor( hsv );
}

} // end of namespace glut

} // end of namespace kvs
