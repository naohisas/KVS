/****************************************************************************/
/**
 *  @file main.cpp
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: main.cpp 1433 2013-03-17 01:23:37Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#include <kvs/MemoryDebugger>
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

    kvscheck::Argument arg( argc, argv );
    if( !arg.parse() ) return false;

    if( arg.hasOption("version") )
    {
        std::cout << kvscheck::VersionChecker() << std::endl;
    }
    if( arg.hasOption("platform") )
    {
        std::cout << kvscheck::PlatformChecker() << std::endl;
    }
    if( arg.hasOption("compiler") )
    {
        std::cout << kvscheck::CompilerChecker() << std::endl;
    }
    if( arg.hasOption("sizeof") )
    {
        std::cout << kvscheck::SizeofChecker() << std::endl;
    }
    if( arg.hasOption("support") )
    {
        std::cout << kvscheck::SupportChecker() << std::endl;
    }
    if( arg.hasOption("minmax") )
    {
        std::cout << kvscheck::MinMaxChecker() << std::endl;
    }
    if( arg.hasOption("opengl") )
    {
        std::cout << kvscheck::OpenGLChecker( argc, argv ) << std::endl;
    }
    if( arg.hasOption("extension") )
    {
        std::cout << kvscheck::ExtensionChecker( argc, argv ) << std::endl;
    }
    if( arg.hasOption("file") )
    {
        const std::string filename = arg.value<std::string>();
        std::cout << kvscheck::FileChecker( filename ) << std::endl;
    }

    return true;
}
