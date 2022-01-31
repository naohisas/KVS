/*****************************************************************************/
/**
 *  @file   ParallelAxis.cpp
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#include "ParallelAxis.h"
#include <kvs/OpenGL>
#include <kvs/Camera>
#include <kvs/Light>
#include <kvs/ObjectBase>
#include <kvs/String>
#include <kvs/RGBColor>
#include <kvs/TableObject>
#include <kvs/Math>



namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Render parallel axis.
 *  @param  object [in] pointer to object
 *  @param  camera [in] pointer to camera
 *  @param  light [in] pointer ot light
 */
/*===========================================================================*/
void ParallelAxis::exec( kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light )
{
    auto* table = kvs::TableObject::DownCast( object );
    if ( !table ) { return; }

    const auto ncols = table->numberOfColumns();
    if ( ncols < 2 ) { return; }

    BaseClass::startTimer();

    kvs::OpenGL::WithPushedAttrib attrib( GL_CURRENT_BIT | GL_ENABLE_BIT );
    m_painter.begin( screen() );
    {
        const auto dpr = camera->devicePixelRatio();
        const auto width = camera->windowWidth();
        const auto height = camera->windowHeight();
        const auto content = m_margins.content( width, height );

        this->drawBackground( content, dpr );
        this->drawAxes( table, content, dpr );
        this->drawTitle( content );
    }
    m_painter.end();

    BaseClass::stopTimer();
}

void ParallelAxis::drawTitle( const kvs::Rectangle& content )
{
    if ( m_title.size() > 0 )
    {
        const float x0 = content.x0();
        const float x1 = content.x1();
        const float y0 = content.y0();
        const kvs::FontMetrics& metrics = m_painter.fontMetrics();
        const kvs::Font font = m_painter.font();
        {
            m_painter.setFont( m_title_font );
            const float px = ( x0 + x1 - metrics.width( m_title ) ) * 0.5f;
            const float py = y0 - m_title_offset;
            m_painter.drawText( kvs::Vec2( px, py ), m_title );
        }
        m_painter.setFont( font );
    }
}

/*===========================================================================*/
/**
 *  @brief  Draw background of the plot region.
 *  @param  content [in] plot region
 *  @param  dpr [in] device pixel ratio
 */
/*===========================================================================*/
void ParallelAxis::drawBackground( const kvs::Rectangle& content, const float dpr )
{
    if ( !m_background_visible ) { return; } // invisible

    if ( m_background_color.a() > 0.0f )
    {
        kvs::OpenGL::WithPushedAttrib attrib( GL_CURRENT_BIT | GL_ENABLE_BIT );
        kvs::OpenGL::Enable( GL_BLEND );
        kvs::OpenGL::SetBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

        const float x0 = content.x0();
        const float x1 = content.x1();
        const float y0 = content.y0();
        const float y1 = content.y1();
        kvs::OpenGL::Begin( GL_QUADS );
        kvs::OpenGL::Color( m_background_color );
        kvs::OpenGL::Vertex( kvs::Vec2( x0, y0 ) * dpr );
        kvs::OpenGL::Vertex( kvs::Vec2( x1, y0 ) * dpr );
        kvs::OpenGL::Vertex( kvs::Vec2( x1, y1 ) * dpr );
        kvs::OpenGL::Vertex( kvs::Vec2( x0, y1 ) * dpr );
        kvs::OpenGL::End();
    }
}

void ParallelAxis::drawAxes( const kvs::TableObject* table,  const kvs::Rectangle& content, const float dpr )
{
    // Draw axes.
    const auto naxes = table->numberOfColumns();
    const float stride = float( content.width() ) / ( naxes - 1 );
    if ( m_axis_visible )
    {
        kvs::OpenGL::SetLineWidth( m_axis_width * dpr );
        kvs::OpenGL::Begin( GL_LINES );
        for ( size_t i = 0; i < naxes; i++ )
        {
            const float x = content.x0() + stride * i;
            kvs::OpenGL::Color( m_axis_color );
            kvs::OpenGL::Vertices( kvs::Vec2( x, content.y0() ) * dpr, kvs::Vec2( x, content.y1() ) * dpr );
        }
        kvs::OpenGL::End();
    }

    bool has_label = false;
    for ( size_t i = 0; i < table->labels().size(); i++ )
    {
        if ( table->labels().at(i).size() > 0 ) { has_label = true; break; }
    }

    const kvs::FontMetrics metrics = m_painter.fontMetrics();

    // Draw min/max values and label.
    for ( size_t i = 0; i < naxes; i++ )
    {
        std::string max_value;
        std::string min_value;
        if ( std::ceil( table->maxValue(i) ) == std::floor( table->maxValue(i) ) )
        {
            max_value = kvs::String::From( int(table->maxValue(i)) );
            min_value = kvs::String::From( int(table->minValue(i)) );
        }
        else
        {
            max_value = kvs::String::From( table->maxValue(i) );
            min_value = kvs::String::From( table->minValue(i) );
        }

        const size_t max_width = metrics.width( max_value );
        const size_t min_width = metrics.width( min_value );
        const size_t height = metrics.height();

        const float max_x = ( content.x0() + stride * i ) - max_width * 0.5f;
        const float max_y = content.y0() - 5.0f;
        const float min_x = ( content.x0() + stride * i ) - min_width * 0.5f;
        const float min_y = content.y1() + height;

        if ( m_range_visible )
        {
            m_painter.font().setColor( m_value_color );
            m_painter.drawText( kvs::Vec2( kvs::Math::Max( 0.0f, max_x ), max_y ), max_value );
            m_painter.drawText( kvs::Vec2( kvs::Math::Max( 0.0f, min_x ), min_y ), min_value );
        }

        if ( has_label && m_label_visible )
        {
            const std::string label( table->label(i) );
            const size_t label_width = metrics.width( label );
            const float label_x = ( content.x0() + stride * i ) - label_width * 0.5f;
            const float label_y = min_y + height;
            m_painter.font().setColor( m_label_color );
            m_painter.drawText( kvs::Vec2( kvs::Math::Max( 0.0f, label_x ), label_y ), label );
        }
    }
}

} // end of namespace kvs
