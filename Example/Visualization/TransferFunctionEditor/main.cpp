/*****************************************************************************/
/**
 *  @file   main.cpp
 *  @brief  Example program for kvs::TransferFunctionEditor class.
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#include <kvs/StructuredVolumeObject>
#include <kvs/StructuredVolumeImporter>
#include <kvs/RayCastingRenderer>
#include <kvs/HydrogenVolumeData>
#include <kvs/RendererManager>
#include <kvs/Application>
#include <kvs/Screen>
#include <kvs/TransferFunctionEditor>


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
    screen.setTitle( "kvs::TransferFunctionEditor" );
    screen.create();

    // Import volume data.
    kvs::StructuredVolumeObject* object = NULL;
    if ( argc > 1 ) object = new kvs::StructuredVolumeImporter( argv[1] );
    else object = new kvs::HydrogenVolumeData( { 32, 32, 32 } );

    // Create ray-casting renderer.
    auto* renderer = new kvs::glsl::RayCastingRenderer();
    renderer->setShader( kvs::Shader::BlinnPhong() );

    screen.registerObject( object, renderer );

    // Transfer function editor.
    kvs::TransferFunctionEditor editor( &screen );
    editor.setVolumeObject( object );
    editor.apply( [&] ( kvs::TransferFunction tfunc )
    {
        renderer->setTransferFunction( tfunc );
        screen.redraw();
    } );
    editor.show();

    return app.run();
}
