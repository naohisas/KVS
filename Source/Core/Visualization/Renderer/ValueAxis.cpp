/*****************************************************************************/
/**
 *  @file   ValueAxis.cpp
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#include "ValueAxis.h"
#include <kvs/String>
#include <kvs/Vector2>
#include <kvs/FontMetrics>
#include <string>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Draw the axis with values and tick marks.
 *  @param  painter [in] painter
 */
/*===========================================================================*/
void ValueAxis::draw( kvs::Painter& painter )
{
    switch ( m_align )
    {
    case Top:
    {
        const int margin = this->drawTickLabelsOnTop( painter );
        this->drawLabelOnTop( painter, margin );
        this->drawTickMarksOnTop( painter );
        this->drawAxisOnTop( painter );
        break;
    }
    case Bottom:
    {
        const int margin = this->drawTickLabelsOnBottom( painter );
        this->drawLabelOnBottom( painter, margin );
        this->drawTickMarksOnBottom( painter );
        this->drawAxisOnBottom( painter );
        break;
    }
    case Left:
    {
        const int margin = this->drawTickLabelsOnLeft( painter );
        this->drawLabelOnLeft( painter, margin );
        this->drawTickMarksOnLeft( painter );
        this->drawAxisOnLeft( painter );
        break;
    }
    case Right:
    {
        const int margin = this->drawTickLabelsOnRight( painter );
        this->drawLabelOnRight( painter, margin );
        this->drawTickMarksOnRight( painter );
        this->drawAxisOnRight( painter );
        break;
    }
    default: break;
    }
}

/*===========================================================================*/
/**
 *  @brief  Draw the tick labels on the top axis.
 *  @param  painter [in] painter
 *  @return margin between the top axis and the axis label
 */
/*===========================================================================*/
int ValueAxis::drawTickLabelsOnTop( kvs::Painter& painter )
{
    if ( !m_tick_label_visible ) { return 0; } // invisible

    const float x0 = m_rect[0];
    const float x1 = m_rect[1];
    const float y0 = m_rect[2];

    // Format
    const int precision = ( m_precision == 0 ) ? 6 : m_precision;
    const bool fixed = ( m_precision == 0 ) ? false : true;
    const bool scientific = m_scientific;

    int margin = 0;
    const kvs::Font font = painter.font();
    painter.setFont( m_tick_label_font );
    {
        const kvs::FontMetrics& metrics = painter.fontMetrics();
        const float dx = ( x1 - x0 ) / ( m_nticks - 1 );
        for ( size_t i = 0; i < m_nticks; ++i )
        {
            const double value = m_min + i * ( m_max - m_min ) / ( m_nticks - 1 );
            const auto v = kvs::String::ToString( value, precision, fixed, scientific );
            const int x = x0 + dx * i - metrics.width( v ) / 2;
            const int y = y0 - metrics.height() - m_label_offset;
            painter.drawText( kvs::Vec2( x, y ), v );
        }
        margin = metrics.height();
    }
    painter.setFont( font );

    return margin;
}

/*===========================================================================*/
/**
 *  @brief  Draw the tick labels on the bottom axis.
 *  @param  painter [in] painter
 *  @return margin between the bottom axis and the axis label
 */
/*===========================================================================*/
int ValueAxis::drawTickLabelsOnBottom( kvs::Painter& painter )
{
    if ( !m_tick_label_visible ) { return 0; } // invisible

    const float x0 = m_rect[0];
    const float x1 = m_rect[1];
    const float y1 = m_rect[3];

    // Format
    const int precision = ( m_precision == 0 ) ? 6 : m_precision;
    const bool fixed = ( m_precision == 0 ) ? false : true;
    const bool scientific = m_scientific;

    int margin = 0;
    const kvs::Font font = painter.font();
    painter.setFont( m_tick_label_font );
    {
        const kvs::FontMetrics& metrics = painter.fontMetrics();
        const float dx = ( x1 - x0 ) / ( m_nticks - 1 );
        for ( size_t i = 0; i < m_nticks; ++i )
        {
            const double value = m_min + i * ( m_max - m_min ) / ( m_nticks - 1 );
            const auto v = kvs::String::ToString( value, precision, fixed, scientific );
            const int x = x0 + dx * i - metrics.width( v ) / 2;
            const int y = y1 + metrics.height() + m_label_offset;
            painter.drawText( kvs::Vec2( x, y ), v );
        }
        margin = metrics.height();
    }
    painter.setFont( font );

    return margin;
}

