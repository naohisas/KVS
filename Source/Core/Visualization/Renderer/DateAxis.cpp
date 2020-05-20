#include "DateAxis.h"
#include <kvs/FontMetrics>
#include <kvs/Vector2>
#include <string>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Draw the axis with dates and tick marks.
 *  @param  painter [in] painter
 *  @param  offset [in] offset
 *  @return margin
 */
/*===========================================================================*/
int DateAxis::draw( kvs::Painter& painter, const int offset )
{
    const int x0 = rect()[0];
    const int x1 = rect()[1];
    const int y0 = rect()[2];
    const int y1 = rect()[3];
    const kvs::FontMetrics& metrics = painter.fontMetrics();
    const size_t nticks = numberOfTicks();
    const int nyears = m_max_date.year() - m_min_date.year();
    const size_t nmonths = nyears * 12 + ( m_max_date.month() - m_min_date.month() ) + 1;

    int margin = 0;
    switch ( align() )
    {
    case Top:
    {
        const double stride = double( x1 - x0 ) / ( nticks - 1 );
        kvs::Date date = m_min_date;
        for ( size_t i = 0; i < nticks; ++i )
        {
            const std::string d = date.monthString() + " " + kvs::String::ToString( date.year() );
            const int x = x0 + stride * i - metrics.width( d ) / 2;
            const int y = y0 - metrics.height() - offset;
            painter.drawText( kvs::Vec2( x, y ), d );
            date.addMonths( ( nmonths / ( nticks - 1 ) ) );
        }
        margin = metrics.height();
        break;
    }
    case Bottom:
    {
        const double stride = double( x1 - x0 ) / ( nticks - 1 );
        kvs::Date date = m_min_date;
        for ( size_t i = 0; i < nticks; ++i )
        {
            const std::string d = date.monthString() + " " + kvs::String::ToString( date.year() );
            const int x = x0 + stride * i - metrics.width( d ) / 2;
            const int y = y1 + metrics.height() + offset;
            painter.drawText( kvs::Vec2( x, y ), d );
            date.addMonths( ( nmonths / ( nticks - 1 ) ) );
        }
        margin = metrics.height();
        break;
    }
    case Left:
    {
        const float stride = float( y1 - y0 ) / ( nticks - 1 );
        kvs::Date date = m_min_date;
        for ( size_t i = 0; i < nticks; ++i )
        {
            const std::string d = date.monthString() + " " + kvs::String::ToString( date.year() );
            const int x = x0 - metrics.width( d ) - offset;
            const int y = y1 - stride * i + metrics.height() / 2;
            painter.drawText( kvs::Vec2( x, y ), d );
            date.addMonths( ( nmonths / ( nticks - 1 ) ) );
            margin = kvs::Math::Max( margin, metrics.width( d ) );
        }
        break;
    }
    case Right:
    {
        const float stride = float( y1 - y0 ) / ( nticks - 1 );
        kvs::Date date = m_min_date;
        for ( size_t i = 0; i < nticks; ++i )
        {
            const std::string d = date.monthString() + " " + kvs::String::ToString( date.year() );
            const int x = x1 + offset;
            const int y = y1 - stride * i + metrics.height() / 2;
            painter.drawText( kvs::Vec2( x, y ), d );
            date.addMonths( ( nmonths / ( nticks - 1 ) ) );
            margin = kvs::Math::Max( margin, metrics.width( d ) );
        }
        break;
    }
    default: break;
    }
    return margin;
}

} // end of namespace kvs
