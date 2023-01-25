#include <kvs/ColorImage>
#include <kvs/GrayImage>


int main( int argc, char** argv )
{
    if ( argc == 1 )
    {
        kvsMessageError() << "Usage: % " << std::string( argv[0] ) << " <image file>" << std::endl;
        return (false);
    }

    const std::string filename( argv[1] );
    kvs::ColorImage image( filename );
    image.write( "output_source.bmp" );

    kvs::GrayImage mean( image, kvs::GrayImage::MeanValue() );
    mean.write( "output_mean.bmp" );

    kvs::GrayImage middle( image, kvs::GrayImage::MiddleValue() );
    middle.write( "output_middle.bmp" );

    kvs::GrayImage median( image, kvs::GrayImage::MedianValue() );
    median.write( "output_median.bmp" );

    kvs::GrayImage ntsc( image, kvs::GrayImage::NTSCWeightedMeanValue() );
    ntsc.write( "output_ntsc.bmp" );

    kvs::GrayImage hdtv( image, kvs::GrayImage::HDTVWeightedMeanValue() );
    hdtv.write( "output_hdtv.bmp" );

    return 0;
}
