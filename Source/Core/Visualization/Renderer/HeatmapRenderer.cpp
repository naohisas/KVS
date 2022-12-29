/*****************************************************************************/
/**
 *  @file   HeatmapRenderer.cpp
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#include "HeatmapRenderer.h"
#include <kvs/IgnoreUnusedVariable>
#include <kvs/TableObject>
#include <algorithm>


namespace kvs
{

using ThisClass = HeatmapRenderer;

/*===========================================================================*/
/**
 *  @brief  Executes rendering process for table object.
 *  @param  object [in] pointer to the table object
 *  @param  camera [in] pointer to the camera
 *  @param  light  [in] pointer to the light
 */
/*===========================================================================*/
void ThisClass::exec( kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light )
{
    kvs::IgnoreUnusedVariable( light );
    auto* table = kvs::TableObject::DownCast( object );
    if ( !m_color_map.hasRange() ) { this->setupColorMapRange( table ); }

    BaseClass::startTimer();

    kvs::OpenGL::WithPushedAttrib attrib( GL_CURRENT_BIT | GL_ENABLE_BIT );
    m_painter.begin( screen() );
    {
        const auto dpr = camera->devicePixelRatio();
        const auto width = camera->windowWidth();
        const auto height = camera->windowHeight();
        const auto rect = m_margins.content( width, height );
        this->drawBackground( rect, dpr );
        this->drawMap( table, rect, dpr );
    }
    m_painter.end();

    BaseClass::stopTimer();
}

void ThisClass::setupColorMapRange( const kvs::TableObject* table )
{
    const auto& min_values = table->minValues();
    const auto& max_values = table->maxValues();
    const auto min_value = std::min_element( min_values.begin(), min_values.end() );
    const auto max_value = std::max_element( max_values.begin(), max_values.end() );
    m_color_map.setRange( *min_value, *max_value );
}

/*===========================================================================*/
/**
 *  @brief  
 *  @param  rect        
 *  @param  dpr 
 */
/*===========================================================================*/
void ThisClass::drawBackground( const kvs::Rectangle& rect, const float dpr )
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

void ThisClass::drawMap(
    const kvs::TableObject* table,
    const kvs::Rectangle& rect,
    const float dpr )
{
    auto* engine = m_painter.device()->renderEngine();
    engine->beginFrame( screen()->width(), screen()->height(), dpr );
    {
        const float x0 = rect.x0();
        const float x1 = rect.x1();
        const float y0 = rect.y0();
        const float y1 = rect.y1();

        const auto nrows = table->numberOfRows();
        const auto ncols = table->numberOfColumns();
        const float dx = ( x1 - x0 ) / ncols;
        const float dy = ( y1 - y0 ) / nrows;

        float x = x0;
        for ( size_t i = 0; i < ncols; ++i, x += dx )
        {
            float y = y0;
            const auto& column = table->column(i);
            for ( size_t j = 0; j < nrows; ++j, y += dy )
            {
                const auto v = column[j].to<kvs::Real32>();
                auto color = m_color_map.at( v );
                engine->beginPath();
                engine->setStrokeWidth( m_edge_width );
                engine->setStrokeColor( { m_edge_color, m_edge_opacity } );
                engine->setFillColor( color );
                engine->rect( x, y, dx, dy );
                engine->fill();
                engine->stroke();
            }
        }
    }
    engine->endFrame();
}

} // end of namespace local
