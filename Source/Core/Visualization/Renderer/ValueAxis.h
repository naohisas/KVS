/*****************************************************************************/
/**
 *  @file   ValueAxis.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#pragma once
#include <kvs/Painter>
#include <kvs/Type>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Value axis class.
 */
/*===========================================================================*/
class ValueAxis
{
public:
    enum Align
    {
        Top,
        Bottom,
        Left,
        Right
    };

private:
    size_t m_nticks; ///< number of ticks
    kvs::Vec4i m_rect; ///< drawing region (x0, x1, y0, y1 )
    Align m_align; ///< allignment
    kvs::Real64 m_min; ///< min value
    kvs::Real64 m_max; ///< max value
    int m_precision; ///< decimal precision
    bool m_scientific; ///< scientific notation

public:
    ValueAxis( Align align = Bottom ):
        m_nticks( 5 ),
        m_rect( 0, 0, 0, 0 ),
        m_align( align ),
        m_min( 0.0 ),
        m_max( 0.0 ),
        m_precision( 0 ),
        m_scientific( false ) {}

    ValueAxis( const ValueAxis& axis ):
        m_nticks( axis.m_nticks ),
        m_rect( axis.m_rect ),
        m_align( axis.m_align ),
        m_min( axis.m_min ),
        m_max( axis.m_max ),
        m_precision( axis.m_precision ),
        m_scientific( axis.m_scientific ) {}

    virtual ~ValueAxis() {}

    size_t numberOfTicks() const { return m_nticks; }
    const kvs::Vec4i rect() const { return m_rect; }
    Align align() const { return m_align; }
    kvs::Real64 min() const { return m_min; }
    kvs::Real64 max() const { return m_max; }

    void setNumberOfTicks( const size_t nticks ) { m_nticks = nticks; }
    void setRect( const kvs::Vec4i& rect ) { m_rect = rect; }
    void setAlign( const Align align ) { m_align = align; }
    void setAlignToBottom() { this->setAlign( Bottom ); }
    void setAlignToLeft() { this->setAlign( Left ); }
    void setMin( const kvs::Real64 min ) { m_min = min; }
    void setMax( const kvs::Real64 max ) { m_max = max; }

    void setRange( const kvs::Real64 min, const kvs::Real64 max )
    {
        this->setMin( min );
        this->setMax( max );
    }

    void setPrecision( const int precision, const bool scientific = false )
    {
        m_precision = precision;
        m_scientific = scientific;
    }

    virtual int draw( kvs::Painter& painter, const int offset = 5 );
};

} // end of namespace kvs
