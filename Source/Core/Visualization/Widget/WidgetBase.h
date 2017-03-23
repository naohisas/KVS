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
 *  $Id$
 */
/*****************************************************************************/
#pragma once
#include <kvs/ClassName>
#include <kvs/RGBAColor>
#include <kvs/EventListener>
#include <kvs/Rectangle>
#include <kvs/OpenGL>
#include <kvs/NanoVG>
#include "TextEngine.h"


namespace kvs
{

class ScreenBase;

/*===========================================================================*/
/**
 *  @brief  Base class of the widget.
 */
/*===========================================================================*/
class WidgetBase : public kvs::Rectangle, public kvs::EventListener
{
private:
    kvs::ScreenBase* m_screen; ///< pointer to the screen
    kvs::OpenGL::Render2D m_render_2d; ///< 2D rendering
    kvs::NanoVG* m_render_engine; ///< vector graphics engine
    kvs::TextEngine* m_text_engine; ///< pointer to the text engine

    int m_margin; ///< margin
    kvs::RGBAColor m_background_color; ///< background color
    kvs::RGBAColor m_background_border_color; ///< background border color
    float m_background_border_width; ///< background border width
    bool m_is_shown; ///< check flag whether the widget is shown or not
    bool m_is_active; ///< check flag for widget activation

public:
    WidgetBase( kvs::ScreenBase* screen );
    virtual ~WidgetBase();

public:
    int margin() const { return m_margin; }
    bool isShown() const { return m_is_shown; }
    const kvs::RGBAColor& backgroundColor() const { return m_background_color; }
    const kvs::RGBAColor& backgroundBorderColor() const { return m_background_border_color; }
    float backgroundBorderWidth() const { return m_background_border_width; }
    bool isActive() const { return m_is_active; }
    void activate() { m_is_active = true; }
    void deactivate() { m_is_active = false; }

    void setMargin( const int margin ) { m_margin = margin; }
    void setBackgroundColor( const kvs::RGBAColor& color ) { m_background_color = color; }
    void setBackgroundBorderColor( const kvs::RGBAColor& color ) { m_background_border_color = color; }
    void setBackgroundBorderWidth( const float width ) { m_background_border_width = width; }
    void setBackgroundOpacity( const float opacity );
    void setBackgroundBorderOpacity( const float opacity );

    template <typename Engine>
    void setTextEngine( const Engine& engine );
    const kvs::TextEngine& textEngine() const { return *m_text_engine; }

    void show();
    void hide();

protected:
    kvs::ScreenBase* screen() { return m_screen; }
    kvs::OpenGL::Render2D& render2D() { return m_render_2d; }
    kvs::NanoVG& renderEngine() { return *m_render_engine; }
    kvs::TextEngine& textEngine() { return *m_text_engine; }

    virtual void drawBackground();
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

template <typename Engine>
inline void WidgetBase::setTextEngine( const Engine& engine )
{
    if ( m_text_engine ) { delete m_text_engine; }
    m_text_engine = new Engine( engine );
}

} // end of namespace kvs
