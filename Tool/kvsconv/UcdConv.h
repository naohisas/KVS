/*****************************************************************************/
/**
 *  @file   UcdConv.h
 *  @author Naohisa Sakamoto
 *  @brief  AVS UCD Data Converter
 */
/*****************************************************************************/
#pragma once
#include <string>
#include <kvs/CommandLine>
#include <kvs/KVSMLUnstructuredVolumeObject>
#include "Argument.h"


namespace kvsconv
{

namespace UcdConv
{

const std::string CommandName( "ucd_conv" );
const std::string Description("AVS UCD Data Converter.");

/*===========================================================================*/
/**
 *  Argument class for a ucd2kvsml.
 */
/*===========================================================================*/
class Argument : public kvsconv::Argument::Common
{
public:
    Argument( int argc, char** argv );
    std::string inputFilename();
    std::string outputFilename( const std::string& filename );
    kvs::KVSMLUnstructuredVolumeObject::WritingDataType writingDataType();
};

/*===========================================================================*/
/**
 *  Main class for a ucd2kvsml.
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

} // end of namespace UcdConv

} // end of namespace kvsconv
