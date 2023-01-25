#include <kvs/GrayImage>
#include <kvs/BitImage>


int main( int argc, char** argv )
{
    if ( argc == 1 )
    {
        kvsMessageError() << "Usage: % " << std::string( argv[0] ) << " <image file>" << std::endl;
        return (false);
    }

    const std::string filename( argv[1] );
    kvs::GrayImage image( filename );
    image.write( "output_source.bmp" );

    kvs::BitImage ptile( image, kvs::BitImage::PTile() );
    ptile.write( "output_ptile.bmp" );

    kvs::BitImage distinction( image, kvs::BitImage::Distinction() );
    distinction.write( "output_distinction.bmp" );

    kvs::BitImage byer( image, kvs::BitImage::Byer() );
    byer.write( "output_byer.bmp" );

    kvs::BitImage halftone( image, kvs::BitImage::Halftone() );
    halftone.write( "output_halftone.bmp" );

    kvs::BitImage emphasized_halftone( image, kvs::BitImage::EmphasizedHalftone() );
    emphasized_halftone.write( "output_emphasized_halftone.bmp" );

    kvs::BitImage screw( image, kvs::BitImage::Screw() );
    screw.write( "output_screw.bmp" );

    kvs::BitImage deformed_screw( image, kvs::BitImage::DeformedScrew() );
    deformed_screw.write( "output_deformed_screw.bmp" );

    kvs::BitImage dot_concentrate( image, kvs::BitImage::DotConcentrate() );
    dot_concentrate.write( "output_dot_concentrate.bmp" );

    return 0;
}
