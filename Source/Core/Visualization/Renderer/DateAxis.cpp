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
    switch ( ValueAxis::align() )
    {
    case Top:
    {
        const int margin = this->drawTickLabelsOnTop( painter );
        this->drawLabelOnTop( painter, margin );
        ValueAxis::drawTickMarksOnTop( painter );
        ValueAxis::drawAxisOnTop( painter );
        break;
    }
    case Bottom:
    {
        const int margin = this->drawTickLabelsOnBottom( painter );
        this->drawLabelOnBottom( painter, margin );
        ValueAxis::drawTickMarksOnTop( painter );
        ValueAxis::drawAxisOnTop( painter );
        break;
    }
    case Left:
    {
        const int margin = this->drawTickLabelsOnLeft( painter );
        this->drawLabelOnLeft( painter, margin );
        ValueAxis::drawTickMarksOnTop( painter );
        ValueAxis::drawAxisOnTop( painter );
        break;
    }
    case Right:
    {
        const int margin = this->drawTickLabelsOnRight( painter );
        this->drawLabelOnRight( painter, margin );
        ValueAxis::drawTickMarksOnTop( painter );
        ValueAxis::drawAxisOnTop( painter );
        break;
    }
    default: break;
    }
}

/*===========================================================================*/
/**
 *  @brief  Draw the date labels on the top axis.
 *  @param  painter [in] painter
 *  @return margin between the top axis and the axis label
 */
/*===========================================================================*/
int DateAxis::drawTickLabelsOnTop( kvs::Painter& painter )
{
    const float x0 = rect()[0];
    const float x1 = rect()[1];
    const float y0 = rect()[2];
    const kvs::FontMetrics& metrics = painter.fontMetrics();
    const size_t nticks = ValueAxis::numberOfTicks();
    const float stride = ( x1 - x0 ) / ( nticks - 1 );

    const int nyears = m_max_date.year() - m_min_date.year();
    const size_t nmonths = nyears * 12 + ( m_max_date.month() - m_min_date.month() ) + 1;

    kvs::Date date = m_min_date;
    for ( size_t i = 0; i < nticks; ++i )
    {
        const auto d = date.monthString() + " " + kvs::String::ToString( date.year() );
        const int x = x0 + stride * i - metrics.width( d ) / 2;
        const int y = y0 - metrics.height() - ValueAxis::labelOffset();
        painter.drawText( kvs::Vec2( x, y ), d );
        date.addMonths( ( nmonths / ( nticks - 1 ) ) );
    }

    return metrics.height();
}

/*===========================================================================*/
/**
 *  @brief  Draw the date labels on the bottom axis.
 *  @param  painter [in] painter
 *  @return margin between the bottom axis and the axis label
 */
/*===========================================================================*/
int DateAxis::drawTickLabelsOnBottom( kvs::Painter& painter )
{
    const float x0 = rect()[0];
    const float x1 = rect()[1];
    const float y1 = rect()[3];
    const kvs::FontMetrics& metrics = painter.fontMetrics();
    const size_t nticks = ValueAxis::numberOfTicks();
    const double stride = double( x1 - x0 ) / ( nticks - 1 );

    const int nyears = m_max_date.year() - m_min_date.year();
    const size_t nmonths = nyears * 12 + ( m_max_date.month() - m_min_date.month() ) + 1;

    kvs::Date date = m_min_date;
    for ( size_t i = 0; i < nticks; ++i )
    {
        const std::string d = date.monthString() + " " + kvs::String::ToString( date.year() );
        const int x = x0 + stride * i - metrics.width( d ) / 2;
        const int y = y1 + metrics.height() + ValueAxis::labelOffset();
        painter.drawText( kvs::Vec2( x, y ), d );
        date.addMonths( ( nmonths / ( nticks - 1 ) ) );
    }

    return metrics.height();
}

/*===========================================================================*/
/**
 *  @brief  Draw the date labels on the left axis.
 *  @param  painter [in] painter
 *  @return margin between the left axis and the axis label
 */
/*===========================================================================*/
int DateAxis::drawTickLabelsOnLeft( kvs::Painter& painter )
{
    const float x0 = rect()[0];
    const float y0 = rect()[2];
    const float y1 = rect()[3];
    const kvs::FontMetrics& metrics = painter.fontMetrics();
    const size_t nticks = ValueAxis::numberOfTicks();
    const float stride = ( y1 - y0 ) / ( nticks - 1 );
    const int offset = 3; // additional offset between the axis and tick label

    const int nyears = m_max_date.year() - m_min_date.year();
    const size_t nmonths = nyears * 12 + ( m_max_date.month() - m_min_date.month() ) + 1;

    int margin = 0;
    kvs::Date date = m_min_date;
    for ( size_t i = 0; i < nticks; ++i )
    {
        const std::string d = date.monthString() + " " + kvs::String::ToString( date.year() );
        const int x = x0 - metrics.width( d ) - ValueAxis::labelOffset() - offset;
        const int y = y1 - stride * i + metrics.height() / 2;
        painter.drawText( kvs::Vec2( x, y ), d );
        date.addMonths( ( nmonths / ( nticks - 1 ) ) );
        margin = kvs::Math::Max( margin, metrics.width( d ) );
    }

    return margin + offset;
}

/*===========================================================================*/
/**
 *  @brief  Draw the date labels on the right axis.
 *  @param  painter [in] painter
 *  @return margin between the right axis and the axis label
 */
/*===========================================================================*/
int DateAxis::drawTickLabelsOnRight( kvs::Painter& painter )
{
    const float x1 = rect()[1];
    const float y0 = rect()[2];
    const float y1 = rect()[3];
    const kvs::FontMetrics& metrics = painter.fontMetrics();
    const size_t nticks = ValueAxis::numberOfTicks();
    const float stride = ( y1 - y0 ) / ( nticks - 1 );
    const int offset = 3; // additional offset between the axis and tick label

    const int nyears = m_max_date.year() - m_min_date.year();
    const size_t nmonths = nyears * 12 + ( m_max_date.month() - m_min_date.month() ) + 1;

    int margin = 0;
    kvs::Date date = m_min_date;
    for ( size_t i = 0; i < nticks; ++i )
    {
        const std::string d = date.monthString() + " " + kvs::String::ToString( date.year() );
        const int x = x1 + ValueAxis::labelOffset() + offset;
        const int y = y1 - stride * i + metrics.height() / 2;
        painter.drawText( kvs::Vec2( x, y ), d );
        date.addMonths( ( nmonths / ( nticks - 1 ) ) );
        margin = kvs::Math::Max( margin, metrics.width( d ) );
    }

    return margin + offset;
}

} // end of namespace kvs
