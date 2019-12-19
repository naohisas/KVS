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
#include <kvs/glut/Application>
#include <kvs/glut/Screen>
#include <kvs/PolygonImporter>
#include <kvs/PolygonObject>
#include <kvs/PolygonRenderer>
#include <kvs/Message>
#include <cstdlib>


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

    kvs::PolygonObject* object = NULL;
    if ( argc > 1 )
    {
        const std::string filename( argv[1] );
        object = new kvs::PolygonImporter( filename );
    }
    else if ( const char* kvs_data_dir = std::getenv( "KVS_DATA_DIR" ) )
    {
        const std::string filename = std::string( kvs_data_dir ) + "/bunny.ply";
        object = new kvs::PolygonImporter( filename );
    }
    else
    {
        kvsMessageError() << "Cannot import polygon data file." << std::endl;
        return (false);
    }

    bool glsl = true;
    if ( glsl )
    {
        kvs::glsl::PolygonRenderer* renderer = new kvs::glsl::PolygonRenderer();
        renderer->setShader( kvs::Shader::Phong() );
        screen.setTitle( "kvs::glsl::PolygonRenderer" );
        screen.registerObject( object, renderer );
    }
    else
    {
        kvs::PolygonRenderer* renderer = new kvs::PolygonRenderer();
        screen.setTitle( "kvs::PolygonRenderer" );
        screen.registerObject( object, renderer );
    }

    screen.show();

    return app.run();
}
