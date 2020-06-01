/*****************************************************************************/
/**
 *  @file   Screen.cpp
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#include "Screen.h"
#include <cstdlib>
#include <kvs/OpenGL>
#include <kvs/RGBColor>
#include <kvs/Camera>
#include <kvs/Light>
#include <kvs/Mouse>
#include <kvs/MouseButton>
#include <kvs/Background>
#include <kvs/InitializeEvent>
#include <kvs/PaintEvent>
#include <kvs/ResizeEvent>
#include <kvs/MouseEvent>
#include <kvs/KeyEvent>
#include <kvs/WheelEvent>
#include <kvs/EventHandler>
#include <kvs/TimerEventListener>
#include <kvs/PaintEventListener>
#include <kvs/ResizeEventListener>
#include <kvs/MousePressEventListener>
#include <kvs/MouseMoveEventListener>
#include <kvs/MouseReleaseEventListener>
#include <kvs/MouseDoubleClickEventListener>
#include <kvs/WheelEventListener>
#include <kvs/KeyPressEventListener>
#include <kvs/ObjectManager>
#include <kvs/RendererManager>
#include <kvs/IDManager>
#include <kvs/ObjectBase>
#include <kvs/RendererBase>
#include <kvs/VisualizationPipeline>
#include "Application.h"
#include "Timer.h"


namespace
{

/*===========================================================================*/
/**
 *  @brief  Listener class for idle mouse event.
 */
/*===========================================================================*/
class IdleMouseEvent : public kvs::TimerEventListener
{
    kvs::glfw::Screen* m_screen; ///< pointer to the screen (reference only)

public:
    IdleMouseEvent( kvs::glfw::Screen* screen ): m_screen( screen ) {}
    void update( kvs::TimeEvent* ) { m_screen->idleMouseEvent(); }
};

}


