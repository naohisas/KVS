/*****************************************************************************/
/**
 *  @file   ScreenBase.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#pragma once
#include <kvs/ScreenBase>
#include <kvs/Timer>
#include <kvs/MouseEvent>
#include <kvs/WheelEvent>
#include <kvs/KeyEvent>
#include "../GLFW.h"


namespace kvs
{

namespace glfw
{

class Application;

/*===========================================================================*/
/**
 *  @brief  Base class of screen based on GLFW.
 */
/*===========================================================================*/
class ScreenBase : public kvs::ScreenBase
{
    friend class kvs::glfw::Application;
    using BaseClass = kvs::ScreenBase;

public:
    static ScreenBase* DownCast( kvs::ScreenBase* screen );
    static const ScreenBase* DownCast( const kvs::ScreenBase* screen );

private:
    GLFWwindow* m_handler = nullptr; ///< glfw window handler
    int m_id = -1; ///< window ID
    kvs::MouseEvent* m_mouse_event = nullptr; ///< mouse event
    kvs::KeyEvent* m_key_event = nullptr; ///< key event
    kvs::WheelEvent* m_wheel_event = nullptr; ///< wheel event
    kvs::Timer m_elapse_time_counter{}; ///< elapse time counter for double click event

public:
    ScreenBase( kvs::glfw::Application* application );
    virtual ~ScreenBase();

    GLFWwindow* handler() { return m_handler; }
    int id() const { return m_id; }

    virtual void setEvent( kvs::EventListener* event, const std::string& name = "" );
    virtual void addEvent( kvs::EventListener* event, const std::string& name = "" );

    virtual void create();
    virtual void show();
    virtual void hide();
    virtual void showFullScreen();
    virtual void showNormal();
    virtual void popUp();
    virtual void pushDown();
    virtual void redraw();
    virtual void resize( int width, int height );

    virtual void enable() {}
    virtual void disable() {}
    virtual void reset() {}

    virtual void initializeEvent() {}
    virtual void paintEvent() {}
    virtual void resizeEvent( int width, int height ) {}
    virtual void mousePressEvent( kvs::MouseEvent* event ) {}
    virtual void mouseMoveEvent( kvs::MouseEvent* event ) {}
    virtual void mouseReleaseEvent( kvs::MouseEvent* event ) {}
    virtual void mouseDoubleClickEvent( kvs::MouseEvent* event ) {}
    virtual void wheelEvent( kvs::WheelEvent* event ) {}
    virtual void keyPressEvent( kvs::KeyEvent* event ) {}
    virtual void keyRepeatEvent( kvs::KeyEvent* event ) {}
    virtual void keyReleaseEvent( kvs::KeyEvent* event ) {}

protected:
    void aquireContext() { glfwMakeContextCurrent( m_handler ); }
    void releaseContext() { glfwMakeContextCurrent( nullptr ); }

private:
    friend void WindowSizeCallback( GLFWwindow* handler, int width, int height );
    friend void FramebufferSizeCallback( GLFWwindow* handler, int width, int height );
    friend void MouseButtonCallback( GLFWwindow* handler, int button, int action, int mods );
    friend void CursorPosCallback( GLFWwindow* handler, double x, double y );
    friend void ScrollCallback( GLFWwindow* handler, double x, double y );
    friend void KeyCallback( GLFWwindow* handler, int key, int scancode, int action, int mods );
};

} // end of namespace glfw

} // end of namespace kvs
