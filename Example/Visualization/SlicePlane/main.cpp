/*****************************************************************************/
/**
 *  @file   main.cpp
 *  @brief  Example program for kvs::SlicePlane class.
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#include <kvs/Message>
#include <kvs/StructuredVolumeObject>
#include <kvs/StructuredVolumeImporter>
#include <kvs/PolygonObject>
#include <kvs/SlicePlane>
#include <kvs/HydrogenVolumeData>
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

    /* Read volume data from the specified data file. If the data file is not
     * specified, scalar hydrogen volume data is created by using
     * kvs::HydrogenVolumeData class.
     */
    kvs::StructuredVolumeObject* volume = NULL;
    if ( argc > 1 ) volume = new kvs::StructuredVolumeImporter( std::string( argv[1] ) );
    else            volume = new kvs::HydrogenVolumeData( kvs::Vector3ui( 64, 64, 64 ) );

    if ( !volume )
    {
        kvsMessageError() << "Cannot create a structured volume object." << std::endl;
        return ( false );
    }

    /* Extract planes by using SlicePlane class.
     *    c: center of gravity of the volume object.
     *    p: point located on the plane.
     *    n: normal vector of the plane.
     *    t: transfer function.
     */
    const kvs::Vector3f c( ( volume->maxObjectCoord() + volume->minObjectCoord() ) * 0.5f );
    const kvs::Vector3f p( c );
    const kvs::Vector3f n( 1.5, 0.8, 2.0 );
    const kvs::TransferFunction t( 256 );
    kvs::PolygonObject* object = new kvs::SlicePlane( volume, p, n, t );
    if ( !object )
    {
        kvsMessageError() << "Cannot create a polygon object by Slice plane." << std::endl;
        delete volume;
        return ( false );
    }

    delete volume;

    kvs::glut::Screen screen( &app );
    screen.registerObject( object );
    screen.setGeometry( 0, 0, 512, 512 );
    screen.setTitle( "kvs::SlicePlane" );
    screen.show();

    return app.run();
}
