/*****************************************************************************/
/**
 *  @file   Window.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#pragma once
#include <kvs/opencv/OpenCV>
#include <kvs/opencv/CaptureDevice>
#include <string>
#include <kvs/ClassName>
#include <kvs/Deprecated>


namespace kvs
{

namespace opencv
{

/*===========================================================================*/
/**
 *  @brief  OpenCV window class.
 */
/*===========================================================================*/
class Window
{
    kvsClassName_without_virtual( kvs::opencv::Window );

public:
    using Image = kvs::opencv::CaptureDevice::Frame;

#if ( CV_MAJOR_VERSION > 3 )
    enum Flag
    {
        Normal = cv::WINDOW_NORMAL,
        AutoSize = cv::WINDOW_AUTOSIZE,
        OpenGL = cv::WINDOW_OPENGL,
        FullScreen = cv::WINDOW_FULLSCREEN,
        FreeRatio = cv::WINDOW_FREERATIO,
        KeepRatio = cv::WINDOW_KEEPRATIO,
        GUIExpanded = cv::WINDOW_GUI_EXPANDED,
        GUINormal = cv::WINDOW_GUI_NORMAL
    };
#else
    enum Flag
    {
        Normal = CV_WINDOW_NORMAL,
        AutoSize = CV_WINDOW_AUTOSIZE,
        OpenGL = CV_WINDOW_OPENGL,
        FullScreen = CV_WINDOW_FULLSCREEN,
        FreeRatio = CV_WINDOW_FREERATIO,
        KeepRatio = CV_WINDOW_KEEPRATIO,
        GUIExpanded = CV_GUI_EXPANDED,
        GUINormal = CV_GUI_NORMAL
    };
#endif

public:
#if ( CV_MAJOR_VERSION > 3 )
    KVS_DEPRECATED( static void* Handle( const std::string ) ) { return nullptr; } // DO NOT USE (NOT WORKING)
    KVS_DEPRECATED( static const char* Name( void* ) ) { return nullptr; } // DO NOT USE (NOT WORKING)
#else
    KVS_DEPRECATED( static void* Handle( const std::string name ) ); // DO NOT USE
    KVS_DEPRECATED( static const char* Name( void* handle ) ); // NO NOT USE
#endif
    static int WaitKey( const int delay = 0 );
    static void DestroyAll();

private:
    std::string m_name = ""; ///< window name

public:
    Window() = default;
    ~Window() { this->destroy(); }

    const std::string& name() const { return m_name; }

    int create( const std::string name, Flag flag = Flag::AutoSize );
    void destroy();
    void show( const Image* image );
    void resize( const int width, const int height );
    void move( const int x, const int y );
};

} // end of namespace opencv

} // end of namespace kvs
