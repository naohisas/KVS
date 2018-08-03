/*****************************************************************************/
/**
 *  @file   Screen.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: Screen.cpp 1768 2014-05-12 09:36:31Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#include "Screen.h"
#include <kvs/DebugNew>
#include <kvs/OpenGL>
#include <kvs/qt/Qt>
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
#include <kvs/InitializeEventListener>
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
#include <kvs/qt/Application>
#include <kvs/qt/Timer>


namespace kvs
{

namespace qt
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new Screen class.
 */
/*===========================================================================*/
Screen::Screen( kvs::qt::Application* application, QWidget* parent ):
    kvs::qt::ScreenBase( application, parent ),
    m_enable_default_paint_event( true ),
    m_enable_default_resize_event( true ),
    m_enable_default_mouse_press_event( true ),
    m_enable_default_mouse_move_event( true ),
    m_enable_default_mouse_release_event( true ),
    m_enable_default_wheel_event( true ),
    m_enable_default_key_press_event( true )
{
    m_scene = new kvs::Scene( this );
    m_idle_mouse_timer = new QTimer( this );

    // Change default focus policy to Qt::StrongFocus from Qt::NoFocus.
    QWidget::setFocusPolicy( Qt::StrongFocus );
}

/*===========================================================================*/
/**
 *  @brief  Destructs the Screen class.
 */
/*===========================================================================*/
Screen::~Screen()
{
    delete m_scene;
    delete m_idle_mouse_timer;
}

/*===========================================================================*/
/**
 *  @brief  Return pointer to the scene.
 *  @return pointer to the scene
 */
/*===========================================================================*/
kvs::Scene* Screen::scene()
{
    return m_scene;
}

/*===========================================================================*/
/**
 *  @brief  Sets a window title.
 *  @param  title [in] title
 */
