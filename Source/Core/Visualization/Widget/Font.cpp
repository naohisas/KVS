/*****************************************************************************/
/**
 *  @file   Font.cpp
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
#include "Font.h"
/*
// NanoVG related headers
// {
#include <cstdio>
#include <cstdlib>
#include <cstring>
//#include <Core/NanoVG/fontstash.h>
#include "../../NanoVG/fons.h"
#include <kvs/OpenGL>
#define GLFONTSTASH_IMPLEMENTATION
//#include <Core/NanoVG/glfontstash.h>
#include "../../NanoVG/fons_gl.h"
// }
#include <string>
#include <vector>
#include <kvs/File>
#include <kvs/Matrix22>
*/

/*
namespace
{

std::string KVSFontPath()
{
    const std::string sep = kvs::File::Separator();
    const char* kvs_dir = std::getenv("KVS_DIR");
    if ( kvs_dir != NULL )
    {
        std::string path = std::string( kvs_dir ) + sep;
        path += "include" + sep;
        path += "Core" + sep;
        path += "Visualization" + sep;
        path += "Font" + sep;
        return path;
    }

    return "";
}

class SearchPath
{
private:

    std::vector<std::string> m_search_path_list; ///< search path list

public:

    SearchPath()
    {
        this->init();
    }

    void init()
    {
        // Add font directory ("$KVS_DIR/include/Core/Visualization/Font").
        const std::string kvs_shader_path = KVSFontPath();
        if ( !kvs_shader_path.empty() )
        {
            m_search_path_list.push_back( kvs_shader_path );
        }

        // Add current directory (".").
        const std::string sep = kvs::File::Separator();
        m_search_path_list.push_back("." + sep );
    }

    void add( const std::string& path )
    {
        m_search_path_list.push_back( path );
    }

    void del()
    {
        m_search_path_list.clear();
    }

    std::string find( const std::string& source )
    {
        // Search the source file from the m_search_path_list.
        std::vector<std::string>::reverse_iterator path = m_search_path_list.rbegin();
        std::vector<std::string>::reverse_iterator last = m_search_path_list.rend();
        while ( path != last )
        {
            const std::string sep = kvs::File::Separator();
            const std::string filename = *path + sep + source;
            const kvs::File file( filename );
            if ( file.exists() ) { return filename; }
            path++;
        }
        return "";
    }
};

SearchPath search_path;

class FontStash
{
public:
    struct Metrics
    {
        float ascender;
        float descender;
        float lineh;
    };

private:
    FONScontext* m_context;

public:
    FontStash(): m_context( NULL ) {}
    ~FontStash() { if ( m_context ) { glfonsDelete( m_context ); } }

    void create( const int width, const int height, const int flag )
    {
        m_context = glfonsCreate( width, height, flag );
        this->addFont( "SansRegular", "NotoSans-Regular.ttf" );
        this->addFont( "SansItalic", "NotoSans-Italic.ttf" );
        this->addFont( "SansBold", "NotoSans-Bold.ttf" );
        this->addFont( "SansBoldItalic", "NotoSans-BoldItalic.ttf" );
        this->addFont( "SerifRegular", "NotoSerif-Regular.ttf" );
        this->addFont( "SerifItalic", "NotoSerif-Italic.ttf" );
        this->addFont( "SerifBold", "NotoSerif-Bold.ttf" );
        this->addFont( "SerifBoldItalic", "NotoSerif-BoldItalic.ttf" );
        this->addFont( "Icon", "entypo.ttf" );
    }

    bool isCreated()
    {
        return m_context != NULL;
    }

    void setSize( const float size )
    {
        fonsSetSize( m_context, size );
    }

    void setColor( const unsigned int color_id )
    {
        fonsSetColor( m_context, color_id );
    }

    void setSpacing( const float spacing )
    {
        fonsSetSpacing( m_context, spacing );
    }

    void setBlur( const float blur )
    {
        fonsSetBlur( m_context, blur );
    }

    void setAlign( const int align )
    {
        fonsSetAlign( m_context, align );
    }

    void setFont( const int font_id )
    {
        fonsSetFont( m_context, font_id );
    }

    int addFont( const std::string& name, const std::string& source )
    {
        const kvs::File file( source );
        if ( file.exists() )
        {
            const std::string filename( source );
            return fonsAddFont( m_context, name.c_str(), filename.c_str() );
        }
        else
        {
            const std::string filename = search_path.find( source );
            if ( filename.empty() )
            {
                kvsMessageError( "Cannot find '%s'.", source.c_str() );
                return FONS_INVALID;
            }
            else
            {
                return fonsAddFont( m_context, name.c_str(), filename.c_str() );
            }
        }
    }

    int fontID( const std::string& name ) const
    {
        return fonsGetFontByName( m_context, name.c_str() );
    }

    unsigned int colorID( const kvs::RGBAColor& color ) const
    {
        const int r = color.r();
        const int g = color.g();
        const int b = color.b();
        const int a = int( 255.0f * color.a() );
        return glfonsRGBA( r, g, b, a );
    }

    Metrics metrics() const
    {
        Metrics m;
        fonsVertMetrics( m_context, &m.ascender, &m.descender, &m.lineh );
        return m;
    }

    float ascender() const
    {
        float v = 0.0f;
        fonsVertMetrics( m_context, &v, NULL, NULL );
        return v;
    }

    float descender() const
    {
        float v = 0.0f;
        fonsVertMetrics( m_context, NULL, &v, NULL );
        return v;
    }

    float lineHeight() const
    {
        float v = 0.0f;
        fonsVertMetrics( m_context, NULL, NULL, &v );
        return v;
    }

    float textBounds( const kvs::Vec2& p, const std::string& text ) const
    {
        return fonsTextBounds( m_context, p.x(), p.y(), text.c_str(), NULL, NULL );
    }

    void clearState()
    {
        fonsClearState( m_context );
    }


    void pushState()
    {
        fonsPushState( m_context );
    }

    void popState()
    {
        fonsPopState( m_context );
    }

    void draw( const kvs::Vec2& p, const std::string& text )
    {
        fonsDrawText( m_context, p.x(), p.y(), text.c_str(), NULL );
    }
};

inline std::string ToUTF8( int cp )
{
    int n = 0;
    if (cp < 0x80) n = 1;
    else if (cp < 0x800) n = 2;
    else if (cp < 0x10000) n = 3;
    else if (cp < 0x200000) n = 4;
    else if (cp < 0x4000000) n = 5;
    else if (cp <= 0x7fffffff) n = 6;

    char str[8];
    str[n] = '\0';
    switch (n) {
    case 6: str[5] = 0x80 | (cp & 0x3f); cp = cp >> 6; cp |= 0x4000000;
    case 5: str[4] = 0x80 | (cp & 0x3f); cp = cp >> 6; cp |= 0x200000;
    case 4: str[3] = 0x80 | (cp & 0x3f); cp = cp >> 6; cp |= 0x10000;
    case 3: str[2] = 0x80 | (cp & 0x3f); cp = cp >> 6; cp |= 0x800;
    case 2: str[1] = 0x80 | (cp & 0x3f); cp = cp >> 6; cp |= 0xc0;
    case 1: str[0] = cp;
    }
    return std::string( str );
}

FontStash Stash;

const std::string FamilyName[] =
{
    "Sans",
    "Serif"
};

const std::string StyleName[] =
{
    "Regular",
    "Italic",
    "Bold",
    "BoldItalic"
};

}
*/

