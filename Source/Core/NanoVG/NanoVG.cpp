/*****************************************************************************/
/**
 *  @file   NanoVG.cpp
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
#include "NanoVG.h"
#include <kvs/Message>
#include <kvs/Math>
#include <kvs/OpenGL>
#define NANOVG_GL2_IMPLEMENTATION
#include "nvg_gl.h"
#include "nvg_gl_utils.h"


namespace
{

inline NVGcolor ToNVGColor( const kvs::RGBAColor& color )
{
    const kvs::Vec4 c = color.toVec4();
    return nvgRGBAf( c[0], c[1], c[2], c[3] );
}

}


namespace kvs
{

NanoVG::Context::Context( int flags )
{
    m_context = nvgCreateGL2( flags );
    if ( !m_context )
    {
        kvsMessageError( "Cannot create a NanoVG context." );
        return;
    }
}

NanoVG::Context::~Context()
{
    if ( m_context )
    {
        nvgDeleteGL2( m_context );
    }
}

NanoVG::NanoVG( int flags ):
    m_context( flags )
{
}

NanoVG::~NanoVG()
{
}

void NanoVG::beginFrame( int width, int height, float pixel_ratio )
{
    nvgBeginFrame( m_context(), width, height, pixel_ratio );
}

void NanoVG::cancelFrame()
{
    nvgCancelFrame( m_context() );
}

void NanoVG::endFrame()
{
    nvgEndFrame( m_context() );
}

void NanoVG::save()
{
    nvgSave( m_context() );
}

void NanoVG::restore()
{
    nvgRestore( m_context() );
}

void NanoVG::reset()
{
    nvgReset( m_context() );
}

void NanoVG::setStrokeColor( NVGcolor color )
{
    nvgStrokeColor( m_context(), color );
}

void NanoVG::setStrokeColor( const kvs::RGBAColor& color )
{
    this->setStrokeColor( ::ToNVGColor( color ) );
}

void NanoVG::setStrokePaint( NVGpaint paint )
{
    nvgStrokePaint( m_context(), paint );
}

void NanoVG::setFillColor( NVGcolor color )
{
    nvgFillColor( m_context(), color );
}

void NanoVG::setFillColor( const kvs::RGBAColor& color )
{
    this->setFillColor( ::ToNVGColor( color ) );
}

void NanoVG::setFillPaint( NVGpaint paint )
{
    nvgFillPaint( m_context(), paint );
}

void NanoVG::setMiterLimit( float limit )
{
    nvgMiterLimit( m_context(), limit );
}

void NanoVG::setStrokeWidth( float width )
{
    nvgStrokeWidth( m_context(), width );
}

void NanoVG::setLineCap( int cap )
{
    nvgLineCap( m_context(), cap );
}

void NanoVG::setLineJoin( int join )
{
    nvgLineJoin( m_context(), join );
}

void NanoVG::setGlobalAlpha( float alpha )
{
    nvgGlobalAlpha( m_context(), alpha );
}

NVGpaint NanoVG::linearGradient( float sx, float sy, float ex, float ey, NVGcolor icol, NVGcolor ocol )
{
    return nvgLinearGradient( m_context(), sx, sy, ex, ey, icol, ocol );
}

NVGpaint NanoVG::linearGradient( const kvs::Vec2& s, const kvs::Vec2& e, const kvs::RGBAColor& icol, const kvs::RGBAColor& ocol )
{
    return this->linearGradient( s.x(), s.y(), e.x(), e.y(), ::ToNVGColor( icol ), ::ToNVGColor( ocol ) );
}

NVGpaint NanoVG::boxGradient( float x, float y, float w, float h, float r, float f, NVGcolor icol, NVGcolor ocol )
{
    return nvgBoxGradient( m_context(), x, y, w, h, r, f, icol, ocol );
}

NVGpaint NanoVG::boxGradient( const kvs::Vec4& rect, float r, float f, const kvs::RGBAColor& icol, const kvs::RGBAColor& ocol )
{
    return this->boxGradient( rect[0], rect[1], rect[2], rect[3], r, f, ::ToNVGColor( icol ), ::ToNVGColor( ocol ) );
}

NVGpaint NanoVG::boxGradient( const kvs::Rectangle& rect, float r, float f, const kvs::RGBAColor& icol, const kvs::RGBAColor& ocol )
{
    return this->boxGradient(
        static_cast<float>( rect.x() ),
        static_cast<float>( rect.y() ),
        static_cast<float>( rect.width() ),
        static_cast<float>( rect.height() ),
        r, f, ::ToNVGColor( icol ), ::ToNVGColor( ocol ) );
}

NVGpaint NanoVG::radialGradient( float cx, float cy, float inr, float outr, NVGcolor icol, NVGcolor ocol )
{
    return nvgRadialGradient( m_context(), cx, cy, inr, outr, icol, ocol );
}

NVGpaint NanoVG::radialGradient( const kvs::Vec2& c, float inr, float outr, const kvs::RGBAColor& icol, const kvs::RGBAColor& ocol )
{
    return this->radialGradient( c.x(), c.y(), inr, outr, ::ToNVGColor( icol ), ::ToNVGColor( ocol ) );
}

void NanoVG::scissor( float x, float y, float w, float h )
{
    nvgScissor( m_context(), x, y, w, h );
}

void NanoVG::intersectScissor( float x, float y, float w, float h )
{
    nvgIntersectScissor( m_context(), x, y, w, h );
}

void NanoVG::resetScissor()
{
    nvgResetScissor( m_context() );
}

void NanoVG::beginPath()
{
    nvgBeginPath( m_context() );
}

void NanoVG::moveTo( float x, float y )
{
    nvgMoveTo( m_context(), x, y );
}

void NanoVG::moveTo( const kvs::Vec2& p )
{
    this->moveTo( p.x(), p.y() );
}

void NanoVG::lineTo( float x, float y )
{
    nvgLineTo( m_context(), x, y );
}

void NanoVG::lineTo( const kvs::Vec2& p )
{
    this->lineTo( p.x(), p.y() );
}

void NanoVG::bezierTo( float c1x, float c1y, float c2x, float c2y, float x, float y )
{
    nvgBezierTo( m_context(), c1x, c1y, c2x, c2y, x, y );
}

void NanoVG::bezierTo( const kvs::Vec2& p1, const kvs::Vec2& p2, const kvs::Vec2& p3 )
{
    this->bezierTo( p1.x(), p1.y(), p2.x(), p2.y(), p3.x(), p3.y() );
}

void NanoVG::quadTo( float cx, float cy, float x, float y )
{
    nvgQuadTo( m_context(), cx, cy, x, y );
}

void NanoVG::quadTo( const kvs::Vec2& p1, const kvs::Vec2& p2 )
{
    this->quadTo( p1.x(), p1.y(), p2.x(), p2.y() );
}

void NanoVG::arcTo( float x1, float y1, float x2, float y2, float radius )
{
    nvgArcTo( m_context(), x1, y1, x2, y2, radius );
}

void NanoVG::arcTo( const kvs::Vec2& p1, const kvs::Vec2& p2, float radius )
{
    this->arcTo( p1.x(), p1.y(), p2.x(), p2.y(), radius );
}

void NanoVG::closePath()
{
    nvgClosePath( m_context() );
}

void NanoVG::pathWinding( int dir )
{
    nvgPathWinding( m_context(), dir );
}

void NanoVG::arc( float cx, float cy, float r, float a0, float a1, int dir )
{
    nvgArc( m_context(), cx, cy, r, a0, a1, dir );
}

void NanoVG::arc( const kvs::Vec2& c, float r, float a0, float a1, int dir )
{
    this->arc( c.x(), c.y(), r, a0, a1, dir );
}

void NanoVG::rect( float x, float y, float w, float h )
{
    nvgRect( m_context(), x, y, w, h );
}

void NanoVG::rect( const kvs::Vec4& rect )
{
    this->rect( rect[0], rect[1], rect[2], rect[3] );
}

void NanoVG::rect( const kvs::Rectangle& rect )
{
    this->rect(
        static_cast<float>( rect.x() ),
        static_cast<float>( rect.y() ),
        static_cast<float>( rect.width() ),
        static_cast<float>( rect.height() ) );
}

void NanoVG::roundedRect( float x, float y, float w, float h, float r )
{
    nvgRoundedRect( m_context(), x, y, w, h, r );
}

void NanoVG::roundedRect( const kvs::Vec4& rect, float r )
{
    this->roundedRect( rect[0], rect[1], rect[2], rect[3], r );
}

void NanoVG::roundedRect( const kvs::Rectangle& rect, float r )
{
    this->roundedRect(
        static_cast<float>( rect.x() ),
        static_cast<float>( rect.y() ),
        static_cast<float>( rect.width() ),
        static_cast<float>( rect.height() ),
        r );
}

void NanoVG::roundedRect( float x, float y, float w, float h, float rad_top_left, float rad_top_right, float rad_bottom_right, float rad_bottom_left )
{
    nvgRoundedRectVarying( m_context(), x, y, w, h, rad_top_left, rad_top_right, rad_bottom_right, rad_bottom_left );
}

void NanoVG::roundedRect( const kvs::Vec4& rect, float rad_top_left, float rad_top_right, float rad_bottom_right, float rad_bottom_left )
{
    this->roundedRect( rect[0], rect[1], rect[2], rect[3], rad_bottom_left, rad_bottom_right, rad_bottom_right, rad_bottom_left );
}

void NanoVG::roundedRect( const kvs::Rectangle& rect, float rad_top_left, float rad_top_right, float rad_bottom_right, float rad_bottom_left )
{
    this->roundedRect(
        static_cast<float>( rect.x() ),
        static_cast<float>( rect.y() ),
        static_cast<float>( rect.width() ),
        static_cast<float>( rect.height() ),
        rad_bottom_left, rad_bottom_right, rad_bottom_right, rad_bottom_left );
}

void NanoVG::ellipse( float cx, float cy, float rx, float ry )
{
    nvgEllipse( m_context(), cx, cy, rx, ry );
}

void NanoVG::ellipse( const kvs::Vec2& c, float rx, float ry )
{
    this->ellipse( c.x(), c.y(), rx, ry );
}

void NanoVG::circle( float cx, float cy, float r )
{
    nvgCircle( m_context(), cx, cy, r );
}

void NanoVG::circle( const kvs::Vec2& c, float r )
{
    this->circle( c.x(), c.y(), r );
}

void NanoVG::fill()
{
    nvgFill( m_context() );
}

void NanoVG::stroke()
{
    nvgStroke( m_context() );
}

void NanoVG::resetTransform()
{
    nvgResetTransform( m_context() );
}

void NanoVG::transform( float a, float b, float c, float d, float e, float f )
{
    nvgTransform( m_context(), a, b, c, d, e, f );
}

void NanoVG::transform( const kvs::Mat2& r, const kvs::Vec2& t )
{
    this->transform( r[0][0], r[1][0], r[0][1], r[1][1], t[0], t[1] );
}

void NanoVG::translate( float x, float y )
{
    nvgTranslate( m_context(), x, y );
}

void NanoVG::translate( const kvs::Vec2& t )
{
    this->translate( t[0], t[1] );
}

void NanoVG::rotate( float angle )
{
    nvgRotate( m_context(), angle );
}

void NanoVG::skewX( float angle )
{
    nvgSkewX( m_context(), angle );
}

void NanoVG::skewY( float angle )
{
    nvgSkewY( m_context(), angle );
}

void NanoVG::scale( float x, float y )
{
    nvgScale( m_context(), x, y );
}

void NanoVG::scale( const kvs::Vec2& s )
{
    this->scale( s[0], s[1] );
}

} // end of namespace kvs