/*===========================================================================*/
/**
 *  @brief  Draw the tick labels on the left axis.
 *  @param  painter [in] painter
 *  @return margin between the left axis and the axis label
 */
/*===========================================================================*/
int ValueAxis::drawTickLabelsOnLeft( kvs::Painter& painter )
{
    if ( !m_tick_label_visible ) { return 0; } // invisible

    const float x0 = m_rect[0];
    const float y0 = m_rect[2];
    const float y1 = m_rect[3];
    const int offset = 3; // additional offset between the axis and tick label

    // Format
    const int precision = ( m_precision == 0 ) ? 6 : m_precision;
    const bool fixed = ( m_precision == 0 ) ? false : true;
    const bool scientific = m_scientific;

    int margin = 0;
    const kvs::Font font = painter.font();
    painter.setFont( m_tick_label_font );
    {
        const kvs::FontMetrics& metrics = painter.fontMetrics();
        const float dy = ( y1 - y0 ) / ( m_nticks - 1 );
        for ( size_t i = 0; i < m_nticks; ++i )
        {
            const double value = m_min + i * ( m_max - m_min ) / ( m_nticks - 1 );
            const auto v = kvs::String::ToString( value, precision, fixed, scientific );
            const int x = x0 - metrics.width( v ) - m_label_offset - offset;
            const int y = y1 - dy * i + metrics.height() / 2;
            painter.drawText( kvs::Vec2( x, y ), v );
            margin = kvs::Math::Max( margin, metrics.width( v ) );
        }
        margin += offset;
    }
    painter.setFont( font );

    return margin;
}

/*===========================================================================*/
/**
 *  @brief  Draw the tick labels on the right axis.
 *  @param  painter [in] painter
 *  @return margin between the right axis and the axis label
 */
/*===========================================================================*/
int ValueAxis::drawTickLabelsOnRight( kvs::Painter& painter )
{
    if ( !m_tick_label_visible ) { return 0; } // invisible

    const float x1 = m_rect[1];
    const float y0 = m_rect[2];
    const float y1 = m_rect[3];
    const int offset = 3; // additional offset between the axis and tick label

    // Format
    const int precision = ( m_precision == 0 ) ? 6 : m_precision;
    const bool fixed = ( m_precision == 0 ) ? false : true;
    const bool scientific = m_scientific;

    int margin = 0;
    const kvs::Font font = painter.font();
    painter.setFont( m_tick_label_font );
    {
        const kvs::FontMetrics& metrics = painter.fontMetrics();
        const float dy = ( y1 - y0 ) / ( m_nticks - 1 );
        for ( size_t i = 0; i < m_nticks; ++i )
        {
            const double value = m_min + i * ( m_max - m_min ) / ( m_nticks - 1 );
            const auto v = kvs::String::ToString( value, precision, fixed, scientific );
            const int x = x1 + m_label_offset + offset;
            const int y = y1 - dy * i + metrics.height() / 2;
            painter.drawText( kvs::Vec2( x, y ), v );
            margin = kvs::Math::Max( margin, metrics.width( v ) );
        }
        margin += offset;
    }
    painter.setFont( font );

    return margin;
}

/*===========================================================================*/
/**
 *  @brief  Draw the tick marks on the top axis.
 *  @param  painter [in] painter
 */
