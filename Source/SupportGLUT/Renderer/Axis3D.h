/*****************************************************************************/
/**
 *  @file   Axis3D.h
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
#ifndef KVS__GLUT__AXIS_3D_H_INCLUDE
#define KVS__GLUT__AXIS_3D_H_INCLUDE

#include <string>
#include <kvs/RendererBase>
#include <kvs/Module>
#include <kvs/Vector3>
#include <kvs/RGBColor>


namespace kvs
{

class ObjectBase;
class Camera;
class Light;
class LineObject;

namespace glut
{

/*===========================================================================*/
/**
 *  @brief  3D axis class.
 */
/*===========================================================================*/
class Axis3D : public kvs::RendererBase
{
    kvsModuleName( kvs::glut::Axis3D );
    kvsModuleCategory( Renderer );
    kvsModuleBaseClass( kvs::RendererBase );

public:

    enum GridDrawMode
    {
        StaticGrid, ///< static grid drawing
        FarGrid ///< far grid drawing
    };

    enum GridlinePattern
    {
        SolidLine, ///< solid line
        DashedLine ///< dahsed line
    };

private:

    kvs::RGBColor m_axis_color; ///< axis color
    kvs::Real32 m_axis_width; ///< axis width
    GridDrawMode m_grid_draw_mode; ///< grid draw mode
    GridlinePattern m_gridline_pattern; ///< gridline pattern
    kvs::Vector3ui m_ngridlines; ///< number of gridlines
    kvs::RGBColor m_gridline_color; ///< gridline color
    kvs::Real32 m_gridline_width; ///< gridline width
    std::string m_x_label; ///< label of the x axis
    std::string m_y_label; ///< label of the y axis
    std::string m_z_label; ///< label of the z axis
    kvs::RGBColor m_label_color; ///< label color
    kvs::RGBColor m_value_color; ///< value color
    bool m_enable_anti_aliasing; ///< flag for anti-aliasing
    bool m_show_labels; ///< flags for showing the labels
    bool m_show_values; ///< flags for showing the values
    bool m_show_gridlines; ///< flag for showing the gridlines
    bool m_show; ///< flags for showing the axis

public:

    Axis3D();

    void exec( kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light );

    void setAxisColor( const kvs::RGBColor& color );
    void setAxisWidth( const kvs::Real32 width );
    void setGridDrawMode( const GridDrawMode mode );
    void setGridlinePattern( const GridlinePattern pattern );
    void setNumberOfGridlines( const kvs::Vector3ui& ngridlines );
    void setNumberOfXGridlines( const size_t ngridlines );
    void setNumberOfYGridlines( const size_t ngridlines );
    void setNumberOfZGridlines( const size_t ngridlines );
    void setGridlineColor( const kvs::RGBColor& color );
    void setGridlineWidth( const kvs::Real32 width );
    void setXLabel( const std::string& label );
    void setYLabel( const std::string& label );
    void setZLabel( const std::string& label );
    void setLabelColor( const kvs::RGBColor& color );
    void setValueColor( const kvs::RGBColor& color );

    void enableAntiAliasing();
    void disableAntiAliasing();
    void showLabels();
    void showValues();
    void showGridlines();
    void show();
    void hideLabels();
    void hideValues();
    void hideGridlines();
    void hide();

    const kvs::RGBColor& axisColor() const;
    kvs::Real32 axisWidth() const;
    GridDrawMode gridDrawMode() const;
    GridlinePattern gridlinePattern() const;
    const kvs::Vector3ui& numberOfGridlines() const;
    size_t numberOfXGridlines() const;
    size_t numberOfYGridlines() const;
    size_t numberOfZGridlines() const;
    const kvs::RGBColor& gridlineColor() const;
    kvs::Real32 gridlineWidth() const;
    const std::string& xLabel() const;
    const std::string& yLabel() const;
    const std::string& zLabel() const;
    const kvs::RGBColor& labelColor() const;
    const kvs::RGBColor& valueColor() const;

public:

    kvs::LineObject* outputLineObject( const kvs::ObjectBase* object ) const;

private:

    void draw_xy_axes( const kvs::Vector3f& min_coord, const kvs::Vector3f& max_coord, const kvs::Real32 depth_z );
    void draw_yz_axes( const kvs::Vector3f& min_coord, const kvs::Vector3f& max_coord, const kvs::Real32 depth_x );
    void draw_zx_axes( const kvs::Vector3f& min_coord, const kvs::Vector3f& max_coord, const kvs::Real32 depth_y );
    void draw_xy_gridlines( const kvs::Vector3f& min_coord, const kvs::Vector3f& max_coord, const kvs::Real32 depth_z );
    void draw_yz_gridlines( const kvs::Vector3f& min_coord, const kvs::Vector3f& max_coord, const kvs::Real32 depth_x );
    void draw_zx_gridlines( const kvs::Vector3f& min_coord, const kvs::Vector3f& max_coord, const kvs::Real32 depth_y );
    void draw_labels( const kvs::Vector3f& min_coord, const kvs::Vector3f& max_coord );
};

} // end of namespace glut

} // end of namespace kvs

#endif // KVS__GLUT__AXIS_3D_H_INCLUDE
