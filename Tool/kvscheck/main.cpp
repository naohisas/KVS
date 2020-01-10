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
#include "Argument.h"
#include "CompilerChecker.h"
#include "FileChecker.h"
#include "PlatformChecker.h"
#include "VersionChecker.h"
#include "SizeofChecker.h"
#include "SupportChecker.h"
#include "MinMaxChecker.h"
#include "OpenGLChecker.h"
#include "ExtensionChecker.h"

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

//    kvscheck::Argument arg( argc, argv );
//    if ( !arg.parse() ) { return 1; }
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
        return kvscheck::Version().exec( argc, argv );
    }

    if ( cl.hasOption("platform") )
    {
        return kvscheck::Platform().exec( argc, argv );
    }

    if ( cl.hasOption("compiler") )
    {
        return kvscheck::Compiler().exec( argc, argv );
    }

    if ( cl.hasOption("sizeof") )
    {
        return kvscheck::SizeOf().exec( argc, argv );
    }

    if ( cl.hasOption("support") )
    {
        return kvscheck::Support().exec( argc, argv );
    }

    if ( cl.hasOption("minmax") )
    {
        return kvscheck::MinMax().exec( argc, argv );
    }

    if ( cl.hasOption("opengl") )
    {
        return kvscheck::OpenGL().exec( argc, argv );
    }

    if ( cl.hasOption("extension") )
    {
        return kvscheck::Extension().exec( argc, argv );
    }

    if ( cl.hasOption("file") )
    {
        return kvscheck::File().exec( argc, argv );
    }

    return 0;
}
