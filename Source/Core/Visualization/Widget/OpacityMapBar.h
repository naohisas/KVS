/*****************************************************************************/
/**
 *  @file   OpacityMapBar.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#pragma once
#include <string>
#include <kvs/OpacityMap>
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
class OpacityMapBar : public kvs::WidgetBase
{
public:
    using BaseClass = kvs::WidgetBase;
    using ScreenUpdatedFunc = std::function<void()>;
    using ScreenResizedFunc = std::function<void()>;

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
    kvs::OpacityMap m_opacity_map;
    kvs::Texture2D m_texture; ///< texture data
    kvs::Texture2D m_checkerboard; ///< checkerboard texture
    bool m_show_range_value; ///< range value showing flag
    bool m_enable_anti_aliasing; ///< check flag for anti-aliasing
    ScreenUpdatedFunc m_screen_updated;
    ScreenResizedFunc m_screen_resized;

public:
    OpacityMapBar( kvs::ScreenBase* screen = 0 );
    virtual ~OpacityMapBar();

    void screenUpdated( ScreenUpdatedFunc func ) { m_screen_updated = func; }
    void screenResized( ScreenResizedFunc func ) { m_screen_resized = func; }

    virtual void screenUpdated() { if ( m_screen_updated ) m_screen_updated(); }
    virtual void screenResized() { if ( m_screen_resized ) m_screen_resized(); }

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
    void setOpacityMap( const kvs::OpacityMap& opacity_map );
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
    void create_checkerboard();
    void draw_opacity_bar( const int x, const int y, const int width, const int height );
    void draw_border( const int x, const int y, const int width, const int height );
};

} // end of namespace kvs
