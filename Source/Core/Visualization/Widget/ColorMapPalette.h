#pragma once
#include <string>
#include <kvs/Texture1D>
#include <kvs/ColorMap>
#include <kvs/Vector2>
#include <kvs/WidgetBase>
#include "ColorPalette.h"


namespace kvs
{

class ScreenBase;

/*===========================================================================*/
/**
 *  @brief  Color map palette class.
 */
/*===========================================================================*/
class ColorMapPalette : public kvs::WidgetBase
{
public:
    typedef kvs::WidgetBase BaseClass;

private:
    std::string m_caption; ///< caption
    kvs::ColorMap m_color_map; ///< color map
    kvs::Texture1D m_texture; ///< color map texture
    kvs::WidgetBase m_palette; ///< palette
    kvs::RGBColor m_drawing_color; ///< drawing color
    kvs::Vec2 m_pressed_position; ///< mouse pressed position
    const kvs::ColorPalette* m_color_palette; ///< pointer to the color palette

public:
    ColorMapPalette( kvs::ScreenBase* screen = 0 );
    virtual ~ColorMapPalette();

    virtual void screenUpdated() {};
    virtual void screenResized() {};

    const std::string& caption() const { return m_caption; }
    const kvs::WidgetBase& palette() const { return m_palette; }
    const kvs::ColorMap colorMap() const;

    void setCaption( const std::string& caption ) { m_caption = caption; }
    void setColorMap( const kvs::ColorMap& color_map );
    void setDrawingColor( const kvs::RGBColor& color ) { m_drawing_color = color; }
    void attachColorPalette( const kvs::ColorPalette* palette ) { m_color_palette = palette; }
    void detachColorPalette() { m_color_palette = NULL; }
    void update() { this->initialize_texture( m_color_map ); }

    void paintEvent();
    void resizeEvent( int width, int height );
    void mousePressEvent( kvs::MouseEvent* event );
    void mouseMoveEvent( kvs::MouseEvent* event );
    void mouseReleaseEvent( kvs::MouseEvent* event );

protected:
    int adjustedWidth();
    int adjustedHeight();

private:
    void initialize_texture( const kvs::ColorMap& color_map );
    void draw_palette();
    void draw_border( const kvs::Rectangle& rect );
};

} // end of namespace kvs
