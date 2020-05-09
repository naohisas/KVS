/*****************************************************************************/
/**
 *  @file   main.cpp
 *  @brief  Example program for kvs::DiamondGlyph class.
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#include <kvs/Message>
#include <kvs/StructuredVolumeObject>
#include <kvs/StructuredVolumeImporter>
#include <kvs/DiamondGlyph>
#include <kvs/TornadoVolumeData>
#include <kvs/glut/Application>
#include <kvs/glut/Screen>


/*===========================================================================*/
/**
 *  @brief  Main function.
 *  @param  argc [i] argument counter
 *  @param  argv [i] argument values
 */
/*===========================================================================*/
int main( int argc, char** argv )
{
    kvs::glut::Application app( argc, argv );

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

    kvs::glut::Screen screen( &app );
    screen.registerObject( object, glyph );
    screen.setGeometry( 0, 0, 512, 512 );
    screen.setTitle( "kvs::DiamondGlyph" );
    screen.show();

    return app.run();
}
