/*****************************************************************************/
/**
 *  @file   Axis2D.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: Axis2D.h 1457 2013-03-24 06:32:17Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#ifndef KVS__GLUT__AXIS_2D_H_INCLUDE
#define KVS__GLUT__AXIS_2D_H_INCLUDE

#include <kvs/RendererBase>
#include <kvs/Module>
#include <kvs/RGBColor>
#include <kvs/RGBAColor>


namespace kvs
{

class ObjectBase;
class Camera;
class Light;

namespace glut
{

/*===========================================================================*/
/**
 *  @brief  Axis 2D class.
 */
/*===========================================================================*/
class Axis2D : public kvs::RendererBase
{
    kvsModuleName( kvs::glut::Axis2D );
    kvsModuleCategory( Renderer );
    kvsModuleBaseClass( kvs::RendererBase );

private:

    int m_top_margin; ///< top margin
    int m_bottom_margin; ///< bottom margin
    int m_left_margin; ///< left margin
    int m_right_margin; ///< right margin
    kvs::Real32 m_axis_width; ///< axis width
    kvs::RGBColor m_axis_color; ///< axis color
    kvs::RGBColor m_value_color; ///< value color
    kvs::RGBColor m_label_color; ///< label color

public:

    Axis2D();

    void setTopMargin( const int top_margin );
    void setBottomMargin( const int bottom_margin );
    void setLeftMargin( const int left_margin );
    void setRightMargin( const int right_margin );
    void setAxisWidth( const kvs::Real32 axis_width );
    void setAxisColor( const kvs::RGBColor axis_color );
    void setValueColor( const kvs::RGBColor value_color );
    void setLabelColor( const kvs::RGBColor label_color );

    int topMargin() const;
    int bottomMargin() const;
    int leftMargin() const;
    int rightMargin() const;
    kvs::Real32 axisWidth() const;
    const kvs::RGBColor& axisColor() const;
    const kvs::RGBColor& valueColor() const;
    const kvs::RGBColor& labelColor() const;

    void exec( kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light );
};

} // end of namespace glut

} // end of namespace kvs

#endif // KVS__GLUT__AXIS_2D_H_INCLUDE
