/*****************************************************************************/
/**
 *  @file   CheckBox.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#pragma once
#include <string>
#include <kvs/ScreenBase>
#include "WidgetBase.h"


namespace kvs
{

class CheckBoxGroup;

/*===========================================================================*/
/**
 *  @brief  Check box class.
 */
/*===========================================================================*/
class CheckBox : public kvs::WidgetBase
{
public:
    friend class kvs::CheckBoxGroup;
    using BaseClass = kvs::WidgetBase;
    using PressedFunc = std::function<void()>;
    using ReleasedFunc = std::function<void()>;
    using ScreenUpdatedFunc = std::function<void()>;
    using ScreenResizedFunc = std::function<void()>;
    using StateChangedFunc = std::function<void()>;

private:
    std::string m_caption; ///< caption
    bool m_state; ///< check state
    kvs::CheckBoxGroup* m_group; ///< pointer to the check box group
    PressedFunc m_pressed;
    ReleasedFunc m_released;
    ScreenUpdatedFunc m_screen_updated;
    ScreenResizedFunc m_screen_resized;
    StateChangedFunc m_state_changed;

public:
    CheckBox( kvs::ScreenBase* screen = 0 );

    void pressed( PressedFunc func ) { m_pressed = func; }
    void released( ReleasedFunc func ) { m_released = func; }
    void screenUpdated( ScreenUpdatedFunc func ) { m_screen_updated = func; }
    void screenResized( ScreenResizedFunc func ) { m_screen_resized = func; }
    void stateChanged( StateChangedFunc func ) { m_state_changed = func; }

    virtual void pressed() { if ( m_pressed ) m_pressed(); }
    virtual void released() { if ( m_released ) m_released(); }
    virtual void screenUpdated() { if ( m_screen_updated ) m_screen_updated(); }
    virtual void screenResized() { if ( m_screen_resized ) m_screen_resized(); };
    virtual void stateChanged() { if ( m_state_changed ) m_state_changed(); };

    const std::string& caption() const { return m_caption; }
    bool state() const { return m_state; }
    void setCaption( const std::string caption ) { m_caption = caption; }
    void setState( const bool state ) { m_state = state; }

protected:
    int adjustedWidth();
    int adjustedHeight();

private:
    void draw_box();
    void draw_mark();
    bool contains( int x, int y );
    void attach_group( kvs::CheckBoxGroup* group ) { m_group = group; }
    void detach_group() { m_group = NULL; }

private:
    void paintEvent();
    void resizeEvent( int width, int height );
    void mousePressEvent( kvs::MouseEvent* event );
    void mouseReleaseEvent( kvs::MouseEvent* event );
};

} // end of namespace kvs
