/*****************************************************************************/
/**
 *  @file   PushButton.h
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
#include <string>
#include <functional>
#include <kvs/RGBColor>
#include <kvs/ScreenBase>
#include "WidgetBase.h"


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  PushButton widget class.
 */
/*===========================================================================*/
class PushButton : public kvs::WidgetBase
{
public:
    using BaseClass = kvs::WidgetBase;
    using PressedFunc = std::function<void()>;
    using ReleasedFunc = std::function<void()>;
    using ScreenUpdatedFunc = std::function<void()>;
    using ScreenResizedFunc = std::function<void()>;

private:
    std::string m_caption; ///< caption
    int m_text_margin; ///< text margin
    bool m_pushed; ///< button status
    kvs::RGBColor m_button_color; ///< button color
    kvs::RGBColor m_grad_top_color;
    kvs::RGBColor m_grad_bottom_color;
    kvs::RGBColor m_border_light_color;
    kvs::RGBColor m_border_dark_color;
    PressedFunc m_pressed;
    ReleasedFunc m_released;
    ScreenUpdatedFunc m_screen_updated;
    ScreenResizedFunc m_screen_resized;

public:
    PushButton( kvs::ScreenBase* screen = 0 );

    const std::string& caption() const { return m_caption; }

    void setCaption( const std::string caption ) { m_caption = caption; }
    void setTextMargin( const int margin ) { m_text_margin = margin; }
    void setButtonColor( const kvs::RGBColor& color ) { m_button_color = color; }

    void pressed( PressedFunc func ) { m_pressed = func; }
    void released( ReleasedFunc func ) { m_released = func; }
    void screenUpdated( ScreenUpdatedFunc func ) { m_screen_updated = func; }
    void screenResized( ScreenResizedFunc func ) { m_screen_resized = func; }

    virtual void pressed() { if ( m_pressed ) m_pressed(); }
    virtual void released() { if ( m_released ) m_released(); }
    virtual void screenUpdated() { if ( m_screen_updated ) m_screen_updated(); }
    virtual void screenResized() { if ( m_screen_resized ) m_screen_resized(); }

protected:
    int adjustedWidth();
    int adjustedHeight();

private:
    void draw_button();
    int get_aligned_x();
    int get_aligned_y();

private:
    void paintEvent();
    void resizeEvent( int width, int height );
    void mousePressEvent( kvs::MouseEvent* event );
    void mouseReleaseEvent( kvs::MouseEvent* event );
};

} // end of namespace kvs
