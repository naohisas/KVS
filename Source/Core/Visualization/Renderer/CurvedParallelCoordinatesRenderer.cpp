/*****************************************************************************/
/**
 *  @file   CurvedParallelCoordinatesRenderer.cpp
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#include "CurvedParallelCoordinatesRenderer.h"
#include <kvs/OpenGL>
#include <kvs/Camera>
#include <kvs/Light>
#include <kvs/ObjectBase>
#include <kvs/TableObject>
#include <kvs/IgnoreUnusedVariable>
#include <kvs/Vector2>


namespace
{

inline kvs::Vec2 Curve(
    const float t,
    const kvs::Vec2& p0,
    const kvs::Vec2& p1,
    const kvs::Vec2& p2,
    const kvs::Vec2& p3 )
{
    // Catmull-Rom with fist 3 points
    const auto b = p3 - 3.0f * p2 + 3.0f * p1 - p0;
    const auto c = 3.0f * p2 - 6.0f * p1 + 3.0f * p0;
    const auto d = 3.0f * p1 - 3.0f * p0;
    const auto e = p0;
    return ( b * t * t * t ) + ( c * t * t ) + ( d * t ) + e;
}

} // end of namespace

namespace kvs
{

using ThisClass = CurvedParallelCoordinatesRenderer;

void ThisClass::exec( kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light )
{
    kvs::IgnoreUnusedVariable( light );

    const auto* table = kvs::TableObject::DownCast( object );

    BaseClass::startTimer();

    const float dpr = camera->devicePixelRatio();
    const int x0 = BaseClass::margins().left();
    const int x1 = camera->windowWidth() - BaseClass::margins().right();
    const int y0 = BaseClass::margins().top();
    const int y1 = camera->windowHeight() - BaseClass::margins().bottom();

    kvs::OpenGL::WithPushedAttrib attrib( GL_ALL_ATTRIB_BITS );
    kvs::OpenGL::Enable( GL_BLEND );
    kvs::OpenGL::SetBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

    BaseClass::updateColorMapRange( table );
    BaseClass::updateAntiAliasing();

    const size_t naxes = table->numberOfColumns();
    const float dx = float( x1 - x0 ) / ( naxes - 1 );

    auto y_normalize = [&] ( double v, double v_min, double v_max )
    {
        return y1 - ( y1 - y0 ) * ( v - v_min ) / ( v_max - v_min );
    };

    auto y_value = [&] ( const size_t axis, const size_t row )
    {
        const kvs::Real64 min_value = table->minValue( axis );
        const kvs::Real64 max_value = table->maxValue( axis );
        const kvs::Real64 value = table->column( axis )[row].to<kvs::Real64>();
        return y_normalize( value, min_value, max_value );
    };

    kvs::OpenGL::Render2D render( kvs::OpenGL::Viewport() );
    render.begin();
    {
        const auto& color_axis_values = table->column( BaseClass::activeAxis() );
        const size_t nrows = table->column(0).size();
        for ( size_t i = 0; i < nrows; i++ )
        {
            if ( !table->insideRange( i ) ) continue;

            kvs::OpenGL::SetLineWidth( BaseClass::lineWidth() * dpr );
            kvs::OpenGL::Begin( GL_LINE_STRIP );
            {
                const auto color_value = color_axis_values[i].to<kvs::Real64>();
                const auto color = BaseClass::colorMap().at( static_cast<float>( color_value ) );
                kvs::OpenGL::Color( kvs::RGBAColor( color, BaseClass::lineOpacity() ) );

                auto x = static_cast<kvs::Real64>( BaseClass::margins().left() );
                for ( size_t j = 0; j < naxes - 1; j++, x += dx )
                {
                    const auto p0 = kvs::Vec2( x, y_value( j, i ) );
                    const auto p1 = kvs::Vec2( x + dx, y_value( j + 1, i ) );

                    const auto c0 = kvs::Vec2( x + dx * 0.5f, p0.y() );
                    const auto c1 = kvs::Vec2( c0.x(), p1.y() );

                    const float step = 1.0f / m_nintervals;
                    for ( size_t k = 0; k < m_nintervals; ++k )
                    {
                        kvs::OpenGL::Vertex( ::Curve( k * step, p0, c0, c1, p1 ) * dpr );
                    }
                    kvs::OpenGL::Vertex( p1 * dpr );
                }
            }
            kvs::OpenGL::End();
        }
    }
    render.end();

    BaseClass::stopTimer();
}

} // end of namespace kvs
