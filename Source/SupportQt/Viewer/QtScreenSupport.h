/****************************************************************************/
/**
 *  @file   QtScreenSupport.h
 *  @author Naohisa Sakamoto
 */
/****************************************************************************/
#pragma once
#include "../Qt.h"
#include <kvs/ClassName>


namespace kvs
{

/*==========================================================================*/
/**
 *  Screen support class.
 */
/*==========================================================================*/
class QtScreenSupport
{
    kvsClassName( kvs::QtScreenSupport );

protected:
//    QGLWidget* m_device = nullptr; ///< reference to the screen device (not allocated)
    QOpenGLWidget* m_device = nullptr; ///< reference to the screen device (not allocated)

public:
//    QtScreenSupport( QGLWidget* device ) { this->attachDevice( device ); }
    QtScreenSupport( QOpenGLWidget* device ) { this->attachDevice( device ); }
    virtual ~QtScreenSupport() {}

//    void attachDevice( QGLWidget* device ) { m_device = device; }
    void attachDevice( QOpenGLWidget* device ) { m_device = device; }
//    QGLWidget* device() { return m_device; }
    QOpenGLWidget* device() { return m_device; }
    const QPoint position() { return m_device->pos(); }
    const QSize size() { return m_device->size(); }
    void redraw() { m_device->updateGL(); }
    void showFullScreen() { m_device->QWidget::showFullScreen(); }
    void showNormal() { m_device->QWidget::showNormal(); }
    void clearFocus() { m_device->QWidget::clearFocus(); }
    void resize( int w, int h ) { m_device->QWidget::resize( w, h ); }
    void move( int x, int y ) { m_device->QWidget::move( x, y ); }
    void activateWindow()
    {
#if ( KVS_QT_VERSION >= 4 )
        m_device->QWidget::activateWindow();
#else
        m_device->QWidget::setActiveWindow();
#endif
    }
};

} // end of namespace kvs
