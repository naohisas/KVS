/*****************************************************************************/
/**
 *  @file   main.cpp
 *  @brief  Example program for kvs::LineRenderer class.
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#include <kvs/Application>
#include <kvs/Screen>
#include <kvs/Message>
#include <kvs/KVSMLLineObject>
#include <kvs/LineImporter>
#include <kvs/LineObject>
#include <kvs/LineRenderer>


/*===========================================================================*/
/**
 *  @brief  Main function.
 *  @param  argc [i] argument count
 *  @param  argv [i] argument values
 *  @return true, if the main process is done succesfully
 */
/*===========================================================================*/
int main( int argc, char** argv )
{
    kvs::Application app( argc, argv );

    const std::string filename( argc > 1 ? argv[1] : "" );
    kvs::LineObject* object = new kvs::LineImporter( filename );
    if ( !object )
    {
        kvsMessageError( "Cannot creat a line object.");
        return ( false );
    }

    kvs::LineRenderer* renderer = new kvs::LineRenderer();
    if ( !renderer )
    {
        kvsMessageError( "Cannot creat a line renderer.");
    }

    kvs::Screen screen( &app );
    screen.registerObject( object, renderer );
    screen.setGeometry( 0, 0, 512, 512 );
    screen.setTitle( "kvs::LineRenderer" );
    screen.create();

    return app.run();
}
