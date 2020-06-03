/*****************************************************************************/
/**
 *  @file   main.cpp
 *  @brief  Example program for colormap bar.
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
    screen.setTitle( "LegendBar" );

    // Color map.
    auto cmap = kvs::ColorMap::BrewerSpectral( 256 );

    // Object and renderer.
    auto* object = new kvs::HydrogenVolumeData( kvs::Vector3ui( 32, 32, 32 ) );
    auto* renderer = new kvs::glsl::RayCastingRenderer();
    renderer->setTransferFunction( kvs::TransferFunction( cmap ) );

    // Screen.
    screen.registerObject( object, renderer );
    screen.create();

    // Colormap bar.
    kvs::ColorMapBar cmap_bar( &screen );
    cmap_bar.setCaption( "Colormap" );
    cmap_bar.setColorMap( cmap );
    cmap_bar.show();

    return app.run();
}
