/*****************************************************************************/
/**
 *  @file   img2img.h
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: img2img.h 602 2010-08-19 02:43:34Z naohisa.sakamoto $
 */
/*****************************************************************************/
#ifndef KVSCONV__IMG2IMG_H_INCLUDE
#define KVSCONV__IMG2IMG_H_INCLUDE

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

public:

    const std::string inputFilename( void );

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
protected:

    int         m_argc;         ///< argument count
    char**      m_argv;         ///< argument values
    std::string m_input_name;   ///< input filename
    std::string m_output_name;  ///< output filename

public:

    Main( int argc, char** argv );

public:

    const bool exec( void );
};

} // end of namespace img2img

} // end of namespace kvsconv

#endif // KVSCONV__IMG2IMG_H_INCLUDE
