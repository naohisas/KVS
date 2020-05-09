/*****************************************************************************/
/**
 *  @file   main.cpp
 *  @brief  Example program for legend bar.
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#include <kvs/glut/Application>
#include <kvs/glut/Screen>
#include <kvs/glut/LegendBar>
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

    // Legend bar.
    kvs::glut::LegendBar legend_bar( &screen );
    legend_bar.setColorMap( cmap );
    legend_bar.show();

    return app.run();
}
