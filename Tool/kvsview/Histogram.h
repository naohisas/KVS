/*****************************************************************************/
/**
 *  @file   Histogram.h
 *  @auhtor Naohisa Sakamoto
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
#pragma once
#include <string>
#include <kvs/FrequencyTable>
#include <kvs/Texture2D>
#include <kvs/CommandLine>
#include <kvs/Program>
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
    const float biasParameter();
};

/*===========================================================================*/
/**
 *  Main class for Histogram.
 */
/*===========================================================================*/
class Main : public kvs::Program
{
private:
    std::string m_input_name; ///< input filename
    std::string m_output_name; ///< output filename

public:
    int exec( int argc, char** argv );
};

} // end of namespace Histogram

} // end of namespace kvsview
