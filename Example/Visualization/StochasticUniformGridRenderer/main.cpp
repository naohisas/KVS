/*****************************************************************************/
/**
 *  @file   main.cpp
 *  @brief  Example program for kvs::StochasticUniformGridRenderer class.
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#include <kvs/Application>
#include <kvs/Screen>
#include <kvs/StructuredVolumeObject>
#include <kvs/StructuredVolumeImporter>
#include <kvs/StochasticUniformGridRenderer>
#include <kvs/HydrogenVolumeData>
#include <kvs/TransferFunction>
#include <iostream>


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
    kvs::Screen screen( &app );
    screen.setTitle( "RayCastingRenderer" );
    screen.create();

    // Read volume data from the specified data file. If the data file is not
    // specified, scalar hydrogen volume data will be created by using
    // kvs::HydrogenVolumeData class.
    auto* object = [&]() -> kvs::StructuredVolumeObject*
    {
        if ( argc > 1 ) return new kvs::StructuredVolumeImporter( argv[1] );
        else return new kvs::HydrogenVolumeData( { 64, 64, 64 } );
    }();
    object->print( std::cout );

    // Parameters for the ray casting renderer.
    auto sampling_step = 0.5f;
    auto transfer_function = kvs::TransferFunction( 256 );

    auto* renderer = new kvs::StochasticUniformGridRenderer();
    renderer->setRepetitionLevel( 10 );
    renderer->setLODControlEnabled( true );
    renderer->setSamplingStep( sampling_step );
    renderer->setTransferFunction( transfer_function );
    screen.registerObject( object, renderer );

    return app.run();
}
