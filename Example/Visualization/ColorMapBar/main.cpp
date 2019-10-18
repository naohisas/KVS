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


int main( int argc, char** argv )
{
    kvs::glut::Application app( argc, argv );

    // Color map.
    kvs::ColorMap cmap( 256 );
    cmap.addPoint( 0, kvs::RGBColor( 0, 0, 0 ) );
    cmap.addPoint( 128, kvs::RGBColor( 255, 0, 0 ) );
    cmap.addPoint( 255, kvs::RGBColor( 255, 255, 255 ) );
    cmap.create();

    // Object and renderer.
    kvs::StructuredVolumeObject* object = new kvs::HydrogenVolumeData( kvs::Vector3ui( 32, 32, 32 ) );
    kvs::glsl::RayCastingRenderer* renderer = new kvs::glsl::RayCastingRenderer();
    renderer->setTransferFunction( kvs::TransferFunction( cmap ) );

    // Screen.
    kvs::glut::Screen screen( &app );
    screen.setTitle( "LegendBar" );
    screen.registerObject( object, renderer );
    screen.show();

    // Colormap bar.
    kvs::ColorMapBar cmap_bar( &screen );
    cmap_bar.setColorMap( cmap );
    cmap_bar.show();

    return app.run();
}
