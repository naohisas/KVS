/*****************************************************************************/
/**
 *  @file   main.cpp
 *  @brief  Example program for kvs::SphereGlyph class.
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: main.cpp 1379 2012-12-03 06:54:05Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#include <kvs/Message>
#include <kvs/StructuredVolumeObject>
#include <kvs/StructuredVolumeImporter>
#include <kvs/SphereGlyph>
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
    else            object = new kvs::TornadoVolumeData( kvs::Vector3ui( 8, 8, 8 ) );

    if ( !object )
    {
        kvsMessageError( "Cannot create a structured volume object." );
        return false;
    }

    // Create an sphere glyph renderer.
    kvs::SphereGlyph* glyph = new kvs::SphereGlyph();
    if ( !glyph )
    {
        kvsMessageError( "Cannot creat a glyph renderer.");
        return false;
    }

    // Set properties.
    const kvs::TransferFunction transfer_function( 256 );
    glyph->setTransferFunction( transfer_function );
    glyph->setNumberOfSlices( 20 );
    glyph->setNumberOfStacks( 20 );

    kvs::glut::Screen screen( &app );
    screen.registerObject( object, glyph );
    screen.setGeometry( 0, 0, 512, 512 );
    screen.setTitle( "kvs::SphereGlyph" );
    screen.show();

    return app.run();
}
