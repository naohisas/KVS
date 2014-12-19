/*****************************************************************************/
/**
 *  @file   ScatterPlotRenderer.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: ScatterPlotRenderer.h 1797 2014-08-04 01:36:37Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#ifndef KVS__SCATTER_PLOT_RENDERER_H_INCLUDE
#define KVS__SCATTER_PLOT_RENDERER_H_INCLUDE

#include <kvs/RendererBase>
#include <kvs/Module>
#include <kvs/RGBColor>
#include <kvs/RGBAColor>
#include <kvs/ColorMap>


namespace kvs
{

class ObjectBase;
class Camera;
class Light;

/*===========================================================================*/
/**
 *  @brief  Scatter plot renderer class.
 */
/*===========================================================================*/
class ScatterPlotRenderer : public kvs::RendererBase
{
    kvsModule( kvs::ScatterPlotRenderer, Renderer );
    kvsModuleBaseClass( kvs::RendererBase );

protected:

    int m_top_margin; ///< top margin
    int m_bottom_margin; ///< bottom margin
    int m_left_margin; ///< left margin
    int m_right_margin; ///< right margin
    bool m_has_point_color; ///< check flag whether the point color is specified
    kvs::RGBColor m_point_color; ///< point color
    kvs::UInt8 m_point_opacity; ///< point opacity
    kvs::Real32 m_point_size; ///< point size
    kvs::ColorMap m_color_map; ///< color map
    kvs::RGBAColor m_background_color; ///< background color

public:

    ScatterPlotRenderer();

    void setTopMargin( const int margin ) { m_top_margin = margin; }
    void setBottomMargin( const int margin ) { m_bottom_margin = margin; }
    void setLeftMargin( const int margin ) { m_left_margin = margin; }
    void setRightMargin( const int margin ) { m_right_margin = margin; }
    void setPointColor( const kvs::RGBColor color ) { m_has_point_color = true; m_point_color = color; }
    void setPointOpacity( const kvs::UInt8 opacity ) { m_point_opacity = opacity; }
    void setPointSize( const kvs::Real32 size ) { m_point_size = size; }
    void setColorMap( const kvs::ColorMap& color_map ) { m_has_point_color = true; m_color_map = color_map; }
    void setBackgroundColor( const kvs::RGBAColor color ) { m_background_color = color; }

    int topMargin() const { return m_top_margin; }
    int bottomMargin() const { return m_bottom_margin; }
    int leftMargin() const { return m_left_margin; }
    int rightMargin() const { return m_right_margin; }
    kvs::UInt8 pointOpacity() const { return m_point_opacity; }
    kvs::Real32 pointSize() const { return m_point_size; }
    const kvs::ColorMap& colorMap() const { return m_color_map; }
    const kvs::RGBAColor& backgroundColor() const { return m_background_color; }

    void exec( kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light );
};

} // end of namespace kvs

#endif // KVS__SCATTER_PLOT_RENDERER_H_INCLUDE
