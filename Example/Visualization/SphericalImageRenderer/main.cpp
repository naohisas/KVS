/*****************************************************************************/
/**
 *  @file   main.cpp
 *  @author Naohisa Sakamoto
 *  @brief  Example program for kvs::SphericalImageRenderer class.
 */
/*****************************************************************************/
#include <kvs/ImageObject>
#include <kvs/ImageImporter>
#include <kvs/SphericalImageRenderer>
#include <kvs/glut/Application>
#include <kvs/glut/Screen>


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
    kvs::glut::Application app( argc, argv );
    kvs::glut::Screen screen( &app );
    screen.setTitle( "kvs::SphericalImageRenderer" );

    const std::string filename( argc > 1 ? argv[1] : "" );
    kvs::ImageObject* object = new kvs::ImageImporter( filename );
    kvs::SphericalImageRenderer* renderer = new kvs::SphericalImageRenderer();

    screen.registerObject( object, renderer );
    screen.show();

    return app.run();
}