/*===========================================================================*/
void ValueAxis::drawTickMarksOnTop( kvs::Painter& painter )
{
    if ( !m_tick_mark_visible ) { return; } // invisible

    const float x0 = m_rect[0];
    const float x1 = m_rect[1];
    const float y0 = m_rect[2];
    const float dpr = painter.devicePixelRatio();

    float x = x0 * dpr;
    const float y = y0 * dpr;
    const float dy = m_tick_direction * m_tick_mark_length * dpr;
    const float dx = ( ( x1 - x0 ) / ( m_nticks - 1 ) ) * dpr;
    for ( size_t i = 0; i < m_nticks; ++i, x += dx )
    {
        kvs::OpenGL::SetLineWidth( m_tick_mark_width );
        kvs::OpenGL::Begin( GL_LINES );
        kvs::OpenGL::Color( m_color );
        kvs::OpenGL::Vertices( kvs::Vec2( x, y ), kvs::Vec2( x, y - dy ) );
        kvs::OpenGL::End();
    }
}

/*===========================================================================*/
/**
 *  @brief  Draw the tick marks on the bottom axis.
 *  @param  painter [in] painter
 */
/*===========================================================================*/
void ValueAxis::drawTickMarksOnBottom( kvs::Painter& painter )
{
    if ( !m_tick_mark_visible ) { return; } // invisible

    const float x0 = m_rect[0];
    const float x1 = m_rect[1];
    const float y1 = m_rect[3];
    const float dpr = painter.devicePixelRatio();

    float x = x0 * dpr;
    const float y = y1 * dpr;
    const float dy = m_tick_direction * m_tick_mark_length * dpr;
    const float dx = ( ( x1 - x0 ) / ( m_nticks - 1 ) ) * dpr;
    for ( size_t i = 0; i < m_nticks; ++i, x += dx )
    {
        kvs::OpenGL::SetLineWidth( m_tick_mark_width );
        kvs::OpenGL::Begin( GL_LINES );
        kvs::OpenGL::Color( m_color );
        kvs::OpenGL::Vertices( kvs::Vec2( x, y ), kvs::Vec2( x, y + dy ) );
        kvs::OpenGL::End();
    }
}

/*===========================================================================*/
/**
 *  @brief  Draw the tick marks on the left axis.
 *  @param  painter [in] painter
 */
/*===========================================================================*/
void ValueAxis::drawTickMarksOnLeft( kvs::Painter& painter )
{
    if ( !m_tick_mark_visible ) { return; } // invisible

    const float x0 = m_rect[0];
    const float y0 = m_rect[2];
    const float y1 = m_rect[3];
    const float dpr = painter.devicePixelRatio();

    float y = y1 * dpr;
    const float x = x0 * dpr;
    const float dx = m_tick_direction * m_tick_mark_length * dpr;
    const float dy = ( ( y1 - y0 ) / ( m_nticks - 1 ) ) * dpr;
    for ( size_t i = 0; i < m_nticks; ++i, y -= dy )
    {
        kvs::OpenGL::SetLineWidth( m_tick_mark_width );
        kvs::OpenGL::Begin( GL_LINES );
        kvs::OpenGL::Color( m_color );
        kvs::OpenGL::Vertices( kvs::Vec2( x, y ), kvs::Vec2( x - dx, y ) );
        kvs::OpenGL::End();
    }
}

/*===========================================================================*/
/**
 *  @brief  Draw the tick marks on the right axis.
 *  @param  painter [in] painter
 */
/*===========================================================================*/
void ValueAxis::drawTickMarksOnRight( kvs::Painter& painter )
{
    if ( !m_tick_mark_visible ) { return; } // invisible

    const float x1 = m_rect[1];
    const float y0 = m_rect[2];
    const float y1 = m_rect[3];
    const float dpr = painter.devicePixelRatio();

    float y = y1 * dpr;
    const float x = x1 * dpr;
    const float dx = m_tick_direction * m_tick_mark_length * dpr;
    const float dy = ( ( y1 - y0 ) / ( m_nticks - 1 ) ) * dpr;
    for ( size_t i = 0; i < m_nticks; ++i, y -= dy )
    {
        kvs::OpenGL::SetLineWidth( m_tick_mark_width );
        kvs::OpenGL::Begin( GL_LINES );
        kvs::OpenGL::Color( m_color );
        kvs::OpenGL::Vertices( kvs::Vec2( x, y ), kvs::Vec2( x + dx, y ) );
        kvs::OpenGL::End();
    }
}

