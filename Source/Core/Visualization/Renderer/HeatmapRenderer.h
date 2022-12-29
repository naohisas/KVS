/*****************************************************************************/
/**
 *  @file   HeatmapRenderer.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#pragma once
#include <kvs/RendererBase>
#include <kvs/Module>
#include <kvs/Painter>
#include <kvs/ObjectBase>
#include <kvs/Camera>
#include <kvs/Light>
#include <kvs/ColorMap>
#include <kvs/RGBAColor>
#include <kvs/Margins>
#include <kvs/UIColor>
#include <kvs/Rectangle>
#include <kvs/TableObject>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  HeatmapRenderer class.
 */
/*===========================================================================*/
class HeatmapRenderer : public kvs::RendererBase
{
    kvsModule( kvs::HeatmapRenderer, Renderer );
    kvsModuleBaseClass( kvs::RendererBase );

private:
    // Margins
    kvs::Margins m_margins{ 30 };

    // Edge
    kvs::RGBColor m_edge_color{ kvs::UIColor::Label() }; ///< edge color
    kvs::Real32 m_edge_opacity = 1.0f; ///< edge opacity
    float m_edge_width = 0.5f; ///< edge width

    // Background
    kvs::RGBAColor m_background_color{ kvs::UIColor::Gray5() }; ///< background color
    bool m_background_visible = false; ///< visibility of the background

    kvs::ColorMap m_color_map{ 256 }; ///< color map
    kvs::Painter m_painter{}; ///< painter

public:
    HeatmapRenderer() { m_color_map.create(); }

    void setMargins( const kvs::Margins& margins ) { m_margins = margins; }
    void setColorMap( const kvs::ColorMap& color_map ) { m_color_map = color_map; }
    void setBackgroundColor( const kvs::RGBAColor color ) { m_background_color = color; }
    void setBackgroundVisible( const bool visible = true ) { m_background_visible = visible; }
    void setEdgeWidth( const float width ) { m_edge_width = width; }

    const kvs::Margins& margins() const { return m_margins; }
    const kvs::RGBAColor& backgroundColor() const { return m_background_color; }
    bool isBackgroundVisible() const { return m_background_visible; }
    const kvs::ColorMap& colorMap() const { return m_color_map; }
    float edgeWidth() const { return m_edge_width; }

    void exec( kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light );

protected:
    kvs::Painter& painter() { return m_painter; }
    void setupColorMapRange( const kvs::TableObject* table );
    void drawBackground( const kvs::Rectangle& rect, const float dpr );
    void drawMap( const kvs::TableObject* table, const kvs::Rectangle& rect, const float dpr );
};

} // end of namespace kvs
