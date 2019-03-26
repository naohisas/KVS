/****************************************************************************/
/**
 *  @file   OrientationAxis.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: OrientationAxis.h 1719 2014-03-12 06:42:32Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#ifndef KVS__GLUT__ORIENTATION_AXIS_H_INCLUDE
#define KVS__GLUT__ORIENTATION_AXIS_H_INCLUDE

#include <kvs/Camera>
#include <kvs/RGBColor>
#if KVS_ENABLE_DEPRECATED
#include <kvs/ObjectBase>
#include <kvs/ScreenBase>
#include <kvs/Scene>
#include <kvs/glut/Screen>
#endif
#include <kvs/glut/WidgetBase>


namespace kvs
{

class ScreenBase;
class ObjectBase;

namespace glut
{

class Screen;

/*===========================================================================*/
/**
 *  @brief  Orientation axis class.
 */
/*===========================================================================*/
class OrientationAxis : public kvs::glut::WidgetBase
{
public:
    typedef kvs::glut::WidgetBase BaseClass;
    typedef kvs::Camera::ProjectionType ProjectionType;

    enum AxisType
    {
        CorneredAxis = 0,
        CenteredAxis,
        NoneAxis
    };

    enum BoxType
    {
        WiredBox = 0,
        SolidBox,
        NoneBox
    };

private:
    const kvs::ObjectBase* m_object; ///< pointer to the object
    std::string m_x_tag; ///< x axis tag
    std::string m_y_tag; ///< y axis tag
    std::string m_z_tag; ///< z axis tag
    kvs::RGBColor m_x_axis_color; ///< x axis color
    kvs::RGBColor m_y_axis_color; ///< y axis color
    kvs::RGBColor m_z_axis_color; ///< z axis color
    float m_axis_line_width; ///< axis line width
    kvs::RGBColor m_box_color; ///< box color
    kvs::RGBColor m_box_line_color; ///< box line color
    float m_box_line_width; ///< box line width
    bool m_enable_anti_aliasing; ///< check flag for anti-aliasing
    AxisType m_axis_type; ///< axis type
    BoxType m_box_type; ///< box type
    ProjectionType m_projection_type; ///< projection type

public:
    OrientationAxis( kvs::glut::Screen* screen );

    OrientationAxis( kvs::ScreenBase* screen, const kvs::Scene* scene );
    OrientationAxis( kvs::ScreenBase* screen, const kvs::ObjectBase* object );
    virtual ~OrientationAxis();

public:
    virtual void screenUpdated() {}
    virtual void screenResized() {}

    const std::string& xTag() const { return m_x_tag; }
    const std::string& yTag() const { return m_y_tag; }
    const std::string& zTag() const { return m_z_tag; }
    const kvs::RGBColor& xAxisColor() const { return m_x_axis_color; }
    const kvs::RGBColor& yAxisColor() const { return m_y_axis_color; }
    const kvs::RGBColor& zAxisColor() const { return m_z_axis_color; }
    const kvs::RGBColor& boxColor() const { return m_box_color; }
    const kvs::RGBColor& boxLineColor() const { return m_box_line_color; }
    float axisLineWidth() const { return m_axis_line_width; }
    float boxLineWidth() const { return m_box_line_width; }
    AxisType axisType() const { return m_axis_type; }
    BoxType boxType() const { return m_box_type; }
    ProjectionType projectionType() const { return m_projection_type; }
    bool isEnabledAntiAliasing() const { return m_enable_anti_aliasing; }

    void setObject( const kvs::ObjectBase* object ) { m_object = object; }
    void setSize( const size_t size ) { BaseClass::setSize( size, size ); }
    void setXTag( const std::string& tag ) { m_x_tag = tag; }
    void setYTag( const std::string& tag ) { m_y_tag = tag; }
    void setZTag( const std::string& tag ) { m_z_tag = tag; }
    void setXAxisColor( const kvs::RGBColor& color ) { m_x_axis_color = color; }
    void setYAxisColor( const kvs::RGBColor& color ) { m_y_axis_color = color; }
    void setZAxisColor( const kvs::RGBColor& color ) { m_z_axis_color = color; }
    void setAxisLineWidth( const float width ) { m_axis_line_width = width; }
    void setBoxColor( const kvs::RGBColor& color ) { m_box_color = color; }
    void setBoxLineColor( const kvs::RGBColor& color ) { m_box_line_color = color; }
    void setBoxLineWidth( const float width ) { m_box_line_width = width; }
    void setAxisType( const AxisType type ) { m_axis_type = type; }
    void setBoxType( const BoxType type ) { m_box_type = type; }
    void setProjectionType( const ProjectionType type ) { m_projection_type = type; }
    void setEnabledAntiAliasing( const bool enable ) { m_enable_anti_aliasing = enable; }
    void enableAntiAliasing() { this->setEnabledAntiAliasing( true ); }
    void disableAntiAliasing() { this->setEnabledAntiAliasing( false ); }

    void paintEvent();
    void resizeEvent( int width, int height );

protected:
    int adjustedWidth();
    int adjustedHeight();

private:
    void draw_centered_axis( const float length );
    void draw_cornered_axis( const float length );
    void draw_wired_box( const float length );
    void draw_solid_box( const float length );
};

} // end of namespace glut

} // end of namespace kvs

#endif // KVS__GLUT__ORIENTATION_AXIS_H_INCLUDE
