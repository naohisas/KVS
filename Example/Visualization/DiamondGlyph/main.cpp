/*****************************************************************************/
/**
 *  @file   main.cpp
 *  @brief  Example program for kvs::DiamondGlyph class.
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#include <kvs/Application>
#include <kvs/Screen>
#include <kvs/Message>
#include <kvs/StructuredVolumeObject>
#include <kvs/StructuredVolumeImporter>
#include <kvs/DiamondGlyph>
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
    screen.setTitle( "kvs::DiamondGlyph" );

    /* Read volume data from the specified data file. If the data file is not
     * specified, tornado volume data is created by using kvs::TornadoVolumeData class.
     */
    kvs::StructuredVolumeObject* object = NULL;
    if ( argc > 1 ) object = new kvs::StructuredVolumeImporter( std::string( argv[1] ) );
    else object = new kvs::TornadoVolumeData( kvs::Vec3u( 8, 8, 8 ) );
    if ( !object )
    {
        kvsMessageError() << "Cannot create a structured volume object." << std::endl;
        return ( false );
    }

    // Create a diamond glyph renderer.
    kvs::DiamondGlyph* glyph = new kvs::DiamondGlyph();
    if ( !glyph )
    {
        kvsMessageError() << "Cannot creat an diamond glyph." << std::endl;
        return ( false );
    }

    // Set properties.
    const kvs::TransferFunction tfunc( 256 );
    glyph->setTransferFunction( tfunc );

    screen.registerObject( object, glyph );
    screen.create();

    return app.run();
}
