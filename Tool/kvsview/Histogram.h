/*****************************************************************************/
/**
 *  @file   Histogram.h
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: Histogram.h 621 2010-09-30 08:04:55Z naohisa.sakamoto $
 */
/*****************************************************************************/
#ifndef KVSVIEW__HISTOGRAM_H_INCLUDE
#define KVSVIEW__HISTOGRAM_H_INCLUDE

#include <string>
#include <kvs/FrequencyTable>
#include <kvs/Texture2D>
#include <kvs/CommandLine>
#include "Argument.h"


namespace kvsview
{

namespace Histogram
{

const std::string CommandName("Histogram");
const std::string Description("Rendering histogram. (optional)");

/*===========================================================================*/
/**
 *  Argument class for Histogram.
 */
/*===========================================================================*/
class Argument : public kvsview::Argument::Common
{
public:

    Argument( int argc, char** argv );

public:

    const float biasParameter( void );
};

/*===========================================================================*/
/**
 *  Main class for Histogram.
 */
/*===========================================================================*/
class Main
{
protected:

    int         m_argc;        ///< argument count
    char**      m_argv;        ///< argument values
    std::string m_input_name;  ///< input filename
    std::string m_output_name; ///< output filename

public:

    Main( int argc, char** argv );

public:

    const bool exec( void );
};

} // end of namespace Histogram

} // end of namespace kvsview

#endif // KVSVIEW__HISTOGRAM_H_INCLUDE
