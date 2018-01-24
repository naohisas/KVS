#pragma once
#include <kvs/RendererBase>
#include <kvs/Module>
#include <kvs/RGBColor>
#include <kvs/RGBAColor>
#include <kvs/Painter>


namespace kvs
{

class ObjectBase;
class Camera;
class Light;

/*===========================================================================*/
/**
 *  @brief  Axis2DMatrix class.
 */
/*===========================================================================*/
class Axis2DMatrix : public kvs::RendererBase
{
    kvsModule( kvs::Axis2DMatrix, Renderer );
    kvsModuleBaseClass( kvs::RendererBase );

private:
    int m_top_margin; ///< top margin
    int m_bottom_margin; ///< bottom margin
    int m_left_margin; ///< left margin
    int m_right_margin; ///< right margin
    int m_margin; ///< margin
    kvs::Real32 m_axis_width; ///< axis width
    kvs::RGBColor m_axis_color; ///< axis color
    kvs::RGBColor m_value_color; ///< value color
    kvs::RGBColor m_label_color; ///< label color
    kvs::Painter m_painter; ///< painter

public:
    Axis2DMatrix();

    void setTopMargin( const int margin ) { m_top_margin = margin; }
    void setBottomMargin( const int margin ) { m_bottom_margin = margin; }
    void setLeftMargin( const int margin ) { m_left_margin = margin; }
    void setRightMargin( const int margin ) { m_right_margin = margin; }
    void setMargin( const int margin ) { m_margin = margin; }
    void setAxisWidth( const kvs::Real32 width ) { m_axis_width = width; }
    void setAxisColor( const kvs::RGBColor color ) { m_axis_color = color; }
    void setValueColor( const kvs::RGBColor color ) { m_value_color = color; }
    void setLabelColor( const kvs::RGBColor color ) { m_label_color = color; }

    int topMargin() const { return m_top_margin; }
    int bottomMargin() const { return m_bottom_margin; }
    int leftMargin() const { return m_left_margin; }
    int rightMargin() const { return m_right_margin; }
    kvs::Real32 axisWidth() const { return m_axis_width; }
    const kvs::RGBColor& axisColor() const { return m_axis_color; }
    const kvs::RGBColor& valueColor() const { return m_value_color; }
    const kvs::RGBColor& labelColor() const { return m_label_color; }

    void exec( kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light );
};

} // end of namespace kvs
