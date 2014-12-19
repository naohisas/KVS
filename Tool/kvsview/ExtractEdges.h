/*****************************************************************************/
/**
 *  @file   ExtractEdges.h
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: ExtractEdges.h 621 2010-09-30 08:04:55Z naohisa.sakamoto $
 */
/*****************************************************************************/
#ifndef KVSVIEW__EXTRACT_EDGES_H_INCLUDE
#define KVSVIEW__EXTRACT_EDGES_H_INCLUDE

#include <string>
#include <kvs/Type>
#include <kvs/CommandLine>
#include <kvs/TransferFunction>
#include "Argument.h"


namespace kvsview
{

namespace ExtractEdges
{

const std::string CommandName("ExtractEdges");
const std::string Description("Extract edges of the volume data. (optional)");

/*===========================================================================*/
/**
 *  Argument class for ExtractEdges.
 */
/*===========================================================================*/
class Argument : public kvsview::Argument::Common
{
public:

    Argument( int argc, char** argv );

public:

    const kvs::Real32 size( void );

    const kvs::TransferFunction transferFunction( const kvs::VolumeObjectBase* volume );
};

/*===========================================================================*/
/**
 *  Main class for ExtractEdges.
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

} // end of namespace ExtractEdges

} // end of namespace kvsview

#endif // KVSVIEW__EXTRACT_EDGES_H_INCLUDE
