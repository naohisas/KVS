/*****************************************************************************/
/**
 *  @file   CheckBoxGroup.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#pragma once
#include <list>
#include <kvs/IgnoreUnusedVariable>
#include <kvs/ScreenBase>
#include "WidgetBase.h"
#include "CheckBox.h"


namespace kvs
{

class CheckBox;

/*===========================================================================*/
/**
 *  @brief  Check box group class.
 */
/*===========================================================================*/
class CheckBoxGroup : public kvs::WidgetBase
{
public:
    using BaseClass = kvs::WidgetBase;
    using PressedBoxFunc = std::function<void(kvs::CheckBox*)>;
    using PressedIdFunc = std::function<void(int)>;
    using ReleasedBoxFunc = std::function<void(kvs::CheckBox*)>;
    using ReleasedIdFunc = std::function<void(int)>;
    using ScreenUpdatedFunc = std::function<void()>;
    using ScreenResizedFunc = std::function<void()>;

private:
    std::list<kvs::CheckBox*> m_boxes; ///< check box list
    PressedBoxFunc m_pressed_box;
    PressedIdFunc m_pressed_id;
    ReleasedBoxFunc m_released_box;
    ReleasedIdFunc m_released_id;
    ScreenUpdatedFunc m_screen_updated;
    ScreenResizedFunc m_screen_resized;

public:
    CheckBoxGroup( kvs::ScreenBase* screen = 0 );

    void pressed( PressedBoxFunc func ) { m_pressed_box = func; }
    void pressed( PressedIdFunc func ) { m_pressed_id = func; }
    void released( ReleasedBoxFunc func ) { m_released_box = func; }
    void released( ReleasedIdFunc func ) { m_released_id = func; }
    void screenUpdated( ScreenUpdatedFunc func ) { m_screen_updated = func; }
    void screenResized( ScreenResizedFunc func ) { m_screen_resized = func; }

    virtual void pressed( kvs::CheckBox* box ) { if ( m_pressed_box ) m_pressed_box( box ); };
    virtual void pressed( int id ) { if ( m_pressed_id ) m_pressed_id( id ); };
    virtual void released( kvs::CheckBox* box ) { if ( m_released_box ) m_released_box( box ); }
    virtual void released( int id ) { if ( m_released_id ) m_released_id( id ); };
    virtual void screenUpdated() { if ( m_screen_updated ) m_screen_updated(); }
    virtual void screenResized() { if ( m_screen_resized ) m_screen_resized(); }

    const std::list<kvs::CheckBox*>& checkBoxes() const { return m_boxes; }
    void add( kvs::CheckBox* box );
    void remove( kvs::CheckBox* box );
    void show();
    void hide();

private:
    void paintEvent();
    void resizeEvent( int width, int height );
    void mousePressEvent( kvs::MouseEvent* event );
    void mouseReleaseEvent( kvs::MouseEvent* event );
};

} // end of namespace kvs
