/*****************************************************************************/
/**
 *  @file   EventListener.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#pragma once
#include <functional>
#include <kvs/Scene>
#include <kvs/ScreenBase>
#include <kvs/EventBase>
#include <kvs/MouseEvent>
#include <kvs/KeyEvent>
#include <kvs/WheelEvent>
#include <kvs/TimeEvent>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Event listener class.
 */
/*===========================================================================*/
class EventListener
{
public:
    using InitializeEvent = std::function<void()>;
    using PaintEvent = std::function<void()>;
    using ResizeEvent = std::function<void(int,int)>;
    using MousePressEvent = std::function<void(kvs::MouseEvent*)>;
    using MouseMoveEvent = std::function<void(kvs::MouseEvent*)>;
    using MouseReleaseEvent = std::function<void(kvs::MouseEvent*)>;
    using MouseDoubleClickEvent = std::function<void(kvs::MouseEvent*)>;
    using WheelEvent = std::function<void(kvs::WheelEvent*)>;
    using KeyPressEvent = std::function<void(kvs::KeyEvent*)>;
    using KeyRepeatEvent = std::function<void(kvs::KeyEvent*)>;
    using KeyReleaseEvent = std::function<void(kvs::KeyEvent*)>;
    using TimerEvent = std::function<void(kvs::TimeEvent*)>;

private:
    int m_event_type; ///< event type
    std::string m_name; ///< name
    kvs::ScreenBase* m_screen; ///< pointer to the screen
    kvs::Scene* m_scene; ///< pointer to the scene
    InitializeEvent m_initialize_event;
    PaintEvent m_paint_event;
    ResizeEvent m_resize_event;
    MousePressEvent m_mouse_press_event;
    MouseMoveEvent m_mouse_move_event;
    MouseReleaseEvent m_mouse_release_event;
    MouseDoubleClickEvent m_mouse_double_click_event;
    WheelEvent m_wheel_event;
    KeyPressEvent m_key_press_event;
    KeyRepeatEvent m_key_repeat_event;
    KeyReleaseEvent m_key_release_event;
    TimerEvent m_timer_event;

public:
    EventListener( const int event_type = kvs::EventBase::AllEvents );
    virtual ~EventListener();

    int eventType() const { return m_event_type; }
    const std::string& name() const { return m_name; }
    kvs::ScreenBase* screen() { return m_screen; }
    kvs::Scene* scene() { return m_scene; }

    void setEventType( const int event_type ) { m_event_type = event_type; }
    void setName( const std::string& name ) { m_name = name; }
    void setScreen( kvs::ScreenBase* screen ) { m_screen = screen; }
    void setScene( kvs::Scene* scene ) { m_scene = scene; }
    void addEventType( const int event_type ) { m_event_type |= event_type; }

    void initializeEvent( InitializeEvent e ) { m_initialize_event = e; }
    void paintEvent( PaintEvent e ) { m_paint_event = e; }
    void resizeEvent( ResizeEvent e ) { m_resize_event = e; }
    void mousePressEvent( MousePressEvent e ) { m_mouse_press_event = e; }
    void mouseMoveEvent( MouseMoveEvent e ) { m_mouse_move_event = e; }
    void mouseReleaseEvent( MouseReleaseEvent e ) { m_mouse_release_event = e; }
    void mouseDoubleClickEvent( MouseDoubleClickEvent e ) { m_mouse_double_click_event = e; }
    void wheelEvent( WheelEvent e ) { m_wheel_event = e; }
    void keyPressEvent( KeyPressEvent e ) { m_key_press_event = e; }
    void keyRepeatEvent( KeyRepeatEvent e ) { m_key_repeat_event = e; }
    void keyReleaseEvent( KeyReleaseEvent e ) { m_key_release_event = e; }
    void timerEvent( TimerEvent e ) { m_timer_event = e; }

    virtual void onEvent( kvs::EventBase* event );
    virtual void initializeEvent() { if ( m_initialize_event ) m_initialize_event(); }
    virtual void paintEvent() { if ( m_paint_event ) m_paint_event(); }
    virtual void resizeEvent( int w, int h ) { if ( m_resize_event ) m_resize_event( w, h ); }
    virtual void mousePressEvent( kvs::MouseEvent* e ) { if ( m_mouse_press_event ) m_mouse_press_event( e ); }
    virtual void mouseMoveEvent( kvs::MouseEvent* e ) { if ( m_mouse_move_event ) m_mouse_move_event( e ); }
    virtual void mouseReleaseEvent( kvs::MouseEvent* e ) { if ( m_mouse_release_event ) m_mouse_release_event( e ); }
    virtual void mouseDoubleClickEvent( kvs::MouseEvent* e ) { if ( m_mouse_double_click_event ) m_mouse_double_click_event( e ); }
    virtual void wheelEvent( kvs::WheelEvent* e ) { if ( m_wheel_event ) m_wheel_event( e ); }
    virtual void keyPressEvent( kvs::KeyEvent* e ) { if ( m_key_press_event ) m_key_press_event( e ); }
    virtual void keyRepeatEvent( kvs::KeyEvent* e ) { if ( m_key_repeat_event ) m_key_repeat_event( e ); }
    virtual void keyReleaseEvent( kvs::KeyEvent* e ) { if ( m_key_release_event ) m_key_release_event( e ); }
    virtual void timerEvent( kvs::TimeEvent* e ) { if ( m_timer_event ) m_timer_event( e ); }
};

} // end of namespace kvs
