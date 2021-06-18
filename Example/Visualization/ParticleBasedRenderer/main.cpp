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


/*===========================================================================*/
/**
 *  @brief  Main function.
 *  @param  argc [i] argument count
 *  @param  argv [i] argument values
 */
/*===========================================================================*/
int main( int argc, char** argv )
{
    kvs::Application app( argc, argv );
    kvs::Screen screen( &app );
    screen.setTitle( "kvs::ParticleBasedRenderer" );
    screen.create();

    // Import volume data as structured volume object.
    auto* volume = [&]() -> kvs::StructuredVolumeObject*
    {
        if ( argc > 1 ) return new kvs::StructuredVolumeImporter( argv[1] );
        else return new kvs::HydrogenVolumeData( { 32, 32, 32 } );
    }();

    // Generate particles as point object.
    const auto repeat = 4; // number of repetitions
    const auto step = 0.5f; // sampling step
    const auto tfunc = kvs::TransferFunction( 256 ); // transfer function
    auto* object = new kvs::CellByCellMetropolisSampling( volume, repeat, step, tfunc );
    delete volume;

    // Create particle-based renderer.
    const auto glsl = true; // true; GPU-based, false: CPU-based
    auto* renderer = [&]() -> kvs::RendererBase*
    {
        if ( glsl )
        {
            // Hardware accelerated particle-based renderer
            auto* r = new kvs::glsl::ParticleBasedRenderer();
            r->setRepetitionLevel( repeat );
            r->setLODControlEnabled( true );
            return r;
        }
        else
        {
            // Software-based particle-based renderer
            auto* r = new kvs::ParticleBasedRenderer();
            r->setSubpixelLevel( size_t( std::sqrt( repeat ) ) );
            return r;
        }
    }();

    screen.registerObject( object, renderer );

    return app.run();
}
