/*****************************************************************************/
/**
 *  @file   WidgetBase.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: WidgetBase.h 1719 2014-03-12 06:42:32Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#ifndef KVS__GLUT__WIDGET_BASE_H_INCLUDE
#define KVS__GLUT__WIDGET_BASE_H_INCLUDE

#include <kvs/ClassName>
#include <kvs/RGBAColor>
#include <kvs/EventListener>
#include <kvs/Rectangle>
#include <kvs/OpenGL>


namespace kvs
{

class ScreenBase;

namespace glut
{

/*===========================================================================*/
/**
 *  @brief  Base class of the widget.
 */
/*===========================================================================*/
class WidgetBase : public kvs::Rectangle, public kvs::EventListener
{
protected:

    kvs::ScreenBase* m_screen; ///< pointer to the screen
    kvs::OpenGL::Render2D m_render_2d; ///< 2D rendering
    int m_margin; ///< margin
    bool m_is_shown; ///< check flag whether the widget is shown or not
    kvs::RGBColor m_text_color; ///< text color
    kvs::RGBAColor m_background_color; ///< background color
    kvs::RGBAColor m_background_border_color; ///< background border color
    float m_background_border_width; ///< background border width
    int m_character_width; ///< character width
    int m_character_height; ///< character height
    bool m_is_active; ///< check flag for widget activation

public:

    WidgetBase( kvs::ScreenBase* screen );
    virtual ~WidgetBase();

public:

    int margin() const { return m_margin; }
    bool isShown() const { return m_is_shown; }
    const kvs::RGBColor& textColor() const { return m_text_color; }
    const kvs::RGBAColor& backgroundColor() const { return m_background_color; }
    const kvs::RGBAColor& backgroundBorderColor() const { return m_background_border_color; }
    float backgroundBorderWidth() const { return m_background_border_width; }
    int characterWidth() const { return m_character_width; }
    int characterHeight() const { return m_character_height; }
    bool isActive() const { return m_is_active; }
    void activate() { m_is_active = true; }
    void deactivate() { m_is_active = false; }

    void setMargin( const int margin ) { m_margin = margin; }
    void setTextColor( const kvs::RGBColor& color ) { m_text_color = color; }
    void setBackgroundColor( const kvs::RGBAColor& color ) { m_background_color = color; }
    void setBackgroundBorderColor( const kvs::RGBAColor& color ) { m_background_border_color = color; }
    void setBackgroundBorderWidth( const float width ) { m_background_border_width = width; }
    void setBackgroundOpacity( const float opacity );
    void setBackgroundBorderOpacity( const float opacity );

    void show();
    void hide();

protected:
    kvs::ScreenBase* screen() { return m_screen; }
    kvs::OpenGL::Render2D& render2D() { return m_render_2d; }

    virtual void drawBackground();
    virtual void drawText( const int x, const int y, const std::string& text );
    virtual void drawRect(
        const kvs::Rectangle& rect,
        const kvs::RGBColor& upper_color,
        const kvs::RGBColor& lower_color,
        const float width = 1.0f );

    virtual void swapColor( kvs::RGBColor& color1, kvs::RGBColor& color2 );
    virtual kvs::RGBColor darkenedColor( const kvs::RGBColor& color, const float darkness );
    virtual int adjustedWidth() { return 0; }
    virtual int adjustedHeight() { return 0; }

private:
    WidgetBase(){}
};

} // end of namespace glut

} // end of namespace kvs

#endif // KVS__GLUT__WIDGET_BASE_H_INCLUDE
