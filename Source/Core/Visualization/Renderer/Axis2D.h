#pragma once
#include <kvs/RendererBase>
#include <kvs/Module>
#include <kvs/RGBColor>
#include <kvs/RGBAColor>
#include <kvs/Painter>
#include <string>


namespace kvs
{

class ObjectBase;
class Camera;
class Light;

/*===========================================================================*/
/**
 *  @brief  Axis 2D class.
 */
/*===========================================================================*/
class Axis2D : public kvs::RendererBase
{
    kvsModule( kvs::Axis2D, Renderer );
    kvsModuleBaseClass( kvs::RendererBase );

private:
    int m_top_margin; ///< top margin
    int m_bottom_margin; ///< bottom margin
    int m_left_margin; ///< left margin
    int m_right_margin; ///< right margin
    kvs::Real32 m_axis_width; ///< axis width
    kvs::RGBColor m_axis_color; ///< axis color
    kvs::RGBColor m_value_color; ///< value color
    kvs::RGBColor m_label_color; ///< label color
    kvs::RGBAColor m_background_color; ///< background color
    std::string m_x_label; ///< label for x axis
    std::string m_y_label; ///< label for y axis
    kvs::Painter m_painter; ///< painter

public:
    Axis2D();

    void setTopMargin( const int margin ){ m_top_margin = margin; }
    void setBottomMargin( const int margin ) { m_bottom_margin = margin; }
    void setLeftMargin( const int margin ) { m_left_margin = margin; }
    void setRightMargin( const int margin ) { m_right_margin = margin; }
    void setAxisWidth( const kvs::Real32 width ) { m_axis_width = width; }
    void setAxisColor( const kvs::RGBColor color ) { m_axis_color = color; }
    void setValueColor( const kvs::RGBColor color ) { m_value_color = color; }
    void setLabelColor( const kvs::RGBColor color ) { m_label_color = color; }
    void setBackgroundColor( const kvs::RGBAColor color ) { m_background_color = color; }
    void setXLabel( const std::string& label ) { m_x_label = label; }
    void setYLabel( const std::string& label ) { m_y_label = label; }

    int topMargin() const { return m_top_margin; }
    int bottomMargin() const { return m_bottom_margin; }
    int leftMargin() const { return m_left_margin; }
    int rightMargin() const { return m_right_margin; }
    kvs::Real32 axisWidth() const { return m_axis_width; }
    const kvs::RGBColor& axisColor() const { return m_axis_color; }
    const kvs::RGBColor& valueColor() const { return m_value_color; }
    const kvs::RGBColor& labelColor() const { return m_label_color; }
    const kvs::RGBAColor& backgroundColor() const { return m_background_color; }
    const std::string& xLabel() const { return m_x_label; }
    const std::string& yLabel() const { return m_y_label; }

    void exec( kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light );
};

} // end of namespace kvs
