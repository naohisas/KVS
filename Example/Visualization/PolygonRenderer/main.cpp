/*****************************************************************************/
/**
 *  @file   main.cpp
 *  @brief  Example program for kvs::PolygonRenderer class.
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: main.cpp 1493 2013-04-04 01:11:26Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#include <kvs/PolygonImporter>
#include <kvs/PolygonObject>
#include <kvs/PolygonRenderer>
#include <kvs/glut/Application>
#include <kvs/glut/Screen>


/*===========================================================================*/
/**
 *  @brief  Main function.
 *  @param  argc [i] argument counter
 *  @param  argv [i] argument values
 */
/*===========================================================================*/
int main( int argc, char** argv )
{
    kvs::glut::Application app( argc, argv );
    kvs::glut::Screen screen( &app );
    screen.setGeometry( 0, 0, 512, 512 );
    screen.setTitle( "PolygonRenderer" );
    screen.show();

    kvs::PolygonObject* object = NULL;
    if ( argc > 1 )
    {
        object = new kvs::PolygonImporter( argv[1] );
        object->print( std::cout );
    }
    else
    {
        std::cerr << "Usage: " << argv[0] << " <filename>" << std::endl;
        exit( EXIT_FAILURE );
    }

    bool glsl = true;
    if ( glsl )
    {
        kvs::glsl::PolygonRenderer* renderer = new kvs::glsl::PolygonRenderer();
        screen.registerObject( object, renderer );
    }
    else
    {
        kvs::PolygonRenderer* renderer = new kvs::PolygonRenderer();
        screen.registerObject( object, renderer );
    }

    return app.run();
}
