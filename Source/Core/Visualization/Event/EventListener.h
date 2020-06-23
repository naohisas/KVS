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
#include <kvs/EventTimer>


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
    using InitializeEventFunc = std::function<void()>;
    using PaintEventFunc = std::function<void()>;
    using ResizeEventFunc = std::function<void(int,int)>;
    using MousePressEventFunc = std::function<void(kvs::MouseEvent*)>;
    using MouseMoveEventFunc = std::function<void(kvs::MouseEvent*)>;
    using MouseReleaseEventFunc = std::function<void(kvs::MouseEvent*)>;
    using MouseDoubleClickEventFunc = std::function<void(kvs::MouseEvent*)>;
    using WheelEventFunc = std::function<void(kvs::WheelEvent*)>;
    using KeyPressEventFunc = std::function<void(kvs::KeyEvent*)>;
    using KeyRepeatEventFunc = std::function<void(kvs::KeyEvent*)>;
    using KeyReleaseEventFunc = std::function<void(kvs::KeyEvent*)>;
    using TimerEventFunc = std::function<void(kvs::TimeEvent*)>;

private:
    int m_event_type; ///< event type
    std::string m_name; ///< name
    kvs::ScreenBase* m_screen; ///< pointer to the screen (not allocated)
    kvs::Scene* m_scene; ///< pointer to the scene (not allocated)
    kvs::EventTimer* m_event_timer; ///< event timer
    int m_timer_interval; ///< interval time in millisecond

    // Event functions
    InitializeEventFunc m_initialize_event;
    PaintEventFunc m_paint_event;
    ResizeEventFunc m_resize_event;
    MousePressEventFunc m_mouse_press_event;
    MouseMoveEventFunc m_mouse_move_event;
    MouseReleaseEventFunc m_mouse_release_event;
    MouseDoubleClickEventFunc m_mouse_double_click_event;
    WheelEventFunc m_wheel_event;
    KeyPressEventFunc m_key_press_event;
    KeyRepeatEventFunc m_key_repeat_event;
    KeyReleaseEventFunc m_key_release_event;
    TimerEventFunc m_timer_event;

public:
    EventListener( const int event_type = kvs::EventBase::AllEvents );
    virtual ~EventListener() {}

    int eventType() const { return m_event_type; }
    const std::string& name() const { return m_name; }
    kvs::ScreenBase* screen() { return m_screen; }
    kvs::Scene* scene() { return m_scene; }
    kvs::EventTimer* eventTimer() { return m_event_timer; }
    int timerInterval() const { return m_timer_interval; }
    void setEventType( const int event_type ) { m_event_type = event_type; }
    void setName( const std::string& name ) { m_name = name; }
    void setScreen( kvs::ScreenBase* screen ) { m_screen = screen; }
    void setScene( kvs::Scene* scene ) { m_scene = scene; }
    void setEventTimer( kvs::EventTimer* timer ) { m_event_timer = timer; }
    void setTimerInterval( int msec ) { m_timer_interval = msec; }
    void addEventType( const int event_type ) { m_event_type |= event_type; }

    void initializeEvent( InitializeEventFunc func ) { m_initialize_event = func; }
    void paintEvent( PaintEventFunc func ) { m_paint_event = func; }
    void resizeEvent( ResizeEventFunc func ) { m_resize_event = func; }
    void mousePressEvent( MousePressEventFunc func ) { m_mouse_press_event = func; }
    void mouseMoveEvent( MouseMoveEventFunc func ) { m_mouse_move_event = func; }
    void mouseReleaseEvent( MouseReleaseEventFunc func ) { m_mouse_release_event = func; }
    void mouseDoubleClickEvent( MouseDoubleClickEventFunc func ) { m_mouse_double_click_event = func; }
    void wheelEvent( WheelEventFunc func ) { m_wheel_event = func; }
    void keyPressEvent( KeyPressEventFunc func ) { m_key_press_event = func; }
    void keyRepeatEvent( KeyRepeatEventFunc func ) { m_key_repeat_event = func; }
    void keyReleaseEvent( KeyReleaseEventFunc func ) { m_key_release_event = func; }
    void timerEvent( TimerEventFunc func, int msec = -1 ) { m_timer_event = func; m_timer_interval = msec; }

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