/*===========================================================================*/
/**
 *  @brief  Draw the top axis.
 *  @param  painter [in] painter
 */
/*===========================================================================*/
void ValueAxis::drawAxisOnTop( kvs::Painter& painter )
{
    if ( !m_visible ) { return; } // invisible

    const float x0 = m_rect[0];
    const float x1 = m_rect[1];
    const float y0 = m_rect[2];
    const float dpr = painter.devicePixelRatio();
    const float d = m_width * 0.5f;
    kvs::OpenGL::SetLineWidth( m_width );
    kvs::OpenGL::Begin( GL_LINES );
    kvs::OpenGL::Color( m_color );
    kvs::OpenGL::Vertices( kvs::Vec2( x0 - d, y0 ) * dpr, kvs::Vec2( x1 + d, y0 ) * dpr );
    kvs::OpenGL::End();
}

/*===========================================================================*/
/**
 *  @brief  Draw the bottom axis.
 *  @param  painter [in] painter
 */
/*===========================================================================*/
void ValueAxis::drawAxisOnBottom( kvs::Painter& painter )
{
    if ( !m_visible ) { return; } // invisible

    const float x0 = m_rect[0];
    const float x1 = m_rect[1];
    const float y1 = m_rect[3];
    const float dpr = painter.devicePixelRatio();
    const float d = m_width * 0.5f;
    kvs::OpenGL::SetLineWidth( m_width );
    kvs::OpenGL::Begin( GL_LINES );
    kvs::OpenGL::Color( m_color );
    kvs::OpenGL::Vertices( kvs::Vec2( x0 - d, y1 ) * dpr, kvs::Vec2( x1 + d, y1 ) * dpr );
    kvs::OpenGL::End();
}

/*===========================================================================*/
/**
 *  @brief  Draw the left axis.
 *  @param  painter [in] painter
 */
/*===========================================================================*/
void ValueAxis::drawAxisOnLeft( kvs::Painter& painter )
{
    if ( !m_visible ) { return; } // invisible

    const float x0 = m_rect[0];
    const float y0 = m_rect[2];
    const float y1 = m_rect[3];
    const float dpr = painter.devicePixelRatio();
    const float d = m_width * 0.5f;
    kvs::OpenGL::SetLineWidth( m_width );
    kvs::OpenGL::Begin( GL_LINES );
    kvs::OpenGL::Color( m_color );
    kvs::OpenGL::Vertices( kvs::Vec2( x0, y1 + d ) * dpr, kvs::Vec2( x0, y0 - d ) * dpr );
    kvs::OpenGL::End();
}

/*===========================================================================*/
/**
 *  @brief  Draw the right axis.
 *  @param  painter [in] painter
 */
/*===========================================================================*/
void ValueAxis::drawAxisOnRight( kvs::Painter& painter )
{
    if ( !m_visible ) { return; } // invisible

    const float x1 = m_rect[1];
    const float y0 = m_rect[2];
    const float y1 = m_rect[3];
    const float dpr = painter.devicePixelRatio();
    const float d = m_width * 0.5f;
    kvs::OpenGL::SetLineWidth( m_width );
    kvs::OpenGL::Begin( GL_LINES );
    kvs::OpenGL::Color( m_color );
    kvs::OpenGL::Vertices( kvs::Vec2( x1, y1 + d ) * dpr, kvs::Vec2( x1, y0 - d ) * dpr );
    kvs::OpenGL::End();
}

/*===========================================================================*/
/**
 *  @brief  Draw the axis label on the top axis.
 *  @param  painter [in] painter
 *  @param  margin [in] margin between the top axis and the axis label
 */
