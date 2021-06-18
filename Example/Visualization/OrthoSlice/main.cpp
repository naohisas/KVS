/*****************************************************************************/
/**
 *  @file   main.cpp
 *  @brief  Example program for kvs::OrthoSlice class.
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#include <kvs/Application>
#include <kvs/Screen>
#include <kvs/Message>
#include <kvs/StructuredVolumeObject>
#include <kvs/StructuredVolumeImporter>
#include <kvs/PolygonObject>
#include <kvs/OrthoSlice>
#include <kvs/HydrogenVolumeData>


/*===========================================================================*/
/**
 *  @brief  Main function.
 *  @param  argc [i] argument count
 *  @param  argv [i] argument values
 */
/*===========================================================================*/
int main( int argc, char** argv )
{
    kvs::Application app( argc, argv );
    kvs::Screen screen( &app );
    screen.setTitle( "kvs::OrthoSlice" );
    screen.create();

    // Import volume data as structured volume object.
    auto* volume = [&]() -> kvs::StructuredVolumeObject*
    {
        if ( argc > 1 ) return new kvs::StructuredVolumeImporter( argv[1] );
        else return new kvs::HydrogenVolumeData( { 64, 64, 64 } );
    }();

    // Extract orthogonal planes as polygon object.
    const auto p = volume->resolution().z() * 0.5f; // plane position
    const auto a = kvs::OrthoSlice::ZAxis; // axis of the orthogonal plane
    const auto t = kvs::TransferFunction( 256 ); // transfer function
    auto* object = new kvs::OrthoSlice( volume, p, a, t );
    delete volume;

    screen.registerObject( object );

    return app.run();
}
