/*****************************************************************************/
/**
 *  @file   ScatterPlotRenderer.cpp
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#include "ScatterPlotRenderer.h"
#include <kvs/OpenGL>
#include <kvs/Camera>
#include <kvs/Light>
#include <kvs/ObjectBase>
#include <kvs/RGBAColor>
#include <kvs/TableObject>
#include <kvs/IgnoreUnusedVariable>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Render scatter plot.
 *  @param  object [in] pointer to object
 *  @param  camera [in] pointer to camera
 *  @param  light [in] pointer ot light
 */
/*===========================================================================*/
void ScatterPlotRenderer::exec( kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light )
{
    kvs::IgnoreUnusedVariable( light );

    kvs::TableObject* table = kvs::TableObject::DownCast( object );
    const bool has_values = ( table->numberOfColumns() < 3 ) ? false : true;

    BaseClass::startTimer();

    kvs::OpenGL::WithPushedAttrib attrib( GL_CURRENT_BIT | GL_ENABLE_BIT );
    m_painter.begin( screen() );
    {
        const float dpr = camera->devicePixelRatio();
        const int width = camera->windowWidth();
        const int height = camera->windowHeight();
        const kvs::Rectangle rect = m_margins.content( width, height );

        // Draw background.
        this->drawBackground( rect, dpr );

        kvs::NanoVG* engine = m_painter.device()->renderEngine();
        engine->beginFrame( screen()->width(), screen()->height(), dpr );
        {
            this->drawPolyline( rect, table, 0, 1 );
            this->drawPoint( rect, table, 0, 1, has_values );
        }
        engine->endFrame();
    }
    m_painter.end();

    BaseClass::stopTimer();
}

void ScatterPlotRenderer::drawBackground( const kvs::Rectangle& rect, const float dpr )
{
    if ( !m_background_visible ) { return; } // invisible

    if ( m_background_color.a() > 0.0f )
    {
        kvs::OpenGL::WithPushedAttrib attrib( GL_CURRENT_BIT | GL_ENABLE_BIT );
        kvs::OpenGL::Enable( GL_BLEND );
        kvs::OpenGL::SetBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

        const float x0 = rect.x0();
        const float x1 = rect.x1();
        const float y0 = rect.y0();
        const float y1 = rect.y1();
        kvs::OpenGL::Begin( GL_QUADS );
        kvs::OpenGL::Color( m_background_color );
        kvs::OpenGL::Vertex( kvs::Vec2( x0, y0 ) * dpr );
        kvs::OpenGL::Vertex( kvs::Vec2( x1, y0 ) * dpr );
        kvs::OpenGL::Vertex( kvs::Vec2( x1, y1 ) * dpr );
        kvs::OpenGL::Vertex( kvs::Vec2( x0, y1 ) * dpr );
        kvs::OpenGL::End();
    }
}

void ScatterPlotRenderer::drawPolyline(
    const kvs::Rectangle& rect,
    kvs::TableObject* table,
    const size_t x_index,
    const size_t y_index )
{
    if ( !m_polyline_visible ) { return; } // invisible

    const auto& x_values = table->column( x_index );
    const auto& y_values = table->column( y_index );
    const auto x_min_value = table->minValue( x_index );
    const auto x_max_value = table->maxValue( x_index );
    const auto y_min_value = table->minValue( y_index );
    const auto y_max_value = table->maxValue( y_index );

    const auto x0 = rect.x0();
    const auto x1 = rect.x1();
    const auto y0 = rect.y0();
    const auto y1 = rect.y1();
    const auto x_ratio = ( x1 - x0 ) / ( x_max_value - x_min_value );
    const auto y_ratio = ( y1 - y0 ) / ( y_max_value - y_min_value );
    const auto nrows = table->numberOfRows();

    auto* engine = m_painter.device()->renderEngine();
    engine->beginPath();

    // Set start point.
    for ( size_t i = 0; i < nrows; i++ )
    {
        if ( !table->insideRange( i ) ) continue;
        const auto x_value = x_values[i].to<kvs::Real64>();
        const auto y_value = y_values[i].to<kvs::Real64>();
        const auto x = x0 + ( x_value - x_min_value ) * x_ratio;
        const auto y = y1 - ( y_value - y_min_value ) * y_ratio;
        engine->moveTo( x, y );
        break;
    }

    // Set points on the polyline.
    for ( size_t i = 0; i < nrows; i++ )
    {
        if ( !table->insideRange( i ) ) continue;
        const auto x_value = x_values[i].to<kvs::Real64>();
        const auto y_value = y_values[i].to<kvs::Real64>();
        const auto x = x0 + ( x_value - x_min_value ) * x_ratio;
        const auto y = y1 - ( y_value - y_min_value ) * y_ratio;
        engine->lineTo( x, y );
    }

    engine->setLineCap( NVG_ROUND );
    engine->setLineJoin( NVG_ROUND );
    engine->setStrokeWidth( m_polyline_width );
    engine->setStrokeColor( kvs::RGBAColor( m_polyline_color, m_polyline_opacity ) );
    engine->stroke();
}

