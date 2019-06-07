/*****************************************************************************/
/**
 *  @file   img2img.cpp
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: img2img.cpp 1191 2012-06-12 01:31:49Z naohisa.sakamoto $
 */
/*****************************************************************************/
#include "img2img.h"
#include <memory>
#include <string>
#include <kvs/File>
#include <kvs/IgnoreUnusedVariable>


namespace kvsconv
{

namespace img2img
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new argument class for img2img.
 *  @param  argc [in] argument count
 *  @param  argv [in] argument values
 */
/*===========================================================================*/
Argument::Argument( int argc, char** argv ):
    kvsconv::Argument::Common( argc, argv, kvsconv::img2img::CommandName )
{
    addOption( kvsconv::img2img::CommandName, kvsconv::img2img::Description, 0 );
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
}

/*===========================================================================*/
/**
 *  @brief  Returns a input filename.
 *  @return input filename
 */
/*===========================================================================*/
const std::string Argument::inputFilename()
{
    return this->value<std::string>();
}

/*===========================================================================*/
/**
 *  @brief  Returns a output filename.
 *  @param  filename [in] input filename
 *  @return output filename.
 */
/*===========================================================================*/
const std::string Argument::outputFilename( const std::string& filename )
{
    kvs::IgnoreUnusedVariable( filename );

    if ( this->hasOption("output") )
    {
        return this->optionValue<std::string>("output");
    }
    else
    {
        return "";
    }
}

/*===========================================================================*/
/**
 *  @brief  Returns the gray image by gray scaling the given color image.
 *  @param  image [in] color image
 *  @return gray image
 */
/*===========================================================================*/
const kvs::GrayImage Argument::grayImage( const kvs::ColorImage& image )
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
const kvs::BitImage Argument::bitImage( const kvs::GrayImage& image )
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

/*===========================================================================*/
/**
 *  @brief  Executes main process.
 */
/*===========================================================================*/
bool Main::exec()
{
    // Parse specified arguments.
    img2img::Argument arg( m_argc, m_argv );
    if ( !arg.parse() ) { return false; }

    // Set a input filename and a output filename.
    m_input_name = arg.inputFilename();
    m_output_name = arg.outputFilename( m_input_name );

    kvs::File file( m_input_name );
    if ( !file.exists() )
    {
        kvsMessageError("Input data file '%s' is not existed.",m_input_name.c_str());
        return false;
    }

    if ( m_output_name.empty() )
    {
        kvsMessageError("Output filename is required for img2img.");
        return false;
    }

    // Read the input image file.
    kvs::ColorImage image;
    if ( !image.read( m_input_name ) )
    {
        kvsMessageError("Cannot read image data file '%s'.",m_input_name.c_str());
        return false;
    }

    if ( arg.hasOption("s") )
    {
        const double ratio = arg.optionValue<double>("s");
        image.scale( ratio );
    }

    if ( arg.hasOption("r") )
    {
        const size_t width  = arg.optionValue<size_t>("r",0);
        const size_t height = arg.optionValue<size_t>("r",1);
        image.resize( width, height );
    }

    if ( arg.hasOption("g") )
    {
        // Gray scaling.
        kvs::GrayImage gray = arg.grayImage( image );
        if ( arg.hasOption("b") )
        {
            // Binarization.
            kvs::BitImage bit = arg.bitImage( gray );
            return bit.write( m_output_name );
        }
        return gray.write( m_output_name );
    }

    if ( arg.hasOption("b") )
    {
        // Binarization.
        kvs::BitImage bit = arg.bitImage( kvs::GrayImage( image ) );
        return bit.write( m_output_name );
    }

    return image.write( m_output_name );
}

} // end of namespace img2img

} // end of namespace kvsconv
