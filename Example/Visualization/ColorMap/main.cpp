/*****************************************************************************/
/**
 *  @file   main.cpp
 *  @brief  Example program for applying colormap.
 *  @author Tomomasa Uchida
 */
/*****************************************************************************/
#include <kvs/Application>
#include <kvs/Screen>
#include <kvs/Message>
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
    screen.setTitle( "ColorMap" );
    screen.create();

    // Create pixels.
    const size_t width = 512;
    const size_t height = 64;
    const size_t channels = 3;
    kvs::ValueArray<kvs::UInt8> pixels(width * height * channels);

    // Apply colormap to the image width.
    // kvs::ColorMap cmap( 256, 0.0, width );
    kvs::ColorMap cmap = kvs::ColorMap::Viridis( 256 );
    for ( size_t j = 0; j < height*channels; j+=channels)
    {
        for ( size_t i = 0, value = 0; i < width*channels; i+=channels, value++ )
        {
            // const kvs::RGBColor color( cmap.at( static_cast<float>(value), kvs::ColorMap::ColorMapType::Viridis) );
            const kvs::RGBColor color( cmap.at(static_cast<float>(value)) );

            pixels[j*width+i + 0] = color.r();
            pixels[j*width+i + 1] = color.g();
            pixels[j*width+i + 2] = color.b();
        }
    }

    // Create an image object.
    auto* object = new kvs::ImageObject( width, height, pixels );
    if ( !object )
    {
        kvsMessageError() << "Cannot creat an image object." << std::endl;
        return( false );
    }

    // Create an image renderer.
    auto* renderer = new kvs::ImageRenderer();
    if ( !renderer )
    {
        kvsMessageError() << "Cannot create an image renderer." << std::endl;
        delete object;
        return ( false );
    }

    screen.setSize( object->width(), object->height() );
    screen.registerObject( object, renderer );

    return app.run();
}