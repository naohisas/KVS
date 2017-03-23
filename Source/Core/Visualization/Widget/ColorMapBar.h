/*****************************************************************************/
/**
 *  @file   ColorMapBar.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id$
 */
/*****************************************************************************/
#pragma once
#include <string>
#include <kvs/ColorMap>
#include <kvs/RGBColor>
#include <kvs/Texture2D>
#include <kvs/ScreenBase>
#include "WidgetBase.h"


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Legend bar class.
 */
/*===========================================================================*/
class ColorMapBar : public kvs::WidgetBase
{
public:
    typedef kvs::WidgetBase BaseClass;

    enum OrientationType
    {
        Horizontal = 0, ///< horizontal bar
        Vertical        ///< vertical bar
    };

private:
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
    ColorMapBar( kvs::ScreenBase* screen = 0 );
    virtual ~ColorMapBar();

    virtual void screenUpdated(){};
    virtual void screenResized(){};

    const std::string& caption() const { return m_caption; }
    double minValue() const { return m_min_value; }
    double maxValue() const { return m_max_value; }

    void setCaption( const std::string& caption ) { m_caption = caption; }
    void setOrientation( const OrientationType orientation ) { m_orientation = orientation; }
    void setOrientationToHorizontal() { this->setOrientation( Horizontal ); }
    void setOrientationToVertical() { this->setOrientation( Vertical ); }
    void setNumberOfDivisions( const size_t ndivisions ) { m_ndivisions = ndivisions; }
    void setDivisionLineWidth( const float width ) { m_division_line_width = width; }
    void setDivisionLineColor( const kvs::RGBColor& color ) { m_division_line_color = color; }
    void setRange( const double min_value, const double max_value ) { m_min_value = min_value; m_max_value = max_value; }
    void setBorderWidth( const float width ) { m_border_width = width; }
    void setBorderColor( const kvs::RGBColor& color ) { m_border_color = color; }
    void setColorMap( const kvs::ColorMap& colormap );
    void setEnabledAntiAliasing( bool enable ) { m_enable_anti_aliasing = enable; }
    void enableAntiAliasing() { this->setEnabledAntiAliasing( true ); }
    void disableAntiAliasing() { this->setEnabledAntiAliasing( false ); }
    void showRange() { m_show_range_value = true; }
    void hideRange() { m_show_range_value = false; }

    void paintEvent();
    void resizeEvent( int width, int height );

protected:
    int adjustedWidth();
    int adjustedHeight();

private:
    void create_texture();
    void release_texture();
    void draw_color_bar( const int x, const int y, const int width, const int height );
    void draw_border( const int x, const int y, const int width, const int height );
};

} // end of namespace kvs
