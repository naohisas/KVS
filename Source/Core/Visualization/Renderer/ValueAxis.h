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

    enum TickDirection
    {
        Inside = -1,
        Outside = 1
    };

private:
    // Axis
    bool m_visible; ///< visibility of the axis
    Align m_align; ///< axis allign
    float m_width; ///< axis width
    kvs::RGBColor m_color; ///< axis and tick mark color

    // Label
    bool m_label_visible; ///< visibility of the label
    std::string m_label; ///< axis label
    kvs::Font m_label_font; ///< axis label font
    int m_label_offset; ///< offset between the axis label and tick label

    // Tick mark
    bool m_tick_mark_visible; ///< visibility of the tick marks
    float m_tick_mark_width; ///< tick mark width
    float m_tick_mark_length; ///< tick mark length

    // Tick label
    bool m_tick_label_visible; ///< visibility of the tick labels
    kvs::Font m_tick_label_font; ///< tick label font
    int m_tick_label_offset; ///< offset between the tick label and the axis
    TickDirection m_tick_direction; ///< tick mark direction
    size_t m_nticks; ///< number of ticks

    // Region
    kvs::Vec4 m_rect; ///< drawing region (x0, x1, y0, y1 )
    kvs::Real64 m_min; ///< min value
    kvs::Real64 m_max; ///< max value

    // Format
    int m_precision; ///< decimal precision
    bool m_scientific; ///< scientific notation

public:
    ValueAxis( Align align = Bottom ):
        m_visible( true ),
        m_align( align ),
        m_width( 2 ),
        m_color( kvs::RGBColor::Black() ),
        m_label_visible( true ),
        m_label( "" ),
        m_label_font(),
        m_label_offset( 5 ),
        m_tick_mark_visible( true ),
        m_tick_mark_width( 2 ),
        m_tick_mark_length( 5 ),
        m_tick_label_visible( true ),
        m_tick_label_font(),
        m_tick_label_offset( 5 ),
        m_tick_direction( Inside ),
        m_nticks( 5 ),
        m_rect( 0, 0, 0, 0 ),
        m_min( 0.0 ),
        m_max( 0.0 ),
        m_precision( 0 ),
        m_scientific( false ) {}

    ValueAxis( const ValueAxis& axis ):
        m_visible( axis.m_visible ),
        m_align( axis.m_align ),
        m_width( axis.m_width ),
        m_color( axis.m_color ),
        m_label_visible( axis.m_label_visible ),
        m_label( axis.m_label ),
        m_label_font( axis.m_label_font ),
        m_label_offset( axis.m_label_offset ),
        m_tick_mark_visible( axis.m_tick_mark_visible ),
        m_tick_mark_width( axis.m_tick_mark_width ),
        m_tick_mark_length( axis.m_tick_mark_length ),
        m_tick_label_visible( axis.m_tick_label_visible ),
        m_tick_label_font( axis.m_tick_label_font ),
        m_tick_label_offset( axis.m_tick_label_offset ),
        m_tick_direction( axis.m_tick_direction ),
        m_nticks( axis.m_nticks ),
        m_rect( axis.m_rect ),
        m_min( axis.m_min ),
        m_max( axis.m_max ),
        m_precision( axis.m_precision ),
        m_scientific( axis.m_scientific ) {}

    virtual ~ValueAxis() {}

    void setVisible( const bool visible = true ) { m_visible = visible; }
    void setAlign( const Align align ) { m_align = align; }
    void setAlignToBottom() { this->setAlign( Bottom ); }
    void setAlignToLeft() { this->setAlign( Left ); }
    void setWidth( const float width ) { m_width = width; }
    void setColor( const kvs::RGBColor& color ) { m_color = color; }
    void setLabelVisible( const bool visible = true ) { m_label_visible = visible; }
    void setLabel( const std::string& label ) { m_label = label; }
    void setLabelFont( const kvs::Font& font ) { m_label_font = font; }
    void setLabelOffset( const int offset ) { m_label_offset = offset; }
    void setTickMarkVisible( const bool visible = true ) { m_tick_mark_visible = visible; }
    void setTickMarkWidth( const float width ) { m_tick_mark_width = width; }
    void setTickMarkLength( const float length ) { m_tick_mark_length = length; }
    void setTickLabelVisible( const bool visible = true ) { m_tick_label_visible = visible; }
    void setTickLabelFont( const kvs::Font& font ) { m_tick_label_font = font; }
    void setTickLabelOffset( const int offset ) { m_tick_label_offset = offset; }
    void setTickDirection( const TickDirection dir ) { m_tick_direction = dir; }
    void setTickDirectionToInside() { this->setTickDirection( Inside ); }
    void setTickDirectionToOutside() { this->setTickDirection( Outside ); }
    void setNumberOfTicks( const size_t nticks ) { m_nticks = nticks; }
    void setRect( const kvs::Vec4& rect ) { m_rect = rect; }
    void setMin( const kvs::Real64 min ) { m_min = min; }
    void setMax( const kvs::Real64 max ) { m_max = max; }
    void setRange( const kvs::Real64 min, const kvs::Real64 max ) { this->setMin( min ); this->setMax( max ); }
    void setPrecision( const int p, const bool s = false ) { m_precision = p; m_scientific = s; }

    bool isVisible() const { return m_visible; }
    Align align() const { return m_align; }
    float width() const { return m_width; }
    const kvs::RGBColor& color() const { return m_color; }
    bool isLabelVisible() const { return m_label_visible; }
    const std::string& label() const { return m_label; }
    const kvs::Font& labelFont() const { return m_label_font; }
    int labelOffset() const { return m_label_offset; }
    bool isTickMarkVisible() const { return m_tick_mark_visible; }
    float tickMarkWidth() const { return m_tick_mark_width; }
    float tickMarkLength() const { return m_tick_mark_length; }
    bool isTickLabelVisible() const { return m_tick_label_visible; }
    const kvs::Font& tickLabelFont() const { return m_tick_label_font; }
    int tickLabelOffset() const { return m_tick_label_offset; }
    TickDirection tickDirection() const { return m_tick_direction; }
    size_t numberOfTicks() const { return m_nticks; }
    const kvs::Vec4& rect() const { return m_rect; }
    kvs::Real64 min() const { return m_min; }
    kvs::Real64 max() const { return m_max; }

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
