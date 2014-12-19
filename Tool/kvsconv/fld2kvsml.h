/*****************************************************************************/
/**
 *  @file   fld2kvsml.h
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: fld2kvsml.h 602 2010-08-19 02:43:34Z naohisa.sakamoto $
 */
/*****************************************************************************/
#ifndef KVSCONV__FLD2KVSML_H_INCLUDE
#define KVSCONV__FLD2KVSML_H_INCLUDE

#include <string>
#include <kvs/CommandLine>
#include <kvs/KVSMLObjectStructuredVolume>
#include "Argument.h"


namespace kvsconv
{

namespace fld2kvsml
{

const std::string CommandName("fld2kvsml");
const std::string Description("AVS Field data to KVSML Structured volume object.");

/*===========================================================================*/
/**
 *  Argument class for a fld2kvsml.
 */
/*===========================================================================*/
class Argument : public kvsconv::Argument::Common
{
public:

    Argument( int argc, char** argv );

public:

    const std::string inputFilename( void );

    const std::string outputFilename( const std::string& filename );

    const kvs::KVSMLObjectStructuredVolume::WritingDataType writingDataType( void );
};

/*===========================================================================*/
/**
 *  Main class for a fld2kvsml.
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

} // end of namespace fld2kvsml

} // end of namespace kvsconv

#endif // KVSCONV__FLD2KVSML_H_INCLUDE
