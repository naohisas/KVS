/*****************************************************************************/
/**
 *  @file   img2img.cpp
 *  @author Naohisa Sakamoto
 *  @brief  Image data converter.
 */
/*****************************************************************************/
#include "ImgConv.h"
#include <memory>
#include <string>
#include <kvs/File>
#include <kvs/SphericalImage>


namespace kvsconv
{

namespace ImgConv
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new argument class for ImgConv.
 *  @param  argc [in] argument count
 *  @param  argv [in] argument values
 */
/*===========================================================================*/
Argument::Argument( int argc, char** argv ):
    kvsconv::Argument::Common( argc, argv, ImgConv::CommandName )
{
    addOption( ImgConv::CommandName, ImgConv::Description, 0 );
    addOption( "s", "Scaling image. (optional: <scale>)", 1 );
    addOption( "r", "Resize image. (optional: <width> <height>)", 2 );
    addOption( "g", "Gray scaling. (default: 0)\n"
               "\t      0 = Mean value\n"
               "\t      1 = Middle value\n"
               "\t      2 = Median value\n"
               "\t      3 = NTSC weighted mean value\n"
               "\t      4 = HDTV weighted mean value", 1 );
    addOption( "b", "Binarization. (default: 0)\n"
               "\t      0 = P-Tile\n"
               "\t      1 = Distinction\n"
               "\t      2 = Byer\n"
               "\t      3 = Halftone\n"
               "\t      4 = Emphasized halftone\n"
               "\t      5 = Screw\n"
               "\t      6 = Deformed screw\n"
               "\t      7 = Dot concentrate", 1 );
    addOption( "sphere", "Sphere mapping. Specify six images: top, left, right, bottom, back and front images.", 6 );
    addOption( "cube", "Cube mapping. Specify six images: top, left, right, bottom, back and front images.", 6 );
    addOption( "l", "Image layout for cube mapping. (default: 0)\n"
               "\t      0 = Cross\n"
               "\t      1 = Rectangle\n"
               "\t      2 = Vertical\n"
               "\t      3 = Horizontal", 1 );
}

/*===========================================================================*/
/**
 *  @brief  Returns a input filename.
 *  @return input filename
 */
/*===========================================================================*/
std::string Argument::inputFilename()
{
    return this->hasValues() ? this->value<std::string>() : "";
}

/*===========================================================================*/
/**
 *  @brief  Returns a output filename.
 *  @param  filename [in] input filename
 *  @return output filename.
 */
/*===========================================================================*/
std::string Argument::outputFilename( const std::string& filename )
{
    if ( this->hasOption("output") )
    {
        return this->optionValue<std::string>("output");
    }
    else
    {
        // Default output filename.
        if ( filename.empty() ) { return "output.png"; }
        else
        {
            // e.g) image_data.bmp -> output_image_data.png
            const std::string basename = kvs::File( filename ).baseName();
            const std::string extension = "png";
            return "output_" + basename + "." + extension;
        }
    }
}

/*===========================================================================*/
/**
 *  @brief  Returns the gray image by gray scaling the given color image.
 *  @param  image [in] color image
 *  @return gray image
 */
/*===========================================================================*/
kvs::GrayImage Argument::grayImage( const kvs::ColorImage& image )
{
    switch ( this->optionValue<int>("g") )
    {
    case 0:  return kvs::GrayImage( image, kvs::GrayImage::MeanValue() );
    case 1:  return kvs::GrayImage( image, kvs::GrayImage::MiddleValue() );
    case 2:  return kvs::GrayImage( image, kvs::GrayImage::MedianValue() );
    case 3:  return kvs::GrayImage( image, kvs::GrayImage::NTSCWeightedMeanValue() );
    case 4:  return kvs::GrayImage( image, kvs::GrayImage::HDTVWeightedMeanValue() );
    default: return kvs::GrayImage( image, kvs::GrayImage::MeanValue() );
    }
}

/*===========================================================================*/
/**
 *  @brief  Returns the bit image by binarizing the given gray image.
 *  @param  image [in] gray image
 *  @return bit image
 */
/*===========================================================================*/
kvs::BitImage Argument::bitImage( const kvs::GrayImage& image )
{
    switch ( this->optionValue<int>("b") )
    {
    case 0:  return kvs::BitImage( image, kvs::BitImage::PTile() );
    case 1:  return kvs::BitImage( image, kvs::BitImage::Distinction() );
    case 2:  return kvs::BitImage( image, kvs::BitImage::Byer() );
    case 3:  return kvs::BitImage( image, kvs::BitImage::Halftone() );
    case 4:  return kvs::BitImage( image, kvs::BitImage::EmphasizedHalftone() );
    case 5:  return kvs::BitImage( image, kvs::BitImage::Screw() );
    case 6:  return kvs::BitImage( image, kvs::BitImage::DeformedScrew() );
    case 7:  return kvs::BitImage( image, kvs::BitImage::DotConcentrate() );
    default: return kvs::BitImage( image, kvs::BitImage::PTile() );
    }
}

kvs::CubicImage Argument::cubicImage( const bool stitch )
{
    kvs::CubicImage image;
    image.setTopImage( kvs::ColorImage( this->optionValue<std::string>( "sphere", 0 ) ) );
    image.setLeftImage( kvs::ColorImage( this->optionValue<std::string>( "sphere", 1 ) ) );
    image.setRightImage( kvs::ColorImage( this->optionValue<std::string>( "sphere", 2 ) ) );
    image.setBottomImage( kvs::ColorImage( this->optionValue<std::string>( "sphere", 3 ) ) );
    image.setBackImage( kvs::ColorImage( this->optionValue<std::string>( "sphere", 4 ) ) );
    image.setFrontImage( kvs::ColorImage( this->optionValue<std::string>( "sphere", 5 ) ) );

    switch ( this->optionValue<int>("l") )
    {
    case 0: image.setLayoutToCross(); break;
    case 1: image.setLayoutToRectangle(); break;
    case 2: image.setLayoutToVertical(); break;
    case 3: image.setLayoutToHorizontal(); break;
    default: break;
    }

    if ( stitch ) { image.stitch(); }

    return image;
}

/*===========================================================================*/
/**
 *  @brief  Executes main process.
 */
/*===========================================================================*/
bool Main::exec()
{
    // Parse specified arguments.
    ImgConv::Argument arg( m_argc, m_argv );
    if ( !arg.parse() ) { return false; }

    // Set a input filename and a output filename.
    m_input_name = arg.inputFilename();
    m_output_name = arg.outputFilename( m_input_name );

    // Read the input image file.
    kvs::ColorImage image;
    if ( !m_input_name.empty() )
    {
        kvs::File file( m_input_name );
        if ( !file.exists() )
        {
            kvsMessageError() << m_input_name << " is not found." << std::endl;
            return false;
        }

        if ( !image.read( m_input_name ) )
        {
            kvsMessageError() << "Cannot read " << m_input_name << "." << std::endl;
            return false;
        }
    }

    // Scaling
    if ( arg.hasOption("s") )
    {
        const double ratio = arg.optionValue<double>("s");
        image.scale( ratio );
    }

    // Resizing
    if ( arg.hasOption("r") )
    {
        const size_t width  = arg.optionValue<size_t>("r",0);
        const size_t height = arg.optionValue<size_t>("r",1);
        image.resize( width, height );
    }

    // Gray-scaling
    if ( arg.hasOption("g") )
    {
        kvs::GrayImage gray = arg.grayImage( image );
        return gray.write( m_output_name );
    }

    // Binarization.
    if ( arg.hasOption("b") )
    {
        kvs::BitImage bit = arg.bitImage( kvs::GrayImage( image ) );
        return bit.write( m_output_name );
    }

    // Sphere mapping.
    if ( arg.hasOption("sphere") )
    {
        kvs::SphericalImage sphere( arg.cubicImage() );
        return sphere.write( m_output_name );
    }

    // Cube mapping.
    if ( arg.hasOption("cube") )
    {
        kvs::CubicImage cube = arg.cubicImage( true );
        return cube.write( m_output_name );
    }

    return image.write( m_output_name );
}

} // end of namespace ImgConv

} // end of namespace kvsconv