namespace
{

const std::string FamilyName[] =
{
    "Sans",
    "Serif"
};

const std::string StyleName[] =
{
    "Regular",
    "Italic",
    "Bold",
    "BoldItalic"
};

}

namespace kvs
{

/*
void Font::AddSearchPath( const std::string& path )
{
    ::search_path.add( path );
}

void Font::SetSearchPath( const std::string& path )
{
    ::search_path.del();
    ::search_path.add( path );
}

void Font::ResetSearchPath()
{
    ::search_path.del();
    ::search_path.init();
}

void Font::RemoveSearchPath()
{
    ::search_path.del();
}
*/

Font::Font()
{
    this->setFamily( Sans );
    this->setStyle( Regular );
    this->setHorizontalAlign( Left );
    this->setVerticalAlign( Baseline );
    this->setSize( 18.0f );
    this->setColor( kvs::RGBColor::Black() );
    this->setEnabledShadow( false );
    this->setShadowColor( kvs::RGBColor::Black() );
    this->setShadowSizeRatio( 1.0f );
    this->setShadowDistance( 6.0f );
    this->setShadowAngle( 45.0f );
    this->setShadowBlur( 4.0f );
//    ::Stash.create( 512, 512, FONS_ZERO_TOPLEFT );
}

Font::Font( const Family& family, const float size )
{
    this->setFamily( family );
    this->setStyle( Regular );
    this->setHorizontalAlign( Left );
    this->setVerticalAlign( Baseline );
    this->setSize( size );
    this->setColor( kvs::RGBColor::Black() );
    this->setEnabledShadow( false );
    this->setShadowColor( kvs::RGBColor::Black() );
    this->setShadowSizeRatio( 1.0f );
    this->setShadowDistance( 6.0f );
    this->setShadowAngle( 45.0f );
    this->setShadowBlur( 4.0f );
//    ::Stash.create( 512, 512, FONS_ZERO_TOPLEFT );
}

Font::Font( const Family& family, const float size, const kvs::RGBAColor& color )
{
    this->setFamily( family );
    this->setStyle( Regular );
    this->setHorizontalAlign( Left );
    this->setVerticalAlign( Baseline );
    this->setSize( size );
    this->setColor( color );
    this->setEnabledShadow( false );
    this->setShadowColor( kvs::RGBColor::Black() );
    this->setShadowSizeRatio( 1.0f );
    this->setShadowDistance( 6.0f );
    this->setShadowAngle( 45.0f );
    this->setShadowBlur( 4.0f );
//    ::Stash.create( 512, 512, FONS_ZERO_TOPLEFT );
}

Font::Font( const Family& family, const Style& style, const float size )
{
    this->setFamily( family );
    this->setStyle( style );
    this->setHorizontalAlign( Left );
    this->setVerticalAlign( Baseline );
    this->setSize( size );
    this->setColor( kvs::RGBColor::Black() );
    this->setEnabledShadow( false );
    this->setShadowColor( kvs::RGBColor::Black() );
    this->setShadowSizeRatio( 1.0f );
    this->setShadowDistance( 6.0f );
    this->setShadowAngle( 45.0f );
    this->setShadowBlur( 4.0f );
//    ::Stash.create( 512, 512, FONS_ZERO_TOPLEFT );
}

Font::Font( const Family& family, const Style& style, const float size, const kvs::RGBAColor& color )
{
    this->setFamily( family );
    this->setStyle( style );
    this->setHorizontalAlign( Left );
    this->setVerticalAlign( Baseline );
    this->setSize( size );
    this->setColor( color );
    this->setEnabledShadow( false );
    this->setShadowColor( kvs::RGBColor::Black() );
    this->setShadowSizeRatio( 1.0f );
    this->setShadowDistance( 6.0f );
    this->setShadowAngle( 45.0f );
    this->setShadowBlur( 4.0f );
//    ::Stash.create( 512, 512, FONS_ZERO_TOPLEFT );
}

Font::~Font()
{
}

std::string Font::familyName() const
{
    return ::FamilyName[ this->family() ];
}

std::string Font::styleName() const
{
    return ::StyleName[ this->style() ];
}

std::string Font::fontName() const
{
    return this->familyName() + this->styleName();
}


//float Font::width( const std::string& text ) const
//{
    /*
    const std::string name = ::FamilyName[this->family()] + ::StyleName[this->style()];
    const int font_id = ::Stash.fontID( name );
    ::Stash.setFont( font_id );
    ::Stash.setSize( this->size() );
    return ::Stash.textBounds( kvs::Vec2( 0.0f, 0.0f ), text );
    */
//    return 0.0f;
//}

//float Font::height() const
//{
    /*
    const std::string name = ::FamilyName[this->family()] + ::StyleName[this->style()];
    const int font_id = ::Stash.fontID( name );
    ::Stash.setFont( font_id );
    ::Stash.setSize( this->size() );
    return ::Stash.lineHeight();
    */
//    return 0.0f;
//}

//void Font::draw( const kvs::Vec2& p, const std::string& text ) const
//{
/*
    ::Stash.clearState();

    const std::string name = ::FamilyName[this->family()] + ::StyleName[this->style()];
    const int font_id = ::Stash.fontID( name );
    ::Stash.setFont( font_id );

    const kvs::Vec2 d( 0.0f, ::Stash.descender() );
    const kvs::Mat2 r( kvs::Mat2::Rotation( this->shadowAngle() ) );
    const kvs::Vec2 v( this->shadowDistance(), 0.0f );
    if ( this->isEnabledShadow() )
    {
        ::Stash.pushState();
        ::Stash.setAlign( this->horizontalAlign() | this->verticalAlign() );
        ::Stash.setBlur( this->shadowBlur() );
        ::Stash.setColor( ::Stash.colorID( this->shadowColor() ) );
        ::Stash.setSize( this->size() * this->shadowSizeRatio() );
        ::Stash.draw( p + d + r * v, text );
        ::Stash.popState();
    }

    ::Stash.setAlign( this->horizontalAlign() | this->verticalAlign() );
    ::Stash.setBlur( 0.0f );
    ::Stash.setColor( ::Stash.colorID( this->color() ) );
    ::Stash.setSize( this->size() );
    ::Stash.draw( p + d, text );
*/
//}

//void Font::draw( const kvs::Vec2& p, const Font::Icon& icon, const float size ) const
//{
    /*
    ::Stash.clearState();

    const int font_id = ::Stash.fontID( "Icon" );
    const kvs::Vec2 d( 0.0f, ::Stash.descender() );
    ::Stash.setFont( font_id );
    ::Stash.setAlign( this->horizontalAlign() | this->verticalAlign() );
    ::Stash.setColor( ::Stash.colorID( this->color() ) );
    ::Stash.setSize( size );
    ::Stash.draw( p + d, ::ToUTF8( icon ) );
    */
//}

} // end of namespace kvs
