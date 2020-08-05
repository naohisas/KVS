/*****************************************************************************/
/**
 *  @file   ImgConv.h
 *  @author Naohisa Sakamoto
 *  @brief  Image data converter.
 */
/*****************************************************************************/
#pragma once
#include <string>
#include <kvs/CommandLine>
#include <kvs/ColorImage>
#include <kvs/GrayImage>
#include <kvs/BitImage>
#include <kvs/CubicImage>
#include "Argument.h"


namespace kvsconv
{

namespace ImgConv
{

const std::string CommandName( "img_conv" );
const std::string Description( "Image Data Converter." );

/*===========================================================================*/
/**
 *  Argument class for img2img.
 */
/*===========================================================================*/
class Argument : public kvsconv::Argument::Common
{
public:
    Argument( int argc, char** argv );
    std::string inputFilename();
    std::string outputFilename( const std::string& filename );
    kvs::GrayImage grayImage( const kvs::ColorImage& image );
    kvs::BitImage bitImage( const kvs::GrayImage& image );
    kvs::CubicImage cubicImage( const std::string& option );
};

/*===========================================================================*/
/**
 *  Main class for img2img.
 */
/*===========================================================================*/
class Main
{
private:
    int m_argc; ///< argument count
    char** m_argv; ///< argument values
    std::string m_input_name; ///< input filename
    std::string m_output_name; ///< output filename

public:
    Main( int argc, char** argv ): m_argc( argc ), m_argv( argv ) {}
    bool exec();
};

} // end of namespace ImgConv

} // end of namespace kvsconv
