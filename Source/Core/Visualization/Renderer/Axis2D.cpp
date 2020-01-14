#include "Axis2D.h"
#include <kvs/OpenGL>
#include <kvs/Camera>
#include <kvs/Light>
#include <kvs/ObjectBase>
#include <kvs/String>
#include <kvs/RGBColor>
#include <kvs/RGBAColor>
#include <kvs/TableObject>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new Axis2D class.
 */
/*===========================================================================*/
Axis2D::Axis2D():
    m_top_margin( 30 ),
    m_bottom_margin( 30 ),
    m_left_margin( 30 ),
    m_right_margin( 30 ),
    m_axis_width( 2.0f ),
    m_axis_color( 0, 0, 0 ),
    m_value_color( 0, 0, 0 ),
    m_label_color( 0, 0, 0 ),
    m_background_color( kvs::RGBAColor::White() ),
    m_x_label( "" ),
    m_y_label( "" )
{
}

/*===========================================================================*/
/**
 *  @brief  Render 2D axis.
 *  @param  object [in] pointer to object
 *  @param  camera [in] pointer to camera
 *  @param  light [in] pointer ot light
 */
/*===========================================================================*/
void Axis2D::exec( kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light )
{
    kvs::TableObject* table = kvs::TableObject::DownCast( object );

    BaseClass::startTimer();

    kvs::OpenGL::WithPushedAttrib attrib( GL_CURRENT_BIT | GL_ENABLE_BIT );
    m_painter.begin( screen() );
    {
        const float dpr = camera->devicePixelRatio();
        const int x0 = m_left_margin;
        const int x1 = camera->windowWidth() - m_right_margin;
        const int y0 = m_top_margin;
        const int y1 = camera->windowHeight() - m_bottom_margin;
        const kvs::FontMetrics metrics = m_painter.fontMetrics();

        // Draw background.
        if ( m_background_color.a() > 0.0f )
        {
            kvs::OpenGL::Begin( GL_QUADS );
            kvs::OpenGL::Color( m_background_color );
            kvs::OpenGL::Vertex( kvs::Vec2( x0, y0 ) * dpr );
            kvs::OpenGL::Vertex( kvs::Vec2( x1, y0 ) * dpr );
            kvs::OpenGL::Vertex( kvs::Vec2( x1, y1 ) * dpr );
            kvs::OpenGL::Vertex( kvs::Vec2( x0, y1 ) * dpr );
            kvs::OpenGL::End();
        }

        // Draw axes.
        const int d = int( m_axis_width * 0.5 );
        kvs::OpenGL::SetLineWidth( m_axis_width * dpr );
        kvs::OpenGL::Begin( GL_LINES );
        kvs::OpenGL::Color( m_axis_color );
        kvs::OpenGL::Vertices( kvs::Vec2( x0 - d, y1 ) * dpr, kvs::Vec2( x1 + d, y1 ) * dpr ); // X axis (bottom)
        kvs::OpenGL::Vertices( kvs::Vec2( x0, y1 + d ) * dpr, kvs::Vec2( x0, y0 - d ) * dpr ); // Y axis (left)
        kvs::OpenGL::Vertices( kvs::Vec2( x0 - d, y0 ) * dpr, kvs::Vec2( x1 + d, y0 ) * dpr ); // X axis (top)
        kvs::OpenGL::Vertices( kvs::Vec2( x1, y1 + d ) * dpr, kvs::Vec2( x1, y0 - d ) * dpr ); // Y axis (right)
        kvs::OpenGL::End();

        // Draw min/max values.
        const std::string x_min_value = kvs::String::ToString( table->minValue(0) );
        const std::string x_max_value = kvs::String::ToString( table->maxValue(0) );
        const kvs::Vec2 x_min_position( x0, y1 + metrics.height() + 5 );
        const kvs::Vec2 x_max_position( x1 - metrics.width( x_max_value ), y1 + metrics.height() + 5 );

        m_painter.drawText( x_min_position, x_min_value );
        m_painter.drawText( x_max_position, x_max_value );

        const std::string y_min_value = kvs::String::ToString( table->minValue(1) );
        const std::string y_max_value = kvs::String::ToString( table->maxValue(1) );
        const kvs::Vec2 y_min_position( x0 - metrics.width( y_min_value ) - 5, y1 );
        const kvs::Vec2 y_max_position( x0 - metrics.width( y_max_value ) - 5, y0 + metrics.height() );

        m_painter.drawText( y_min_position, y_min_value );
        m_painter.drawText( y_max_position, y_max_value );

        // Draw x label.
        const std::string x_label = m_x_label.empty() ? table->label(0) : m_x_label;
        if ( x_label.size() > 0 )
        {
            const float x_label_position_x = ( x0 + x1 - metrics.width( x_label ) ) * 0.5f;
            const float x_label_position_y = y1 + metrics.height() * 2.0f + 5.0f;
            m_painter.drawText( kvs::Vec2( x_label_position_x, x_label_position_y ), x_label );
        }

        // Draw y label.
        const std::string y_label = m_y_label.empty() ? table->label(1) : m_y_label;
        if ( y_label.size() > 0 )
        {
            const float y_value_position = kvs::Math::Min( y_min_position.x(), y_max_position.x() );
            const float y_label_position_x = y_value_position - 5.0f;
            const float y_label_position_y = ( y0 + y1 + metrics.width( y_label ) ) * 0.5f;
            kvs::OpenGL::PushMatrix();
            kvs::OpenGL::Translate( y_label_position_x * dpr, y_label_position_y * dpr, 0.0f );
            kvs::OpenGL::Rotate( -90.0, 0, 0, 1 );
            m_painter.drawText( kvs::Vec2( 0, 0 ), y_label );
            kvs::OpenGL::PopMatrix();
        }
    }
    m_painter.end();

    BaseClass::stopTimer();
}

} // end of namespace kvs
