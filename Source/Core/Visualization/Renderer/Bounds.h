/****************************************************************************/
/**
 *  @file   Bounds.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: Bounds.h 1797 2014-08-04 01:36:37Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#ifndef KVS__BOUNDS_H_INCLUDE
#define KVS__BOUNDS_H_INCLUDE

#include <kvs/Module>
#include <kvs/RendererBase>
#include <kvs/RGBColor>


namespace kvs
{

class ObjectBase;
class Camera;
class Light;
class LineObject;

/*==========================================================================*/
/**
 *  Create bounds object from volume data.
 */
/*==========================================================================*/
class Bounds : public kvs::RendererBase
{
    kvsModule( kvs::Bounds, Renderer );
    kvsModuleBaseClass( kvs::RendererBase );

public:

    enum Type
    {
        Box = 0, ///< box type bounds
        Corner = 1, ///< corner type bounds
        Circle = 2, ///< circle type bounds
    };

protected:

    Type m_type; ///< bounds type
    kvs::RGBColor m_line_color;
    kvs::Real32 m_line_width;
    kvs::Real32 m_corner_scale; ///< length of corner line
    kvs::Real32 m_division; ///< division of circle
    bool m_enable_anti_aliasing; ///< flag for anti-aliasing
    bool m_show; ///< flag for showing the bounds

public:

    Bounds();

    void exec( kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light );
    void setType( const Bounds::Type type ) { m_type = type; }
    void setLineColor( const kvs::RGBColor& color ) { m_line_color = color; }
    void setLineWidth( const kvs::Real32 width ) { m_line_width = width; }
    void setCornerScale( const float corner_scale ) { m_corner_scale = corner_scale; }
    void setCircleDivision( const float division ) { m_division = division; }
    void enableAntiAliasing() { m_enable_anti_aliasing = true; }
    void disableAntiAliasing() { m_enable_anti_aliasing = false; }
    void show() { m_show = true; }
    void hide() { m_show = false; }

public:

    kvs::LineObject* outputLineObject( const kvs::ObjectBase* object ) const;

private:

    void draw_box_bounds( const kvs::ObjectBase* object );
    void draw_corner_bounds( const kvs::ObjectBase* object );
    void draw_circle_bounds( const kvs::ObjectBase* object );
    kvs::LineObject* output_box_bounds( const kvs::ObjectBase* object ) const;
    kvs::LineObject* output_corner_bounds( const kvs::ObjectBase* object ) const;
    kvs::LineObject* output_circle_bounds( const kvs::ObjectBase* object ) const;
};

} // end of namespace kvs

#endif // KVS__BOUNDS_H_INCLUDE
