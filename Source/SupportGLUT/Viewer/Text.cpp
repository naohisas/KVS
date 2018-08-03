/****************************************************************************/
/**
 *  @file Text.cpp
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: Text.cpp 1138 2012-05-10 04:35:42Z s.yamada0808@gmail.com $
 */
/****************************************************************************/
#include "Text.h"
#include "../GLUT.h"
#include <cstdio>
#include <cstdarg>
#include <kvs/OpenGL>


namespace
{

const size_t CharacterHeight = 12;
const size_t MaxLineLength   = 255;

inline void DrawString( const std::string& str, const kvs::Vec2& pos )
{
    kvs::OpenGL::SetRasterPos( pos );
    char* y_char = const_cast<char*>( str.c_str() );
    for ( char* p = y_char; *p; p++ )
    {
        glutBitmapCharacter( GLUT_BITMAP_8_BY_13, *p );
    }
}

inline void DrawStrings( const std::vector<std::string>& strs, const kvs::Vec2& pos )
{
    for ( size_t line = 0; line < strs.size(); line++ )
    {
        const kvs::Vec2 offset( 0.0f, CharacterHeight * line );
        DrawString( strs[line],  pos - offset );
    }
}

} // end of namespace

namespace kvs
{

namespace glut
{

/*==========================================================================*/
/**
 *  Constructor.
 */
/*==========================================================================*/
Text::Text():
    m_x( 0 ),
    m_y( 0 ),
    m_color( kvs::RGBColor( 0, 0, 0 ) )
{
}

/*==========================================================================*/
/**
 *  Set the text position.
 *  @param x [in] x value of the text position
 *  @param y [in] y value of the text position
 */
/*==========================================================================*/
void Text::setPosition( const size_t x, const size_t y )
{
    m_x = x;
    m_y = y;
}

/*==========================================================================*/
/**
 *  Set the text color.
 *  @param color [in] text color
 */
/*==========================================================================*/
void Text::setColor( const kvs::RGBColor& color )
{
    m_color = color;
}

/*==========================================================================*/
/**
 *  Set the text string.
 *  @param text [in] text string
 */
/*==========================================================================*/
void Text::setText( const char* text, ... )
{
    m_text.clear();

    char buffer[ ::MaxLineLength ];

    va_list args;
    va_start( args, text );
    vsprintf( buffer, text, args );
    va_end( args );

    m_text.push_back( std::string( buffer ) );
}

/*==========================================================================*/
/**
 *  Set the text string.
 *  @param text [in] text string
 */
/*==========================================================================*/
void Text::setText( const std::string& text )
{
    m_text.clear();
    m_text.push_back( text );
}

/*==========================================================================*/
/**
 *  Add the text string.
 *  @param text [in] text string
 */
/*==========================================================================*/
void Text::addText( const char* text, ... )
{
    char buffer[ ::MaxLineLength ];

    va_list args;
    va_start( args, text );
    vsprintf( buffer, text, args );
    va_end( args );

    m_text.push_back( std::string( buffer ) );
}

/*==========================================================================*/
/**
 *  Add the text string.
 *  @param text [in] text string
 */
/*==========================================================================*/
void Text::addText( const std::string& text )
{
    m_text.push_back( text );
}

/*==========================================================================*/
/**
 *  Draw the text string.
 *  @param x [in] drawing position (x coordinate value)
 *  @param y [in] drawing position (y coordinate value)
 */
/*==========================================================================*/
void Text::draw()
{
    kvs::OpenGL::WithPushedAttrib attrib( GL_ALL_ATTRIB_BITS );
    attrib.disable( GL_LIGHTING );
    attrib.disable( GL_TEXTURE_2D );

    const kvs::Vec4 vp = kvs::OpenGL::Viewport();

    kvs::OpenGL::WithPushedMatrix p0( GL_PROJECTION );
    p0.loadIdentity();
    {
        const float front = 0.0f;
        const float back = 2000.0f;
        kvs::OpenGL::SetOrtho( vp, front, back );

        kvs::OpenGL::WithPushedMatrix p1( GL_MODELVIEW );
        p1.loadIdentity();
        {
            kvs::OpenGL::Color( m_color );
            ::DrawStrings( m_text, kvs::Vec2( m_x, m_y ) );
        }
    }
}

} // end of namespace glut

} // end of namespace kvs