void ScatterPlotRenderer::drawPoint(
    const kvs::Rectangle& rect,
    kvs::TableObject* table,
    const size_t x_index,
    const size_t y_index,
    const bool has_values )
{
    const auto& x_values = table->column( x_index );
    const auto& y_values = table->column( y_index );
    const auto x_min_value = table->minValue( x_index );
    const auto x_max_value = table->maxValue( x_index );
    const auto y_min_value = table->minValue( y_index );
    const auto y_max_value = table->maxValue( y_index );

    const auto x0 = rect.x0();
    const auto x1 = rect.x1();
    const auto y0 = rect.y0();
    const auto y1 = rect.y1();
    const auto x_ratio = ( x1 - x0 ) / ( x_max_value - x_min_value );
    const auto y_ratio = ( y1 - y0 ) / ( y_max_value - y_min_value );
    const auto nrows = table->numberOfRows();

    auto* engine = m_painter.device()->renderEngine();
    const auto opacity = m_point_opacity;
    const auto size = m_point_size;
    if( has_values )
    {
        const auto color_axis_min_value = static_cast<float>( table->minValue(2) );
        const auto color_axis_max_value = static_cast<float>( table->maxValue(2) );
        const auto& color_axis_values = table->column(2);
        m_color_map.setRange( color_axis_min_value, color_axis_max_value );

        engine->setStrokeWidth( m_edge_width );
        engine->setStrokeColor( kvs::RGBAColor( m_edge_color, m_edge_opacity ) );
        for ( size_t i = 0; i < nrows; i++ )
        {
            if ( !table->insideRange( i ) ) continue;

            const auto color_value = color_axis_values[i].to<kvs::Real64>();
            const auto color = m_color_map.at( static_cast<float>( color_value) );
            engine->setFillColor( kvs::RGBAColor( color, opacity ) );

            const auto x_value = x_values[i].to<kvs::Real64>();
            const auto y_value = y_values[i].to<kvs::Real64>();
            const auto x = x0 + ( x_value - x_min_value ) * x_ratio;
            const auto y = y1 - ( y_value - y_min_value ) * y_ratio;
            engine->beginPath();
            engine->circle( x, y, size );
            engine->fill();
            engine->stroke();
        }
    }
    else
    {
        const auto color = m_point_color;
        engine->setFillColor( kvs::RGBAColor( color, opacity ) );
        engine->setStrokeWidth( m_edge_width );
        engine->setStrokeColor( kvs::RGBAColor( m_edge_color, m_edge_opacity ) );
        for ( size_t i = 0; i < nrows; i++ )
        {
            if ( !table->insideRange( i ) ) continue;
            const auto x_value = x_values[i].to<kvs::Real64>();
            const auto y_value = y_values[i].to<kvs::Real64>();
            const auto x = x0 + ( x_value - x_min_value ) * x_ratio;
            const auto y = y1 - ( y_value - y_min_value ) * y_ratio;
            engine->beginPath();
            engine->circle( x, y, size );
            engine->fill();
            engine->stroke();
        }
    }
}

} // end of namespace kvs
