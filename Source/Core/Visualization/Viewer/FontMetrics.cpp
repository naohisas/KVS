/*****************************************************************************/
/**
 *  @file   FontMetrics.cpp
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#include "FontMetrics.h"
#include <kvs/Assert>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new FontMetrics class.
 *  @param  font [in] font data
 *  @param  device [in] paint device
 */
/*===========================================================================*/
FontMetrics::FontMetrics( const kvs::Font& font, kvs::PaintDevice* device ):
    m_font( font ),
    m_device( device )
{
}

/*===========================================================================*/
/**
 *  @brief  Returns height of the characters represented by the specified font.
 *  @return height in pixels of the characters
 */
/*===========================================================================*/
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

/*===========================================================================*/
/**
 *  @brief  Returns width of the specfied text with the font.
 *  @return width in pixels of the text
 */
/*===========================================================================*/
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
