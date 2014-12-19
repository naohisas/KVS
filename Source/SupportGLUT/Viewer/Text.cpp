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


namespace
{

const size_t CharacterHeight = 12;
const size_t MaxLineLength   = 255;

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
Text::Text( void ):
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
void Text::draw( void )
{
    glPushAttrib( GL_CURRENT_BIT | GL_ENABLE_BIT );

    glDisable( GL_LIGHTING );
    glDisable( GL_TEXTURE_2D );

    // Get viewport information.
    GLint vp[4]; glGetIntegerv( GL_VIEWPORT, vp );
    const GLint left   = vp[0];
    const GLint bottom = vp[1];
    const GLint right  = vp[2];
    const GLint top    = vp[3];

    glMatrixMode( GL_PROJECTION );
    glPushMatrix();
    {
        glLoadIdentity();

        const float front = 0.0f;
        const float back = 2000.0f;
        glOrtho( left, right, bottom, top, front, back );

        glMatrixMode( GL_MODELVIEW );
        glPushMatrix();
        {
            glLoadIdentity();
            glColor3ub( m_color.r(), m_color.g(), m_color.b() );

            for ( size_t line = 0; line < m_text.size(); line++ )
            {
                glRasterPos2i( m_x, m_y -::CharacterHeight * line );

                char* line_head = const_cast<char*>( m_text[line].c_str() );
                for( char* p = line_head; *p; p++ )
                {
                    glutBitmapCharacter( GLUT_BITMAP_8_BY_13, *p );
                }
            }
        }
        glPopMatrix();
        glMatrixMode( GL_PROJECTION );
    }
    glPopMatrix();
    glMatrixMode( GL_MODELVIEW );

    glPopAttrib();
}

} // end of namespace glut

} // end of namespace kvs
