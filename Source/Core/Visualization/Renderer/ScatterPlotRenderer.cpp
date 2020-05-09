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
    m_has_point_color( false ),
    m_point_color( kvs::RGBColor( 0, 0, 0 ) ),
    m_point_opacity( 1.0f ),
    m_point_size( 1.0f ),
    m_edge_color( kvs::RGBColor( 0, 0, 0 ) ),
    m_edge_opacity( 1.0f ),
    m_edge_width( 1.0f ),
    m_polyline_color( kvs::RGBColor( 0, 0, 0 ) ),
    m_polyline_opacity( 1.0f ),
    m_polyline_width( 1.0f ),
    m_color_map( 256 ),
    m_background_color( 0, 0, 0, 0.0f ),
    m_enable_polyline( false )
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
    if ( !m_has_point_color ) if ( table->numberOfColumns() < 3 ) m_has_point_color = true;

    BaseClass::startTimer();

    kvs::OpenGL::WithPushedAttrib attrib( GL_CURRENT_BIT | GL_ENABLE_BIT );
    m_painter.begin( screen() );
    {
        const int x0 = m_left_margin;
        const int x1 = camera->windowWidth() - m_right_margin;
        const int y0 = m_top_margin;
        const int y1 = camera->windowHeight() - m_bottom_margin;

        // Draw background.
        if ( m_background_color.a() > 0.0f )
        {
            kvs::OpenGL::Begin( GL_QUADS );
            kvs::OpenGL::Color( m_background_color );
            kvs::OpenGL::Vertex( kvs::Vec2( x0, y0 ) );
            kvs::OpenGL::Vertex( kvs::Vec2( x1, y0 ) );
            kvs::OpenGL::Vertex( kvs::Vec2( x1, y1 ) );
            kvs::OpenGL::Vertex( kvs::Vec2( x0, y1 ) );
            kvs::OpenGL::End();
        }

        // X and Y values.
        const kvs::AnyValueArray& x_values = table->column(0);
        const kvs::AnyValueArray& y_values = table->column(1);
        const kvs::Real64 x_min_value = table->minValue(0);
        const kvs::Real64 x_max_value = table->maxValue(0);
        const kvs::Real64 y_min_value = table->minValue(1);
        const kvs::Real64 y_max_value = table->maxValue(1);

        kvs::NanoVG* engine = m_painter.device()->renderEngine();
        const kvs::Real32 opacity = m_point_opacity;
        const kvs::Real32 size = m_point_size;
        if ( m_has_point_color )
        {
            const kvs::RGBColor color = m_point_color;
            const kvs::Real64 x_ratio = kvs::Real64( x1 - x0 ) / ( x_max_value - x_min_value );
            const kvs::Real64 y_ratio = kvs::Real64( y1 - y0 ) / ( y_max_value - y_min_value );
            const size_t nrows = table->numberOfRows();

            engine->beginFrame( screen()->width(), screen()->height(), camera->devicePixelRatio() );

            if ( m_enable_polyline )
            {
                engine->beginPath();
                for ( size_t i = 0; i < nrows; i++ )
                {
                    if ( !table->insideRange( i ) ) continue;
                    const kvs::Real64 x_value = x_values[i].to<kvs::Real64>();
                    const kvs::Real64 y_value = y_values[i].to<kvs::Real64>();
                    const double x = x0 + ( x_value - x_min_value ) * x_ratio;
                    const double y = y1 - ( y_value - y_min_value ) * y_ratio;
                    engine->moveTo( x, y );
                    break;
                }

                for ( size_t i = 0; i < nrows; i++ )
                {
                    if ( !table->insideRange( i ) ) continue;
                    const kvs::Real64 x_value = x_values[i].to<kvs::Real64>();
                    const kvs::Real64 y_value = y_values[i].to<kvs::Real64>();
                    const double x = x0 + ( x_value - x_min_value ) * x_ratio;
                    const double y = y1 - ( y_value - y_min_value ) * y_ratio;
                    engine->lineTo( x, y );
                }

                engine->setLineCap( NVG_ROUND );
                engine->setLineJoin( NVG_ROUND );
                engine->setStrokeWidth( m_polyline_width );
                engine->setStrokeColor( kvs::RGBAColor( m_polyline_color, m_polyline_opacity ) );
                engine->stroke();
            }

            engine->setFillColor( kvs::RGBAColor( color, opacity ) );
            engine->setStrokeWidth( m_edge_width );
            engine->setStrokeColor( kvs::RGBAColor( m_edge_color, m_edge_opacity ) );
            for ( size_t i = 0; i < nrows; i++ )
            {
                if ( !table->insideRange( i ) ) continue;
                const kvs::Real64 x_value = x_values[i].to<kvs::Real64>();
                const kvs::Real64 y_value = y_values[i].to<kvs::Real64>();
                const double x = x0 + ( x_value - x_min_value ) * x_ratio;
                const double y = y1 - ( y_value - y_min_value ) * y_ratio;
                engine->beginPath();
                engine->circle( x, y, size );
                engine->fill();
                engine->stroke();
            }

            engine->endFrame();
        }
        else
        {
            const float color_axis_min_value = static_cast<float>( table->minValue(2) );
            const float color_axis_max_value = static_cast<float>( table->maxValue(2) );
            const kvs::AnyValueArray& color_axis_values = table->column(2);
            m_color_map.setRange( color_axis_min_value, color_axis_max_value );

            const kvs::Real64 x_ratio = kvs::Real64( x1 - x0 ) / ( x_max_value - x_min_value );
            const kvs::Real64 y_ratio = kvs::Real64( y1 - y0 ) / ( y_max_value - y_min_value );
            const size_t nrows = table->numberOfRows();

            engine->beginFrame( screen()->width(), screen()->height(), camera->devicePixelRatio() );

            if ( m_enable_polyline )
            {
                engine->beginPath();
                for ( size_t i = 0; i < nrows; i++ )
                {
                    if ( !table->insideRange( i ) ) continue;
                    const kvs::Real64 x_value = x_values[i].to<kvs::Real64>();
                    const kvs::Real64 y_value = y_values[i].to<kvs::Real64>();
                    const double x = x0 + ( x_value - x_min_value ) * x_ratio;
                    const double y = y1 - ( y_value - y_min_value ) * y_ratio;
                    engine->moveTo( x, y );
                    break;
                }

                for ( size_t i = 0; i < nrows; i++ )
                {
                    if ( !table->insideRange( i ) ) continue;
                    const kvs::Real64 x_value = x_values[i].to<kvs::Real64>();
                    const kvs::Real64 y_value = y_values[i].to<kvs::Real64>();
                    const double x = x0 + ( x_value - x_min_value ) * x_ratio;
                    const double y = y1 - ( y_value - y_min_value ) * y_ratio;
                    engine->lineTo( x, y );
                }

                engine->setLineCap( NVG_ROUND );
                engine->setLineJoin( NVG_ROUND );
                engine->setStrokeWidth( m_polyline_width );
                engine->setStrokeColor( kvs::RGBAColor( m_polyline_color, m_polyline_opacity ) );
                engine->stroke();
            }

            engine->setStrokeWidth( m_edge_width );
            engine->setStrokeColor( kvs::RGBAColor( m_edge_color, m_edge_opacity ) );
            engine->beginPath();
            for ( size_t i = 0; i < nrows; i++ )
            {
                if ( !table->insideRange( i ) ) continue;

                const kvs::Real64 color_value = color_axis_values[i].to<kvs::Real64>();
                const kvs::RGBColor color = m_color_map.at( static_cast<float>( color_value) );
                engine->setFillColor( kvs::RGBAColor( color, opacity ) );

                const kvs::Real64 x_value = x_values[i].to<kvs::Real64>();
                const kvs::Real64 y_value = y_values[i].to<kvs::Real64>();
                const double x = x0 + ( x_value - x_min_value ) * x_ratio;
                const double y = y1 - ( y_value - y_min_value ) * y_ratio;
                engine->circle( x, y, size );
                engine->fill();
                engine->stroke();
            }
            engine->endFrame();
        }

    }
    m_painter.end();

    BaseClass::stopTimer();
}

} // end of namespace kvs
