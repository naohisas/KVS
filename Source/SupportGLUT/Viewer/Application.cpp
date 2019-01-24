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
 *  $Id: Application.cpp 1595 2013-06-12 03:27:28Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#include "Application.h"
#include <kvs/glut/GLUT>
#include <kvs/ScreenBase>
#include <kvs/glut/ScreenBase>
#include <cstdlib>


namespace kvs
{

namespace glut
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new Application class.
 *  @param  argc [in] argument count
 *  @param  argv [in] argument values
 */
/*===========================================================================*/
Application::Application( int argc, char** argv ):
    kvs::ApplicationBase( argc, argv )
{
    static bool flag = true;
    if ( flag )
    {
        glutInit( &argc, argv );
        flag = false;
    }
}

/*===========================================================================*/
/**
 *  @brief  Destructs the Application class.
 */
/*===========================================================================*/
Application::~Application()
{
}

/*===========================================================================*/
/**
 *  @brief  Runs the GLUT application.
 *  @return true, if the process is done successfully
 */
/*===========================================================================*/
int Application::run()
{
    static bool flag = true;
    if ( flag )
    {
        // Call initialize event function for each screen.
        std::list<kvs::ScreenBase*>::iterator screen = screens().begin();
        std::list<kvs::ScreenBase*>::iterator end = screens().end();
        while ( screen != end )
        {
            kvs::glut::ScreenBase* this_screen = static_cast<kvs::glut::ScreenBase*>(*screen);
            this_screen->initializeEvent();
            screen++;
        }

        // Run GLUT main loop.
        glutMainLoop();

        flag = false;
    }

    return true;
}

/*===========================================================================*/
/**
 *  @brief  Quits the application.
 */
/*===========================================================================*/
void Application::quit()
{
    exit(0);
}

} // end of namespace glut

} // end of namespace kvs
