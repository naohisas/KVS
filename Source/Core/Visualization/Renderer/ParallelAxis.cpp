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
 *  @brief  Constructs a new ParallelAxis class.
 */
/*===========================================================================*/
ParallelAxis::ParallelAxis():
    m_top_margin( 20 ),
    m_bottom_margin( 20 ),
    m_left_margin( 30 ),
    m_right_margin( 30 ),
    m_axis_width( 2.0f ),
    m_axis_color( 0, 0, 0 ),
    m_value_color( 0, 0, 0 ),
    m_label_color( 0, 0, 0 )
{
}

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
    kvs::TableObject* table = kvs::TableObject::DownCast( object );

    BaseClass::startTimer();

    kvs::OpenGL::WithPushedAttrib attrib( GL_CURRENT_BIT | GL_ENABLE_BIT );
    m_painter.begin( screen() );
    {
        bool has_label = false;
        for ( size_t i = 0; i < table->labels().size(); i++ )
        {
            if ( table->labels().at(i).size() > 0 ) { has_label = true; break; }
        }

        const float dpr = camera->devicePixelRatio();
        const int x0 = m_left_margin;
        const int x1 = camera->windowWidth() - m_right_margin;
        const int y0 = m_top_margin;
        const int y1 = camera->windowHeight() - m_bottom_margin;
        const kvs::FontMetrics metrics = m_painter.fontMetrics();

        // Draw axes.
        kvs::OpenGL::SetLineWidth( m_axis_width * dpr );
        kvs::OpenGL::Begin( GL_LINES );
        const size_t naxes = table->numberOfColumns();
        const float stride = float( x1 - x0 ) / ( naxes - 1 );
        for ( size_t i = 0; i < naxes; i++ )
        {
            const float x = m_left_margin + stride * i;
            kvs::OpenGL::Color( m_axis_color );
            kvs::OpenGL::Vertices( kvs::Vec2( x, y0 ) * dpr, kvs::Vec2( x, y1 ) * dpr );
        }
        kvs::OpenGL::End();

        // Draw min/max values and label.
        for ( size_t i = 0; i < naxes; i++ )
        {
            std::string max_value;
            std::string min_value;
            if ( std::ceil( table->maxValue(i) ) == std::floor( table->maxValue(i) ) )
            {
                max_value = kvs::String::ToString( int(table->maxValue(i)) );
                min_value = kvs::String::ToString( int(table->minValue(i)) );
            }
            else
            {
                max_value = kvs::String::ToString( table->maxValue(i) );
                min_value = kvs::String::ToString( table->minValue(i) );
            }

            const size_t max_width = metrics.width( max_value );
            const size_t min_width = metrics.width( min_value );
            const size_t height = metrics.height();

            const float max_x = ( m_left_margin + stride * i ) - max_width * 0.5f;
            const float max_y = y0 - 5.0f;
            const float min_x = ( m_left_margin + stride * i ) - min_width * 0.5f;
            const float min_y = y1 + height;

            m_painter.font().setColor( m_value_color );
            m_painter.drawText( kvs::Vec2( kvs::Math::Max( 0.0f, max_x ), max_y ), max_value );
            m_painter.drawText( kvs::Vec2( kvs::Math::Max( 0.0f, min_x ), min_y ), min_value );

            if ( has_label )
            {
                const std::string label( table->label(i) );
                const size_t label_width = metrics.width( label );
                const float label_x = ( m_left_margin + stride * i ) - label_width * 0.5f;
                const float label_y = min_y + height;
                m_painter.font().setColor( m_label_color );
                m_painter.drawText( kvs::Vec2( kvs::Math::Max( 0.0f, label_x ), label_y ), label );
            }
        }
    }
    m_painter.end();

    BaseClass::stopTimer();
}

} // end of namespace kvs
