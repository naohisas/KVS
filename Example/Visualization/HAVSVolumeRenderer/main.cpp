/*****************************************************************************/
/**
 *  @file   main.cpp
 *  @author Naohisa Sakamoto
 *  @brief  Example program for kvs::glew::HAVSVolumeRenderer class.
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id$
 */
/*****************************************************************************/
#include <kvs/glut/Application>
#include <kvs/glut/Screen>
#include <kvs/UnstructuredVolumeObject>
#include <kvs/UnstructuredVolumeImporter>
#include <kvs/HAVSVolumeRenderer>


/*===========================================================================*/
/**
 *  @brief  Main function.
 *  @param  argc [i] argument count
 *  @param  argv [i] argument values
 */
/*===========================================================================*/
int main( int argc, char** argv )
{
    kvs::glut::Application app( argc, argv );
    kvs::glut::Screen screen;
    screen.setGeometry( 0, 0, 512, 512 );
    screen.setTitle( "HAVSVolumeRenderer" );
    screen.show();

    kvs::UnstructuredVolumeObject* object = new kvs::UnstructuredVolumeImporter( argv[1] );
    object->print( std::cout );

    kvs::HAVSVolumeRenderer* renderer = new kvs::HAVSVolumeRenderer();
    renderer->setKBufferSize( 2 );

    screen.registerObject( object, renderer );

    return app.run();
}
