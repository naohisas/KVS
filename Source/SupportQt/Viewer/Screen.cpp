/*****************************************************************************/
/**
 *  @file   Screen.cpp
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#include "Screen.h"
#include <kvs/DebugNew>
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
#include <kvs/ObjectManager>
#include <kvs/RendererManager>
#include <kvs/IDManager>
#include <kvs/ObjectBase>
#include <kvs/RendererBase>
#include <kvs/VisualizationPipeline>
#include <kvs/TrackballInteractor>
#include <kvs/qt/Qt>
#include <kvs/qt/Application>


namespace kvs
{

namespace qt
{

/*===========================================================================*/
/**
 *  @brief  Returns the pointer to the qt screen downcasted from the screen base.
 *  @param  screen [in] the screen base.
 *  @return pointer to the qt screen
 */
/*===========================================================================*/
Screen* Screen::DownCast( kvs::ScreenBase* screen )
{
    return dynamic_cast<Screen*>( screen );
}

/*===========================================================================*/
/**
 *  @brief  Returns the const pointer to the qt screen downcasted from the screen base.
 *  @param  screen [in] the screen base.
 *  @return const pointer to the qt screen
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
Screen::Screen( kvs::qt::Application* application, QWidget* parent ):
    kvs::qt::ScreenBase( application, parent )
{
    m_scene = new kvs::Scene( this );
    m_interactor = new kvs::TrackballInteractor();
    BaseClass::setEvent( m_interactor, "DefaultInteractor" );

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
    if ( m_scene ) { delete m_scene; }
    if ( m_interactor ) { delete m_interactor; }
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
 *  @brief  Sets an event.
 *  @param  event [in] pointer to an event
 *  @param  name [in] event name
 */
/*===========================================================================*/
void Screen::setEvent( kvs::EventListener* event, const std::string& name )
{
    event->setScene( m_scene );
    BaseClass::setEvent( event, name );
    if ( m_interactor ) { delete m_interactor; m_interactor = nullptr; }
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
    kvs::OpenGL::WithPushedMatrix p( GL_MODELVIEW );
    p.loadIdentity();

    kvs::PaintEvent event;
    BaseClass::eventHandler()->notify( &event );
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
    kvs::ResizeEvent event( width, height );
    BaseClass::eventHandler()->notify( &event );
}

/*===========================================================================*/
/**
 *  @brief  Mouse press event.
 *  @param  event [in] pointer to the mouse event information
 */
/*===========================================================================*/
void Screen::mousePressEvent( kvs::MouseEvent* event )
{
    BaseClass::eventHandler()->notify( event );
}

/*===========================================================================*/
/**
 *  @brief  Mouse move event.
 *  @param  event [in] pointer to the mouse event information
 */
/*===========================================================================*/
void Screen::mouseMoveEvent( kvs::MouseEvent* event )
{
    BaseClass::eventHandler()->notify( event );
}

/*===========================================================================*/
/**
 *  @brief  Mouse release event.
 *  @param  event [in] pointer to the mouse event information
 */
/*===========================================================================*/
void Screen::mouseReleaseEvent( kvs::MouseEvent* event )
{
    BaseClass::eventHandler()->notify( event );
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
    BaseClass::eventHandler()->notify( event );
}

/*===========================================================================*/
/**
 *  @brief  Key press event.
 *  @param  event [in] pointer to the key event information
 */
/*===========================================================================*/
void Screen::keyPressEvent( kvs::KeyEvent* event )
{
    BaseClass::eventHandler()->notify( event );
}

/*===========================================================================*/
/**
 *  @brief  Key repeat event.
 *  @param  event [in] pointer to the key event information
 */
/*===========================================================================*/
void Screen::keyRepeatEvent( kvs::KeyEvent* event )
{
    BaseClass::eventHandler()->notify( event );
}

/*===========================================================================*/
/**
 *  @brief  Key release event.
 *  @param  event [in] pointer to the key event information
 */
/*===========================================================================*/
void Screen::keyReleaseEvent( kvs::KeyEvent* event )
{
    BaseClass::eventHandler()->notify( event );
}

} // end of namespace qt

} // end of namespace kvs
