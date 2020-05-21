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
    float m_width; ///< axis width
    kvs::RGBColor m_color; ///< axis color
    std::string m_label; ///< axis label
    int m_label_offset; ///< offset between the axis label and tick label
    float m_tick_mark_width; ///< tick mark width
    float m_tick_mark_length; ///< tick mark length
    int m_tick_label_offset; ///< offset between the tick label and the axis
    size_t m_nticks; ///< number of ticks
    kvs::Vec4i m_rect; ///< drawing region (x0, x1, y0, y1 )
    Align m_align; ///< allignment
    kvs::Real64 m_min; ///< min value
    kvs::Real64 m_max; ///< max value
    int m_precision; ///< decimal precision
    bool m_scientific; ///< scientific notation

public:
    ValueAxis( Align align = Bottom ):
        m_width( 4 ),
        m_color( kvs::RGBColor::Black() ),
        m_label( "" ),
        m_label_offset( 5 ),
        m_tick_mark_width( 4 ),
        m_tick_mark_length( 5 ),
        m_tick_label_offset( 5 ),
        m_nticks( 5 ),
        m_rect( 0, 0, 0, 0 ),
        m_align( align ),
        m_min( 0.0 ),
        m_max( 0.0 ),
        m_precision( 0 ),
        m_scientific( false ) {}

    ValueAxis( const ValueAxis& axis ):
        m_width( axis.m_width ),
        m_color( axis.m_color ),
        m_label( axis.m_label ),
        m_label_offset( axis.m_label_offset ),
        m_tick_mark_width( axis.m_tick_mark_width ),
        m_tick_mark_length( axis.m_tick_mark_length ),
        m_tick_label_offset( axis.m_tick_label_offset ),
        m_nticks( axis.m_nticks ),
        m_rect( axis.m_rect ),
        m_align( axis.m_align ),
        m_min( axis.m_min ),
        m_max( axis.m_max ),
        m_precision( axis.m_precision ),
        m_scientific( axis.m_scientific ) {}

    virtual ~ValueAxis() {}

    const kvs::RGBColor& color() const { return m_color; }
    float width() const { return m_width; }
    const std::string& label() const { return m_label; }
    int labelOffset() const { return m_label_offset; }
    float tickMarkWidth() const { return m_tick_mark_width; }
    float tickMarkLength() const { return m_tick_mark_length; }
    int tickMakrkOffset() const { return m_tick_label_offset; }
    size_t numberOfTicks() const { return m_nticks; }
    const kvs::Vec4i& rect() const { return m_rect; }
    Align align() const { return m_align; }
    kvs::Real64 min() const { return m_min; }
    kvs::Real64 max() const { return m_max; }

    void setLabel( const std::string& label ) { m_label = label; }
    void setLabelOffset( const int offset ) { m_label_offset = offset; }
    void setTickLabelOffset( const int offset ) { m_tick_label_offset = offset; }
    void setTickMarkWidth( const float width ) { m_tick_mark_width = width; }
    void setTickMarkLength( const float length ) { m_tick_mark_length = length; }
    void setTickMarkOffset( const int offset ) { m_tick_label_offset = offset; }
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

    virtual void draw( kvs::Painter& painter );

protected:
    int drawTickLabelsOnTop( kvs::Painter& painter );
    int drawTickLabelsOnBottom( kvs::Painter& painter );
    int drawTickLabelsOnLeft( kvs::Painter& painter );
    int drawTickLabelsOnRight( kvs::Painter& painter );

    void drawTickMarksOnTop( kvs::Painter& painter );
    void drawTickMarksOnBottom( kvs::Painter& painter );
    void drawTickMarksOnLeft( kvs::Painter& painter );
    void drawTickMarksOnRight( kvs::Painter& painter );

    void drawAxisOnTop( kvs::Painter& painter );
    void drawAxisOnBottom( kvs::Painter& painter );
    void drawAxisOnLeft( kvs::Painter& painter );
    void drawAxisOnRight( kvs::Painter& painter );

    void drawLabelOnTop( kvs::Painter& painter, const int margin );
    void drawLabelOnBottom( kvs::Painter& painter, const int margin );
    void drawLabelOnLeft( kvs::Painter& painter, const int margin );
    void drawLabelOnRight( kvs::Painter& painter, const int margin );
};

} // end of namespace kvs
