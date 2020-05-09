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

/*===========================================================================*/
/**
 *  @brief  Painter class
 */
/*===========================================================================*/
class Painter
{
private:
    kvs::PaintDevice* m_device; ///< paint device (not allocated, just reference)
    float m_device_pixel_ratio; ///< device pixel ratio set from the screen
    kvs::Font m_font; ///< font
    GLdouble m_model[16]; ///< modelview matrix
    GLdouble m_proj[16]; ///< projection matrix
    GLint m_view[4]; ///< viewport

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
    void drawIcon( const kvs::Vec2& p, const kvs::Font::Icon& icon, const float size ) const;

private:
    void draw_text( const kvs::Vec2& p, const std::string& text ) const;
};

} // end of namespace kvs
