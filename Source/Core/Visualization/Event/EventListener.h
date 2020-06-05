/*****************************************************************************/
/**
 *  @file   EventListener.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#pragma once
#include <string>


namespace kvs
{

class Scene;
class ScreenBase;
class EventBase;
class MouseEvent;
class KeyEvent;
class WheelEvent;
class TimeEvent;

/*===========================================================================*/
/**
 *  @brief  Event listener class.
 */
/*===========================================================================*/
class EventListener
{
private:
    int m_event_type; ///< event type
    std::string m_name; ///< name
    kvs::ScreenBase* m_screen; ///< pointer to the screen
    kvs::Scene* m_scene; ///< pointer to the scene

public:
    EventListener();
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

    virtual void onEvent( kvs::EventBase* event );
    virtual void initializeEvent() {}
    virtual void paintEvent() {}
    virtual void resizeEvent( int, int ) {}
    virtual void mousePressEvent( kvs::MouseEvent* ) {}
    virtual void mouseMoveEvent( kvs::MouseEvent* ) {}
    virtual void mouseReleaseEvent( kvs::MouseEvent* ) {}
    virtual void mouseDoubleClickEvent( kvs::MouseEvent* ) {}
    virtual void wheelEvent( kvs::WheelEvent* ) {}
    virtual void keyPressEvent( kvs::KeyEvent* ) {}
    virtual void keyRepeatEvent( kvs::KeyEvent* ) {}
    virtual void keyReleaseEvent( kvs::KeyEvent* ) {}
    virtual void timerEvent( kvs::TimeEvent* ) {}
};

} // end of namespace kvs