namespace kvs
{

namespace glfw
{

/*===========================================================================*/
/**
 *  @brief  Returns the pointer to the glfw screen downcasted from the screen base.
 *  @param  screen [in] the screen base.
 *  @return pointer to the glfw screen
 */
/*===========================================================================*/
Screen* Screen::DownCast( kvs::ScreenBase* screen )
{
    return dynamic_cast<Screen*>( screen );
}

/*===========================================================================*/
/**
 *  @brief  Returns the const pointer to the glfw screen downcasted from the screen base.
 *  @param  screen [in] the screen base.
 *  @return const pointer to the glfw screen
 */
/*===========================================================================*/
const Screen* Screen::DownCast( const kvs::ScreenBase* screen )
{
    return dynamic_cast<Screen*>( const_cast<kvs::ScreenBase*>( screen ) );
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new Screen class.
 */
/*===========================================================================*/
Screen::Screen( kvs::glfw::Application* application ):
    kvs::glfw::ScreenBase( application ),
    m_enable_default_paint_event( true ),
    m_enable_default_resize_event( true ),
    m_enable_default_mouse_press_event( true ),
    m_enable_default_mouse_move_event( true ),
    m_enable_default_mouse_release_event( true ),
    m_enable_default_wheel_event( true ),
    m_enable_default_key_press_event( true )
{
    m_scene = new kvs::Scene( this );
    m_idle_mouse_event_listener = new ::IdleMouseEvent( this );
    m_idle_mouse_timer = new kvs::glfw::Timer( 10 );
    BaseClass::addTimerEvent( m_idle_mouse_event_listener, m_idle_mouse_timer );
}

/*===========================================================================*/
/**
 *  @brief  Destructs the Screen class.
 */
/*===========================================================================*/
Screen::~Screen()
{
    delete m_scene;
    delete m_idle_mouse_event_listener;
    delete m_idle_mouse_timer;
}

/*===========================================================================*/
/**
 *  @brief  Returns the pointer to the scene.
 *  @return pointer to the scene
 */
/*===========================================================================*/
kvs::Scene* Screen::scene()
{
    return m_scene;
}

/*===========================================================================*/
/**
 *  @brief  Sets a screen position.
 *  @param  x [in] x coordinate value of the screen position
 *  @param  y [in] y coordinate value of the screen position
 */
/*===========================================================================*/
void Screen::setPosition( const int x, const int y )
{
    BaseClass::setPosition( x, y );
}

/*===========================================================================*/
/**
 *  @brief  Sets a screen size.
 *  @param  width [in] screen width
 *  @param  height [in] screen height
 */
/*===========================================================================*/
void Screen::setSize( const int width, const int height )
{
    BaseClass::setSize( width, height );

    if ( m_scene->camera() ) m_scene->camera()->setWindowSize( width, height );
}

/*===========================================================================*/
/**
 *  @brief  Sets a screen geometry (position and size).
 *  @param  x [in] x coordinate value of the screen position
 *  @param  y [in] y coordinate value of the screen position
 *  @param  width [in] screen width
 *  @param  height [in] screen height
 */
/*===========================================================================*/
void Screen::setGeometry( const int x, const int y, const int width, const int height )
{
    this->setPosition( x, y );
    this->setSize( width, height );
}

/*===========================================================================*/
/**
 *  @brief  Sets a background color.
 *  @param  color [in] RGB color
 */
/*===========================================================================*/
void Screen::setBackgroundColor( const kvs::RGBColor& color )
{
    m_scene->background()->setColor( color );
}

/*===========================================================================*/
/**
 *  @brief  Sets background colors
 *  @param  color1 [in] the color at the top side of the screen
 *  @param  color2 [in] the color at the bottom side of the screen
 */
/*===========================================================================*/
void Screen::setBackgroundColor( const kvs::RGBColor& color1, const kvs::RGBColor& color2 )
{
    m_scene->background()->setColor( color1, color2 );
}

/*===========================================================================*/
/**
 *  @brief  Sets a background image.
 *  @param  image [in] RGB image
 */
/*===========================================================================*/
void Screen::setBackgroundImage( const kvs::ColorImage& image )
{
    m_scene->background()->setImage( image );
}

/*===========================================================================*/
/**
 *  @brief  Sets a control target.
 *  @param  target [in] control target
 */
/*===========================================================================*/
void Screen::setControlTarget( const ControlTarget target )
{
    m_scene->controlTarget() = target;
}

/*===========================================================================*/
/**
 *  @brief  Sets a control target to an object.
 */
/*===========================================================================*/
void Screen::setControlTargetToObject()
{
    this->setControlTarget( kvs::Scene::TargetObject );
}

/*===========================================================================*/
/**
 *  @brief  Sets a control target to a camera.
 */
/*===========================================================================*/
void Screen::setControlTargetToCamera()
{
    this->setControlTarget( kvs::Scene::TargetCamera );
}

/*===========================================================================*/
/**
 *  @brief  Sets a control target to a light.
 */
/*===========================================================================*/
void Screen::setControlTargetToLight()
{
    this->setControlTarget( kvs::Scene::TargetLight );
}

/*===========================================================================*/
/**
 *  @brief  Sets an event (disable defualt event correspond to the given event).
 *  @param  event [in] pointer to an event
 *  @param  name [in] event name
 */
/*===========================================================================*/
void Screen::setEvent( kvs::EventListener* event, const std::string& name )
{
    switch ( event->eventType() )
    {
    case kvs::EventBase::PaintEvent: m_enable_default_paint_event =false; break;
    case kvs::EventBase::ResizeEvent: m_enable_default_resize_event =false; break;
    case kvs::EventBase::MousePressEvent: m_enable_default_mouse_press_event =false; break;
    case kvs::EventBase::MouseMoveEvent: m_enable_default_mouse_move_event =false; break;
    case kvs::EventBase::MouseReleaseEvent: m_enable_default_mouse_release_event =false; break;
    case kvs::EventBase::WheelEvent: m_enable_default_wheel_event = false; break;
    case kvs::EventBase::KeyPressEvent: m_enable_default_key_press_event = false; break;
    default: break;
    }
    event->setScene( m_scene );
    BaseClass::addEvent( event, name );
}

/*===========================================================================*/
/**
 *  @brief  Adds an event to the event handler.
 *  @param  event [in] pointer to an event
 *  @param  name [in] event name
 */
/*===========================================================================*/
void Screen::addEvent( kvs::EventListener* event, const std::string& name )
{
    event->setScene( m_scene );
    BaseClass::addEvent( event, name );
}

/*===========================================================================*/
/**
 *  @brief  Registers an object with a renderer.
 *  @param  object [in] pointer to the object
 *  @param  renderer [in] pointer to the renderer
 *  @return Pair of IDs (object ID and renderer ID)
 */
/*===========================================================================*/
const std::pair<int,int> Screen::registerObject( kvs::ObjectBase* object, kvs::RendererBase* renderer )
{
    return m_scene->registerObject( object, renderer );
}

/*===========================================================================*/
/**
 *  @brief  Registers an object that is processed through the visualization pipeline.
 *  @param  pipeline [in] pointer to the visualization pipeline
 *  @return Pair of IDs (object ID and renderer ID)
 */
/*===========================================================================*/
const std::pair<int,int> Screen::registerObject( kvs::VisualizationPipeline* pipeline )
{
    // WARNING: It is necessary to increment the reference counter of the
    // pipeline.object() and the pipeline.renderer().
    auto* object = const_cast<kvs::ObjectBase*>( pipeline->object() );
    auto* renderer = const_cast<kvs::RendererBase*>( pipeline->renderer() );

    const int object_id = m_scene->objectManager()->insert( object );
    const int renderer_id = m_scene->rendererManager()->insert( renderer );
    m_scene->IDManager()->insert( object_id, renderer_id );

    return std::pair<int,int>( object_id, renderer_id );
}

/*===========================================================================*/
/**
 *  @brief  Creates a screen.
 */
/*===========================================================================*/
void Screen::create()
{
    BaseClass::create();
    m_scene->initializeFunction();
}

/*===========================================================================*/
/**
 *  @brief  Enables mosue operation.
 */
/*===========================================================================*/
void Screen::enable()
{
    m_scene->enableObjectOperation();
}

/*===========================================================================*/
/**
 *  @brief  Disables mosue operation.
 */
/*===========================================================================*/
void Screen::disable()
{
    m_scene->disableObjectOperation();
}

/*===========================================================================*/
/**
 *  @brief  Resets the x-form matrix of the basic screen components.
 */
/*===========================================================================*/
void Screen::reset()
{
    m_scene->reset();
}

/*===========================================================================*/
/**
 *  @brief  Initialization event.
 */
/*===========================================================================*/
void Screen::initializeEvent()
{
    for ( auto& timer : BaseClass::timerEventHandler() ) { timer->start(); }
    kvs::InitializeEvent event;
    BaseClass::eventHandler()->notify( &event );
}

/*===========================================================================*/
/**
 *  @brief  Paint event.
 */
/*===========================================================================*/
void Screen::paintEvent()
{
    if ( m_enable_default_paint_event ) this->defaultPaintEvent();
    else
    {
        kvs::PaintEvent event;
        BaseClass::eventHandler()->notify( &event );
    }

    glfwSwapBuffers( BaseClass::handler() );
}

/*===========================================================================*/
/**
 *  @brief  Resize event.
 *  @param  width [in] resized screen width
 *  @param  height [in] resized screen height
 */
/*===========================================================================*/
void Screen::resizeEvent( int width, int height )
{
    if ( m_enable_default_resize_event ) this->defaultResizeEvent( width, height );
    else
    {
        kvs::ResizeEvent event( width, height );
        BaseClass::eventHandler()->notify( &event );
    }
}

/*===========================================================================*/
/**
 *  @brief  Mouse press event.
 *  @param  event [in] pointer to the mouse event information
 */
/*===========================================================================*/
void Screen::mousePressEvent( kvs::MouseEvent* event )
{
    if ( m_enable_default_mouse_press_event ) this->defaultMousePressEvent( event );
    else BaseClass::eventHandler()->notify( event );
}

/*===========================================================================*/
/**
 *  @brief  Mouse move event.
 *  @param  event [in] pointer to the mouse event information
 */
/*===========================================================================*/
void Screen::mouseMoveEvent( kvs::MouseEvent* event )
{
    if ( m_enable_default_mouse_move_event ) this->defaultMouseMoveEvent( event );
    else BaseClass::eventHandler()->notify( event );
}

/*===========================================================================*/
/**
 *  @brief  Mouse release event.
 *  @param  event [in] pointer to the mouse event information
 */
/*===========================================================================*/
void Screen::mouseReleaseEvent( kvs::MouseEvent* event )
{
    if ( m_enable_default_mouse_release_event ) this->defaultMouseReleaseEvent( event );
    else BaseClass::eventHandler()->notify( event );
}

/*===========================================================================*/
/**
 *  @brief  Mouse double-click event.
 *  @param  event [in] pointer to the mouse event information
 */
/*===========================================================================*/
void Screen::mouseDoubleClickEvent( kvs::MouseEvent* event )
{
    BaseClass::eventHandler()->notify( event );
}

/*===========================================================================*/
/**
 *  @brief  Wheel event.
 *  @param  event [in] pointer to the wheel event information
 */
/*===========================================================================*/
void Screen::wheelEvent( kvs::WheelEvent* event )
{
    if ( m_enable_default_wheel_event ) this->defaultWheelEvent( event );
    else BaseClass::eventHandler()->notify( event );
}

/*===========================================================================*/
/**
 *  @brief  Key press event.
 *  @param  event [in] pointer to the key event information
 */
/*===========================================================================*/
void Screen::keyPressEvent( kvs::KeyEvent* event )
{
    if ( m_enable_default_key_press_event ) this->defaultKeyPressEvent( event );
    else BaseClass::eventHandler()->notify( event );
}

/*===========================================================================*/
/**
 *  @brief  Default idle mouse event.
 */
/*===========================================================================*/
void Screen::idleMouseEvent()
{
    if ( m_scene->mouse()->idle() )
    {
        if ( !( m_scene->controlTarget() == kvs::Scene::TargetObject &&
                !m_scene->isEnabledObjectOperation() &&
                !m_scene->objectManager()->hasActiveObject() ) )
        {
            m_scene->updateXform();
            BaseClass::redraw();
        }
    }
}

/*===========================================================================*/
/**
 *  @brief  Default paint event.
 */
/*===========================================================================*/
void Screen::defaultPaintEvent()
{
    kvs::OpenGL::WithPushedMatrix p( GL_MODELVIEW );
    p.loadIdentity();
    {
        m_scene->paintFunction();
        kvs::PaintEvent event;
        BaseClass::eventHandler()->notify( &event );
    }
}

/*===========================================================================*/
/**
 *  @brief  Default resize event.
 *  @param  width [in] screen width
 *  @param  height [in] screen height
 */
/*===========================================================================*/
void Screen::defaultResizeEvent( int width, int height )
{
    if ( !BaseClass::isFullScreen() ) { BaseClass::setSize( width, height ); }

    m_scene->resizeFunction( width, height );
    kvs::ResizeEvent event( width, height );
    BaseClass::eventHandler()->notify( &event );
}

/*===========================================================================*/
/**
 *  @brief  Default mouse press event.
 *  @param  event [in] pointer to the mouse event
 */
/*===========================================================================*/
void Screen::defaultMousePressEvent( kvs::MouseEvent* event )
{
    if ( event->button() == kvs::MouseButton::Left )
    {
        BaseClass::eventHandler()->notify( event );
        if ( !m_scene->isActiveMove( event->x(), event->y() ) ) return;

        kvs::Mouse::OperationMode mode;
        switch ( event->modifiers() )
        {
        case kvs::Key::ShiftModifier:
            mode = kvs::Mouse::Scaling;
            break;
        case kvs::Key::ControlModifier:
            mode = kvs::Mouse::Translation;
            break;
        default:
            mode = kvs::Mouse::Rotation;
            m_scene->updateCenterOfRotation();
            break;
        }

        m_scene->mousePressFunction( event->x(), event->y(), mode );
    }

    else if ( event->button() == kvs::MouseButton::Middle )
    {
        BaseClass::eventHandler()->notify( event );
        if ( !m_scene->isActiveMove( event->x(), event->y() ) ) return;

        m_scene->mousePressFunction( event->x(), event->y(), kvs::Mouse::Scaling );
    }

    else if ( event->button() == kvs::MouseButton::Right )
    {
        BaseClass::eventHandler()->notify( event );
        if ( !m_scene->isActiveMove( event->x(), event->y() ) ) return;

        m_scene->mousePressFunction( event->x(), event->y(), kvs::Mouse::Translation );
    }
}

/*===========================================================================*/
/**
 *  @brief  Default mouse move event.
 *  @param  event [in] pointer to the mouse event
 */
/*===========================================================================*/
void Screen::defaultMouseMoveEvent( kvs::MouseEvent* event )
{
    BaseClass::eventHandler()->notify( event );
    if ( m_scene->controlTarget() == kvs::Scene::TargetObject )
    {
        if ( !m_scene->isEnabledObjectOperation() )
        {
            if ( !m_scene->objectManager()->hasActiveObject() )
            {
                return;
            }
        }
    }

    m_scene->mouseMoveFunction( event->x(), event->y() );
    BaseClass::redraw();
}

/*===========================================================================*/
/**
 *  @brief  Default mouse release event.
 *  @param  event [in] pointer to the mouse event
 */
/*===========================================================================*/
void Screen::defaultMouseReleaseEvent( kvs::MouseEvent* event )
{
    BaseClass::eventHandler()->notify( event );
    m_scene->mouseReleaseFunction( event->x(), event->y() );
    BaseClass::redraw();
}

/*===========================================================================*/
/**
 *  @brief  Default wheel event.
 *  @param  event [in] pointer to the wheel event
 */
/*===========================================================================*/
void Screen::defaultWheelEvent( kvs::WheelEvent* event )
{
    BaseClass::eventHandler()->notify( event );
    if ( !m_scene->isActiveMove( event->x(), event->y() ) ) return;

    if ( event->direction() > 0 ) { m_scene->wheelFunction( 50 ); }
    else { m_scene->wheelFunction( -50 ); }

    m_scene->updateXform();
    BaseClass::redraw();
}

/*===========================================================================*/
/**
 *  @brief  Default key press event.
 *  @param  event [in] pointer to the key event
 */
/*===========================================================================*/
void Screen::defaultKeyPressEvent( kvs::KeyEvent* event )
{
    switch( event->key() )
    {
    case kvs::Key::Escape:
    case kvs::Key::q:
        exit( 0 );
        break;
    case kvs::Key::Home:
        m_scene->reset();
        break;
    case kvs::Key::Tab:
        m_scene->enableCollisionDetection();
        break;
    default:
        break;
    }

    BaseClass::eventHandler()->notify( event );
    BaseClass::redraw();
}

} // end of namespace glfw

} // end of namespace kvs
