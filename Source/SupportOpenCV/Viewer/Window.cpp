/*****************************************************************************/
/**
 *  @file   Window.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: Window.cpp 602 2010-08-19 02:43:34Z naohisa.sakamoto $
 */
/*****************************************************************************/
#include "Window.h"


namespace kvs
{

namespace opencv
{

/*===========================================================================*/
/**
 *  @brief  Returns the window handle.
 *  @param  name [in] window name
 *  @return window handle
 */
/*===========================================================================*/
void* Window::Handle( const std::string name )
{
    return cvGetWindowHandle( name.c_str() );
}

/*===========================================================================*/
/**
 *  @brief  Returns the window name.
 *  @param  handle [in] window handle
 *  @return window name
 */
/*===========================================================================*/
const char* Window::Name( void* handle )
{
    return cvGetWindowName( handle );
}

/*===========================================================================*/
/**
 *  @brief  Waits for a pressed key.
 *  @param  delay [in] delay in milliseconds
 *  @return pressed key code
 */
/*===========================================================================*/
int Window::WaitKey( const int delay )
{
    return cvWaitKey( delay );
}

/*===========================================================================*/
/**
 *  @brief  Destroys all the window.
 */
/*===========================================================================*/
void Window::DestroyAll()
{
    cvDestroyAllWindows();
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new OpenCV window class.
 */
/*===========================================================================*/
Window::Window():
    m_name("")
{
}

/*===========================================================================*/
/**
 *  @brief  Destructs the OpenCV window class.
 */
/*===========================================================================*/
Window::~Window()
{
    this->destroy();
}

/*===========================================================================*/
/**
 *  @brief  Creates new window.
 *  @param  name [in] window name
 *  @param  flag [in] window flag (CV_WINDOW_AUTOSIZE)
 */
/*===========================================================================*/
int Window::create( const std::string name, int flag )
{
    m_name = name;
    return cvNamedWindow( m_name.c_str(), flag );
}

/*===========================================================================*/
/**
 *  @brief  Destroy the window.
 */
/*===========================================================================*/
void Window::destroy()
{
    cvDestroyWindow( m_name.c_str() );
}

/*===========================================================================*/
/**
 *  @brief  Shows the image.
 *  @param  image [in] pointer to the image
 */
/*===========================================================================*/
void Window::show( const CvArr* image )
{
    cvShowImage( m_name.c_str(), image );
}

/*===========================================================================*/
/**
 *  @brief  Resizes the window.
 *  @param  width [in] window width
 *  @param  height [in] window height
 */
/*===========================================================================*/
void Window::resize( const int width, const int height )
{
    cvResizeWindow( m_name.c_str(), width, height );
}

/*===========================================================================*/
/**
 *  @brief  Moves the window
 *  @param  x [in] x position
 *  @param  y [in] y position
 */
/*===========================================================================*/
void Window::move( const int x, const int y )
{
    cvMoveWindow( m_name.c_str(), x, y );
}

} // end of namespace opencv

} // end of namespace kvs
