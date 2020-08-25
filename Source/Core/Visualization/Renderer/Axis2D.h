/*****************************************************************************/
/**
 *  @file   Axis2D.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#pragma once
#include <kvs/RendererBase>
#include <kvs/Module>
#include <kvs/RGBColor>
#include <kvs/RGBAColor>
#include <kvs/Painter>
#include <kvs/Margins>
#include <kvs/Deprecated>
#include <string>
#include <vector>
#include "ValueAxis.h"


namespace kvs
{

class ObjectBase;
class Camera;
class Light;
class TableObject;

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
    kvs::Margins m_margins;

    // Title
    std::string m_title; ///< title
    kvs::Font m_title_font; ///< title font
    int m_title_offset; ///< offset between the title and top axis

    // Background
    bool m_background_visible; ///< visibility of the background
    kvs::RGBAColor m_background_color; ///< background color

    // Border
    bool m_border_visible; ///< visibility of the border
    kvs::RGBAColor m_border_color; ///< border color
    float m_border_width; ///< border width

    // Grid lines
    bool m_gridline_visible; ///< visibility of the grid lines
    GridlinePattern m_gridline_pattern; ///< grid line pattern
    kvs::RGBColor m_gridline_color; ///< grid line color
    float m_gridline_width; ///< grid line width
    kvs::Vec2u m_ngridlines; ///< number of grid lines for each axis

    // Axes
    std::vector<kvs::ValueAxis*> m_axes; ///< axes ([0]: x-axis, [1]: y-axis)

    kvs::Painter m_painter; ///< painter

public:
    Axis2D();
    virtual ~Axis2D();

    // Title
    void setTitle( const std::string& title ) { m_title = title; }
    void setTitleFont( const kvs::Font& font ) { m_title_font = font; }
    void setTitleOffset( const int offset ) { m_title_offset = offset; }

    // Margin
    void setMargins( const kvs::Margins& margins ) { m_margins = margins; }

    // Background
    void setBackgroundVisible( const bool visible = true ) { m_background_visible = visible; }
    void setBackgroundColor( const kvs::RGBAColor& color ) { m_background_color = color; }

    // Border
    void setBorderVisible( const bool visible = true ) { m_border_visible = visible; }
    void setBorderColor( const kvs::RGBAColor& color ) { m_border_color = color; }
    void setBorderWidth( const float width ) { m_border_width = width; }

    // Gridline
    void setGridlineVisible( const bool visible = true ) { m_gridline_visible = visible; }
    void setGridlinePattern( const GridlinePattern pattern ) { m_gridline_pattern = pattern; }
    void setGridlinePatternToSolid() { this->setGridlinePattern( Solid ); }
    void setGridlinePatternToDash() { this->setGridlinePattern( Dash ); }
    void setGridlinePatternToDot() { this->setGridlinePattern( Dash ); }
    void setGridlinePatternToDashDot() { this->setGridlinePattern( Dash ); }
    void setGridlineColor( const kvs::RGBColor& color ) { m_gridline_color = color; }
    void setGridlineWidth( const float width ) { m_gridline_width = width; }
    void setNumberOfGridlines( const kvs::Vec2u& ngrids ) { m_ngridlines = ngrids; }
    void setNumberOfXGridlines( const size_t ngrids ) { m_ngridlines[0] = ngrids; }
    void setNumberOfYGridlines( const size_t ngrids ) { m_ngridlines[1] = ngrids; }

    // Axis
    template <typename Axis> void setXAxis( const Axis& axis ) { this->setAxis( axis, 0 ); }
    template <typename Axis> void setYAxis( const Axis& axis ) { this->setAxis( axis, 1 ); }
    template <typename Axis> void setAxis( const Axis& axis, const size_t index )
    {
        KVS_ASSERT( index < m_axes.size() );
        if ( m_axes[ index ] ) { delete m_axes[ index ]; }
        m_axes[ index ] = new Axis( axis );
    }

    // Get methods.
    const std::string& title() const { return m_title; }
    const kvs::Font& titleFont() const { return m_title_font; }
    int titleOffset() const { return m_title_offset; }
    const kvs::Margins& margins() const { return m_margins; }
    const kvs::RGBAColor& backgroundColor() const { return m_background_color; }
    bool isBackgroundVisible() const { return m_background_visible; }
    const kvs::RGBAColor& borderColor() const { return m_border_color; }
    bool isBorderVisible() const { return m_border_visible; }
    float borderWidth() const { return m_border_width; }
    const kvs::Vec2u& numberOfGridlines() const { return m_ngridlines; }
    size_t numberOfXGridlines() const { return m_ngridlines[0]; }
    size_t numberOfYGridlines() const { return m_ngridlines[1]; }
    GridlinePattern gridlinePattern() const { return m_gridline_pattern; }
    const kvs::RGBColor& gridlineColor() const { return m_gridline_color; }
    float gridlineWidth() const { return m_gridline_width; }
    bool isGridlineVisible() const { return m_gridline_visible; }

    kvs::ValueAxis& xAxis() { return *m_axes[0]; }
    kvs::ValueAxis& yAxis() { return *m_axes[1]; }
    kvs::ValueAxis& axis( const size_t index ) { return *m_axes[ index ]; }

    void exec( kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light );

protected:
    kvs::Painter& painter() { return m_painter; }
    std::vector<kvs::ValueAxis*>& axes() { return m_axes; }
    void drawTitle( const kvs::Rectangle& rect );
    void drawBackground( const kvs::Rectangle& rect, const float dpr );
    void drawBorder( const kvs::Rectangle& rect, const float dpr );
    void drawGridlines( const kvs::Rectangle& rect, const float dpr );
    void drawAxis( const kvs::Rectangle& rect, kvs::ValueAxis* axis );
    void drawAxes( const kvs::Rectangle& rect );
    void updateAxes( const kvs::TableObject* table );

public:
    KVS_DEPRECATED( void setTopMargin( const int margin ) ) { m_margins.setTop( margin ); }
    KVS_DEPRECATED( void setBottomMargin( const int margin ) ) { m_margins.setBottom( margin ); }
    KVS_DEPRECATED( void setLeftMargin( const int margin ) ) { m_margins.setLeft( margin ); }
    KVS_DEPRECATED( void setRightMargin( const int margin ) ) { m_margins.setRight( margin ); }

    KVS_DEPRECATED( void setAxisWidth( const kvs::Real32 width ) )
    {
        m_axes[0]->setWidth( width );
        m_axes[1]->setWidth( width );
    }

    KVS_DEPRECATED( void setAxisColor( const kvs::RGBColor color ) )
    {
        m_axes[0]->setColor( color );
        m_axes[1]->setColor( color );
    }

    KVS_DEPRECATED( void setValueColor( const kvs::RGBColor color ) )
    {
        kvs::Font font = m_axes[0]->tickLabelFont();
        font.setColor( color );
        m_axes[0]->setTickLabelFont( font );
        m_axes[1]->setTickLabelFont( font );
    }

    KVS_DEPRECATED( void setLabelColor( const kvs::RGBColor color ) )
    {
        kvs::Font font = m_axes[0]->labelFont();
        font.setColor( color );
        m_axes[0]->setLabelFont( font );
        m_axes[1]->setLabelFont( font );
    }

    KVS_DEPRECATED( void setXLabel( const std::string& label ) )
    {
        m_axes[0]->setLabel( label );
    }

    KVS_DEPRECATED( void setYLabel( const std::string& label ) )
    {
        m_axes[1]->setLabel( label );
    }

    KVS_DEPRECATED( int topMargin() const ) { return m_margins.top(); }
    KVS_DEPRECATED( int bottomMargin() const ) { return m_margins.bottom(); }
    KVS_DEPRECATED( int leftMargin() const ) { return m_margins.left(); }
    KVS_DEPRECATED( int rightMargin() const ) { return m_margins.right(); }
    KVS_DEPRECATED( kvs::Real32 axisWidth() const ) { return m_axes[0]->width(); }
    KVS_DEPRECATED( const kvs::RGBColor& axisColor() const ) { return m_axes[0]->color(); }
    KVS_DEPRECATED( const kvs::RGBColor& valueColor() const ) { return m_axes[0]->tickLabelFont().color(); }
    KVS_DEPRECATED( const kvs::RGBColor& labelColor() const ) { return m_axes[0]->labelFont().color(); }
    KVS_DEPRECATED( const std::string& xLabel() const ) { return m_axes[0]->label(); }
    KVS_DEPRECATED( const std::string& yLabel() const ) { return m_axes[1]->label(); }
};

} // end of namespace kvs
