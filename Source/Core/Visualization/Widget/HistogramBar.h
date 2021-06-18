/*****************************************************************************/
/**
 *  @file   HistogramBar.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#pragma once
#include <string>
#include <kvs/RGBColor>
#include <kvs/RGBAColor>
#include <kvs/FrequencyTable>
#include <kvs/Texture2D>
#include <kvs/Vector2>
#include <kvs/ScreenBase>
#include "WidgetBase.h"


namespace kvs
{

class VolumeObjectBase;
class ImageObject;
class MouseEvent;

/*===========================================================================*/
/**
 *  @brief  HistogramBar class.
 */
/*===========================================================================*/
class HistogramBar : public kvs::WidgetBase
{
public:
    using BaseClass = kvs::WidgetBase;
    using ScreenUpdatedFunc = std::function<void()>;
    using ScreenResizedFunc = std::function<void()>;

private:
    std::string m_caption; ///< caption
    kvs::FrequencyTable m_table; ///< frequency distribution table
    kvs::RGBAColor m_graph_color; ///< graph color
    float m_bias_parameter; ///< bias parameter
    kvs::Texture2D m_texture; ///< histogram texture
    kvs::WidgetBase m_palette; ///< palette
    kvs::Vec2i m_previous_position; ///< mouse previous position
    ScreenUpdatedFunc m_screen_updated;
    ScreenResizedFunc m_screen_resized;

public:
    HistogramBar( kvs::ScreenBase* screen = 0 );
    virtual ~HistogramBar();

    void screenUpdated( ScreenUpdatedFunc func ) { m_screen_updated = func; }
    void screenResized( ScreenResizedFunc func ) { m_screen_resized = func; }

    virtual void screenUpdated() { if ( m_screen_updated ) m_screen_updated(); }
    virtual void screenResized() { if ( m_screen_resized ) m_screen_resized(); }

    const std::string& caption() const { return m_caption; }
    const kvs::FrequencyTable& table() const { return m_table; }

    void setCaption( const std::string& caption ) { m_caption = caption; }
    void setGraphColor( const kvs::RGBAColor& graph_color ) { m_graph_color = graph_color; }
    void setBiasParameter( const float bias_parameter ) { m_bias_parameter = bias_parameter; }
    void setIgnoreValue( const kvs::Real64 value ) { m_table.setIgnoreValue( value ); }
    void setRange( const kvs::Real64 min_range, const kvs::Real64 max_range ) { m_table.setRange( min_range, max_range ); }
    void setNumberOfBins( const kvs::UInt64 nbins ) { m_table.setNumberOfBins( nbins ); }

    void create( const kvs::VolumeObjectBase* volume );
    void create( const kvs::ImageObject* image );

    void paintEvent();
    void resizeEvent( int width, int height );
    void mousePressEvent( kvs::MouseEvent* event );
    void mouseMoveEvent( kvs::MouseEvent* event );
    void mouseReleaseEvent( kvs::MouseEvent* event );

protected:
    int adjustedWidth();
    int adjustedHeight();

private:
    void draw_palette();
    const kvs::ValueArray<kvs::UInt8> get_histogram_image() const;
//    void calculate_density_curve();
    void create_texture();
    void update_texture();
};

} // end of namespace kvs
