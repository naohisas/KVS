/*****************************************************************************/
/**
 *  @file   FldConv.h
 *  @author Naohisa Sakamoto
 *  @breif  AVS Field data converter
 */
/*****************************************************************************/
#pragma once
#include <string>
#include <kvs/CommandLine>
#include <kvs/KVSMLStructuredVolumeObject>
#include "Argument.h"


namespace kvsconv
{

namespace FldConv
{

const std::string CommandName( "fld_conv" );
const std::string Description( "AVS Field Data Converter." );

/*===========================================================================*/
/**
 *  Argument class for a fld2kvsml.
 */
/*===========================================================================*/
class Argument : public kvsconv::Argument::Common
{
public:
    Argument( int argc, char** argv );
    std::string inputFilename();
    std::string outputFilename( const std::string& filename );
};

/*===========================================================================*/
/**
 *  Main class for a fld2kvsml.
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

} // end of namespace FldConv

} // end of namespace kvsconv
