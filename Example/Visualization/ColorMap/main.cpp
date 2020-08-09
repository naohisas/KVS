/*****************************************************************************/
/**
 *  @file   main.cpp
 *  @brief  Example program for applying colormap.
 *  @author Tomomasa Uchida
 */
/*****************************************************************************/
#include <kvs/ColorMap>
#include <kvs/Message>
#include <kvs/ImageObject>
#include <kvs/ImageRenderer>
#include <kvs/RGBColor>
#include <kvs/glut/Application>
#include <kvs/glut/Screen>


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
    // GLUT viewer application.
    kvs::glut::Application app( argc, argv );

    // Create pixels.
    const size_t width = 512;
    const size_t height = 512;
    const size_t channels = 3;
    kvs::ValueArray<kvs::UInt8> pixels(width * height * channels);

    // Apply colormap to the image width.
    kvs::ColorMap cmap( 256, 0.0, width );
    for ( size_t j = 0; j < height*channels; j+=channels)
    {
        for ( size_t i = 0, value = 0; i < width*channels; i+=channels, value++ )
        {
            const kvs::RGBColor color( cmap.at( static_cast<float>(value), kvs::ColorMap::ColorMapType::Viridis) );

            pixels[j*width+i + 0] = color.r();
            pixels[j*width+i + 1] = color.g();
            pixels[j*width+i + 2] = color.b();
        }
    }

    // Create an image object.
    kvs::ImageObject* object = new kvs::ImageObject( width, height, pixels );
    if ( !object )
    {
        kvsMessageError("Cannot creat an image object.");
        return( false );
    }

    // Create an image renderer.
    const kvs::ImageRenderer::Type type = kvs::ImageRenderer::Stretching;
    kvs::ImageRenderer* renderer = new kvs::ImageRenderer( type );
    if ( !renderer )
    {
        kvsMessageError("Cannot create an image renderer.");
        delete object;
        return( false );
    }

    // Screen.
    kvs::glut::Screen screen( &app );
    screen.registerObject( object, renderer );
    screen.setSize( object->width(), object->height() );
    screen.setTitle( "ColorMap" );
    screen.show();

    return app.run();
}