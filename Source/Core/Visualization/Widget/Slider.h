/*****************************************************************************/
/**
 *  @file   Slider.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#pragma once
#include <kvs/RGBColor>
#include <kvs/ScreenBase>
#include "WidgetBase.h"


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Slider widget.
 */
/*===========================================================================*/
class Slider : public kvs::WidgetBase
{
public:
    using BaseClass = kvs::WidgetBase;
    using SliderPressedFunc = std::function<void()>;
    using SliderMovedFunc = std::function<void()>;
    using SliderReleasedFunc = std::function<void()>;
    using ValueChangedFunc = std::function<void()>;
    using ScreenUpdatedFunc = std::function<void()>;
    using ScreenResizedFunc = std::function<void()>;

private:
    std::string m_caption; ///< caption
    float m_value; ///< value
    float m_min_value; ///< min. value
    float m_max_value; ///< max. value
    bool m_change_value; ///< value change flag
    bool m_show_range_value; ///< range value showing flag
    bool m_pushed;
    kvs::RGBColor m_slider_color; ///< slider (cursor) color
    kvs::RGBColor m_cursor_color; ///< cursor color
    SliderPressedFunc m_slider_pressed;
    SliderMovedFunc m_slider_moved;
    SliderReleasedFunc m_slider_released;
    ValueChangedFunc m_value_changed;
    ScreenUpdatedFunc m_screen_updated;
    ScreenResizedFunc m_screen_resized;

public:
    Slider( kvs::ScreenBase* screen = 0 );

    void sliderPressed( SliderPressedFunc func ) { m_slider_pressed = func; }
    void sliderMoved( SliderMovedFunc func ) { m_slider_moved = func; }
    void sliderReleased( SliderReleasedFunc func ) { m_slider_released = func; }
    void valueChanged( ValueChangedFunc func ) { m_value_changed = func; }
    void screenUpdated( ScreenUpdatedFunc func ) { m_screen_updated = func; }
    void screenResized( ScreenResizedFunc func ) { m_screen_resized = func; }

    virtual void sliderPressed() { if ( m_slider_pressed ) m_slider_pressed(); }
    virtual void sliderMoved() { if ( m_slider_moved ) m_slider_moved(); }
    virtual void sliderReleased() { if ( m_slider_released ) m_slider_released(); }
    virtual void valueChanged() { if ( m_value_changed ) m_value_changed(); }
    virtual void screenUpdated() { if ( m_screen_updated ) m_screen_updated(); }
    virtual void screenResized() { if ( m_screen_resized ) m_screen_resized(); }

    const std::string& caption() const { return m_caption; }
    float value() const { return m_value; }
    float minValue() const { return m_min_value; }
    float maxValue() const { return m_max_value; }

    void setCaption( const std::string caption ) { m_caption = caption; }
    void setValue( const float value ) { m_value = value; }
    void setRange( const float min_value, const float max_value );
    void setSliderColor( const kvs::RGBColor& color );
    void setCursorColor( const kvs::RGBColor& color ) { m_cursor_color = color; }
    void showRange() { m_show_range_value = true; }
    void hideRange() { m_show_range_value = false; }

protected:
    int adjustedWidth();
    int adjustedHeight();

private:
    void draw_slider_bar( const int x, const int y, const int width );
    void draw_cursor( const int x, const int y, const int width );
    bool is_in_slider( const int x, const int y, const bool proper = false );
    bool is_in_cursor( const int x, const int y, const bool proper = false );
    float get_value( const int x );

private:
    void paintEvent();
    void resizeEvent( int width, int height );
    void mousePressEvent( kvs::MouseEvent* event );
    void mouseMoveEvent( kvs::MouseEvent* event );
    void mouseReleaseEvent( kvs::MouseEvent* event );
};

} // end of namespace kvs
