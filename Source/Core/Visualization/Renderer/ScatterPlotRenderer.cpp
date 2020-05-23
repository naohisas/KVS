/*****************************************************************************/
/**
 *  @file   ScatterPlotRenderer.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: ScatterPlotRenderer.cpp 1797 2014-08-04 01:36:37Z naohisa.sakamoto@gmail.com $
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
#include <kvs/UIColor>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new ScatterPlotRenderer class.
 */
/*===========================================================================*/
ScatterPlotRenderer::ScatterPlotRenderer():
    m_top_margin( 30 ),
    m_bottom_margin( 30 ),
    m_left_margin( 30 ),
    m_right_margin( 30 ),
    m_point_color( kvs::UIColor::Blue() ),
    m_point_opacity( 1.0f ),
    m_point_size( 1.0f ),
    m_edge_color( kvs::UIColor::Label() ),
    m_edge_opacity( 1.0f ),
    m_edge_width( 1.0f ),
    m_polyline_color( kvs::UIColor::Label() ),
    m_polyline_opacity( 1.0f ),
    m_polyline_width( 1.0f ),
    m_polyline_visible( false ),
    m_background_color( kvs::UIColor::Gray5() ),
    m_background_visible( false ),
    m_color_map( 256 )
{
    m_color_map.create();
}

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
        const float x0 = m_left_margin;
        const float x1 = camera->windowWidth() - m_right_margin;
        const float y0 = m_top_margin;
        const float y1 = camera->windowHeight() - m_bottom_margin;
        const kvs::Vec4 rect( x0, x1, y0, y1 );

        // Draw background.
        this->draw_background( rect, dpr );

        kvs::NanoVG* engine = m_painter.device()->renderEngine();
        engine->beginFrame( screen()->width(), screen()->height(), camera->devicePixelRatio() );
        {
            this->draw_polyline( rect, table );
            this->draw_point( rect, table, has_values );
        }
        engine->endFrame();
    }
    m_painter.end();

    BaseClass::stopTimer();
}

void ScatterPlotRenderer::draw_background( const kvs::Vec4& rect, const float dpr )
{
    if ( !m_background_visible ) { return; } // invisible

    if ( m_background_color.a() > 0.0f )
    {
        kvs::OpenGL::WithPushedAttrib attrib( GL_CURRENT_BIT | GL_ENABLE_BIT );
        kvs::OpenGL::Enable( GL_BLEND );
        kvs::OpenGL::SetBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

        const float x0 = rect[0];
        const float x1 = rect[1];
        const float y0 = rect[2];
        const float y1 = rect[3];
        kvs::OpenGL::Begin( GL_QUADS );
        kvs::OpenGL::Color( m_background_color );
        kvs::OpenGL::Vertex( kvs::Vec2( x0, y0 ) * dpr );
        kvs::OpenGL::Vertex( kvs::Vec2( x1, y0 ) * dpr );
        kvs::OpenGL::Vertex( kvs::Vec2( x1, y1 ) * dpr );
        kvs::OpenGL::Vertex( kvs::Vec2( x0, y1 ) * dpr );
        kvs::OpenGL::End();
    }
}

void ScatterPlotRenderer::draw_polyline( const kvs::Vec4& rect, kvs::TableObject* table )
{
    if ( !m_polyline_visible ) { return; } // invisible

    const auto& x_values = table->column(0);
    const auto& y_values = table->column(1);
    const auto x_min_value = table->minValue(0);
    const auto x_max_value = table->maxValue(0);
    const auto y_min_value = table->minValue(1);
    const auto y_max_value = table->maxValue(1);

    const auto x0 = rect[0];
    const auto x1 = rect[1];
    const auto y0 = rect[2];
    const auto y1 = rect[3];
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

void ScatterPlotRenderer::draw_point(
    const kvs::Vec4& rect,
    kvs::TableObject* table,
    const bool has_values )
{
    const auto& x_values = table->column(0);
    const auto& y_values = table->column(1);
    const auto x_min_value = table->minValue(0);
    const auto x_max_value = table->maxValue(0);
    const auto y_min_value = table->minValue(1);
    const auto y_max_value = table->maxValue(1);

    const auto x0 = rect[0];
    const auto x1 = rect[1];
    const auto y0 = rect[2];
    const auto y1 = rect[3];
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
