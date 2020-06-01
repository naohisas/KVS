/*****************************************************************************/
/**
 *  @file   Screen.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#pragma once
#include "ScreenBase.h"
#include <kvs/Scene>
#include <kvs/Mouse>
#include <kvs/RGBColor>
#include <kvs/ColorImage>
#include <list>


namespace kvs
{

class RGBColor;
class Scene;
class ObjectBase;
class RendererBase;
class VisualizationPipeline;
class EventHandler;

namespace glfw
{

class Application;
class Timer;

/*===========================================================================*/
/**
 *  @brief  GLFW screen class.
 */
/*===========================================================================*/
class Screen : public kvs::glfw::ScreenBase
{
public:
    typedef kvs::glfw::ScreenBase BaseClass;
    typedef kvs::Scene::ControlTarget ControlTarget;

    static Screen* DownCast( kvs::ScreenBase* screen );
    static const Screen* DownCast( const kvs::ScreenBase* screen );

private:
    bool m_enable_default_paint_event; ///< flag for default paint event
    bool m_enable_default_resize_event; ///< flag for default resize event
    bool m_enable_default_mouse_press_event; ///< flag for default mouse press event
    bool m_enable_default_mouse_move_event; ///< flag for default mouse move event
    bool m_enable_default_mouse_release_event; ///< flag for default mouse release event
    bool m_enable_default_wheel_event; ///< flag for default wheel event
    bool m_enable_default_key_press_event; ///< flag for default key press event
    kvs::Scene* m_scene; ///< default scene
    kvs::glfw::Timer* m_idle_mouse_timer; ///< timer for idle mouse event
    kvs::TimerEventListener* m_idle_mouse_event_listener; ///< idle mouse event listener

public:
    Screen( kvs::glfw::Application* application = 0 );
    virtual ~Screen();

    kvs::Scene* scene();

    void setPosition( const int x, const int y );
    void setSize( const int width, const int height );
    void setGeometry( const int x, const int y, const int width, const int height );
    void setBackgroundColor( const kvs::RGBColor& color );
    void setBackgroundColor( const kvs::RGBColor& color1, const kvs::RGBColor& color2 );
    void setBackgroundImage( const kvs::ColorImage& image );
    void setControlTarget( const ControlTarget target );
    void setControlTargetToObject();
    void setControlTargetToCamera();
    void setControlTargetToLight();
    void setEvent( kvs::EventListener* event, const std::string& name = "" );
    void addEvent( kvs::EventListener* event, const std::string& name = "" );

    const std::pair<int,int> registerObject( kvs::ObjectBase* object, kvs::RendererBase* renderer = 0 );
    const std::pair<int,int> registerObject( kvs::VisualizationPipeline* pipeline );

    virtual void create();
    virtual void enable();
    virtual void disable();
    virtual void reset();

    virtual void initializeEvent();
    virtual void paintEvent();
    virtual void resizeEvent( int width, int height );
    virtual void mousePressEvent( kvs::MouseEvent* event );
    virtual void mouseMoveEvent( kvs::MouseEvent* event );
    virtual void mouseReleaseEvent( kvs::MouseEvent* event );
    virtual void mouseDoubleClickEvent( kvs::MouseEvent* event );
    virtual void wheelEvent( kvs::WheelEvent* event );
    virtual void keyPressEvent( kvs::KeyEvent* event );
    virtual void idleMouseEvent();

protected:
    virtual void defaultPaintEvent();
    virtual void defaultResizeEvent( int width, int height );
    virtual void defaultMousePressEvent( kvs::MouseEvent* event );
    virtual void defaultMouseMoveEvent( kvs::MouseEvent* event );
    virtual void defaultMouseReleaseEvent( kvs::MouseEvent* event );
    virtual void defaultWheelEvent( kvs::WheelEvent* event );
    virtual void defaultKeyPressEvent( kvs::KeyEvent* event );
};

} // end of namespace glfw

} // end of namespace kvs
