/*****************************************************************************/
/**
 *  @file   main.cpp
 *  @brief  Example program for kvs::RayCastingRenderer class.
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#include <kvs/Application>
#include <kvs/Screen>
#include <kvs/StructuredVolumeObject>
#include <kvs/StructuredVolumeImporter>
#include <kvs/RayCastingRenderer>
#include <kvs/HydrogenVolumeData>
#include <kvs/TransferFunction>


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
    screen.setTitle( "kvs::RayCastingRenderer" );
    screen.create();

    // Import volume data as structured volume object.
    auto* object = [&]() -> kvs::StructuredVolumeObject*
    {
        if ( argc > 1 ) return new kvs::StructuredVolumeImporter( argv[1] );
        else return new kvs::HydrogenVolumeData( { 32, 32, 32 } );
    }();

    // Create ray casting renderer.
    const auto step = 0.5f; // sampling step
    const auto opaque = 0.97f; // opaque value for eary-ray-termination
    const auto tfunc = kvs::TransferFunction( 256 ); // transfer function
    const auto glsl = true; // true: GPU-based, false: CPU-based
    auto* renderer = [&]() -> kvs::RendererBase*
    {
        if ( glsl )
        {
            // Hardware accelerated ray casting renderer.
            auto* r = new kvs::glsl::RayCastingRenderer();
            r->setSamplingStep( step );
            r->setOpaqueValue( opaque );
            r->setTransferFunction( tfunc );
            return r;
        }
        else
        {
            // Software based ray casting renderer.
            auto* r = new kvs::RayCastingRenderer();
            r->setSamplingStep( step );
            r->setOpaqueValue( opaque );
            r->setTransferFunction( tfunc );
            r->enableLODControl();
            return r;
        }
    }();

    screen.registerObject( object, renderer );

    return app.run();
}
