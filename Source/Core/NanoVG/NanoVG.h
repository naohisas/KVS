/*****************************************************************************/
/**
 *  @file   NanoVG.h
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
#pragma once
#include "nvg.h"
#include <kvs/RGBAColor>
#include <kvs/Vector2>
#include <kvs/Vector4>
#include <kvs/Matrix22>
#include <kvs/Rectangle>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  NanoVG class.
 */
/*===========================================================================*/
class NanoVG
{
public:
    enum
    {
        Antialias = 1 << 0,
        StencilStroke = 1 << 1,
        Debug = 1 << 2,
    };

public:

    class Context
    {
    private:
        NVGcontext* m_context;
    public:
        Context( int flags );
        ~Context();
        NVGcontext* operator () () { return m_context; }
    };

private:
    Context m_context; ///< context

public:
    NanoVG( int flags );
    ~NanoVG();

    // Frame
//    void beginFrame( int width, int height, float pixel_ratio = 1.0f );
    void beginFrame( int width, int height, float pixel_ratio );
    void cancelFrame();
    void endFrame();

    // State handling
    void save();
    void restore();
    void reset();

    // Render style
    void setStrokeColor( NVGcolor color );
    void setStrokeColor( const kvs::RGBAColor& color );
    void setStrokePaint( NVGpaint paint );
    void setFillColor( NVGcolor color );
    void setFillColor( const kvs::RGBAColor& color );
    void setFillPaint( NVGpaint paint );
    void setMiterLimit( float limit );
    void setStrokeWidth( float width );
    void setLineCap( int cap );
    void setLineJoin( int join );
    void setGlobalAlpha( float alpha );

    // Paint
    NVGpaint linearGradient( float sx, float sy, float ex, float ey, NVGcolor icol, NVGcolor ocal );
    NVGpaint linearGradient( const kvs::Vec2& s, const kvs::Vec2& e, const kvs::RGBAColor& icol, const kvs::RGBAColor& ocal );
    NVGpaint boxGradient( float x, float y, float w, float h, float r, float f, NVGcolor icol, NVGcolor ocol );
    NVGpaint boxGradient( const kvs::Vec4& rect, float r, float f, const kvs::RGBAColor& icol, const kvs::RGBAColor& ocol );
    NVGpaint boxGradient( const kvs::Rectangle& rect, float r, float f, const kvs::RGBAColor& icol, const kvs::RGBAColor& ocol );
    NVGpaint radialGradient( float cx, float cy, float inr, float outr, NVGcolor icol, NVGcolor ocol );
    NVGpaint radialGradient( const kvs::Vec2& c, float inr, float outr, const kvs::RGBAColor& icol, const kvs::RGBAColor& ocol );

    // Scissor
    void scissor( float x, float y, float w, float h );
    void intersectScissor( float x, float y, float w, float h );
    void resetScissor();

    // Path
    void beginPath();
    void moveTo( float x, float y );
    void moveTo( const kvs::Vec2& p );
    void lineTo( float x, float y );
    void lineTo( const kvs::Vec2& p );
    void bezierTo( float c1x, float c1y, float c2x, float c2y, float x, float y );
    void bezierTo( const kvs::Vec2& p1, const kvs::Vec2& p2, const kvs::Vec2& p3 );
    void quadTo( float cx, float cy, float x, float y );
    void quadTo( const kvs::Vec2& p1, const kvs::Vec2& p2 );
    void arcTo( float x1, float y1, float x2, float y2, float radius );
    void arcTo( const kvs::Vec2& p1, const kvs::Vec2& p2, float radius );
    void closePath();
    void pathWinding( int dir );

    void arc( float cx, float cy, float r, float a0, float a1, int dir );
    void arc( const kvs::Vec2& c, float r, float a0, float a1, int dir );
    void rect( float x, float y, float w, float h );
    void rect( const kvs::Vec4& rect );
    void rect( const kvs::Rectangle& rect );
    void roundedRect( float x, float y, float w, float h, float r );
    void roundedRect( const kvs::Vec4& rect, float r );
    void roundedRect( const kvs::Rectangle& rect, float r );
    void roundedRect( float x, float y, float w, float h, float rad_top_left, float rad_top_right, float rad_bottom_right, float rad_bottom_left );
    void roundedRect( const kvs::Vec4& rect, float rad_top_left, float rad_top_right, float rad_bottom_right, float rad_bottom_left );
    void roundedRect( const kvs::Rectangle& rect, float rad_top_left, float rad_top_right, float rad_bottom_right, float rad_bottom_left );
    void ellipse( float cx, float cy, float rx, float ry );
    void ellipse( const kvs::Vec2& c, float rx, float ry );
    void circle( float cx, float cy, float r );
    void circle( const kvs::Vec2& c, float r );
    void fill();
    void stroke();

    // Transformation
    void resetTransform();
    void transform( float a, float b, float c, float d, float e, float f );
    void transform( const kvs::Mat2& r, const kvs::Vec2& t );
    void translate( float x, float y );
    void translate( const kvs::Vec2& t );
    void rotate( float angle );
    void skewX( float angle );
    void skewY( float angle );
    void scale( float x, float y );
    void scale( const kvs::Vec2& s );
};

} // end of namespace kvs
