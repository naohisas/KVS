/*****************************************************************************/
/**
 *  @file   main.cpp
 *  @brief  Example program for kvs::PointRenderer class.
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#include <kvs/PointImporter>
#include <kvs/PointObject>
#include <kvs/PointRenderer>
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
    kvs::glut::Application app( argc, argv );
    kvs::glut::Screen screen( &app );
    screen.setGeometry( 0, 0, 512, 512 );
    screen.setTitle( "PointRenderer" );
    screen.show();

    kvs::PointObject* object = NULL;
    if ( argc > 1 )
    {
        object = new kvs::PointImporter( argv[1] );
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
        kvs::glsl::PointRenderer* renderer = new kvs::glsl::PointRenderer();
        screen.registerObject( object, renderer );
    }
    else
    {
        kvs::PointRenderer* renderer = new kvs::PointRenderer();
        screen.registerObject( object, renderer );
    }

    return app.run();
}
