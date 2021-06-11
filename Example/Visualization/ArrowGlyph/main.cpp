/*****************************************************************************/
/**
 *  @file   main.cpp
 *  @brief  Example program for kvs::ArrowGlyph class.
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#include <kvs/Application>
#include <kvs/Screen>
#include <kvs/Message>
#include <kvs/StructuredVolumeObject>
#include <kvs/StructuredVolumeImporter>
#include <kvs/ArrowGlyph>
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
    screen.setTitle( "kvs::ArrowGlyph" );
    screen.create();

    // Import volume data as structured volume object.
    auto* object = [&]() -> kvs::StructuredVolumeObject*
    {
        if ( argc > 1 ) return new kvs::StructuredVolumeImporter( argv[1] );
        else return new kvs::TornadoVolumeData( { 8, 8, 8 } );
    }();

    // Create an arrow glyph renderer.
    const auto tfunc = kvs::TransferFunction( 256 ); // transfer function
    auto* glyph = new kvs::ArrowGlyph();
    glyph->setTransferFunction( tfunc );
    glyph->setArrowTypeToTube();

    screen.registerObject( object, glyph );

    return app.run();
}
