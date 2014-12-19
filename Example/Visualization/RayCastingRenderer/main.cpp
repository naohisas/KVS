/*****************************************************************************/
/**
 *  @file   main.cpp
 *  @brief  Example program for kvs::RayCastingRenderer class.
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: main.cpp 1493 2013-04-04 01:11:26Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#include <kvs/StructuredVolumeObject>
#include <kvs/StructuredVolumeImporter>
#include <kvs/RayCastingRenderer>
#include <kvs/HydrogenVolumeData>
#include <kvs/TransferFunction>
#include <kvs/glut/Application>
#include <kvs/glut/Screen>
#include <iostream>


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
    kvs::glut::Screen screen( &app );
    screen.setGeometry( 0, 0, 512, 512 );
    screen.setTitle( "RayCastingRenderer" );
    screen.show();

    // Read volume data from the specified data file. If the data file is not
    // specified, scalar hydrogen volume data will be created by using
    // kvs::HydrogenVolumeData class.
    kvs::StructuredVolumeObject* object = NULL;
    if ( argc > 1 )
    {
        object = new kvs::StructuredVolumeImporter( std::string( argv[1] ) );
        object->print( std::cout );
    }
    else
    {
        object = new kvs::HydrogenVolumeData( kvs::Vector3ui( 32, 32, 32 ) );
        object->print( std::cout );
    }

    // Parameters for the ray casting renderer.
    kvs::Real32 sampling_step = 0.5f;
    kvs::Real32 opaque_value = 0.97f;
    kvs::TransferFunction transfer_function( 256 );

    bool glsl = false;
    if ( glsl )
    {
        // Hardware accelerated ray casting renderer with GLSL.
        kvs::glsl::RayCastingRenderer* renderer = new kvs::glsl::RayCastingRenderer();
        renderer->setSamplingStep( sampling_step );
        renderer->setOpaqueValue( opaque_value );
        renderer->setTransferFunction( transfer_function );
        screen.registerObject( object, renderer );
    }
    else
    {
        // Software based ray casting renderer.
        kvs::RayCastingRenderer* renderer = new kvs::RayCastingRenderer();
        renderer->setSamplingStep( 0.5f );
        renderer->setOpaqueValue( 0.97f );
        renderer->setTransferFunction( transfer_function );
        renderer->enableLODControl();
        screen.registerObject( object, renderer );
    }

    return app.run();
}
