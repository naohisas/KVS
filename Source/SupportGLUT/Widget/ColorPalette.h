/*****************************************************************************/
/**
 *  @file   ColorPalette.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: ColorPalette.h 1319 2012-10-02 10:32:10Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#ifndef KVS__GLUT__COLOR_PALETTE_H_INCLUDE
#define KVS__GLUT__COLOR_PALETTE_H_INCLUDE

#include <string>
#include <kvs/glut/WidgetBase>


namespace kvs
{

class ScreenBase;
class MouseEvent;

namespace glut
{

/*===========================================================================*/
/**
 *  @brief  Color palette class.
 */
/*===========================================================================*/
class ColorPalette : public kvs::glut::WidgetBase
{
public:
    typedef kvs::glut::WidgetBase BaseClass;

private:
    std::string m_caption; ///< caption
    kvs::glut::WidgetBase m_SV_palette; ///< SV (saturation/value) palette
    kvs::glut::WidgetBase m_H_palette; ///< H (hue) palette
    kvs::glut::WidgetBase m_selected_color_box; ///< selected color box
    int m_H_indicator; ///< hue indicator
    int m_S_indicator; ///< saturation indicator
    int m_V_indicator; ///< value indicator
    kvs::RGBColor m_upper_edge_color; ///< upper edge color
    kvs::RGBColor m_lower_edge_color; ///< lower edge color

public:
    ColorPalette( kvs::ScreenBase* screen = 0 );
    virtual ~ColorPalette();

    virtual void screenUpdated() {};
    virtual void screenResized() {};

    const std::string& caption() const { return m_caption; }
    const kvs::RGBColor color() const;

    void setCaption( const std::string& caption ) { m_caption = caption; }

public:
    void paintEvent();
    void resizeEvent( int width, int height );
    void mousePressEvent( kvs::MouseEvent* event );
    void mouseMoveEvent( kvs::MouseEvent* event );
    void mouseReleaseEvent( kvs::MouseEvent* event );

protected:
    int adjustedWidth();
    int adjustedHeight();

private:
    void draw_SV_palette();
    void draw_H_palette();
    void draw_selected_color_box();
    void draw_selected_color_value();
    float get_H_value() const;
    float get_S_value() const;
    float get_V_value() const;
};

} // end of namespace glut

} // end of namespace kvs

#endif // KVS__GLUT__COLOR_PALETTE_H_INCLUDE
