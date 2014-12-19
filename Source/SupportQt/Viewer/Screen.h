/*****************************************************************************/
/**
 *  @file   Screen.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: Screen.h 1768 2014-05-12 09:36:31Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#ifndef KVS__QT__SCREEN_H_INCLUDE
#define KVS__QT__SCREEN_H_INCLUDE

#include "ScreenBase.h"
#include <kvs/Scene>
#include <kvs/Mouse>
#include <kvs/RGBColor>
#include <kvs/ColorImage>
#include <kvs/qt/Timer>
#include <kvs/Deprecated>
#include <list>


namespace kvs
{

class RGBColor;
class Scene;
class ObjectBase;
class RendererBase;
class VisualizationPipeline;

/*KVS_DEPRECATED*/ class InitializeEventListener;
/*KVS_DEPRECATED*/ class PaintEventListener;
/*KVS_DEPRECATED*/ class ResizeEventListener;
/*KVS_DEPRECATED*/ class MousePressEventListener;
/*KVS_DEPRECATED*/ class MouseMoveEventListener;
/*KVS_DEPRECATED*/ class MouseReleaseEventListener;
/*KVS_DEPRECATED*/ class MouseDoubleClickEventListener;
/*KVS_DEPRECATED*/ class WheelEventListener;
/*KVS_DEPRECATED*/ class KeyPressEventListener;

namespace qt
{

class Application;

/*===========================================================================*/
/**
 *  @brief  Qt screen class.
 */
/*===========================================================================*/
class Screen : public kvs::qt::ScreenBase
{
    Q_OBJECT

public:

    typedef kvs::qt::ScreenBase BaseClass;
    typedef kvs::Scene::ControlTarget ControlTarget;

private:

    bool m_enable_default_paint_event; ///< flag for default paint event
    bool m_enable_default_resize_event; ///< flag for default resize event
    bool m_enable_default_mouse_press_event; ///< flag for default mouse press event
    bool m_enable_default_mouse_move_event; ///< flag for default mouse move event
    bool m_enable_default_mouse_release_event; ///< flag for default mouse release event
    bool m_enable_default_wheel_event; ///< flag for default mouse wheel event
    bool m_enable_default_key_press_event; ///< flag for default key press event
    kvs::Scene* m_scene; ///< default scene
    QTimer* m_idle_mouse_timer; ///< timer for idle mouse event

public:

    Screen( kvs::qt::Application* application = 0, QWidget* parent = 0 );
    virtual ~Screen();

    kvs::Scene* scene();

    void setTitle( const std::string& title );
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

public slots:

    virtual void idleMouseEvent();

protected:

    virtual void defaultPaintEvent();
    virtual void defaultResizeEvent( int width, int height );
    virtual void defaultMousePressEvent( kvs::MouseEvent* event );
    virtual void defaultMouseMoveEvent( kvs::MouseEvent* event );
    virtual void defaultMouseReleaseEvent( kvs::MouseEvent* event );
    virtual void defaultWheelEvent( kvs::WheelEvent* event );
    virtual void defaultKeyPressEvent( kvs::KeyEvent* event );

public:
    KVS_DEPRECATED( kvs::Camera* camera() );
    KVS_DEPRECATED( kvs::Light* light() );
    KVS_DEPRECATED( kvs::Mouse* mouse() );
    KVS_DEPRECATED( kvs::Background* background() );
    KVS_DEPRECATED( kvs::ObjectManager* objectManager() );
    KVS_DEPRECATED( kvs::RendererManager* rendererManager() );
    KVS_DEPRECATED( kvs::IDManager* IDManager() );
    KVS_DEPRECATED( ControlTarget& controlTarget() );
    KVS_DEPRECATED( void setPaintEvent( kvs::PaintEventListener* event ) );
    KVS_DEPRECATED( void setResizeEvent( kvs::ResizeEventListener* event ) );
    KVS_DEPRECATED( void setMousePressEvent( kvs::MousePressEventListener* event ) );
    KVS_DEPRECATED( void setMouseMoveEvent( kvs::MouseMoveEventListener* event ) );
    KVS_DEPRECATED( void setMouseReleaseEvent( kvs::MouseReleaseEventListener* event ) );
    KVS_DEPRECATED( void setMouseDoubleClickEvent( kvs::MouseDoubleClickEventListener* event ) );
    KVS_DEPRECATED( void setWheelEvent( kvs::WheelEventListener* event ) );
    KVS_DEPRECATED( void setKeyPressEvent( kvs::KeyPressEventListener* event ) );
};

} // end of namespace qt

} // end of namespace kvs

#endif // KVS__QT__SCREEN_H_INCLUDE
