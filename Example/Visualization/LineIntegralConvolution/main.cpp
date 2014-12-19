/*****************************************************************************/
/**
 *  @file   main.cpp
 *  @brief  Example program for kvs::LineIntegralConvolution class.
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: main.cpp 602 2010-08-19 02:43:34Z naohisa.sakamoto $
 */
/*****************************************************************************/
#include <kvs/Message>
#include <kvs/StructuredVolumeObject>
#include <kvs/StructuredVolumeImporter>
#include <kvs/LineIntegralConvolution>
#include <kvs/RayCastingRenderer>
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
     * specified, scalar hydrogen volume data is created by using
     * kvs::HydrogenVolumeData class.
     */
    kvs::StructuredVolumeObject* volume = NULL;
    if ( argc > 1 ) volume = new kvs::StructuredVolumeImporter( std::string( argv[1] ) );
    else            volume = new kvs::TornadoVolumeData( kvs::Vector3ui( 32, 32, 32 ) );

    if ( !volume )
    {
        kvsMessageError( "Cannot create a structured volume object." );
        return( false );
    }

    kvs::StructuredVolumeObject* object = new kvs::LineIntegralConvolution( volume , 0.2 );
    if ( !object )
    {
        kvsMessageError( "Cannot create LIC structured volume object.");
        delete volume;
        return( false );
    }

    delete volume;

    kvs::RayCastingRenderer* renderer = new kvs::RayCastingRenderer();
    if ( !renderer )
    {
        kvsMessageError( "Cannot create a ray-casting renderer." );
        delete object;
        return( false );
    }

    kvs::glut::Screen screen( &app );
    screen.registerObject( object, renderer );
    screen.setGeometry( 0, 0, 512, 512 );
    screen.setTitle( "kvs::LineIntegralConvolution" );
    screen.show();

    return( app.run() );
}
