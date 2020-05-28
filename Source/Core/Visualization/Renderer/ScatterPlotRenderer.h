/*****************************************************************************/
/**
 *  @file   ScatterPlotRenderer.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#pragma once
#include <kvs/RendererBase>
#include <kvs/Module>
#include <kvs/RGBColor>
#include <kvs/RGBAColor>
#include <kvs/ColorMap>
#include <kvs/Painter>
#include <kvs/Margins>
#include <kvs/Deprecated>


namespace kvs
{

class ObjectBase;
class Camera;
class Light;
class TableObject;

/*===========================================================================*/
/**
 *  @brief  Scatter plot renderer class.
 */
/*===========================================================================*/
class ScatterPlotRenderer : public kvs::RendererBase
{
    kvsModule( kvs::ScatterPlotRenderer, Renderer );
    kvsModuleBaseClass( kvs::RendererBase );

private:
    // Margins
    kvs::Margins m_margins;
    /*
    int m_top_margin; ///< top margin
    int m_bottom_margin; ///< bottom margin
    int m_left_margin; ///< left margin
    int m_right_margin; ///< right margin
    */

    // Point
    kvs::RGBColor m_point_color; ///< point color
    kvs::Real32 m_point_opacity; ///< point opacity
    kvs::Real32 m_point_size; ///< point size

    // Edge
    kvs::RGBColor m_edge_color; ///< edge color
    kvs::Real32 m_edge_opacity; ///< edge opacity
    kvs::Real32 m_edge_width; ///< edge width

    // Polyline
    kvs::RGBColor m_polyline_color; ///< polyline color
    kvs::Real32 m_polyline_opacity; ///< polyline opacity
    kvs::Real32 m_polyline_width; ///< polyline width
    bool m_polyline_visible; ///< visibility of the polyline

    // Background
    kvs::RGBAColor m_background_color; ///< background color
    bool m_background_visible; ///< visibility of the background

    kvs::ColorMap m_color_map; ///< color map
    kvs::Painter m_painter; ///< painter

public:
    ScatterPlotRenderer();

    void setMargins( const kvs::Margins& margins ) { m_margins = margins; }
    void setPointColor( const kvs::RGBColor color ) { m_point_color = color; }
    void setPointOpacity( const kvs::Real32 opacity ) { m_point_opacity = opacity; }
    void setPointSize( const kvs::Real32 size ) { m_point_size = size; }
    void setEdgeColor( const kvs::RGBColor color ) { m_edge_color = color; }
    void setEdgeOpacity( const kvs::Real32 opacity ) { m_edge_opacity = opacity; }
    void setEdgeWidth( const kvs::Real32 width ) { m_edge_width = width; }
    void setPolylineColor( const kvs::RGBColor color ) { m_polyline_color = color; }
    void setPolylineOpacity( const kvs::Real32 opacity ) { m_polyline_opacity = opacity; }
    void setPolylineWidth( const kvs::Real32 width ) { m_polyline_width = width; }
    void setPolylineVisible( const bool visible = true ) { m_polyline_visible = visible; }
    void setBackgroundColor( const kvs::RGBAColor color ) { m_background_color = color; }
    void setBackgroundVisible( const bool visible = true ) { m_background_visible = visible; }
    void setColorMap( const kvs::ColorMap& color_map ) { m_color_map = color_map; }

    const kvs::Margins& margins() const { return m_margins; }
    const kvs::RGBColor& pointColor() const { return m_point_color; }
    kvs::Real32 pointOpacity() const { return m_point_opacity; }
    kvs::Real32 pointSize() const { return m_point_size; }
    const kvs::RGBColor& edgeColor() const { return m_edge_color; }
    kvs::Real32 edgeOpacity() const { return m_edge_opacity; }
    kvs::Real32 edgeWidth() const { return m_edge_width; }
    const kvs::RGBColor& polylineColor() const { return m_polyline_color; }
    kvs::Real32 polylineOpacity() const { return m_polyline_opacity; }
    kvs::Real32 polylineWidth() const { return m_polyline_width; }
    bool isPolylineVisible() const { return m_polyline_visible; }
    const kvs::RGBAColor& backgroundColor() const { return m_background_color; }
    bool isBackgroundVisible() const { return m_background_visible; }
    const kvs::ColorMap& colorMap() const { return m_color_map; }

    void exec( kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light );

protected:
    kvs::Painter& painter() { return m_painter; }
    void drawBackground( const kvs::Rectangle& rect, const float dpr );
    void drawPolyline( const kvs::Rectangle& rect, kvs::TableObject* table, const size_t x_index, const size_t y_index );
    void drawPoint( const kvs::Rectangle& rect, kvs::TableObject* table, const size_t x_index, const size_t y_index, const bool has_values );

public:
    KVS_DEPRECATED( void setTopMargin( const int margin ) ) { m_margins.setTop( margin ); }
    KVS_DEPRECATED( void setBottomMargin( const int margin ) ) { m_margins.setBottom( margin ); }
    KVS_DEPRECATED( void setLeftMargin( const int margin ) ) { m_margins.setLeft( margin ); }
    KVS_DEPRECATED( void setRightMargin( const int margin ) ) { m_margins.setRight( margin ); }
    KVS_DEPRECATED( void setEnabledPolyline( const bool enabled ) ) { m_polyline_visible = enabled; }
    KVS_DEPRECATED( int topMargin() const ) { return m_margins.top(); }
    KVS_DEPRECATED( int bottomMargin() const ) { return m_margins.bottom(); }
    KVS_DEPRECATED( int leftMargin() const ) { return m_margins.left(); }
    KVS_DEPRECATED( int rightMargin() const ) { return m_margins.right(); }
};

} // end of namespace kvs
