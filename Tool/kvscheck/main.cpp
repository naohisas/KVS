/*****************************************************************************/
/**
 *  @file   main.cpp
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
#include <kvs/MemoryDebugger>
#include <kvs/CommandLine>
#include "Compiler.h"
#include "File.h"
#include "Platform.h"
#include "Version.h"
#include "SizeOf.h"
#include "Support.h"
#include "MinMax.h"
#include "OpenGL.h"
#include "Extension.h"

KVS_MEMORY_DEBUGGER;


/*==========================================================================*/
/**
 *  @brief  Main function.
 *  @param  argc [in] argument count
 *  @param  argv [in] argument values
 */
/*==========================================================================*/
int main( int argc, char** argv )
{
    KVS_MEMORY_DEBUGGER__SET_ARGUMENT( argc, argv );

    kvs::CommandLine cl( argc, argv );
    cl.addHelpOption();
    cl.addOption( "version", "Output KVS version. (optional)" );
    cl.addOption( "platform", "Output platforma information. (optional)" );
    cl.addOption( "compiler", "Output compiler information. (optional)" );
    cl.addOption( "sizeof", "Output 'sizeof' information. (optional)" );
    cl.addOption( "support", "Output supported library information. (optional)" );
    cl.addOption( "minmax", "Output min/max information. (optional)" );
    cl.addOption( "opengl", "Output OpenGL information. (optional)" );
    cl.addOption( "extension", "Output OpenGL extension information. (optional)" );
    cl.addOption( "file", "Output file information. (optional)" );
    if ( !cl.parse() ) { return 1; }

    if ( cl.hasOption("version") )
    {
        return kvscheck::Version().start( argc, argv );
    }

    if ( cl.hasOption("platform") )
    {
        return kvscheck::Platform().start( argc, argv );
    }

    if ( cl.hasOption("compiler") )
    {
        return kvscheck::Compiler().start( argc, argv );
    }

    if ( cl.hasOption("sizeof") )
    {
        return kvscheck::SizeOf().start( argc, argv );
    }

    if ( cl.hasOption("support") )
    {
        return kvscheck::Support().start( argc, argv );
    }

    if ( cl.hasOption("minmax") )
    {
        return kvscheck::MinMax().start( argc, argv );
    }

    if ( cl.hasOption("opengl") )
    {
        return kvscheck::OpenGL().start( argc, argv );
    }

    if ( cl.hasOption("extension") )
    {
        return kvscheck::Extension().start( argc, argv );
    }

    if ( cl.hasOption("file") )
    {
        return kvscheck::File().start( argc, argv );
    }

    return 0;
}
