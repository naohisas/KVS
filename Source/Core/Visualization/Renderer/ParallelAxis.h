/*****************************************************************************/
/**
 *  @file   ParallelAxis.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#pragma once
#include <kvs/RendererBase>
#include <kvs/Module>
#include <kvs/Margins>
#include <kvs/RGBColor>
#include <kvs/UIColor>
#include <kvs/Painter>
#include <kvs/TableObject>
#include <kvs/Deprecated>
#include <kvs/ValueAxis>
#include <kvs/Font>
#include <vector>
#include <string>


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
    // Margins
    kvs::Margins m_margins{ 30 }; ///< margins

    // Title
    std::string m_title = ""; ///< title
    kvs::Font m_title_font{ kvs::Font::Sans, kvs::Font::Bold, 22.0f }; ///< title font
    int m_title_offset = 5; ///< offset between the title and top axis

    // Background
    bool m_background_visible = true; ///< visibility of the background
    kvs::RGBAColor m_background_color = kvs::UIColor::Gray5(); ///< background color

    // Axes
    std::vector<kvs::ValueAxis*> m_axes{}; ///< axes ([0]: x-axis, [1]: y-axis)

    kvs::Real32 m_axis_width = 2.0f; ///< axis width
    kvs::RGBColor m_axis_color = kvs::UIColor::Label(); ///< axis color
    kvs::RGBColor m_value_color = kvs::UIColor::Label(); ///< value color
    kvs::RGBColor m_label_color = kvs::UIColor::Label(); ///< label color

    kvs::Painter m_painter{}; ///< painter

public:
    ParallelAxis() = default;

    // Margins
    void setMargins( const kvs::Margins& margins ) { m_margins = margins; }

    // Title
    void setTitle( const std::string& title ) { m_title = title; }
    void setTitleFont( const kvs::Font& font ) { m_title_font = font; }
    void setTitleOffset( const int offset ) { m_title_offset = offset; }

    // Background
    void setBackgroundVisible( const bool visible = true ) { m_background_visible = visible; }
    void setBackgroundColor( const kvs::RGBAColor& color ) { m_background_color = color; }

    const kvs::Margins& margins() const { return m_margins; }
    const kvs::RGBAColor& backgroundColor() const { return m_background_color; }
    bool isBackgroundVisible() const { return m_background_visible; }

    kvs::Real32 axisWidth() const { return m_axis_width; }
    const kvs::RGBColor& axisColor() const { return m_axis_color; }
    const kvs::RGBColor& valueColor() const { return m_value_color; }
    const kvs::RGBColor& labelColor() const { return m_label_color; }

    void setAxisWidth( const kvs::Real32 width ) { m_axis_width = width; }
    void setAxisColor( const kvs::RGBColor color ) { m_axis_color = color; }
    void setValueColor( const kvs::RGBColor color ) { m_value_color = color; }
    void setLabelColor( const kvs::RGBColor color ) { m_label_color = color; }

    virtual void exec( kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light );

protected:
    kvs::Painter& painter() { return m_painter; }
    void drawTitle( const kvs::Rectangle& content );
    void drawBackground( const kvs::Rectangle& content, const float dpr );
    void drawAxes( const kvs::TableObject* table,  const kvs::Rectangle& content, const float dpr );

public:
    KVS_DEPRECATED( void setTopMargin( const size_t margin ) ) { m_margins.setTop( margin ); }
    KVS_DEPRECATED( void setBottomMargin( const size_t margin ) ) { m_margins.setBottom( margin ); }
    KVS_DEPRECATED( void setLeftMargin( const size_t margin ) ) { m_margins.setLeft( margin ); }
    KVS_DEPRECATED( void setRightMargin( const size_t margin ) ) { m_margins.setRight( margin ); }

    KVS_DEPRECATED( size_t topMargin() const ) { return m_margins.top(); }
    KVS_DEPRECATED( size_t bottomMargin() const ) { return m_margins.bottom(); }
    KVS_DEPRECATED( size_t leftMargin() const ) { return m_margins.left(); }
    KVS_DEPRECATED( size_t rightMargin() const ) { return m_margins.right(); }
};

} // end of namespace kvs
