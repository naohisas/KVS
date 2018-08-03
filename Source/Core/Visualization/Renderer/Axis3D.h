#pragma once
#include <string>
#include <kvs/RendererBase>
#include <kvs/Module>
#include <kvs/Vector3>
#include <kvs/RGBColor>
#include <kvs/Painter>


namespace kvs
{

class ObjectBase;
class Camera;
class Light;
class LineObject;

/*===========================================================================*/
/**
 *  @brief  3D axis class.
 */
/*===========================================================================*/
class Axis3D : public kvs::RendererBase
{
    kvsModule( kvs::Axis3D, Renderer );
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
    kvs::RGBAColor m_background_color; ///< background color
    bool m_enable_anti_aliasing; ///< flag for anti-aliasing
    bool m_show_labels; ///< flags for showing the labels
    bool m_show_values; ///< flags for showing the values
    bool m_show_gridlines; ///< flag for showing the gridlines
    bool m_show; ///< flags for showing the axis
    kvs::Painter m_painter; ///< painter

public:
    Axis3D();

    void exec( kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light );

    void setAxisColor( const kvs::RGBColor& color ) { m_axis_color = color; }
    void setAxisWidth( const kvs::Real32 width ) { m_axis_width = width; }
    void setGridDrawMode( const GridDrawMode mode ) { m_grid_draw_mode = mode; }
    void setGridlinePattern( const GridlinePattern pattern ) { m_gridline_pattern = pattern; }
    void setNumberOfGridlines( const kvs::Vec3u& ngridlines ) { m_ngridlines = ngridlines; }
    void setNumberOfXGridlines( const size_t ngridlines ) { m_ngridlines[0] = ngridlines; }
    void setNumberOfYGridlines( const size_t ngridlines ) { m_ngridlines[1] = ngridlines; }
    void setNumberOfZGridlines( const size_t ngridlines ) { m_ngridlines[2] = ngridlines; }
    void setGridlineColor( const kvs::RGBColor& color ) { m_gridline_color = color; }
    void setGridlineWidth( const kvs::Real32 width ) { m_gridline_width = width; }
    void setXLabel( const std::string& label ) { m_x_label = label; }
    void setYLabel( const std::string& label ) { m_y_label = label; }
    void setZLabel( const std::string& label ) { m_z_label = label; }
    void setLabelColor( const kvs::RGBColor& color ) { m_label_color = color; }
    void setValueColor( const kvs::RGBColor& color ) { m_value_color = color; }
    void setBackgroundColor( const kvs::RGBAColor color ) { m_background_color = color; }
    void setEnabledAntiAliasing( const bool enable ) { m_enable_anti_aliasing = enable; }

    void enableAntiAliasing() { this->setEnabledAntiAliasing( true ); }
    void disableAntiAliasing() { this->setEnabledAntiAliasing( false ); }
    void showLabels() { m_show_labels = true; }
    void showValues() { m_show_values = true; }
    void showGridlines() { m_show_gridlines = true; }
    void show() { m_show = true; }
    void hideLabels() { m_show_labels = false; }
    void hideValues() { m_show_values = false; }
    void hideGridlines() { m_show_gridlines = false; }
    void hide() { m_show = false; }

    const kvs::RGBColor& axisColor() const { return m_axis_color; }
    kvs::Real32 axisWidth() const { return m_axis_width; }
    GridDrawMode gridDrawMode() const { return m_grid_draw_mode; }
    GridlinePattern gridlinePattern() const { return m_gridline_pattern; }
    const kvs::Vec3u& numberOfGridlines() const { return m_ngridlines; }
    size_t numberOfXGridlines() const { return m_ngridlines.x(); }
    size_t numberOfYGridlines() const { return m_ngridlines.y(); }
    size_t numberOfZGridlines() const { return m_ngridlines.z(); }
    const kvs::RGBColor& gridlineColor() const { return m_gridline_color; }
    kvs::Real32 gridlineWidth() const { return m_gridline_width; }
    const std::string& xLabel() const { return m_x_label; }
    const std::string& yLabel() const { return m_y_label; }
    const std::string& zLabel() const { return m_z_label; }
    const kvs::RGBColor& labelColor() const { return m_label_color; }
    const kvs::RGBColor& valueColor() const { return m_value_color; }
    const kvs::RGBAColor& backgroundColor() const { return m_background_color; }

    kvs::LineObject* outputLineObject( const kvs::ObjectBase* object ) const;

private:
    void draw_xy_axes( const kvs::Vec3& min_coord, const kvs::Vec3& max_coord, const kvs::Real32 depth_z );
    void draw_yz_axes( const kvs::Vec3& min_coord, const kvs::Vec3& max_coord, const kvs::Real32 depth_x );
    void draw_zx_axes( const kvs::Vec3& min_coord, const kvs::Vec3& max_coord, const kvs::Real32 depth_y );
    void draw_xy_gridlines( const kvs::Vec3& min_coord, const kvs::Vec3& max_coord, const kvs::Real32 depth_z );
    void draw_yz_gridlines( const kvs::Vec3& min_coord, const kvs::Vec3& max_coord, const kvs::Real32 depth_x );
    void draw_zx_gridlines( const kvs::Vec3& min_coord, const kvs::Vec3& max_coord, const kvs::Real32 depth_y );
    void draw_labels( const kvs::Vec3& min_coord, const kvs::Vec3& max_coord );
};

} // end of namespace kvs
