/*****************************************************************************/
/**
 *  @file   main.cpp
 *  @brief  Example program for colormap bar.
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#include <kvs/glut/Application>
#include <kvs/glut/Screen>
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
    kvs::glut::Application app( argc, argv );

    // Color map.
    auto cmap = kvs::ColorMap::BrewerSpectral( 256 );

    // Object and renderer.
    auto* object = new kvs::HydrogenVolumeData( kvs::Vector3ui( 32, 32, 32 ) );
    auto* renderer = new kvs::glsl::RayCastingRenderer();
    renderer->setTransferFunction( kvs::TransferFunction( cmap ) );

    // Screen.
    kvs::glut::Screen screen( &app );
    screen.setTitle( "LegendBar" );
    screen.registerObject( object, renderer );
    screen.show();

    // Colormap bar.
    kvs::ColorMapBar cmap_bar( &screen );
    cmap_bar.setCaption( "Colormap" );
    cmap_bar.setColorMap( cmap );
    cmap_bar.show();

    return app.run();
}
