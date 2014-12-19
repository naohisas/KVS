/*****************************************************************************/
/**
 *  @file   main.cpp
 *  @brief  Example program of kvs::opencv::VideoRenderer.
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: main.cpp 1319 2012-10-02 10:32:10Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#include <kvs/glut/Application>
#include <kvs/glut/Screen>
#include <kvs/opencv/VideoObject>
#include <kvs/opencv/VideoRenderer>

/*===========================================================================*/
/**
 *  @brief  Main function.
 */
/*===========================================================================*/
int main( int argc, char** argv )
{
    kvs::glut::Application app( argc, argv );

    // Initialize capture device.
    kvs::opencv::CaptureDevice device;
    if ( !device.create( CV_CAP_ANY ) )
    {
        kvsMessageError("Capture device is NULL.");
        return 1;
    }

    const IplImage* frame = device.queryFrame();
    const size_t width = static_cast<size_t>( frame->width );
    const size_t height = static_cast<size_t>( frame->height );

    kvs::opencv::VideoObject* object = new kvs::opencv::VideoObject( CV_CAP_ANY );
    kvs::opencv::VideoRenderer* renderer = new kvs::opencv::VideoRenderer();

    kvs::glut::Screen screen( &app );
    screen.setTitle( "kvs::opencv::VideoRenderer" );
    screen.setSize( width, height );
    screen.registerObject( object, renderer );
    screen.show();

    return app.run();
}
