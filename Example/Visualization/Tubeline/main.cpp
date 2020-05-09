/*****************************************************************************/
/**
 *  @file   main.cpp
 *  @brief  Example program for kvs::Tubeline class.
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#include <kvs/Message>
#include <kvs/StructuredVolumeObject>
#include <kvs/StructuredVolumeImporter>
#include <kvs/LineObject>
#include <kvs/LineRenderer>
#include <kvs/Streamline>
#include <kvs/Tubeline>
#include <kvs/TornadoVolumeData>
#include <kvs/PolygonObject>
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
    else volume = new kvs::TornadoVolumeData( kvs::Vec3u( 16, 16, 16 ) );

    std::vector<kvs::Real32> v;
    kvs::Vec3i min_coord( 7, 7,  0 );
    kvs::Vec3i max_coord( 10, 10, 15 );
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
    kvs::PointObject* point = new kvs::PointObject;
    point->setCoords( kvs::ValueArray<kvs::Real32>( v ) );

    const kvs::TransferFunction transfer_function( 256 );
    kvs::LineObject* line = new kvs::Streamline( volume, point, transfer_function );
    line->setSize( 0.2 );

    delete point;
    delete volume;

    const size_t ndivisions = 20;
    kvs::PolygonObject* object = new kvs::Tubeline( line, ndivisions );
    delete line;

    kvs::glut::Screen screen( &app );
    screen.registerObject( object );
    screen.setGeometry( 0, 0, 512, 512 );
    screen.setTitle( "kvs::Tubeline" );
    screen.show();

    return( app.run() );
}
