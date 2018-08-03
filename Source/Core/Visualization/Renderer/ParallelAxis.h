#pragma once
#include <kvs/RendererBase>
#include <kvs/Module>
#include <kvs/RGBColor>
#include <kvs/Painter>


namespace kvs
{

class ObjectBase;
class Camera;
class Light;

/*===========================================================================*/
/**
 *  @brief  ParallelAxis class.
 */
/*===========================================================================*/
class ParallelAxis : public kvs::RendererBase
{
    kvsModule( kvs::ParallelAxis, Renderer );
    kvsModuleBaseClass( kvs::RendererBase );

private:
    size_t m_top_margin; ///< top margin
    size_t m_bottom_margin; ///< bottom margin
    size_t m_left_margin; ///< left margin
    size_t m_right_margin; ///< right margin
    kvs::Real32   m_axis_width; ///< axis width
    kvs::RGBColor m_axis_color; ///< axis color
    kvs::RGBColor m_value_color; ///< value color
    kvs::RGBColor m_label_color; ///< label color
    kvs::Painter m_painter; ///< painter

public:
    ParallelAxis();

    size_t topMargin() const { return m_top_margin; }
    size_t bottomMargin() const { return m_bottom_margin; }
    size_t leftMargin() const { return m_left_margin; }
    size_t rightMargin() const { return m_right_margin; }
    kvs::Real32 axisWidth() const { return m_axis_width; }
    const kvs::RGBColor& axisColor() const { return m_axis_color; }
    const kvs::RGBColor& valueColor() const { return m_value_color; }
    const kvs::RGBColor& labelColor() const { return m_label_color; }

    void setTopMargin( const size_t margin ) { m_top_margin = margin; }
    void setBottomMargin( const size_t margin ) { m_bottom_margin = margin; }
    void setLeftMargin( const size_t margin ) { m_left_margin = margin; }
    void setRightMargin( const size_t margin ) { m_right_margin = margin; }
    void setAxisWidth( const kvs::Real32 width ) { m_axis_width = width; }
    void setAxisColor( const kvs::RGBColor color ) { m_axis_color = color; }
    void setValueColor( const kvs::RGBColor color ) { m_value_color = color; }
    void setLabelColor( const kvs::RGBColor color ) { m_label_color = color; }

    void exec( kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light );
};

} // end of namespace kvs
