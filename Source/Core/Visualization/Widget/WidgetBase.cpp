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
 *  $Id$
 */
/*****************************************************************************/
#include "WidgetBase.h"
#include <kvs/OpenGL>
#include <kvs/HSVColor>
#include <kvs/RGBColor>
#include <kvs/EventHandler>
#include <kvs/ScreenBase>

// Default parameters.
namespace { namespace Default
{
const kvs::RGBAColor BackgroundColor = kvs::RGBAColor( 200, 200, 200, 0.0f );
const kvs::RGBAColor BackgroundBorderColor = kvs::RGBAColor( 0, 0, 0, 1.0f );
const float BackgroundBorderWidth = 0.0f;
} }


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new WidgetBase class.
 *  @param  screen [in] pointer to the screen
 */
/*===========================================================================*/
WidgetBase::WidgetBase( kvs::ScreenBase* screen ):
    m_screen( screen ),
    m_painter(),
    m_margin( 0 ),
    m_is_shown( false ),
    m_is_active( false )
{
    addEventType( kvs::EventBase::InitializeEvent );

    if ( screen ) screen->eventHandler()->attach( this );

    // Set default parameters.
    this->setBackgroundColor( ::Default::BackgroundColor );
    this->setBackgroundBorderColor( ::Default::BackgroundBorderColor );
    this->setBackgroundBorderWidth( ::Default::BackgroundBorderWidth );
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
    attrib.disable( GL_TEXTURE_3D );
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

void WidgetBase::initializeEvent()
{
    if ( Rectangle::width() == 0 ) Rectangle::setWidth( this->adjustedWidth() );
    if ( Rectangle::height() == 0 ) Rectangle::setHeight( this->adjustedHeight() );
}

} // end of namespace kvs
