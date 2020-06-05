/*****************************************************************************/
/**
 *  @file   main.cpp
 *  @brief  Example program for kvs::SphereGlyph class.
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#include <kvs/Application>
#include <kvs/Screen>
#include <kvs/Message>
#include <kvs/StructuredVolumeObject>
#include <kvs/StructuredVolumeImporter>
#include <kvs/SphereGlyph>
#include <kvs/TornadoVolumeData>


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
    kvs::Application app( argc, argv );

    /* Read volume data from the specified data file. If the data file is not
     * specified, tornado volume data is created by using kvs::TornadoVolumeData class.
     */
    kvs::StructuredVolumeObject* object = NULL;
    if ( argc > 1 ) object = new kvs::StructuredVolumeImporter( std::string( argv[1] ) );
    else object = new kvs::TornadoVolumeData( kvs::Vector3ui( 8, 8, 8 ) );
    if ( !object )
    {
        kvsMessageError() << "Cannot create a structured volume object." << std::endl;
        return ( false );
    }

    // Create an sphere glyph renderer.
    kvs::SphereGlyph* glyph = new kvs::SphereGlyph();
    if ( !glyph )
    {
        kvsMessageError() << "Cannot creat a glyph renderer." << std::endl;
        return ( false );
    }

    // Set properties.
    const kvs::TransferFunction tfunc( 256 );
    glyph->setTransferFunction( tfunc );
    glyph->setNumberOfSlices( 20 );
    glyph->setNumberOfStacks( 20 );

    kvs::Screen screen( &app );
    screen.registerObject( object, glyph );
    screen.setGeometry( 0, 0, 512, 512 );
    screen.setTitle( "kvs::SphereGlyph" );
    screen.create();

    return app.run();
}
