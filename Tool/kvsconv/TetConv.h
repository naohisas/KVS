/*****************************************************************************/
/**
 *  @file   TetConv.h
 *  @author Naohisa Sakamoto
 *  @brief  Tetrahedral volume data converter
 */
/*****************************************************************************/
#pragma once
#include <string>
#include <kvs/CommandLine>
#include <kvs/KVSMLUnstructuredVolumeObject>
#include <kvs/TetrahedraToTetrahedra>
#include "Argument.h"


namespace kvsconv
{

namespace TetConv
{

const std::string CommandName( "tet_conv" );
const std::string Description( "Tetrahedral Volume Data Converter.");

/*===========================================================================*/
/**
 *  Argument class for a tet2tet.
 */
/*===========================================================================*/
class Argument : public kvsconv::Argument::Common
{
public:
    Argument( int argc, char** argv );
    std::string inputFilename();
    std::string outputFilename( const std::string& filename );
    kvs::KVSMLUnstructuredVolumeObject::WritingDataType writingDataType();
    kvs::TetrahedraToTetrahedra::Method conversionMethod();
};

/*===========================================================================*/
/**
 *  Main class for a tet2tet.
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

} // end of namespace TetConv

} // end of namespace kvsconv
