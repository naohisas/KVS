#pragma once
#include <string>
#include <kvs/WidgetBase>


namespace kvs
{

class ScreenBase;
class MouseEvent;

/*===========================================================================*/
/**
 *  @brief  Color palette class.
 */
/*===========================================================================*/
class ColorPalette : public kvs::WidgetBase
{
public:
    typedef kvs::WidgetBase BaseClass;

private:
    std::string m_caption; ///< caption
    kvs::WidgetBase m_SV_palette; ///< SV (saturation/value) palette
    kvs::WidgetBase m_H_palette; ///< H (hue) palette
    kvs::WidgetBase m_selected_color_box; ///< selected color box
    int m_H_indicator; ///< hue indicator
    int m_S_indicator; ///< saturation indicator
    int m_V_indicator; ///< value indicator

public:
    ColorPalette( kvs::ScreenBase* screen = 0 );
    virtual ~ColorPalette();

    virtual void screenUpdated() {};
    virtual void screenResized() {};

    const std::string& caption() const { return m_caption; }
    const kvs::RGBColor color() const;

    void setCaption( const std::string& caption ) { m_caption = caption; }

public:
    void paintEvent();
    void resizeEvent( int width, int height );
    void mousePressEvent( kvs::MouseEvent* event );
    void mouseMoveEvent( kvs::MouseEvent* event );
    void mouseReleaseEvent( kvs::MouseEvent* event );

protected:
    int adjustedWidth();
    int adjustedHeight();

private:
    void draw_SV_palette();
    void draw_H_palette();
    void draw_selected_color_box();
    void draw_selected_color_value();
    void draw_border( const kvs::Rectangle& rect );
    float get_H_value() const;
    float get_S_value() const;
    float get_V_value() const;
};

} // end of namespace kvs
