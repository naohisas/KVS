#include "FontMetrics.h"
#include <kvs/Assert>


namespace kvs
{

FontMetrics::FontMetrics( const kvs::Font& font, kvs::PaintDevice* device ):
    m_font( font ),
    m_device( device )
{
}

int FontMetrics::height() const
{
    KVS_ASSERT( m_device != NULL );

    int line_height = 0;
    kvs::FontStash* engine = m_device->textEngine();
    engine->pushState();
    {
        const std::string name = m_font.fontName();
        const int font_id = engine->fontID( name );
        engine->setFont( font_id );
        engine->setSize( m_font.size() );
        line_height = engine->lineHeight();
    }
    engine->popState();
    return line_height;
}

int FontMetrics::width( const std::string& text ) const
{
    KVS_ASSERT( m_device != NULL );

    int text_width = 0;
    kvs::FontStash* engine = m_device->textEngine();
    engine->pushState();
    {
        const std::string name = m_font.fontName();
        const int font_id = engine->fontID( name );
        engine->setFont( font_id );
        engine->setSize( m_font.size() );
        text_width = engine->textBounds( kvs::Vec2( 0.0f, 0.0f ), text );
    }
    engine->popState();
    return text_width;
}

} // end of namespace kvs
