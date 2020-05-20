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
 *  @param  offset [in] offset
 *  @return margin
 */
/*===========================================================================*/
int ValueAxis::draw( kvs::Painter& painter, const int offset )
{
    const int x0 = m_rect[0];
    const int x1 = m_rect[1];
    const int y0 = m_rect[2];
    const int y1 = m_rect[3];
    const kvs::FontMetrics& metrics = painter.fontMetrics();
    const size_t nticks = m_nticks;
    const int precision = ( m_precision == 0 ) ? 6 : m_precision;
    const bool fixed = ( m_precision == 0 ) ? false : true;
    const bool scientific = m_scientific;

    switch ( m_align )
    {
    case Top:
    {
        const double stride = double( x1 - x0 ) / ( nticks - 1 );
        for ( size_t i = 0; i < nticks; ++i )
        {
            const double value = m_min + i * ( m_max - m_min ) / ( nticks - 1 );
            const std::string v = kvs::String::ToString( value, precision, fixed, scientific );
            const int x = x0 + stride * i - metrics.width( v ) / 2;
            const int y = y0 - metrics.height() - offset;
            painter.drawText( kvs::Vec2( x, y ), v );
        }
        return metrics.height(); // height of value labels
    }
    case Bottom:
    {
        const double stride = double( x1 - x0 ) / ( nticks - 1 );
        for ( size_t i = 0; i < nticks; ++i )
        {
            const double value = m_min + i * ( m_max - m_min ) / ( nticks - 1 );
            const std::string v = kvs::String::ToString( value, precision, fixed, scientific );
            const int x = x0 + stride * i - metrics.width( v ) / 2;
            const int y = y1 + metrics.height() + offset;
            painter.drawText( kvs::Vec2( x, y ), v );
        }
        return metrics.height(); // height of value labels
    }
    case Left:
    {
        int margin = 0;
        const double stride = double( y1 - y0 ) / ( nticks - 1 );
        for ( size_t i = 0; i < nticks; ++i )
        {
            const double value = m_min + i * ( m_max - m_min ) / ( nticks - 1 );
            const std::string v = kvs::String::ToString( value, precision, fixed, scientific );
            const int x = x0 - metrics.width( v ) - offset;
            const int y = y1 - stride * i + metrics.height() / 2;
            painter.drawText( kvs::Vec2( x, y ), v );
            margin = kvs::Math::Max( margin, metrics.width( v ) );
        }
        return margin; // max width of value labels
    }
    case Right:
    {
        int margin = 0;
        const double stride = double( y1 - y0 ) / ( nticks - 1 );
        for ( size_t i = 0; i < nticks; ++i )
        {
            const double value = m_min + i * ( m_max - m_min ) / ( nticks - 1 );
            const std::string v = kvs::String::ToString( value, precision, fixed, scientific );
            const int x = x1 + offset;
            const int y = y1 - stride * i + metrics.height() / 2;
            painter.drawText( kvs::Vec2( x, y ), v );
            margin = kvs::Math::Max( margin, metrics.width( v ) );
        }
        return margin; // max width of value labels
    }
    default: break;
    }

    return 0;
}

} // end of namespace kvs
