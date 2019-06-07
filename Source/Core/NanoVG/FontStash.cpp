#include "FontStash.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "fons.h"
#include <kvs/OpenGL>
#define GLFONTSTASH_IMPLEMENTATION
#include "fons_gl.h"
#include <kvs/Message>
#include <string>
#include <vector>
#include <kvs/File>


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

    //void add( const std::string& path ) { m_search_path_list.push_back( path ); }
    //void del() { m_search_path_list.clear(); }

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

}

namespace kvs
{

FontStash::Context::Context( int width, int height, int flag )
{
    m_context = glfonsCreate( width, height, flag );
    if ( !m_context )
    {
        kvsMessageError( "Cannot create a FontStash context." );
        return;
    }
}

FontStash::Context::~Context()
{
    if ( m_context )
    {
        glfonsDelete( m_context );
    }
}

FontStash::FontStash( const int width, const int height, const int flag ):
    m_context( width, height, flag )
{
}

FontStash::~FontStash()
{
}

void FontStash::setSize( const float size )
{
    fonsSetSize( m_context(), size );
}

void FontStash::setColor( const unsigned int color_id )
{
    fonsSetColor( m_context(), color_id );
}

void FontStash::setSpacing( const float spacing )
{
    fonsSetSpacing( m_context(), spacing );
}

void FontStash::setBlur( const float blur )
{
    fonsSetBlur( m_context(), blur );
}

void FontStash::setAlign( const int align )
{
    fonsSetAlign( m_context(), align );
}

void FontStash::setFont( const int font_id )
{
    fonsSetFont( m_context(), font_id );
}

int FontStash::addFont( const std::string& name, const std::string& source )
{
    const kvs::File file( source );
    if ( file.exists() )
    {
        const std::string filename( source );
        return fonsAddFont( m_context(), name.c_str(), filename.c_str() );
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
            return fonsAddFont( m_context(), name.c_str(), filename.c_str() );
        }
    }
}

int FontStash::fontID( const std::string& name ) const
{
    return fonsGetFontByName( m_context(), name.c_str() );
}

unsigned int FontStash::colorID( const kvs::RGBAColor& color ) const
{
    const int r = color.r();
    const int g = color.g();
    const int b = color.b();
    const int a = int( 255.0f * color.a() );
    return glfonsRGBA( r, g, b, a );
}

FontStash::Metrics FontStash::metrics() const
{
    Metrics m;
    fonsVertMetrics( m_context(), &m.ascender, &m.descender, &m.lineh );
    return m;
}

float FontStash::ascender() const
{
    float v = 0.0f;
    fonsVertMetrics( m_context(), &v, NULL, NULL );
    return v;
}

float FontStash::descender() const
{
    float v = 0.0f;
    fonsVertMetrics( m_context(), NULL, &v, NULL );
    return v;
}

float FontStash::lineHeight() const
{
    float v = 0.0f;
    fonsVertMetrics( m_context(), NULL, NULL, &v );
    return v;
}

float FontStash::textBounds( const kvs::Vec2& p, const std::string& text ) const
{
    return fonsTextBounds( m_context(), p.x(), p.y(), text.c_str(), NULL, NULL );
}

void FontStash::clearState()
{
    fonsClearState( m_context() );
}

void FontStash::pushState()
{
    fonsPushState( m_context() );
}

void FontStash::popState()
{
    fonsPopState( m_context() );
}

void FontStash::draw( const kvs::Vec2& p, const std::string& text )
{
    fonsDrawText( m_context(), p.x(), p.y(), text.c_str(), NULL );
}

} // end of namespace kvs
