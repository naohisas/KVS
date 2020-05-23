#pragma once
#include <kvs/RendererBase>
#include <kvs/Module>
#include <kvs/RGBColor>
#include <kvs/RGBAColor>
#include <kvs/Painter>
#include <kvs/Deprecated>
#include <string>
#include "ValueAxis.h"


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

public:
    enum GridlinePattern
    {
        Solid,
        Dash,
        Dot,
        DashDot
    };

private:
    // Margin
    int m_top_margin; ///< top margin
    int m_bottom_margin; ///< bottom margin
    int m_left_margin; ///< left margin
    int m_right_margin; ///< right margin

    // Title
    std::string m_title; ///< title
    kvs::Font m_title_font; ///< title font
    int m_title_offset; ///< offset between the title and top axis

    // Background and borderline
    kvs::RGBAColor m_background_color; ///< background color
    bool m_background_visible; ///< visibility of the background
    kvs::RGBAColor m_borderline_color; ///< borderline color
    float m_borderline_width; ///< borderline width
    bool m_borderline_visible; ///< visibility of the borderline

    // Grid lines
    kvs::Vec2u m_ngridlines; ///< number of grid lines for each axis
    GridlinePattern m_gridline_pattern; ///< grid line pattern
    kvs::RGBColor m_gridline_color; ///< grid line color
    float m_gridline_width; ///< grid line width
    bool m_gridline_visible; ///< visibility of the grid lines

    // Axes
    kvs::ValueAxis* m_x_axis; ///< x axis
    kvs::ValueAxis* m_y_axis; ///< y axis

    kvs::Painter m_painter; ///< painter

public:
    Axis2D();
    virtual ~Axis2D();

    void setTitle( const std::string& title ) { m_title = title; }
    void setTitleFont( const kvs::Font& font ) { m_title_font = font; }
    void setTitleOffset( const int offset ) { m_title_offset = offset; }
    void setTopMargin( const int margin ){ m_top_margin = margin; }
    void setBottomMargin( const int margin ) { m_bottom_margin = margin; }
    void setLeftMargin( const int margin ) { m_left_margin = margin; }
    void setRightMargin( const int margin ) { m_right_margin = margin; }
    void setBackgroundColor( const kvs::RGBAColor& color ) { m_background_color = color; }
    void setBackgroundVisible( const bool visible = true ) { m_background_visible = visible; }
    void setBorderlineColor( const kvs::RGBAColor& color ) { m_borderline_color = color; }
    void setBorderlineWidth( const float width ) { m_borderline_width = width; }
    void setBorderlineVisible( const bool visible = true ) { m_borderline_visible = visible; }
    void setNumberOfGridlines( const kvs::Vec2u& ngrids ) { m_ngridlines = ngrids; }
    void setNumberOfXGridlines( const size_t ngrids ) { m_ngridlines[0] = ngrids; }
    void setNumberOfYGridlines( const size_t ngrids ) { m_ngridlines[1] = ngrids; }
    void setGridlinePattern( const GridlinePattern pattern ) { m_gridline_pattern = pattern; }
    void setGridlinePatternToSolid() { this->setGridlinePattern( Solid ); }
    void setGridlinePatternToDash() { this->setGridlinePattern( Dash ); }
    void setGridlineColor( const kvs::RGBColor& color ) { m_gridline_color = color; }
    void setGridlineWidth( const float width ) { m_gridline_width = width; }
    void setGridlineVisible( const bool visible = true ) { m_gridline_visible = visible; }

    template <typename Axis>
    void setXAxis( const Axis& axis )
    {
        if ( m_x_axis ) { delete m_x_axis; }
        m_x_axis = new Axis( axis );
    }

    template <typename Axis>
    void setYAxis( const Axis& axis )
    {
        if ( m_y_axis ) { delete m_y_axis; }
        m_y_axis = new Axis( axis );
    }

    const std::string& title() const { return m_title; }
    const kvs::Font& titleFont() const { return m_title_font; }
    int titleOffset() const { return m_title_offset; }
    int topMargin() const { return m_top_margin; }
    int bottomMargin() const { return m_bottom_margin; }
    int leftMargin() const { return m_left_margin; }
    int rightMargin() const { return m_right_margin; }
    const kvs::RGBAColor& backgroundColor() const { return m_background_color; }
    bool isBackgroundVisible() const { return m_background_visible; }
    const kvs::RGBAColor& borderlineColor() const { return m_borderline_color; }
    bool isBorderlineVisible() const { return m_borderline_visible; }
    float borderlineWidth() const { return m_borderline_width; }
    const kvs::Vec2u& numberOfGridlines() const { return m_ngridlines; }
    size_t numberOfXGridlines() const { return m_ngridlines[0]; }
    size_t numberOfYGridlines() const { return m_ngridlines[1]; }
    GridlinePattern gridlinePattern() const { return m_gridline_pattern; }
    const kvs::RGBColor& gridlineColor() const { return m_gridline_color; }
    float gridlineWidth() const { return m_gridline_width; }
    bool isGridlineVisible() const { return m_gridline_visible; }

    kvs::ValueAxis& xAxis() { return *m_x_axis; }
    kvs::ValueAxis& yAxis() { return *m_y_axis; }

    void exec( kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light );

private:
    void draw_title( const kvs::Vec4& rect );
    void draw_background( const kvs::Vec4& rect, const float dpr );
    void draw_borderline( const kvs::Vec4& rect, const float dpr );
    void draw_gridlines( const kvs::Vec4& rect, const float dpr );

public:
    KVS_DEPRECATED( void setAxisWidth( const kvs::Real32 width ) )
    {
        m_x_axis->setWidth( width );
        m_y_axis->setWidth( width );
    }

    KVS_DEPRECATED( void setAxisColor( const kvs::RGBColor color ) )
    {
        m_x_axis->setColor( color );
        m_y_axis->setColor( color );
    }

    KVS_DEPRECATED( void setValueColor( const kvs::RGBColor color ) )
    {
        kvs::Font font = m_x_axis->tickLabelFont();
        font.setColor( color );
        m_x_axis->setTickLabelFont( font );
        m_y_axis->setTickLabelFont( font );
    }

    KVS_DEPRECATED( void setLabelColor( const kvs::RGBColor color ) )
    {
        kvs::Font font = m_x_axis->labelFont();
        font.setColor( color );
        m_x_axis->setLabelFont( font );
        m_y_axis->setLabelFont( font );
    }

    KVS_DEPRECATED( void setXLabel( const std::string& label ) )
    {
        m_x_axis->setLabel( label );
    }

    KVS_DEPRECATED( void setYLabel( const std::string& label ) )
    {
        m_y_axis->setLabel( label );
    }

    KVS_DEPRECATED( kvs::Real32 axisWidth() const ) { return m_x_axis->width(); }
    KVS_DEPRECATED( const kvs::RGBColor& axisColor() const ) { return m_x_axis->color(); }
    KVS_DEPRECATED( const kvs::RGBColor& valueColor() const ) { return m_x_axis->tickLabelFont().color(); }
    KVS_DEPRECATED( const kvs::RGBColor& labelColor() const ) { return m_x_axis->labelFont().color(); }
    KVS_DEPRECATED( const std::string& xLabel() const ) { return m_x_axis->label(); }
    KVS_DEPRECATED( const std::string& yLabel() const ) { return m_y_axis->label(); }
};

} // end of namespace kvs
