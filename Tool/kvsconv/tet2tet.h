/*****************************************************************************/
/**
 *  @file   tet2tet.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: tet2tet.h 602 2010-08-19 02:43:34Z naohisa.sakamoto $
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

namespace tet2tet
{

const std::string CommandName("tet2tet");
const std::string Description("Tetrahedral volume data (quadratic) to tetrahedral volume data (linear).");

/*===========================================================================*/
/**
 *  Argument class for a tet2tet.
 */
/*===========================================================================*/
class Argument : public kvsconv::Argument::Common
{
public:
    Argument( int argc, char** argv );
    const std::string inputFilename();
    const std::string outputFilename( const std::string& filename );
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

} // end of namespace tet2tet

} // end of namespace kvsconv
