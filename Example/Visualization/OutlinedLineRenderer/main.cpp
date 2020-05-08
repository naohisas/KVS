/*****************************************************************************/
/**
 *  @file   main.cpp
 *  @brief  Example program for kvs::Streamline class.
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#include <kvs/Message>
#include <kvs/StructuredVolumeObject>
#include <kvs/StructuredVolumeImporter>
#include <kvs/LineObject>
#include <kvs/LineRenderer>
#include <kvs/Streamline>
#include <kvs/TornadoVolumeData>
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

    /* Read volume data from the specified data file. If the data file is not
     * specified, scalar hydrogen volume data is created by using
     * kvs::HydrogenVolumeData class.
     */
    kvs::StructuredVolumeObject* volume = NULL;
    if ( argc > 1 ) volume = new kvs::StructuredVolumeImporter( std::string( argv[1] ) );
    else            volume = new kvs::TornadoVolumeData( kvs::Vector3ui( 32, 32, 32 ) );

    std::vector<kvs::Real32> v;
    kvs::Vector3i min_coord( 15, 15,  0 );
    kvs::Vector3i max_coord( 20, 20, 30 );
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
    kvs::LineObject* object = new kvs::Streamline( volume, point, transfer_function );
    object->setSize( 3 );

    delete volume;
    delete point;

    kvs::glsl::LineRenderer* renderer = new kvs::glsl::LineRenderer();
    renderer->setOutlineWidth( 1 );
    renderer->setOutlineColor( kvs::RGBColor::Black() );

    kvs::glut::Screen screen( &app );
    screen.registerObject( object, renderer );
    screen.setGeometry( 0, 0, 512, 512 );
    screen.setTitle( "kvs::Streamline" );
    screen.show();

    return( app.run() );
}
