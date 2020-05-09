/*****************************************************************************/
/**
 *  @file   ScreenBase.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id$
 */
/*****************************************************************************/
#include "ScreenBase.h"
#include <kvs/Assert>
#include <kvs/MouseEvent>
#include <kvs/KeyEvent>
#include <kvs/WheelEvent>
#include <kvs/TimerEventListener>
#include <kvs/qt/Qt>
#include <kvs/qt/Application>
#include <kvs/qt/Timer>
#include <SupportQt/Viewer/KVSMouseButton.h>
#include <SupportQt/Viewer/KVSKey.h>
#include <kvs/OpenGL>
#include <kvs/Version>


namespace kvs
{

namespace qt
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new ScreenBase class.
 *  @param  application [in] pointer to the application
 *  @param  parent [in] parent widget
 */
/*===========================================================================*/
ScreenBase::ScreenBase( kvs::qt::Application* application, QWidget* parent ):
    QGLWidget( parent ),
    m_id( -1 ),
    m_mouse_event( 0 ),
    m_key_event( 0 ),
    m_wheel_event( 0 ),
    m_is_fullscreen( false )
{
    if ( application ) application->attach( this );

    m_mouse_event = new kvs::MouseEvent();
    m_key_event = new kvs::KeyEvent();
    m_wheel_event = new kvs::WheelEvent();
}

/*===========================================================================*/
/**
 *  @brief  Destroys the ScreenBase class.
 */
/*===========================================================================*/
ScreenBase::~ScreenBase()
{
    delete m_mouse_event;
    delete m_key_event;
    delete m_wheel_event;
}

/*===========================================================================*/
/**
 *  @brief  Creates a screen.
 */
/*===========================================================================*/
void ScreenBase::create()
{
    KVS_ASSERT( m_id == -1 );

    // Initialize display mode.
    QGLFormat f = QGLFormat::defaultFormat();
    f.setDoubleBuffer( displayFormat().doubleBuffer() );
    f.setRgba( displayFormat().colorBuffer() );
    f.setDepth( displayFormat().depthBuffer() );
    f.setAccum( displayFormat().accumulationBuffer() );
    f.setStencil( displayFormat().stencilBuffer() );
    f.setStereo( displayFormat().stereoBuffer() );
    f.setSampleBuffers( displayFormat().multisampleBuffer() );
    f.setAlpha( displayFormat().alphaChannel() );
    QGLFormat::setDefaultFormat( f );

    // Set screen geometry.
    QWidget::setGeometry( BaseClass::x(), BaseClass::y(), BaseClass::width(), BaseClass::height() );

    QGLWidget::makeCurrent();

    // Create window.
    static int counter = 0;
    m_id = counter++;
}

/*===========================================================================*/
/**
 *  @brief  Shows the screen.
 *  @return window ID
 */
/*===========================================================================*/
void ScreenBase::show()
{
#if 1 // KVS_ENABLE_DEPRECATED
    if ( m_id == -1 ) { this->create(); QWidget::show(); }
    else {
#endif

    QWidget::show();

#if 1 // KVS_ENABLE_DEPRECATED
    }
#endif
}

/*===========================================================================*/
/**
 *  @brief  Shows the window as full-screen.
 */
/*===========================================================================*/
void ScreenBase::showFullScreen()
{
    if ( m_is_fullscreen ) return;
    m_is_fullscreen = true;

    const int x = QGLWidget::pos().x();
    const int y = QGLWidget::pos().y();
    BaseClass::setPosition( x, y );

    QWidget::showFullScreen();
}

/*===========================================================================*/
/**
 *  @brief  Shows the window as normal screen.
 */
/*===========================================================================*/
void ScreenBase::showNormal()
{
    if ( !m_is_fullscreen ) return;
    m_is_fullscreen = false;

    QWidget::resize( BaseClass::width(), BaseClass::height() );
    QWidget::move( BaseClass::x(), BaseClass::y() );
    QWidget::showNormal();
}

/*===========================================================================*/
/**
 *  @brief  Hides the window.
 */
/*===========================================================================*/
void ScreenBase::hide()
{
    QWidget::hide();
}

/*===========================================================================*/
/**
 *  @brief  Pops up the window.
 */
/*===========================================================================*/
void ScreenBase::popUp()
{
#if ( KVS_QT_VERSION >= 4 )
    QWidget::activateWindow();
#else
    QWidget::setActiveWindow();
#endif
}

/*===========================================================================*/
/**
 *  @brief  Pushes down the window.
 */
/*===========================================================================*/
void ScreenBase::pushDown()
{
    QWidget::clearFocus();
}

/*===========================================================================*/
/**
 *  @brief  Redraws the window.
 */
/*===========================================================================*/
void ScreenBase::redraw()
{
    QGLWidget::updateGL();
}

/*===========================================================================*/
/**
 *  @brief  Resizes the window.
 *  @param  width [in] resized window width
 *  @param  height [in] resized window height
 */
/*===========================================================================*/
void ScreenBase::resize( int width, int height )
{
    BaseClass::setSize( width, height );
    QGLWidget::resize( width, height );
}

/*===========================================================================*/
/**
 *  @brief  Checks whether the window is full-screen or not.
 *  @return true, if the window is full-screen
 */
/*===========================================================================*/
bool ScreenBase::isFullScreen() const
{
    return m_is_fullscreen;
}

void ScreenBase::enable(){}
void ScreenBase::disable(){}
void ScreenBase::reset(){}

void ScreenBase::initializeEvent(){}
void ScreenBase::paintEvent(){}
void ScreenBase::resizeEvent( int, int ){}
void ScreenBase::mousePressEvent( kvs::MouseEvent* ){}
void ScreenBase::mouseMoveEvent( kvs::MouseEvent* ){}
void ScreenBase::mouseReleaseEvent( kvs::MouseEvent* ){}
void ScreenBase::mouseDoubleClickEvent( kvs::MouseEvent* ){}
void ScreenBase::wheelEvent( kvs::WheelEvent* ){}
void ScreenBase::keyPressEvent( kvs::KeyEvent* ){}

/*===========================================================================*/
/**
 *  @brief  Initialize event for Qt.
 */
/*===========================================================================*/
void ScreenBase::initializeGL()
{
#if defined( KVS_ENABLE_GLEW )
    // Initialize GLEW.
    GLenum result = glewInit();
    if ( result != GLEW_OK )
    {
        const GLubyte* message = glewGetErrorString( result );
        kvsMessageError( "GLEW initialization failed: %s.", message );
    }
#endif

    // Create paint device.
    BaseClass::paintDevice()->create();

    // Set device pixel ratio.
    const kvs::Vec4 vp = kvs::OpenGL::Viewport();
    BaseClass::setDevicePixelRatio( vp[2] / BaseClass::width() );

    this->initializeEvent();
}

/*===========================================================================*/
/**
 *  @brief  Paint event for Qt.
 */
/*===========================================================================*/
void ScreenBase::paintGL()
{
    this->paintEvent();
}

/*===========================================================================*/
/**
 *  @brief  Resize event for Qt.
 *  @param  width [in] window width
 *  @param  height [in] window height
 */
/*===========================================================================*/
void ScreenBase::resizeGL( int width, int height )
{
#if QT_VERSION >= QT_VERSION_CHECK( 5, 0, 0 )
//#if ( KVS_VERSION_MAJOR > 2 || ( KVS_VERSION_MAJOR == 2 && KVS_VERSION_MINOR > 8 ) )
    // NOTE: High-dpi model such as retina display has been supported in Qt5.
    // Therefore, when using Qt5 on Mac with retina display, the 'width' and
    // 'height' specified as arguments of this method are scaled by a device
    // pixel ratio (DPR) parameter. The DPR has been supported in KVS since
    // 2.9. In KVS with SupportQt, the width and height which are not scaled
    // are passed to the resizeEvent method in order to keep the compatibility
    // with the previous version of KVS-based applications.
    const qreal scale = QGLWidget::devicePixelRatio();
    width = static_cast<size_t>( width / scale + 0.5 );
    height = static_cast<size_t>( height / scale + 0.5 );
//#endif
#endif

    this->resizeEvent( width, height );
}

/*===========================================================================*/
/**
 *  @brief  Mouse press event for Qt.
 *  @param  event [in] pointer to the mouse event
 */
/*===========================================================================*/
void ScreenBase::mousePressEvent( QMouseEvent* event )
{
#if ( KVS_QT_VERSION >= 4 )
    const int modifier = kvs::qt::KVSKey::Modifier( event->modifiers() );
#else
    const int modifier = kvs::qt::KVSKey::Modifier( event->state() );
#endif
    m_mouse_event->setPosition( event->x(), event->y() );
    m_mouse_event->setButton( kvs::qt::KVSMouseButton::Button( event->button() ) );
    m_mouse_event->setState( kvs::MouseButton::Down );
    m_mouse_event->setModifiers( modifier );
    m_mouse_event->setAction( kvs::MouseButton::Pressed );

    this->mousePressEvent( m_mouse_event );
}

/*===========================================================================*/
/**
 *  @brief  Mouse move event for Qt.
 *  @param  event [in] pointer to the mouse event
 */
/*===========================================================================*/
void ScreenBase::mouseMoveEvent( QMouseEvent* event )
{
#if ( KVS_QT_VERSION >= 4 )
    const int modifier = kvs::qt::KVSKey::Modifier( event->modifiers() );
#else
    const int modifier = kvs::qt::KVSKey::Modifier( event->state() );
#endif
    m_mouse_event->setPosition( event->x(), event->y() );
    m_mouse_event->setButton( kvs::qt::KVSMouseButton::Button( event->button() ) );
    m_mouse_event->setState( kvs::MouseButton::Down );
    m_mouse_event->setModifiers( modifier );
    m_mouse_event->setAction( kvs::MouseButton::Moved );

    this->mouseMoveEvent( m_mouse_event );
}

/*===========================================================================*/
/**
 *  @brief  Mouse release event for Qt
 *  @param  event [in] pointer to the mouse event
 */
/*===========================================================================*/
void ScreenBase::mouseReleaseEvent( QMouseEvent* event )
{
#if ( KVS_QT_VERSION >= 4 )
    const int modifier = kvs::qt::KVSKey::Modifier( event->modifiers() );
#else
    const int modifier = kvs::qt::KVSKey::Modifier( event->state() );
#endif
    m_mouse_event->setPosition( event->x(), event->y() );
    m_mouse_event->setButton( kvs::qt::KVSMouseButton::Button( event->button() ) );
    m_mouse_event->setState( kvs::MouseButton::Up );
    m_mouse_event->setModifiers( modifier );
    m_mouse_event->setAction( kvs::MouseButton::Released );

    this->mouseReleaseEvent( m_mouse_event );
}

/*===========================================================================*/
/**
 *  @brief  Mouse double-click event for Qt.
 *  @param  event [in] pointer to the mouse event
 */
/*===========================================================================*/
void ScreenBase::mouseDoubleClickEvent( QMouseEvent* event )
{
#if ( KVS_QT_VERSION >= 4 )
    const int modifier = kvs::qt::KVSKey::Modifier( event->modifiers() );
#else
    const int modifier = kvs::qt::KVSKey::Modifier( event->state() );
#endif
    m_mouse_event->setPosition( event->x(), event->y() );
    m_mouse_event->setButton( kvs::qt::KVSMouseButton::Button( event->button() ) );
    m_mouse_event->setState( kvs::MouseButton::Down );
    m_mouse_event->setModifiers( modifier );
    m_mouse_event->setAction( kvs::MouseButton::DoubleClicked );

    this->mouseDoubleClickEvent( m_mouse_event );
}

/*===========================================================================*/
/**
 *  @brief  Wheel event for Qt.
 *  @param  event [in] pointer to the wheel event
 */
/*===========================================================================*/
void ScreenBase::wheelEvent( QWheelEvent* event )
{
    m_wheel_event->setDirection( event->delta() > 0 ? 1 : -1 );
    m_wheel_event->setPosition( event->x(), event->y() );

    this->wheelEvent( m_wheel_event );
}

/*===========================================================================*/
/**
 *  @brief  Key press event for Qt.
 *  @param  event [in] pointer to the key event
 */
/*===========================================================================*/
void ScreenBase::keyPressEvent( QKeyEvent* event )
{
    m_key_event->setPosition( 0, 0 );
    m_key_event->setKey( kvs::qt::KVSKey::Code( event->key() ) );

    this->keyPressEvent( m_key_event );
}

std::list<kvs::qt::Timer*>& ScreenBase::timerEventHandler()
{
    return m_timer_event_handler;
}

/*===========================================================================*/
/**
 *  @brief  Adds a timer event listener.
 *  @param  event [in] pointer to a timer event listener
 *  @param  timer [in] pointer to timer
 */
/*===========================================================================*/
void ScreenBase::addTimerEvent( kvs::TimerEventListener* event, kvs::qt::Timer* timer )
{
    event->setScreen( this );
    timer->setEventListener( event );
    m_timer_event_handler.push_back( timer );
}

} // end of namespace qt

} // end of namespace kvs
