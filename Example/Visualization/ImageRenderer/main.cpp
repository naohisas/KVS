/*****************************************************************************/
/**
 *  @file   main.cpp
 *  @brief  Example program for kvs::ImageRenderer class.
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#include <kvs/Message>
#include <kvs/ImageObject>
#include <kvs/ImageImporter>
#include <kvs/ImageRenderer>
#include <kvs/glut/Application>
#include <kvs/glut/Screen>


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
    // GLUT viewer application.
    kvs::glut::Application app( argc, argv );

    // Create an image object.
    const std::string filename( argc > 1 ? argv[1] : "" );
    kvs::ImageObject* object = new kvs::ImageImporter( filename );
    if ( !object )
    {
        kvsMessageError() << "Cannot creat an image object." << std::endl;
        return ( false );
    }

    // Create an image renderer.
    kvs::ImageRenderer* renderer = new kvs::ImageRenderer();
    if ( !renderer )
    {
        kvsMessageError() << "Cannot create an image renderer." << std::endl;
        delete object;
        return ( false );
    }

    // Screen.
    kvs::glut::Screen screen( &app );
    screen.registerObject( object, renderer );
    screen.setSize( object->width(), object->height() );
    screen.setTitle( "kvs::ImageRenderer" );
    screen.show();

    return app.run();
}
