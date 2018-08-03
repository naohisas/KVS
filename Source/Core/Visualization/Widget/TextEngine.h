/*****************************************************************************/
/**
 *  @file   TextEngine.h
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
#pragma once
#include <kvs/RGBColor>
#include <kvs/Vector2>
#include <kvs/Vector3>
#include <string>
#include <vector>
#include <kvs/ScreenBase>
#include "Font.h"


namespace kvs
{

class TextEngine
{
private:
    kvs::Font m_font;

public:
    TextEngine();
    TextEngine( const kvs::Font& font );
    TextEngine( TextEngine* engine );
    virtual ~TextEngine();

    kvs::Font& font() { return m_font; }
    const kvs::Font& font() const { return m_font; }
    void setFont( const kvs::Font& font ) { m_font = font; }

    virtual int width( const char c ) const;
    virtual int width( const std::string& text ) const;
    virtual int height() const;
    virtual void draw( const kvs::Vec2i& p, const std::string& text, kvs::ScreenBase* screen ) const;
    virtual void draw( const kvs::Vec2& p, const std::string& text, kvs::ScreenBase* screen ) const;
    virtual void draw( const kvs::Vec3& p, const std::string& text, kvs::ScreenBase* screen ) const;
    void draw( const kvs::Vec2& p, const kvs::Font::Icon& icon, const float size ) const;
};

} // end of namespace kvs
