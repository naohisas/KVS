/*****************************************************************************/
/**
 *  @file   ColorMapPalette.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: ColorMapPalette.h 1319 2012-10-02 10:32:10Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#ifndef KVS__GLUT__COLOR_MAP_PALETTE_H_INCLUDE
#define KVS__GLUT__COLOR_MAP_PALETTE_H_INCLUDE

#include <string>
#include <kvs/Texture1D>
#include <kvs/ColorMap>
#include <kvs/Vector2>
#include <kvs/glut/WidgetBase>
#include <kvs/glut/ColorPalette>


namespace kvs
{

class ScreenBase;

namespace glut
{

/*===========================================================================*/
/**
 *  @brief  Color map palette class.
 */
/*===========================================================================*/
class ColorMapPalette : public kvs::glut::WidgetBase
{
public:
    typedef kvs::glut::WidgetBase BaseClass;

private:
    std::string m_caption; ///< caption
    kvs::ColorMap m_color_map; ///< color map
    kvs::Texture1D m_texture; ///< color map texture
    kvs::glut::WidgetBase m_palette; ///< palette
    kvs::RGBColor m_upper_edge_color; ///< upper edge color
    kvs::RGBColor m_lower_edge_color; ///< lower edge color
    kvs::RGBColor m_drawing_color; ///< drawing color
    kvs::Vector2i m_pressed_position; ///< mouse pressed position
    const kvs::glut::ColorPalette* m_color_palette; ///< pointer to the color palette

public:
    ColorMapPalette( kvs::ScreenBase* screen = 0 );
    virtual ~ColorMapPalette();

    virtual void screenUpdated() {};
    virtual void screenResized() {};

    const std::string& caption() const { return m_caption; }
    const kvs::glut::WidgetBase& palette() const { return m_palette; }
    const kvs::ColorMap colorMap() const;

    void setCaption( const std::string& caption ) { m_caption = caption; }
    void setColorMap( const kvs::ColorMap& color_map );
    void setDrawingColor( const kvs::RGBColor& color ) { m_drawing_color = color; }
    void attachColorPalette( const kvs::glut::ColorPalette* palette ) { m_color_palette = palette; }
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
};

} // end of namespace glut

} // end of namespace kvs

#endif // KVS__GLUT__COLOR_MAP_PALETTE_H_INCLUDE
