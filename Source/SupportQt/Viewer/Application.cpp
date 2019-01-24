/*****************************************************************************/
/**
 *  @file   Application.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: Application.cpp 1443 2013-03-18 07:00:28Z naohisa.sakamoto@gmail.com $
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
    m_app = new QApplication( argc, argv );
}

/*===========================================================================*/
/**
 *  @brief  Destructs the Application class.
 */
/*===========================================================================*/
Application::~Application()
{
    if ( m_app ) delete m_app;
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
    if ( flag )
    {
        flag = false;
        return m_app->exec();
    }

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