/*===========================================================================*/
void Screen::setTitle( const std::string& title )
{
    BaseClass::setTitle( title );
#if ( KVS_QT_VERSION >= 4 )
    QWidget::setWindowTitle( QString( title.c_str() ) );
#else
    QWidget::setCaption( QString( title.c_str() ) );
#endif
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
    QWidget::move( x, y );
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

    QWidget::resize( width, height );
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

void Screen::setBackgroundColor( const kvs::RGBColor& color1, const kvs::RGBColor& color2 )
{
    m_scene->background()->setColor( color1, color2 );
}

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

void Screen::setControlTargetToObject()
{
    this->setControlTarget( kvs::Scene::TargetObject );
}

void Screen::setControlTargetToCamera()
{
    this->setControlTarget( kvs::Scene::TargetCamera );
}

void Screen::setControlTargetToLight()
{
    this->setControlTarget( kvs::Scene::TargetLight );
}

/*===========================================================================*/
/**
 *  @brief  Sets an event.
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
    kvs::ObjectBase* object = const_cast<kvs::ObjectBase*>( pipeline->object() );
    kvs::RendererBase* renderer = const_cast<kvs::RendererBase*>( pipeline->renderer() );

    const int object_id = m_scene->objectManager()->insert( object );
    const int renderer_id = m_scene->rendererManager()->insert( renderer );
    m_scene->IDManager()->insert( object_id, renderer_id );

    return std::pair<int,int>( object_id, renderer_id );
}

/*===========================================================================*/
/**
 *  @brief  Enables mouse operation.
 */
/*===========================================================================*/
void Screen::enable()
{
    m_scene->enableObjectOperation();
}

/*===========================================================================*/
/**
 *  @brief  Disables mouse operation.
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
    QObject::connect( m_idle_mouse_timer, SIGNAL( timeout() ), this, SLOT( idleMouseEvent() ) );
    const int interval = 10; // in msec.
    m_idle_mouse_timer->start( interval );

    std::list<kvs::qt::Timer*>::iterator timer = BaseClass::timerEventHandler().begin();
    std::list<kvs::qt::Timer*>::iterator end = BaseClass::timerEventHandler().end();
    while ( timer != end )
    {
        (*timer)->start();
        ++timer;
    }

    m_scene->initializeFunction();
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
                !m_scene->isEnabledObjectOperation()   &&
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

    if ( event->direction() > 0 ) { m_scene->wheelFunction( 10 ); }
    else { m_scene->wheelFunction( -10 ); }

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

/*===========================================================================*/
/**
 *  @brief  Returns the pointer to the camera.
 *  @return pointer to the camera
 */
/*===========================================================================*/
kvs::Camera* Screen::camera()
{
    return m_scene->camera();
}

/*===========================================================================*/
/**
 *  @brief  Returns the pointer to the light.
 *  @return pointer to the light
 */
/*===========================================================================*/
kvs::Light* Screen::light()
{
    return m_scene->light();
}

/*===========================================================================*/
/**
 *  @brief  Returns the pointer to the mouse.
 *  @return pointer to the mouse
 */
/*===========================================================================*/
kvs::Mouse* Screen::mouse()
{
    return m_scene->mouse();
}

/*===========================================================================*/
/**
 *  @brief  Returns the pointer to the background class.
 *  @return pointer to the background class
 */
/*===========================================================================*/
kvs::Background* Screen::background()
{
    return m_scene->background();
}

/*===========================================================================*/
/**
 *  @brief  Returns the pointer to the object manager.
 *  @return pointer to the object manager
 */
/*===========================================================================*/
kvs::ObjectManager* Screen::objectManager()
{
    return m_scene->objectManager();
}

/*===========================================================================*/
/**
 *  @brief  Returns the pointer to the renderer manager.
 *  @return pointer to the renderer manager
 */
/*===========================================================================*/
kvs::RendererManager* Screen::rendererManager()
{
    return m_scene->rendererManager();
}

/*===========================================================================*/
/**
 *  @brief  Returns the pointer to the ID manager.
 *  @return pointer to the ID manager
 */
/*===========================================================================*/
kvs::IDManager* Screen::IDManager()
{
    return m_scene->IDManager();
}

/*===========================================================================*/
/**
 *  @brief  Returns the control target.
 *  @return control target
 */
/*===========================================================================*/
Screen::ControlTarget& Screen::controlTarget()
{
    return m_scene->controlTarget();
}

/*===========================================================================*/
/**
 *  @brief  Sets a paint event listener.
 *  @param  event [in] pointer to a paint event listener
 */
/*===========================================================================*/
void Screen::setPaintEvent( kvs::PaintEventListener* event )
{
    m_enable_default_paint_event = false;
    BaseClass::addPaintEvent( event );
}

/*===========================================================================*/
/**
 *  @brief  Sets a resize event listener.
 *  @param  event [in] pointer to a resize event listener
 */
/*===========================================================================*/
void Screen::setResizeEvent( kvs::ResizeEventListener* event )
{
    m_enable_default_resize_event = false;
    BaseClass::addResizeEvent( event );
}

/*===========================================================================*/
/**
 *  @brief  Sets a mouse press event listener.
 *  @param  event [in] pointer to a mouse press event listener
 */
/*===========================================================================*/
void Screen::setMousePressEvent( kvs::MousePressEventListener* event )
{
    m_enable_default_mouse_press_event = false;
    BaseClass::addMousePressEvent( event );
}

/*===========================================================================*/
/**
 *  @brief  Sets a mouse move event listener.
 *  @param  event [in] pointer to a mouse move event listener
 */
/*===========================================================================*/
void Screen::setMouseMoveEvent( kvs::MouseMoveEventListener* event )
{
    m_enable_default_mouse_move_event = false;
    BaseClass::addMouseMoveEvent( event );
}

/*===========================================================================*/
/**
 *  @brief  Sets a mouse release event listener.
 *  @param  event [in] pointer to a mouse release event listener
 */
/*===========================================================================*/
void Screen::setMouseReleaseEvent( kvs::MouseReleaseEventListener* event )
{
    m_enable_default_mouse_release_event = false;
    BaseClass::addMouseReleaseEvent( event );
}

/*===========================================================================*/
/**
 *  @brief  Sets a mouse double-click event listener.
 *  @param  event [in] pointer to a mouse double-click event listener
 */
/*===========================================================================*/
void Screen::setMouseDoubleClickEvent( kvs::MouseDoubleClickEventListener* event )
{
    // nothing default mouse double click event.
    BaseClass::addMouseDoubleClickEvent( event );
}

/*===========================================================================*/
/**
 *  @brief  Sets a wheel event listener.
 *  @param  event [in] pointer to a wheel event listener
 */
/*===========================================================================*/
void Screen::setWheelEvent( kvs::WheelEventListener* event )
{
    m_enable_default_wheel_event = false;
    BaseClass::addWheelEvent( event );
}

/*===========================================================================*/
/**
 *  @brief  Sets a key press event listener.
 *  @param  event [in] pointer to a key press event listener
 */
/*===========================================================================*/
void Screen::setKeyPressEvent( kvs::KeyPressEventListener* event )
{
    m_enable_default_key_press_event = false;
    BaseClass::addKeyPressEvent( event );
}

} // end of namespace qt

} // end of namespace kvs
