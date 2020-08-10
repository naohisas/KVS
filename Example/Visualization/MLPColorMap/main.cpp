/*****************************************************************************/
/**
 *  @file   main.cpp
 *  @brief  Example program for applying matplotlib colormap.
 *  @author Tomomasa Uchida
 */
/*****************************************************************************/
#include <kvs/Application>
#include <kvs/Screen>
#include <kvs/ImageObject>
#include <kvs/ImageRenderer>
#include <kvs/RGBColor>
#include <kvs/ColorMap>


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

    // Create pixels.
    const size_t width = 512;
    const size_t height = 64;
    const size_t channels = 3;
    kvs::ValueArray<kvs::UInt8> pixels(width * height * channels);

    // Color map.
    auto cmap = kvs::ColorMap::Viridis( 256 );
    // auto cmap = kvs::ColorMap::Plasma( 256 );
    // auto cmap = kvs::ColorMap::Inferno( 256 );
    // auto cmap = kvs::ColorMap::Magma( 256 );
    // auto cmap = kvs::ColorMap::Cividis( 256 );
    cmap.setRange( 0, width );

    // Apply colormap to the image width.
    for ( size_t j = 0; j < height*channels; j+=channels )
    {
        for ( size_t i = 0, value = 0; i < width*channels; i+=channels, value++ )
        {
            const kvs::RGBColor color( cmap.at( value ) );

            pixels[ j * width + i + 0 ] = color.r();
            pixels[ j * width + i + 1 ] = color.g();
            pixels[ j * width + i + 2 ] = color.b();
        }
    }

    // Create an image object and image renderer.
    auto* object = new kvs::ImageObject( width, height, pixels );
    auto* renderer = new kvs::ImageRenderer();

    // Screen.
    screen.registerObject( object, renderer );
    screen.setTitle( "MLPColorMap" );
    screen.setSize( width, height );
    screen.create();

    return app.run();
}