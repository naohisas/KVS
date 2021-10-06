/*****************************************************************************/
/**
 *  @file   Window.cpp
 *  @author Naohisa Sakamoto
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
#if ( CV_MAJOR_VERSION <= 3 )
void* Window::Handle( const std::string name )
{
    return cvGetWindowHandle( name.c_str() );
}
#endif

/*===========================================================================*/
/**
 *  @brief  Returns the window name.
 *  @param  handle [in] window handle
 *  @return window name
 */
/*===========================================================================*/
#if ( CV_MAJOR_VERSION <= 3 )
const char* Window::Name( void* handle )
{
    return cvGetWindowName( handle );
}
#endif

/*===========================================================================*/
/**
 *  @brief  Waits for a pressed key.
 *  @param  delay [in] delay in milliseconds
 *  @return pressed key code
 */
/*===========================================================================*/
int Window::WaitKey( const int delay )
{
#if ( CV_MAJOR_VERSION > 3 )
    return cv::waitKey( delay );
#else
    return cvWaitKey( delay );
#endif
}

/*===========================================================================*/
/**
 *  @brief  Destroys all the window.
 */
/*===========================================================================*/
void Window::DestroyAll()
{
#if ( CV_MAJOR_VERSION > 3 )
    cv::destroyAllWindows();
#else
    cvDestroyAllWindows();
#endif
}

/*===========================================================================*/
/**
 *  @brief  Creates new window.
 *  @param  name [in] window name
 *  @param  flag [in] window flag (Falg::AutoSize)
 */
/*===========================================================================*/
int Window::create( const std::string name, Flag flag )
{
    m_name = name;
#if ( CV_MAJOR_VERSION > 3 )
    cv::namedWindow( m_name, flag );
    return 1;
#else
    return cvNamedWindow( m_name.c_str(), flag );
#endif
}

/*===========================================================================*/
/**
 *  @brief  Destroy the window.
 */
/*===========================================================================*/
void Window::destroy()
{
#if ( CV_MAJOR_VERSION > 3 )
    cv::destroyWindow( m_name );
#else
    cvDestroyWindow( m_name.c_str() );
#endif
}

/*===========================================================================*/
/**
 *  @brief  Shows the image.
 *  @param  image [in] pointer to the image
 */
/*===========================================================================*/
void Window::show( const Window::Image* image )
{
#if ( CV_MAJOR_VERSION > 3 )
    cv::imshow( m_name, image->mat );
#else
    cvShowImage( m_name.c_str(), image );
#endif
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
#if ( CV_MAJOR_VERSION > 3 )
    cv::resizeWindow( m_name, width, height );
#else
    cvResizeWindow( m_name.c_str(), width, height );
#endif
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
#if ( CV_MAJOR_VERSION > 3 )
    cv::moveWindow( m_name, x, y );
#else
    cvMoveWindow( m_name.c_str(), x, y );
#endif
}

} // end of namespace opencv

} // end of namespace kvs
