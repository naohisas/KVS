/*****************************************************************************/
/**
 *  @file   img2img.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id$
 */
/*****************************************************************************/
#pragma once
#include <string>
#include <kvs/CommandLine>
#include <kvs/ColorImage>
#include <kvs/GrayImage>
#include <kvs/BitImage>
#include "Argument.h"


namespace kvsconv
{

namespace img2img
{

const std::string CommandName("img2img");
const std::string Description("Image data to image data.");

/*===========================================================================*/
/**
 *  Argument class for img2img.
 */
/*===========================================================================*/
class Argument : public kvsconv::Argument::Common
{
public:
    Argument( int argc, char** argv );
    const std::string inputFilename();
    const std::string outputFilename( const std::string& filename );
    const kvs::GrayImage grayImage( const kvs::ColorImage& image );
    const kvs::BitImage bitImage( const kvs::GrayImage& image );
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

} // end of namespace img2img

} // end of namespace kvsconv
