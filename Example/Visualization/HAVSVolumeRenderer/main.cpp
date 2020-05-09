/*****************************************************************************/
/**
 *  @file   main.cpp
 *  @author Naohisa Sakamoto
 *  @brief  Example program for kvs::glew::HAVSVolumeRenderer class.
 */
/*****************************************************************************/
#include <kvs/glut/Application>
#include <kvs/glut/Screen>
#include <kvs/UnstructuredVolumeObject>
#include <kvs/UnstructuredVolumeImporter>
#include <kvs/HAVSVolumeRenderer>
#include <cstdlib>


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
    screen.setTitle( "kvs::HAVSVolumeRenderer" );

    kvs::UnstructuredVolumeObject* object = NULL;
    if ( argc > 1 )
    {
        const std::string filename( argv[1] );
        object = new kvs::UnstructuredVolumeImporter( filename );
    }
    else if ( const char* kvs_data_dir = std::getenv( "KVS_DATA_DIR" ) )
    {
        const std::string filename = std::string( kvs_data_dir ) + "/spx.inp";
        object = new kvs::UnstructuredVolumeImporter( filename );
    }
    else
    {
        kvsMessageError() << "Cannot import data file." << std::endl;
        return ( false );
    }

    kvs::HAVSVolumeRenderer* renderer = new kvs::HAVSVolumeRenderer();
    renderer->setKBufferSize( 2 );

    screen.registerObject( object, renderer );
    screen.show();

    return app.run();
}
