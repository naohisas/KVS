/*****************************************************************************/
/**
 *  @file   ParallelCoordinatesRenderer.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id$
 */
/*****************************************************************************/
#ifndef KVS__PARALLEL_COORDINATES_RENDERER_H_INCLUDE
#define KVS__PARALLEL_COORDINATES_RENDERER_H_INCLUDE

#include <kvs/RendererBase>
#include <kvs/Module>
#include <kvs/ColorMap>


namespace kvs
{

class ObjectBase;
class Camera;
class Light;

/*===========================================================================*/
/**
 *  @brief  Parallel coordinates renderer class.
 */
/*===========================================================================*/
class ParallelCoordinatesRenderer : public kvs::RendererBase
{
    kvsModule( kvs::ParallelCoordinatesRenderer, Renderer );
    kvsModuleBaseClass( kvs::RendererBase );

private:

    int m_top_margin; ///< top margin
    int m_bottom_margin; ///< bottom margin
    int m_left_margin; ///< left margin
    int m_right_margin; ///< right margin
    mutable bool m_enable_anti_aliasing; ///< flag for anti-aliasing (AA)
    mutable bool m_enable_multisample_anti_aliasing; ///< flag for multisample anti-aliasing (MSAA)
    size_t m_active_axis; ///< index of active axis
    kvs::UInt8 m_line_opacity; ///< line opacity
    kvs::Real32 m_line_width; ///< line width
    kvs::ColorMap m_color_map; ///< color map

public:

    ParallelCoordinatesRenderer();

    void setTopMargin( const int top_margin ) { m_top_margin = top_margin; }
    void setBottomMargin( const int bottom_margin ) { m_bottom_margin = bottom_margin; }
    void setLeftMargin( const int left_margin ) { m_left_margin = left_margin; }
    void setRightMargin( const int right_margin ) { m_right_margin = right_margin; }
    void setLineOpacity( const kvs::UInt8 opacity ) { m_line_opacity = opacity; }
    void setLineWidth( const kvs::Real32 width ) { m_line_width = width; }
    void setColorMap( const kvs::ColorMap& color_map ) { m_color_map = color_map; }
    void selectAxis( const size_t index ) { m_active_axis = index; }
    int topMargin() const { return m_top_margin; }
    int bottomMargin() const { return m_bottom_margin; }
    int leftMargin() const { return m_left_margin; }
    int rightMargin() const { return m_right_margin; }
    size_t activeAxis() const { return m_active_axis; }
    kvs::UInt8 lineOpacity() const { return m_line_opacity; }
    kvs::Real32 lineWidth() const { return m_line_width; }
    void enableAntiAliasing( const bool multisample = false ) const;
    void disableAntiAliasing() const;

    void exec( kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light );
};

} // end of namespace kvs

#endif // KVS__PARALLEL_COORDINATES_RENDERER_H_INCLUDE
