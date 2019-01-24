/****************************************************************************/
/**
 *  @file Text.h
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: Text.h 1319 2012-10-02 10:32:10Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#pragma once
#include <string>
#include <vector>
#include <kvs/RGBColor>


namespace kvs
{

namespace glut
{

/*==========================================================================*/
/**
 *  Text drawing class.
 */
/*==========================================================================*/
class Text
{
protected:
    size_t m_x; ///< position x
    size_t m_y; ///< position y
    kvs::RGBColor m_color; ///< text color
    std::vector<std::string> m_text;  ///< text string

public:
    Text();

    void setPosition( const size_t x, const size_t y );
    void setColor( const kvs::RGBColor& color );
    void setText( const char* text, ... );
    void setText( const std::string& text );
    void addText( const char* text, ... );
    void addText( const std::string& text );

    void draw();
};

} // end of namespace glut

} // end of namespace kvs
