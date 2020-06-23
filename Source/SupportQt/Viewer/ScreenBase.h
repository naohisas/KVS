/*****************************************************************************/
/**
 *  @file   ScreenBase.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#pragma once
#include <kvs/ScreenBase>
#include <kvs/qt/Qt>


namespace kvs
{

class MouseEvent;
class WheelEvent;
class KeyEvent;

namespace qt
{

class Application;
//class Timer;

/*===========================================================================*/
/**
 *  @brief  Qt screen base class.
 */
/*===========================================================================*/
class ScreenBase : public QGLWidget, public kvs::ScreenBase
{
    Q_OBJECT

    using BaseClass = kvs::ScreenBase;

public:
    static ScreenBase* DownCast( kvs::ScreenBase* screen );
    static const ScreenBase* DownCast( const kvs::ScreenBase* screen );

private:
    int m_id; ///< window ID
    kvs::MouseEvent* m_mouse_event; ///< mouse event
    kvs::KeyEvent* m_key_event; ///< key event
    kvs::WheelEvent* m_wheel_event; ///< wheel event
    bool m_is_fullscreen; ///< check flag whether the window is fullscreen

public:
    ScreenBase( kvs::qt::Application* application = 0, QWidget* parent = 0 );
    virtual ~ScreenBase();

    int id() const { return m_id; }
    int x() const { return BaseClass::x(); }
    int y() const { return BaseClass::y(); }
    int width() const { return BaseClass::width(); }
    int height() const { return BaseClass::height(); }

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
    virtual bool isFullScreen() const;
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
    virtual void keyRepeatEvent( kvs::KeyEvent* event );
    virtual void keyReleaseEvent( kvs::KeyEvent* event );

private:
    // Callback functions for Qt.
    void initializeGL();
    void paintGL();
    void resizeGL( int width, int height );
    void mousePressEvent( QMouseEvent* event );
    void mouseMoveEvent( QMouseEvent* event );
    void mouseReleaseEvent( QMouseEvent* event );
    void mouseDoubleClickEvent( QMouseEvent* event );
    void wheelEvent( QWheelEvent* event );
    void keyPressEvent( QKeyEvent* event );
    void keyReleaseEvent( QKeyEvent* event );
};

} // end of namespace qt

} // end of namespace kvs
