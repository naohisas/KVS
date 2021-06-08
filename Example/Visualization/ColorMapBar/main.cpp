/*****************************************************************************/
/**
 *  @file   main.cpp
 *  @brief  Example program for kvs::ColorMapBar class.
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#include <kvs/Application>
#include <kvs/Screen>
#include <kvs/ColorMapBar>
#include <kvs/HydrogenVolumeData>
#include <kvs/StructuredVolumeObject>
#include <kvs/RayCastingRenderer>


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
    screen.setTitle( "kvs::ColorMapBar" );
    screen.create();

    // Color map.
    auto cmap = kvs::ColorMap::BrewerSpectral( 256 );
    auto tfunc = kvs::TransferFunction( cmap );

    // Object and renderer.
    auto* object = new kvs::HydrogenVolumeData( { 32, 32, 32 } );
    auto* renderer = new kvs::glsl::RayCastingRenderer( tfunc );
    screen.registerObject( object, renderer );

    // Colormap bar.
    kvs::ColorMapBar cmap_bar( &screen );
    cmap_bar.setCaption( "Colormap" );
    cmap_bar.setColorMap( cmap );
    cmap_bar.show();

    return app.run();
}
