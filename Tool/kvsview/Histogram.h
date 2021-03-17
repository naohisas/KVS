/*****************************************************************************/
/**
 *  @file   Histogram.h
 *  @auhtor Naohisa Sakamoto
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
    int m_argc;
    char** m_argv;

public:
    Main( int argc, char** argv ): m_argc( argc ), m_argv( argv ) {}
    int exec();
};

} // end of namespace Histogram

} // end of namespace kvsview
