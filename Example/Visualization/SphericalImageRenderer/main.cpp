/*****************************************************************************/
/**
 *  @file   main.cpp
 *  @author Naohisa Sakamoto
 *  @brief  Example program for kvs::SphericalImageRenderer class.
 */
/*****************************************************************************/
#include <kvs/Application>
#include <kvs/Screen>
#include <kvs/ImageObject>
#include <kvs/ImageImporter>
#include <kvs/SphericalImageRenderer>


/*===========================================================================*/
/**
 *  @brief  Main function.
 *  @param  argc [i] argument counter
 *  @param  argv [i] argument values
 *  @return true, if the main process is done succesfully
 */
/*===========================================================================*/
int main( int argc, char** argv )
{
    kvs::Application app( argc, argv );
    kvs::Screen screen( &app );
    screen.setTitle( "kvs::SphericalImageRenderer" );

    const std::string filename( argc > 1 ? argv[1] : "" );
    kvs::ImageObject* object = new kvs::ImageImporter( filename );
    kvs::SphericalImageRenderer* renderer = new kvs::SphericalImageRenderer();

    screen.registerObject( object, renderer );
    screen.create();

    return app.run();
}
