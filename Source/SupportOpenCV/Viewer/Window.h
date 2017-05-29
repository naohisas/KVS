/*****************************************************************************/
/**
 *  @file   Window.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: Window.h 602 2010-08-19 02:43:34Z naohisa.sakamoto $
 */
/*****************************************************************************/
#ifndef KVS__OPENCV__WINDOW_H_INCLUDE
#define KVS__OPENCV__WINDOW_H_INCLUDE

#include <kvs/opencv/OpenCV>
#include <string>
#include <kvs/ClassName>


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
    static void* Handle( const std::string name );
    static const char* Name( void* handle );
    static int WaitKey( const int delay = 0 );
    static void DestroyAll();

private:
    std::string m_name; ///< window name

public:
    Window();
    ~Window();

    const std::string& name() const { return m_name; }

    int create( const std::string name, int flag = CV_WINDOW_AUTOSIZE );
    void destroy();
    void show( const CvArr* image );
    void resize( const int width, const int height );
    void move( const int x, const int y );
};

} // end of namespace opencv

} // end of namespace kvs

#endif // KVS__OPENCV__WINDOW_H_INCLUDE
