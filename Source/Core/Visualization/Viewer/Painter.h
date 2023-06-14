/*****************************************************************************/
/**
 *  @file   Painter.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
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
    kvs::PaintDevice* m_device = nullptr; ///< paint device (not allocated, just reference)
    float m_device_pixel_ratio = 1.0f; ///< device pixel ratio set from the screen
    kvs::Font m_font{}; ///< font
    GLdouble m_model[16] = {0}; ///< modelview matrix
    GLdouble m_proj[16] = {0}; ///< projection matrix
    GLint m_view[4] = {0}; ///< viewport

public:
    Painter() = default;
    Painter( kvs::ScreenBase* screen ) { this->begin( screen ); }
    virtual ~Painter() { this->end(); }

    kvs::PaintDevice* device() const { return m_device; }
    float devicePixelRatio() const { return m_device_pixel_ratio; }
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
