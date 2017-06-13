#include "Axis2DMatrix.h"
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
 *  @brief  Constructs a new Axis2DMatrix class.
 */
/*===========================================================================*/
Axis2DMatrix::Axis2DMatrix():
    m_top_margin( 30 ),
    m_bottom_margin( 30 ),
    m_left_margin( 30 ),
    m_right_margin( 30 ),
    m_margin( 1 ),
    m_axis_width( 2.0f ),
    m_axis_color( 0, 0, 0 ),
    m_value_color( 0, 0, 0 ),
    m_label_color( 0, 0, 0 )
{
}

/*===========================================================================*/
/**
 *  @brief  Render 2D axis matrix.
 *  @param  object [in] pointer to object
 *  @param  camera [in] pointer to camera
 *  @param  light [in] pointer ot light
 */
/*===========================================================================*/
void Axis2DMatrix::exec( kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light )
{
    kvs::TableObject* table = kvs::TableObject::DownCast( object );

    BaseClass::startTimer();

    kvs::OpenGL::WithPushedAttrib attrib( GL_CURRENT_BIT | GL_ENABLE_BIT );
    m_painter.begin( screen() );
    {
        const int X0 = m_left_margin;
        const int X1 = camera->windowWidth() - m_right_margin;
        const int Y0 = m_top_margin;
        const int Y1 = camera->windowHeight() - m_bottom_margin;

        const int ncolumns = table->numberOfColumns();
        const float X_stride = float( X1 - X0 - m_margin * ( ncolumns - 1 ) ) / ncolumns;
        const float Y_stride = float( Y1 - Y0 - m_margin * ( ncolumns - 1 ) ) / ncolumns;

        for ( int i = 0; i < ncolumns; i++ )
        {
            for ( int j = 0; j < ncolumns; j++ )
            {
                const float x0 = X0 + ( X_stride + m_margin ) * j;
                const float y0 = Y0 + ( Y_stride + m_margin ) * i;
                const float x1 = x0 + X_stride;
                const float y1 = y0 + Y_stride;
                const int d0 = int( m_axis_width * 0.5 );
                const int d1 = int( m_axis_width * 0.5 - 0.5 );
                const kvs::FontMetrics metrics = m_painter.fontMetrics();

                // Draw axes.
                kvs::OpenGL::SetLineWidth( m_axis_width );
                kvs::OpenGL::Begin( GL_LINES );
                kvs::OpenGL::Color( m_axis_color );
                kvs::OpenGL::Vertices( kvs::Vec2( x0 - d0, y1 ), kvs::Vec2( x1, y1 ) ); // X axis
                kvs::OpenGL::Vertices( kvs::Vec2( x0, y1 + d1 ), kvs::Vec2( x0, y0 ) ); // Y axis
                kvs::OpenGL::End();

                // Draw min/max values.
                const size_t x_index = j;
                const size_t y_index = ncolumns - i - 1;

                if ( y_index == 0 )
                {
                    const std::string x_min_value = kvs::String::ToString( table->minValue( x_index ) );
                    const std::string x_max_value = kvs::String::ToString( table->maxValue( x_index ) );

                    const kvs::Vec2 x_min_position( x0 - metrics.width( x_min_value ) * 0.5, y1 + metrics.height() + 5 );
                    m_painter.font().setColor( m_value_color );
                    m_painter.drawText( x_min_position, x_min_value );

                    const kvs::Vec2 x_max_position( x1 - metrics.width( x_max_value ) * 0.5, y1 + metrics.height() + 5 );
                    m_painter.font().setColor( m_value_color );
                    m_painter.drawText( x_max_position, x_max_value );
                }

                if ( x_index == 0 )
                {
                    const std::string y_min_value = kvs::String::ToString( table->minValue( y_index ) );
                    const std::string y_max_value = kvs::String::ToString( table->maxValue( y_index ) );

                    const kvs::Vec2 y_min_position( x0 - metrics.width( y_min_value ) - 5, y1 + metrics.height() * 0.5 );
                    m_painter.font().setColor( m_value_color );
                    m_painter.drawText( y_min_position, y_min_value );

                    const kvs::Vec2 y_max_position( x0 - metrics.width( y_max_value ) - 5, y0 + metrics.height() * 0.5 );
                    m_painter.font().setColor( m_value_color );
                    m_painter.drawText( y_max_position, y_max_value );
                }

                // Draw labels.
                if ( x_index != y_index )
                {
                    const std::string x_label( table->label(x_index) );
                    const std::string y_label( table->label(y_index) );
                    const size_t x_label_width = metrics.width( x_label );

                    const kvs::Vec2 x_label_position( x1 - x_label_width - 5, y1 - 5 );
                    m_painter.font().setColor( m_label_color );
                    m_painter.drawText( x_label_position, x_label );

                    const kvs::Vec2 y_label_position( x0 + 5, y0 + metrics.width("x") + 5 );
                    m_painter.font().setColor( m_label_color );
                    m_painter.drawText( y_label_position, y_label );
                }
                else
                {
                    const std::string label( table->label(x_index) );
                    const size_t label_width = metrics.width( label );
                    const kvs::Vec2 label_position( ( x1 + x0 ) * 0.5f - label_width * 0.5f, ( y1 + y0 ) * 0.5f + metrics.height() * 0.5f );
                }
            }
        }
    }

    m_painter.end();

    BaseClass::stopTimer();
}

} // end of namespace kvs
