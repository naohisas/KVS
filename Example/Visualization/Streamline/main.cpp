/*****************************************************************************/
/**
 *  @file   main.cpp
 *  @brief  Example program for kvs::Streamline class.
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#include <kvs/Application>
#include <kvs/Screen>
#include <kvs/Message>
#include <kvs/StructuredVolumeObject>
#include <kvs/StructuredVolumeImporter>
#include <kvs/LineObject>
#include <kvs/Streamline>
#include <kvs/TornadoVolumeData>


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
    screen.setTitle( "kvs::Streamline" );
    screen.create();

    // Import volume data as structured volume object.
    auto* volume = [&]() -> kvs::StructuredVolumeObject*
    {
        if ( argc > 1 ) return new kvs::StructuredVolumeImporter( argv[1] );
        else return new kvs::TornadoVolumeData( { 32, 32, 32 } );
    }();

    // Generate seed points as point object.
    const kvs::Vec3i min_coord( 15, 15,  0 );
    const kvs::Vec3i max_coord( 20, 20, 30 );
    auto* point = new kvs::PointObject;
    point->setCoords( [&]
    {
        std::vector<kvs::Real32> v;
        for ( int k = min_coord.z(); k < max_coord.z(); k++ )
        {
            for ( int j = min_coord.y(); j < max_coord.y(); j++ )
            {
                for ( int i = min_coord.x(); i < max_coord.x(); i++ )
                {
                    v.push_back( static_cast<kvs::Real32>(i) );
                    v.push_back( static_cast<kvs::Real32>(j) );
                    v.push_back( static_cast<kvs::Real32>(k) );
                }
            }
        }
        return kvs::ValueArray<kvs::Real32>( v );
    } () );

    // Generate streamlines as line object.
    const auto tfunc = kvs::TransferFunction( 256 ); // transfer function
    auto* object = new kvs::Streamline( volume, point, tfunc );
    delete point;
    delete volume;

    screen.registerObject( object );

    return app.run();
}
