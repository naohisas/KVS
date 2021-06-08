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
 */
/*===========================================================================*/
int main( int argc, char** argv )
{
    kvs::Application app( argc, argv );
    kvs::Screen screen( &app );
    screen.setTitle( "kvs::SphereGlyph" );
    screen.create();

    // Import volume data.
    kvs::StructuredVolumeObject* object = NULL;
    if ( argc > 1 ) object = new kvs::StructuredVolumeImporter( argv[1] );
    else object = new kvs::TornadoVolumeData( { 8, 8, 8 } );

    // Create an sphere glyph renderer.
    const kvs::TransferFunction tfunc( 256 );
    auto* glyph = new kvs::SphereGlyph();
    glyph->setTransferFunction( tfunc );
    glyph->setNumberOfSlices( 20 );
    glyph->setNumberOfStacks( 20 );

    screen.registerObject( object, glyph );

    return app.run();
}
