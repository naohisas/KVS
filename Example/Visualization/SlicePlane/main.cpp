/*****************************************************************************/
/**
 *  @file   main.cpp
 *  @brief  Example program for kvs::SlicePlane class.
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#include <kvs/Application>
#include <kvs/Screen>
#include <kvs/Message>
#include <kvs/StructuredVolumeObject>
#include <kvs/StructuredVolumeImporter>
#include <kvs/PolygonObject>
#include <kvs/SlicePlane>
#include <kvs/HydrogenVolumeData>


/*===========================================================================*/
/**
 *  @brief  Main function.
 *  @param  argc [i] argument counter
 *  @param  argv [i] argument values
 */
/*===========================================================================*/
int main( int argc, char** argv )
{
    kvs::Application app( argc, argv );
    kvs::Screen screen( &app );
    screen.setTitle( "kvs::SlicePlane" );
    screen.create();

    // Import volume data as structured volume object.
    auto* volume = [&]() -> kvs::StructuredVolumeObject*
    {
        if ( argc > 1 ) return new kvs::StructuredVolumeImporter( argv[1] );
        else return new kvs::HydrogenVolumeData( { 64, 64, 64 } );
    }();

    // Extract planes as polygon object.
    const auto c = ( volume->maxObjectCoord() + volume->minObjectCoord() ) * 0.5f; // center
    const auto p = c; // point located on the plane
    const auto n = kvs::Vec3( 1.5, 0.8, 2.0 ); // normal vector of the plane
    const auto t = kvs::TransferFunction( 256 ); // transfer function
    auto* object = new kvs::SlicePlane( volume, p, n, t );
    delete volume;

    screen.registerObject( object );

    return app.run();
}
