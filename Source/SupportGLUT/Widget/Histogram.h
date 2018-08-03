/****************************************************************************/
/**
 *  @file Histogram.h
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: Histogram.h 1690 2014-01-01 08:14:08Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#ifndef KVS__GLUT__HISTOGRAM_H_INCLUDE
#define KVS__GLUT__HISTOGRAM_H_INCLUDE

#include <string>
#if KVS_ENABLE_DEPRECATED
#include <kvs/StructuredVolumeObject>
#include <kvs/ImageObject>
#include <kvs/ScreenBase>
#endif
#include <kvs/RGBColor>
#include <kvs/RGBAColor>
#include <kvs/FrequencyTable>
#include <kvs/Texture2D>
#include <kvs/ClassName>
#include <kvs/Vector2>
#include <kvs/glut/WidgetBase>


namespace kvs
{

class StructuredVolumeObject;
class ImageObject;
class ScreenBase;
class MouseEvent;

namespace glut
{

/*===========================================================================*/
/**
 *  @brief  Histogram class.
 */
/*===========================================================================*/
class Histogram : public kvs::glut::WidgetBase
{
public:
    typedef kvs::glut::WidgetBase BaseClass;

private:
    std::string m_caption; ///< caption
    kvs::FrequencyTable m_table; ///< frequency distribution table
    kvs::RGBAColor m_graph_color; ///< graph color
    float m_bias_parameter; ///< bias parameter
    kvs::Texture2D m_texture; ///< histogram texture
    kvs::glut::WidgetBase m_palette; ///< palette
    kvs::RGBColor m_upper_edge_color; ///< upper edge color
    kvs::RGBColor m_lower_edge_color; ///< lower edge color
    kvs::Vector2i m_previous_position; ///< mouse previous position

public:
    Histogram( kvs::ScreenBase* screen = 0 );
    virtual ~Histogram();

    virtual void screenUpdated() {}
    virtual void screenResized() {}

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

} // end of namespace glut

} // end of namespace kvs

#endif // KVS__GLUT__HISTOGRAM_H_INCLUDE
