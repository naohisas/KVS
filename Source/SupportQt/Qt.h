/****************************************************************************/
/**
 *  @file   Qt.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: Qt.h 1591 2013-06-05 09:28:54Z naohisa.sakamoto $
 */
/****************************************************************************/
#ifndef KVS__QT__QT_H_INCLUDE
#define KVS__QT__QT_H_INCLUDE

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#if defined( KVS_ENABLE_GLEW )
#include <GL/glew.h>
#endif

#include <string>

/* WARNING */
#ifdef KVS_QT_HAS_QGLOBAL_H
#include <qglobal.h>
#else
#include <QtCore>
#endif

#if   ( QT_VERSION >= 0x020000 && QT_VERSION < 0x030000 )
#define KVS_QT_VERSION 2
#elif ( QT_VERSION >= 0x030000 && QT_VERSION < 0x040000 )
#define KVS_QT_VERSION 3
#elif ( QT_VERSION >= 0x040000 && QT_VERSION < 0x050000 )
#define KVS_QT_VERSION 4
#elif ( QT_VERSION >= 0x050000 && QT_VERSION < 0x060000 )
#define KVS_QT_VERSION 5
#else
#define KVS_QT_VERSION 0
#endif


#if ( KVS_QT_VERSION >= 5 )
#include <QtCore>
#include <QtGui>
#include <QtOpenGL>
#else
#if ( KVS_QT_VERSION == 4 )
#include <QtCore>
#include <QtGui>
#include <QGLWidget>
#include <QGLFramebufferObject>
#include <QGLPixelBuffer>
#if QT_VERSION >= 0x040700
#include <QGLBuffer>
#endif
#else // KVS_QT_VERSION <= 3
#include <qapplication.h>
#include <qnamespace.h>
#include <qtimer.h>
#include <qevent.h>
#include <qgl.h>
#endif
#endif


namespace kvs
{

namespace qt
{

/*===========================================================================*/
/**
 *  @brief  Returns Qt description.
 *  @return description
 */
/*===========================================================================*/
inline std::string Description()
{
    const std::string description( "Qt - Cross-Platform Rich Client Development Framework" );
    return( description );
}

/*===========================================================================*/
/**
 *  @brief  Returns Qt version.
 *  @return Qt version
 */
/*===========================================================================*/
inline std::string Version()
{
    const std::string version( QT_VERSION_STR );
    return version;
}

} // end of namespace qt

} // end of namespace kvs

#endif // KVS__QT__QT_H_INCLUDE
