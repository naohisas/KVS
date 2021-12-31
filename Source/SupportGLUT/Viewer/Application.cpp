/*****************************************************************************/
/**
 *  @file   Application.cpp
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#include "Application.h"
#include <kvs/glut/GLUT>
#include <kvs/ScreenBase>
#include <kvs/glut/ScreenBase>
#include <kvs/EventListener>
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

        const int width = glutGet( GLUT_SCREEN_WIDTH );
        const int height = glutGet( GLUT_SCREEN_HEIGHT );
        BaseClass::setDesktop( { width, height } );
    }
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
        for ( auto& s : BaseClass::screens() )
        {
            auto* screen = kvs::glut::ScreenBase::DownCast( s );
            screen->initializeEvent();

            // Start time events registered in the screen.
            for ( auto& l : screen->eventHandler()->listeners() )
            {
                if ( l->eventType() & kvs::EventBase::TimerEvent )
                {
                    auto* t = l->eventTimer();
                    if ( t )
                    {
                        auto i = l->timerInterval();
                        l->eventTimer()->start( i );
                    }
                }
            }
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
