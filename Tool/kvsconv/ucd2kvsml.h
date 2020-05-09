/*****************************************************************************/
/**
 *  @file   ucd2kvsml.h
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
#include <kvs/KVSMLUnstructuredVolumeObject>
#include "Argument.h"


namespace kvsconv
{

namespace ucd2kvsml
{

const std::string CommandName("ucd2kvsml");
const std::string Description("AVS UCD data to KVSML Unstructured volume object.");

/*===========================================================================*/
/**
 *  Argument class for a ucd2kvsml.
 */
/*===========================================================================*/
class Argument : public kvsconv::Argument::Common
{
public:
    Argument( int argc, char** argv );
    const std::string inputFilename();
    const std::string outputFilename( const std::string& filename );
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

} // end of namespace ucd2kvsml

} // end of namespace kvsconv
