#pragma once
#include <kvs/ScreenBase>
#include <kvs/PaintDevice>
#include <kvs/Font>
#include <kvs/FontMetrics>
#include <kvs/Vector2>
#include <kvs/OpenGL>
#include <string>


namespace kvs
{

class Painter
{
private:
    kvs::PaintDevice* m_device; ///< paint device (not allocated, just reference)
    kvs::Font m_font;

public:
    Painter();
    Painter( kvs::ScreenBase* screen );
    virtual ~Painter();

    kvs::PaintDevice* device() const { return m_device; }
    kvs::Font& font() { return m_font; }
    const kvs::Font& font() const { return m_font; }
    void setFont( const kvs::Font& font ) { m_font = font; }
    bool isActive() const { return m_device != NULL; }

    bool begin( kvs::ScreenBase* screen );
    bool end();

    kvs::FontMetrics fontMetrics() const;
    void drawText( const kvs::Vec2i& p, const std::string& text ) const;
    void drawText( const kvs::Vec2& p, const std::string& text ) const;
    void drawText( const kvs::Vec3& p, const std::string& text ) const;
    void drawText( const kvs::Vec2& p, const kvs::Font::Icon& icon, const float size ) const;
};

} // end of namespace kvs
