/*****************************************************************************/
/**
 *  @file   DateAxis.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#pragma once
#include "ValueAxis.h"
#include <kvs/Date>
#include <kvs/Painter>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Date axis class.
 */
/*===========================================================================*/
class DateAxis : public kvs::ValueAxis
{
private:
    kvs::Date m_min_date; ///< start date
    kvs::Date m_max_date; ///< end date

public:
    DateAxis( Align align = Bottom ):
        kvs::ValueAxis( align ),
        m_min_date( 0 ),
        m_max_date( 0 ) {}

    DateAxis( const DateAxis& axis ):
        kvs::ValueAxis( axis ),
        m_min_date( axis.m_min_date ),
        m_max_date( axis.m_max_date ) {}

    virtual ~DateAxis() {}

    const kvs::Date& minDate() const { return m_min_date; }
    const kvs::Date& maxDate() const { return m_max_date; }
    void setMinDate( const kvs::Date& date ) { m_min_date = date; }
    void setMaxDate( const kvs::Date& date ) { m_max_date = date; }
    void setDateRange( const kvs::Date& min_date, const kvs::Date& max_date )
    {
        this->setMinDate( min_date );
        this->setMaxDate( max_date );
    }

    virtual void draw( kvs::Painter& painter );

protected:
    int drawTickLabelsOnTop( kvs::Painter& painter );
    int drawTickLabelsOnBottom( kvs::Painter& painter );
    int drawTickLabelsOnLeft( kvs::Painter& painter );
    int drawTickLabelsOnRight( kvs::Painter& painter );
};

} // end of namespace kvs
