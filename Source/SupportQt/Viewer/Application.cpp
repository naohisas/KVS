/*****************************************************************************/
/**
 *  @file   Application.cpp
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#include "Application.h"
#include <QApplication>
#include <kvs/DebugNew>


namespace kvs
{

namespace qt
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new Application class.
 *  @param  argc [in] argument count
 *  @param  argv [in] argument values
 */
/*===========================================================================*/
Application::Application( int& argc, char** argv ):
    kvs::ApplicationBase( argc, argv )
{
#if ( QT_VERSION >= QT_VERSION_CHECK( 5, 5, 0 ) )
    // NOTE: Avoiding problems related to the OpenGL context when developing
    // linked-view applications using the QMainWinodow and QDockWidget classes.
    // 'Qt::AA_ShareOpenGLContexts' must be set before instantiating QApplication.
    QApplication::setAttribute( Qt::AA_ShareOpenGLContexts );
#endif
    m_app = new QApplication( argc, argv );

#if ( KVS_QT_VERSION >= 6 )
    const QRect desk = QGuiApplication::primaryScreen()->geometry();
#else
    const QRect desk = QApplication::desktop()->screenGeometry();
#endif
    BaseClass::setDesktop( { desk.width(), desk.height() } );
}

/*===========================================================================*/
/**
 *  @brief  Destructs the Application class.
 */
/*===========================================================================*/
Application::~Application()
{
    if ( m_app ) { delete m_app; }
}

/*===========================================================================*/
/**
 *  @brief  Runs the Qt application.
 *  @return true, if the process is done successfully
 */
/*===========================================================================*/
int Application::run()
{
    static bool flag = true;
    if ( flag ) { flag = false; return m_app->exec(); }
    return -1;
}

/*===========================================================================*/
/**
 *  @brief  Quits the application.
 */
/*===========================================================================*/
void Application::quit()
{
    m_app->exit();
}

} // end of namespace qt

} // end of namespace kvs