/*===========================================================================*/
void ValueAxis::drawLabelOnTop( kvs::Painter& painter, const int margin )
{
    if ( !m_label_visible ) { return; } // invisible
    if ( m_label.size() == 0 ) { return; } // empty

    const float x0 = m_rect[0];
    const float x1 = m_rect[1];
    const float y0 = m_rect[2];

    const kvs::Font font = painter.font();
    painter.setFont( m_label_font );
    {
        const kvs::FontMetrics& metrics = painter.fontMetrics();
        const float px = ( x0 + x1 - metrics.width( m_label ) ) * 0.5f;
        const float py = y0 - m_label_offset - margin - metrics.height();
        painter.drawText( kvs::Vec2( px, py ), m_label );
        }
    painter.setFont( font );
}

/*===========================================================================*/
/**
 *  @brief  Draw the axis label on the bottom axis.
 *  @param  painter [in] painter
 *  @param  margin [in] margin between the bottom axis and the axis label
 */
/*===========================================================================*/
void ValueAxis::drawLabelOnBottom( kvs::Painter& painter, const int margin )
{
    if ( !m_label_visible ) { return; } // invisible
    if ( m_label.size() == 0 ) { return; } // empty

    const float x0 = m_rect[0];
    const float x1 = m_rect[1];
    const float y1 = m_rect[3];

    const kvs::Font font = painter.font();
    painter.setFont( m_label_font );
    {
        const kvs::FontMetrics& metrics = painter.fontMetrics();
        const float px = ( x0 + x1 - metrics.width( m_label ) ) * 0.5f;
        const float py = y1 + m_label_offset + margin + metrics.height();
        painter.drawText( kvs::Vec2( px, py ), m_label );
    }
    painter.setFont( font );
}

/*===========================================================================*/
/**
 *  @brief  Draw the axis label on the left axis.
 *  @param  painter [in] painter
 *  @param  margin [in] margin between the left axis and the axis label
 */
/*===========================================================================*/
void ValueAxis::drawLabelOnLeft( kvs::Painter& painter,  const int margin )
{
    if ( !m_label_visible ) { return; } // invisible
    if ( m_label.size() == 0 ) { return; } // empty

    const float x0 = m_rect[0];
    const float y0 = m_rect[2];
    const float y1 = m_rect[3];
    const float dpr = painter.devicePixelRatio();

    const kvs::Font font = painter.font();
    painter.setFont( m_label_font );
    {
        const kvs::FontMetrics& metrics = painter.fontMetrics();
        const float px = x0 - m_label_offset - margin - m_label_offset;
        const float py = ( y0 + y1 + metrics.width( m_label ) ) * 0.5f;
        kvs::OpenGL::PushMatrix();
        kvs::OpenGL::Translate( px * dpr, py * dpr, 0.0f );
        kvs::OpenGL::Rotate( -90.0, 0, 0, 1 );
        painter.drawText( kvs::Vec2( 0, 0 ), m_label );
        kvs::OpenGL::PopMatrix();
    }
    painter.setFont( font );
}

/*===========================================================================*/
/**
 *  @brief  Draw the axis label on the right axis.
 *  @param  painter [in] painter
 *  @param  margin [in] margin between the right axis and the axis label
 */
/*===========================================================================*/
void ValueAxis::drawLabelOnRight( kvs::Painter& painter, const int margin )
{
    if ( !m_label_visible ) { return; } // invisible
    if ( m_label.size() == 0 ) { return; } // empty

    const float x1 = m_rect[1];
    const float y0 = m_rect[2];
    const float y1 = m_rect[3];
    const float dpr = painter.devicePixelRatio();

    const kvs::Font font = painter.font();
    painter.setFont( m_label_font );
    {
        const kvs::FontMetrics& metrics = painter.fontMetrics();
        const float px = x1 + m_label_offset + margin + m_label_offset;
        const float py = ( y0 + y1 + metrics.width( m_label ) ) * 0.5f;
        kvs::OpenGL::PushMatrix();
        kvs::OpenGL::Translate( px * dpr, py * dpr, 0.0f );
        kvs::OpenGL::Rotate( -90.0, 0, 0, 1 );
        painter.drawText( kvs::Vec2( 0, 0 ), m_label );
        kvs::OpenGL::PopMatrix();
    }
    painter.setFont( font );
}

} // end of namespace kvs
