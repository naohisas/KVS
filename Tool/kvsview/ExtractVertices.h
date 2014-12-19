/*****************************************************************************/
/**
 *  @file   ExtractVertices.h
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: ExtractVertices.h 621 2010-09-30 08:04:55Z naohisa.sakamoto $
 */
/*****************************************************************************/
#ifndef KVSVIEW__EXTRACT_VERTICES_H_INCLUDE
#define KVSVIEW__EXTRACT_VERTICES_H_INCLUDE

#include <string>
#include <kvs/Type>
#include <kvs/CommandLine>
#include <kvs/TransferFunction>
#include "Argument.h"


namespace kvsview
{

namespace ExtractVertices
{

const std::string CommandName("ExtractVertices");
const std::string Description("Extract vertices of the volume data. (optional)");

/*===========================================================================*/
/**
 *  Argument class for ExtractVertices.
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
 *  Main class for ExtractVertices.
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

} // end of namespace ExtractVertices

} // end of namespace kvsview

#endif // KVSVIEW__EXTRACT_VERTICES_H_INCLUDE
