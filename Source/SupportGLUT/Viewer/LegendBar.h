/****************************************************************************/
/**
 *  @file LegendBar.h
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: LegendBar.h 1719 2014-03-12 06:42:32Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#ifndef KVS__GLUT__LEGEND_BAR_H_INCLUDE
#define KVS__GLUT__LEGEND_BAR_H_INCLUDE

#include <string>
#include <kvs/ColorMap>
#include <kvs/RGBColor>
#include <kvs/Texture2D>
#include <kvs/glut/Screen>
#include <kvs/glut/WidgetBase>


namespace kvs
{

namespace glut
{

/*===========================================================================*/
/**
 *  @brief  Legend bar class.
 */
/*===========================================================================*/
class LegendBar : public kvs::glut::WidgetBase
{
public:

    typedef kvs::glut::WidgetBase BaseClass;

    enum OrientationType
    {
        Horizontal = 0, ///< horizontal bar
        Vertical        ///< vertical bar
    };

protected:

    std::string m_caption; ///< caption
    OrientationType m_orientation; ///< bar layout
    size_t m_ndivisions; ///< number of divisions of the bar
    float m_division_line_width; ///< division line width
    kvs::RGBColor m_division_line_color; ///< division line color
    double m_min_value; ///< min. value
    double m_max_value; ///< max. value
    float m_border_width; ///< border line width
    kvs::RGBColor m_border_color; ///< border line color
    kvs::ColorMap m_colormap; ///< color map
    kvs::Texture2D m_texture; ///< texture data
    bool m_show_range_value; ///< range value showing flag
    bool m_enable_anti_aliasing; ///< check flag for anti-aliasing
    bool m_texture_downloaded; ///< check flag for texture

public:

    LegendBar( kvs::ScreenBase* screen = 0 );
    virtual ~LegendBar();

    virtual void screenUpdated(){};
    virtual void screenResized(){};

    const std::string& caption() const;
    double minValue() const;
    double maxValue() const;

    void setCaption( const std::string& caption );
    void setOrientation( const OrientationType orientation );
    void setNumberOfDivisions( const size_t ndivisions );
    void setDivisionLineWidth( const float division_line_width );
    void setDivisionLineColor( const kvs::RGBColor& division_line_color );
    void setRange( const double min_value, const double max_value );
    void setBorderWidth( const float border_width );
    void setBorderColor( const kvs::RGBColor& border_color );
    void setColorMap( const kvs::ColorMap& colormap );
    void enableAntiAliasing();
    void disableAntiAliasing();
    void showRange();
    void hideRange();

    void paintEvent();
    void resizeEvent( int width, int height );

private:

    int get_fitted_width();
    int get_fitted_height();
    void create_texture();
    void release_texture();
    void draw_color_bar( const int x, const int y, const int width, const int height );
    void draw_border( const int x, const int y, const int width, const int height );
};

} // end of namesapce glut

} // end of namespace kvs

#endif // KVS__GLUT__LEGEND_BAR_H_INCLUDE
