/*****************************************************************************/
/**
 *  @file   ScatterPlotMatrixRenderer.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: ScatterPlotMatrixRenderer.h 1797 2014-08-04 01:36:37Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#ifndef KVS__SCATTER_PLOT_MATRIX_RENDERER_H_INCLUDE
#define KVS__SCATTER_PLOT_MATRIX_RENDERER_H_INCLUDE

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
 *  @brief  ScatterPlotMatrixRenderer class.
 */
/*===========================================================================*/
class ScatterPlotMatrixRenderer : public kvs::RendererBase
{
    kvsModule( kvs::ScatterPlotMatrixRenderer, Renderer );
    kvsModuleBaseClass( kvs::RendererBase );

protected:

    int m_top_margin; ///< top margin
    int m_bottom_margin; ///< bottom margin
    int m_left_margin; ///< left margin
    int m_right_margin; ///< right margin
    int m_margin; ///< margin
    int m_active_axis; ///< active axis
    kvs::RGBColor m_point_color; ///< point color
    kvs::UInt8 m_point_opacity; ///< point opacity
    kvs::Real32 m_point_size; ///< point size
    kvs::ColorMap m_color_map; ///< color map
    kvs::RGBAColor m_background_color; ///< background color

public:

    ScatterPlotMatrixRenderer();

    void setTopMargin( const int top_margin ) { m_top_margin = top_margin; }
    void setBottomMargin( const int bottom_margin ) { m_bottom_margin = bottom_margin; }
    void setLeftMargin( const int left_margin ) { m_left_margin = left_margin; }
    void setRightMargin( const int right_margin ) { m_right_margin = right_margin; }
    void setMargin( const int margin ) { m_margin = margin; }
    void setPointColor( const kvs::RGBColor point_color ) { m_active_axis = -1; m_point_color = point_color; }
    void setPointOpacity( const kvs::UInt8 point_opacity ) { m_point_opacity = point_opacity; }
    void setPointSize( const kvs::Real32 point_size ){ m_point_size = point_size; }
    void setColorMap( const kvs::ColorMap& color_map ) { m_color_map = color_map;}
    void setBackgroundColor( const kvs::RGBAColor background_color ) { m_background_color = background_color; }
    void selectAxis( const int index ) { m_active_axis = index; }

    int topMargin() const { return m_top_margin; }
    int bottomMargin() const { return m_bottom_margin; }
    int leftMargin() const { return m_left_margin; }
    int rightMargin() const { return m_right_margin; }
    int margin() const { return m_margin; }
    int activeAxis() const { return m_active_axis; }
    kvs::UInt8 pointOpacity() const { return m_point_opacity; }
    kvs::Real32 pointSize() const { return m_point_size; }
    const kvs::ColorMap& colorMap() const { return m_color_map; }
    const kvs::RGBAColor& backgroundColor() const { return m_background_color; }

    void exec( kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light );
};

} // end of namespace kvs

#endif // KVS__SCATTER_PLOT_MATRIX_RENDERER_H_INCLUDE
