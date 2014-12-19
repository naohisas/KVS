/*****************************************************************************/
/**
 *  @file   ucd2kvsml.h
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: ucd2kvsml.h 602 2010-08-19 02:43:34Z naohisa.sakamoto $
 */
/*****************************************************************************/
#ifndef KVSCONV__UCD2KVSML_H_INCLUDE
#define KVSCONV__UCD2KVSML_H_INCLUDE

#include <string>
#include <kvs/CommandLine>
#include <kvs/KVSMLObjectUnstructuredVolume>
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

public:

    const std::string inputFilename( void );

    const std::string outputFilename( const std::string& filename );

    const kvs::KVSMLObjectUnstructuredVolume::WritingDataType writingDataType( void );
};

/*===========================================================================*/
/**
 *  Main class for a ucd2kvsml.
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

} // end of namespace ucd2kvsml

} // end of namespace kvsconv

#endif // KVSCONV__UCD2KVSML_H_INCLUDE
