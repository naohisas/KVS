/*****************************************************************************/
/**
 *  @file   RadioButtonGroup.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#pragma once
#include <list>
#include <kvs/IgnoreUnusedVariable>
#include <kvs/ScreenBase>
#include "WidgetBase.h"
#include "RadioButton.h"


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Radio button group class.
 */
/*===========================================================================*/
class RadioButtonGroup : public kvs::WidgetBase
{
public:
    using BaseClass = kvs::WidgetBase;
    using PressedButtonFunc = std::function<void(kvs::RadioButton*)>;
    using PressedIdFunc = std::function<void(int)>;
    using ReleasedButtonFunc = std::function<void(kvs::RadioButton*)>;
    using ReleasedIdFunc = std::function<void(int)>;
    using ScreenUpdatedFunc = std::function<void()>;
    using ScreenResizedFunc = std::function<void()>;

private:
    std::list<kvs::RadioButton*> m_buttons; ///< radio button list
    PressedButtonFunc m_pressed_button;
    PressedIdFunc m_pressed_id;
    ReleasedButtonFunc m_released_button;
    ReleasedIdFunc m_released_id;
    ScreenUpdatedFunc m_screen_updated;
    ScreenResizedFunc m_screen_resized;

public:
    RadioButtonGroup( kvs::ScreenBase* screen = 0 );

    void pressed( PressedButtonFunc func ) { m_pressed_button = func; }
    void pressed( PressedIdFunc func ) { m_pressed_id = func; }
    void released( ReleasedButtonFunc func ) { m_released_button = func; }
    void released( ReleasedIdFunc func ) { m_released_id = func; }
    void screenUpdated( ScreenUpdatedFunc func ) { m_screen_updated = func; }
    void screenResized( ScreenResizedFunc func ) { m_screen_resized = func; }

    virtual void pressed( kvs::RadioButton* button ) { if ( m_pressed_button ) m_pressed_button( button ); };
    virtual void pressed( int id ) { if ( m_pressed_id ) m_pressed_id( id ); };
    virtual void released( kvs::RadioButton* button ) { if ( m_released_button ) m_released_button( button ); }
    virtual void released( int id ) { if ( m_released_id ) m_released_id( id ); };
    virtual void screenUpdated() { if ( m_screen_updated ) m_screen_updated(); }
    virtual void screenResized() { if ( m_screen_resized ) m_screen_resized(); }

    const std::list<kvs::RadioButton*>& radioButtons() const { return m_buttons; }
    void add( kvs::RadioButton* button );
    void remove( kvs::RadioButton* button );
    void show();
    void hide();

private:
    void paintEvent();
    void resizeEvent( int width, int height );
    void mousePressEvent( kvs::MouseEvent* event );
    void mouseReleaseEvent( kvs::MouseEvent* event );
};

} // end of namespace kvs
