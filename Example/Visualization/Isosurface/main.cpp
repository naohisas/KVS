/*****************************************************************************/
/**
 *  @file   main.cpp
 *  @brief  Example program for kvs::Isosurface class.
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#include <kvs/Application>
#include <kvs/Screen>
#include <kvs/Message>
#include <kvs/StructuredVolumeObject>
#include <kvs/StructuredVolumeImporter>
#include <kvs/PolygonObject>
#include <kvs/Isosurface>
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
    screen.setTitle( "kvs::Isosurface" );
    screen.create();

    // Import volume data as structured volume object.
    auto* volume = [&]() -> kvs::StructuredVolumeObject*
    {
        if ( argc > 1 ) return new kvs::StructuredVolumeImporter( argv[1] );
        else return new kvs::HydrogenVolumeData( { 64, 64, 64 } );
    }();

    // Extract isosurfaces as polygon object.
    const auto i = ( volume->maxValue() + volume->minValue() ) * 0.5; // isolevel
    const auto n = kvs::PolygonObject::VertexNormal; // normal type
    const auto d = false; // false: duplicated vertices will be removed
    const auto t = kvs::TransferFunction( 256 ); // transfer function
    auto* object = new kvs::Isosurface( volume, i, n, d, t );
    delete volume;

    screen.registerObject( object );

    return app.run();
}
