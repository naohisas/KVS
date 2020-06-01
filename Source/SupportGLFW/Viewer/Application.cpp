/*****************************************************************************/
/**
 *  @file   Application.cpp
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#include "Application.h"
#include "../GLFW.h"
#include "ScreenBase.h"
#include <kvs/Message>
#include <cstdlib>


namespace
{

/*===========================================================================*/
/**
 *  @brief  Callback function, which is called when an error occurs.
 *  @param  error [in] error code
 *  @param  message [in] error message
 */
/*===========================================================================*/
void ErrorCallback( int error, const char* message )
{
    kvsMessageError() << "GLFW error: " << std::string( message ) << std::endl;
}

}

namespace kvs
{

namespace glfw
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
        if ( !glfwInit() )
        {
            kvsMessageError() << "GLFW initialization failed." << std::endl;
            exit( EXIT_FAILURE );
        }
        flag = false;
        atexit( glfwTerminate );
    }
}

/*===========================================================================*/
/**
 *  @brief  Destructs the application class.
 */
/*===========================================================================*/
Application::~Application()
{
}

/*===========================================================================*/
/**
 *  @brief  Runs the application.
 *  @return true, if the application has been executed successfully
 */
/*===========================================================================*/
int Application::run()
{
    static bool flag = true;
    if ( flag )
    {
        glfwSetErrorCallback( ::ErrorCallback );

        // Call initialize event function for each screen.
        for ( auto& s : BaseClass::screens() )
        {
            auto* screen = kvs::glfw::ScreenBase::DownCast( s );
            screen->aquireContext();
            screen->initializeEvent();
            screen->releaseContext();
        }

        this->main_loop();
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
    exit( EXIT_SUCCESS );
}

/*===========================================================================*/
/**
 *  @brief  Main application loop.
 */
/*===========================================================================*/
void Application::main_loop()
{
    while ( !this->should_close() )
    {
        bool focused = false;
        for ( auto& s : BaseClass::screens() )
        {
            auto* screen = kvs::glfw::ScreenBase::DownCast( s );
            screen->aquireContext();
            screen->paintEvent();

            for ( auto& t : screen->timerEventHandler() )
            {
                t->timerEvent();
            }

            if ( glfwGetWindowAttrib( screen->handler(), GLFW_FOCUSED ) )
            {
                glfwPollEvents();
                focused = true;
            }

            screen->releaseContext();
        }

        if ( !focused ) { glfwPollEvents(); }
    }
}

/*===========================================================================*/
/**
 *  @brief  Checks the close flags of the registered screens.
 *  @return true, if all of the screens have been closed.
 */
/*===========================================================================*/
bool Application::should_close()
{
    if ( screens().empty() ) { return true; }

    std::list<kvs::ScreenBase*> screen_list;
    for ( auto& s : BaseClass::screens() )
    {
        auto* screen = kvs::glfw::ScreenBase::DownCast( s );
        if ( glfwWindowShouldClose( screen->handler() ) )
        {
            screen_list.push_back( s );
        }
    }

    if ( !screen_list.empty() )
    {
        for ( auto& s : BaseClass::screens() )
        {
            BaseClass::screens().remove( s );
        }
    }

    if ( BaseClass::screens().empty() ) { return true; }

    return false;
}

} // end of namespace glfw

} // end of namespace kvs
