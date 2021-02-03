/*****************************************************************************/
/**
 *  @file   main.cpp
 *  @author Naohisa Sakamoto
 *  @brief  Example program for kvs::ParticleBasedRenderer class.
 */
/*****************************************************************************/
#include <kvs/Application>
#include <kvs/Screen>
#include <kvs/HydrogenVolumeData>
#include <kvs/StructuredVolumeObject>
#include <kvs/StructuredVolumeImporter>
#include <kvs/PointObject>
#include <kvs/CellByCellMetropolisSampling>
#include <kvs/ParticleBasedRenderer>
#include <kvs/TransferFunction>
#include <kvs/Vector3>
#include <iostream>
#include <string>


/*===========================================================================*/
/**
 *  @brief  Import structured volume object
 *  @param  argc [i] argument count
 *  @param  argv [i] argument values
 *  @return pointer to the imported structured volume object
 */
/*===========================================================================*/
kvs::StructuredVolumeObject* Import( int argc, char** argv )
{
    if ( argc > 1 )
    {
        const std::string filename( argv[1] );
        return new kvs::StructuredVolumeImporter( argv[1] );
    }

    auto resolution = kvs::Vec3u::Constant( 32 );
    return new kvs::HydrogenVolumeData( resolution );
}

/*===========================================================================*/
/**
 *  @brief  Main function.
 *  @param  argc [i] argument count
 *  @param  argv [i] argument values
 *  @return true, if the main process is done succesfully
 */
/*===========================================================================*/
int main( int argc, char** argv )
{
    kvs::Application app( argc, argv );
    kvs::Screen screen( &app );
    screen.setTitle( "kvs::ParticleBasedRenderer" );
    screen.create();

    // Import volume object.
    auto* volume = Import( argc, argv );
    volume->print( std::cout << "IMPORTED VOLUME" << std::endl, kvs::Indent(4) );

    // Generate particles.
    using Sampling = kvs::CellByCellMetropolisSampling;
    const size_t repeat = 4;
    const float step = 0.5f;
    const kvs::TransferFunction tfunc( 256 );
    auto* object = new Sampling( volume, repeat, step, tfunc );
    object->print( std::cout << "GENERATED PARTICLES" << std::endl, kvs::Indent(4) );

    delete volume;

    // Setup renderer.
    using Renderer = kvs::glsl::ParticleBasedRenderer;
    auto* renderer = new Renderer();
    renderer->setRepetitionLevel( repeat );
    renderer->setLODControlEnabled( true );

    screen.registerObject( object, renderer );
    screen.show();

    return app.run();
}
