#include <kvs/ColorImage>


int main( int argc, char** argv )
{
    if ( argc == 1 )
    {
        kvsMessageError() << "Usage: % " << std::string( argv[0] ) << " <image file>" << std::endl;
        return (false);
    }

    const std::string filename( argv[1] );
    kvs::ColorImage image( filename );
    image.scale( 0.5 );
    image.scale( 2.0, kvs::ColorImage::Bilinear() );
    image.write( "output.bmp" );

    return 0;
}
