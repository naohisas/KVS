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
 */
/*===========================================================================*/
void DateAxis::draw( kvs::Painter& painter )
{
    const int x0 = rect()[0];
    const int x1 = rect()[1];
    const int y0 = rect()[2];
    const int y1 = rect()[3];
    const kvs::FontMetrics& metrics = painter.fontMetrics();
    const size_t nticks = numberOfTicks();
    const int nyears = m_max_date.year() - m_min_date.year();
    const size_t nmonths = nyears * 12 + ( m_max_date.month() - m_min_date.month() ) + 1;

    switch ( align() )
    {
    case Top:
    {
        // Draw dates
        const double stride = double( x1 - x0 ) / ( nticks - 1 );
        kvs::Date date = m_min_date;
        for ( size_t i = 0; i < nticks; ++i )
        {
            const std::string d = date.monthString() + " " + kvs::String::ToString( date.year() );
            const int x = x0 + stride * i - metrics.width( d ) / 2;
            const int y = y0 - metrics.height() - labelOffset();
            painter.drawText( kvs::Vec2( x, y ), d );
            date.addMonths( ( nmonths / ( nticks - 1 ) ) );
        }

        this->drawLabelOnTop( painter, metrics.height() );
        break;
    }
    case Bottom:
    {
        // Draw dates
        const double stride = double( x1 - x0 ) / ( nticks - 1 );
        kvs::Date date = m_min_date;
        for ( size_t i = 0; i < nticks; ++i )
        {
            const std::string d = date.monthString() + " " + kvs::String::ToString( date.year() );
            const int x = x0 + stride * i - metrics.width( d ) / 2;
            const int y = y1 + metrics.height() + labelOffset();
            painter.drawText( kvs::Vec2( x, y ), d );
            date.addMonths( ( nmonths / ( nticks - 1 ) ) );
        }

        this->drawLabelOnBottom( painter, metrics.height() );
        break;
    }
    case Left:
    {
        // Draw dates
        const float stride = float( y1 - y0 ) / ( nticks - 1 );
        int margin = 0;
        kvs::Date date = m_min_date;
        for ( size_t i = 0; i < nticks; ++i )
        {
            const std::string d = date.monthString() + " " + kvs::String::ToString( date.year() );
            const int x = x0 - metrics.width( d ) - labelOffset();
            const int y = y1 - stride * i + metrics.height() / 2;
            painter.drawText( kvs::Vec2( x, y ), d );
            date.addMonths( ( nmonths / ( nticks - 1 ) ) );
            margin = kvs::Math::Max( margin, metrics.width( d ) );
        }

        this->drawLabelOnLeft( painter, margin );
        break;
    }
    case Right:
    {
        // Draw dates
        const float stride = float( y1 - y0 ) / ( nticks - 1 );
        int margin = 0;
        kvs::Date date = m_min_date;
        for ( size_t i = 0; i < nticks; ++i )
        {
            const std::string d = date.monthString() + " " + kvs::String::ToString( date.year() );
            const int x = x1 + labelOffset();
            const int y = y1 - stride * i + metrics.height() / 2;
            painter.drawText( kvs::Vec2( x, y ), d );
            date.addMonths( ( nmonths / ( nticks - 1 ) ) );
            margin = kvs::Math::Max( margin, metrics.width( d ) );
        }

        this->drawLabelOnRight( painter, margin );
        break;
    }
    default: break;
    }
}

} // end of namespace kvs
