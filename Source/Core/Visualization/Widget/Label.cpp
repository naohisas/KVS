/*****************************************************************************/
/**
 *  @file   Label.cpp
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
#include "Label.h"
#include <kvs/OpenGL>
#include <cstdio>
#include <cstdarg>
#include <kvs/IgnoreUnusedVariable>
#include <kvs/EventBase>


// Constant variables
namespace
{
const size_t MaxLineLength = 255;
}


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new Label class.
 *  @param  screen [in] pointer to the parent screen
 */
/*===========================================================================*/
Label::Label( kvs::ScreenBase* screen ):
    kvs::WidgetBase( screen )
{
    BaseClass::addEventType(
        kvs::EventBase::PaintEvent |
        kvs::EventBase::ResizeEvent );
}

/*==========================================================================*/
/**
 *  @brief Sets text string.
 *  @param text [in] text string
 */
/*==========================================================================*/
void Label::setText( const char* text, ... )
{
    char buffer[ ::MaxLineLength ];
    va_list args;
    va_start( args, text );
    vsprintf( buffer, text, args );
    va_end( args );
    this->setText( std::string( buffer ) );
}

/*==========================================================================*/
/**
 *  @brief Add the text string.
 *  @param text [in] text string
 */
/*==========================================================================*/
void Label::addText( const char* text, ... )
{
    char buffer[ ::MaxLineLength ];
    va_list args;
    va_start( args, text );
    vsprintf( buffer, text, args );
    va_end( args );
    this->addText( std::string( buffer ) );
}

/*===========================================================================*/
/**
 *  @brief  Returns the fitted width.
 *  @return fitted width
 */
/*===========================================================================*/
int Label::adjustedWidth()
{
    size_t max_width = 0;
    BaseClass::painter().begin( BaseClass::screen() );
    const kvs::FontMetrics metrics = BaseClass::painter().fontMetrics();
    for ( size_t i = 0; i < m_text.size(); i++ )
    {
        const size_t line_width = metrics.width( m_text[i] );
        max_width = kvs::Math::Max( max_width, line_width );
    }
    BaseClass::painter().end();

    return max_width + BaseClass::margin() * 2;
}

/*===========================================================================*/
/**
 *  @brief  Returns the fitted height.
 *  @return fitted height
 */
/*===========================================================================*/
int Label::adjustedHeight()
{
    BaseClass::painter().begin( BaseClass::screen() );
    const kvs::FontMetrics metrics = BaseClass::painter().fontMetrics();
    const size_t nlines = m_text.size();
    const size_t character_height = metrics.height();
    BaseClass::painter().end();
    return nlines * character_height + BaseClass::margin() * 2;
}

/*===========================================================================*/
/**
 *  @brief  Paint event.
 */
/*===========================================================================*/
void Label::paintEvent()
{
    this->screenUpdated();

    if ( !BaseClass::isShown() ) return;

    BaseClass::painter().begin( BaseClass::screen() );
    BaseClass::drawBackground();

    const int x = BaseClass::x() + BaseClass::margin();
    const int y = BaseClass::y() + BaseClass::margin();
    const size_t character_height = BaseClass::painter().fontMetrics().height();
    for ( size_t line = 0; line < m_text.size(); line++ )
    {
        const kvs::Vec2i p( x, y + character_height * ( line + 1 ) );
        BaseClass::painter().drawText( p, m_text[line] );
    }

    BaseClass::painter().end();
}

/*===========================================================================*/
/**
 *  @brief  Resize event.
 *  @param  width [in] resized screen width
 *  @param  height [in] resized screen height
 */
/*===========================================================================*/
void Label::resizeEvent( int width, int height )
{
    kvs::IgnoreUnusedVariable( width );
    kvs::IgnoreUnusedVariable( height );
    this->screenResized();
}

} // end of namespace kvs
