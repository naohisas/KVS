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
#include <kvs/glut/Rectangle>


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
class WidgetBase : public kvs::glut::Rectangle, public kvs::EventListener
{
protected:

    kvs::ScreenBase* m_screen; ///< pointer to the screen
    int m_margin; ///< margin
    bool m_is_shown; ///< check flag whether the widget is shown or not
    kvs::RGBColor m_text_color; ///< text color
    kvs::RGBAColor m_background_color; ///< background color
    kvs::RGBAColor m_background_border_color; ///< background border color
    float m_background_border_width; ///< background border width
    int m_character_width; ///< character width
    int m_character_height; ///< character height

public:

    WidgetBase( kvs::ScreenBase* screen );
    virtual ~WidgetBase();

public:

    int margin() const;
    bool isShown() const;
    const kvs::RGBColor& textColor() const;
    const kvs::RGBAColor& backgroundColor() const;
    const kvs::RGBAColor& backgroundBorderColor() const;
    float backgroundBorderWidth() const;
    int characterWidth() const;
    int characterHeight() const;

    void setMargin( const int margin );
    void setTextColor( const kvs::RGBColor& text_color );
    void setBackgroundColor( const kvs::RGBAColor& background_color );
    void setBackgroundOpacity( const float opacity );
    void setBackgroundBorderColor( const kvs::RGBAColor& border_color );
    void setBackgroundBorderOpacity( const float opacity );
    void setBackgroundBorderWidth( const float border_width );

    void show();
    void hide();

protected:

    kvs::ScreenBase* screen();

    virtual void begin_draw();
    virtual void end_draw();
    virtual void draw_background();
    virtual void draw_text( const int x, const int y, const std::string& text );
    virtual void swap_color( kvs::RGBColor& color1, kvs::RGBColor& color2 );
    virtual kvs::RGBColor get_darkened_color( const kvs::RGBColor& color, const float darkness );
    virtual int get_fitted_width();
    virtual int get_fitted_height();

private:

    WidgetBase(){}
};

} // end of namespace glut

} // end of namespace kvs

#endif // KVS__GLUT__WIDGET_BASE_H_INCLUDE
