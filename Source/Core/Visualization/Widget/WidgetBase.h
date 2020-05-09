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
#include <kvs/Painter>

/**/
#include <kvs/OpenGL>
#include <kvs/NanoVG>
#include "TextEngine.h"
/**/


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
    kvs::Painter m_painter; ///< painter

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
    const kvs::Font& font() const { return m_painter.font(); }
    int margin() const { return m_margin; }
    bool isShown() const { return m_is_shown; }
    const kvs::RGBAColor& backgroundColor() const { return m_background_color; }
    const kvs::RGBAColor& backgroundBorderColor() const { return m_background_border_color; }
    float backgroundBorderWidth() const { return m_background_border_width; }
    bool isActive() const { return m_is_active; }
    void activate() { m_is_active = true; }
    void deactivate() { m_is_active = false; }

    void setFont( const kvs::Font& font ) { m_painter.setFont( font ); }
    void setMargin( const int margin ) { m_margin = margin; }
    void setBackgroundColor( const kvs::RGBAColor& color ) { m_background_color = color; }
    void setBackgroundBorderColor( const kvs::RGBAColor& color ) { m_background_border_color = color; }
    void setBackgroundBorderWidth( const float width ) { m_background_border_width = width; }
    void setBackgroundOpacity( const float opacity );
    void setBackgroundBorderOpacity( const float opacity );

    void show();
    void hide();

protected:
    kvs::ScreenBase* screen() { return m_screen; }
    kvs::Painter& painter() { return m_painter; }
    virtual void drawBackground();
    virtual int adjustedWidth() { return 0; }
    virtual int adjustedHeight() { return 0; }
    virtual void initializeEvent();

private:
//    WidgetBase(){}
};

} // end of namespace kvs
