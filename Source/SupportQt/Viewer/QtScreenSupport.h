/****************************************************************************/
/**
 *  @file QtScreenSupport.h
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: QtScreenSupport.h 634 2010-10-13 07:04:05Z naohisa.sakamoto $
 */
/****************************************************************************/
#ifndef KVS__QT_SCREEN_SUPPORT_H_INCLUDE
#define KVS__QT_SCREEN_SUPPORT_H_INCLUDE

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

    QGLWidget* m_device; ///< reference to the screen device (not allocated)

public:

    QtScreenSupport( QGLWidget* device );

    virtual ~QtScreenSupport( void );

public:

    void attachDevice( QGLWidget* device );

    QGLWidget* device( void );

    const QPoint position( void );

    const QSize size( void );

    void redraw( void );

    void showFullScreen( void );

    void showNormal( void );

    void activateWindow( void );

    void clearFocus( void );

    void resize( int w, int h );

    void move( int x, int y );
};

inline QtScreenSupport::QtScreenSupport( QGLWidget* device )
{
    attachDevice( device );
}

inline QtScreenSupport::~QtScreenSupport( void )
{
}

inline void QtScreenSupport::attachDevice( QGLWidget* device )
{
    m_device = device;
}

inline QGLWidget* QtScreenSupport::device( void )
{
    return( m_device );
}

inline const QPoint QtScreenSupport::position( void )
{
    return( m_device->pos() );
}

inline const QSize QtScreenSupport::size( void )
{
    return( m_device->size() );
}

inline void QtScreenSupport::redraw( void )
{
    m_device->updateGL();
}

inline void QtScreenSupport::showFullScreen( void )
{
    m_device->QWidget::showFullScreen();
}

inline void QtScreenSupport::showNormal( void )
{
    m_device->QWidget::showNormal();
}

inline void QtScreenSupport::activateWindow( void )
{
#if ( KVS_QT_VERSION >= 4 )
    m_device->QWidget::activateWindow();
#else
    m_device->QWidget::setActiveWindow();
#endif
}

inline void QtScreenSupport::clearFocus( void )
{
    m_device->QWidget::clearFocus();
}

inline void QtScreenSupport::resize( int w, int h )
{
    m_device->QWidget::resize( w, h );
}

inline void QtScreenSupport::move( int x, int y )
{
    m_device->QWidget::move( x, y );
}

} // end of namespace kvs

#endif // KVS__QT_SCREEN_SUPPORT_H_INCLUDE
