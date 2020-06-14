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

    // Read volume data from the specified data file. If the data file is not
    // specified, tornado volume data is created by using kvs::TornadoVolumeData class.
    kvs::StructuredVolumeObject* object = NULL;
    if ( argc > 1 ) object = new kvs::StructuredVolumeImporter( std::string( argv[1] ) );
    else object = new kvs::TornadoVolumeData( kvs::Vec3u( 8, 8, 8 ) );
    if ( !object )
    {
        kvsMessageError() << "Cannot create a structured volume object." << std::endl;
        return ( false );
    }

    // Create an arrow glyph renderer.
    kvs::ArrowGlyph* glyph = new kvs::ArrowGlyph();
    if ( !glyph )
    {
        kvsMessageError() << "Cannot creat an arrow glyph." << std::endl;
        return ( false );
    }

    // Set properties.
    const kvs::TransferFunction tfunc( 256 );
    glyph->setTransferFunction( tfunc );
    glyph->setArrowTypeToTube();

    screen.registerObject( object, glyph );
    screen.create();

    return app.run();
}
