/*****************************************************************************/
/**
 *  @file   main.cpp
 *  @brief  Example program for kvs::ImageRenderer class.
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#include <kvs/Application>
#include <kvs/Screen>
#include <kvs/Message>
#include <kvs/ImageObject>
#include <kvs/ImageImporter>
#include <kvs/ImageRenderer>


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
    kvs::Screen screen( &app );
    screen.setTitle( "kvs::ImageRenderer" );
    screen.create();

    // Create an image object.
    const std::string filename( argc > 1 ? argv[1] : "" );
    auto* object = new kvs::ImageImporter( filename );
    if ( !object )
    {
        kvsMessageError() << "Cannot creat an image object." << std::endl;
        return ( false );
    }

    // Create an image renderer.
    auto* renderer = new kvs::ImageRenderer();
    if ( !renderer )
    {
        kvsMessageError() << "Cannot create an image renderer." << std::endl;
        delete object;
        return ( false );
    }

    screen.setSize( object->width(), object->height() );
    screen.registerObject( object, renderer );

    return app.run();
}
