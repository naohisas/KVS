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
#include <kvs/Application>
#include <kvs/Screen>
#include <kvs/opencv/VideoObject>
#include <kvs/opencv/VideoRenderer>
#include <kvs/TimerEventListener>


/*===========================================================================*/
/**
 *  @brief  Main function.
 */
/*===========================================================================*/
int main( int argc, char** argv )
{
    kvs::Application app( argc, argv );
    kvs::Screen screen( &app );

    const auto device_id = 0;
    auto* object = new kvs::opencv::VideoObject( device_id );
    auto* renderer = new kvs::opencv::VideoRenderer();

    auto msec = 0;
    auto redraw = kvs::TimerEventListener(
        [&] ( kvs::TimeEvent* ) { screen.redraw(); },
        msec );

    screen.setTitle( "kvs::opencv::VideoRenderer" );
    screen.setSize( object->width(), object->height() );
    screen.registerObject( object, renderer );
    screen.addEvent( &redraw );
    screen.create();

    return app.run();
}
