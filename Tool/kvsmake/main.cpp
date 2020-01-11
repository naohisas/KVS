/****************************************************************************/
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
 *  $Id: main.cpp 1409 2012-12-21 15:19:08Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#include <cstdlib>
#include <string>
#include <kvs/MemoryDebugger>
#include <kvs/Compiler>
#include <kvs/Message>
#include <kvs/Directory>
#include <kvs/File>
#include <kvs/CommandLine>
#include "Constant.h"
#include "WriteMakefile.h"
#include "WriteQtProject.h"
//#if defined ( KVS_COMPILER_VC )
#include "WriteVCProject.h"
#include "WriteVCXProject.h"
#include "WriteVCProjectCUDA.h"
//#endif

KVS_MEMORY_DEBUGGER;

/*==========================================================================*/
/**
 *  @brief  Main function for kvsmake.
 *  @param  argc [in] argument count
 *  @param  argv [in] argument values
 */
/*==========================================================================*/
int main( int argc, char** argv )
{
    KVS_MEMORY_DEBUGGER__SET_ARGUMENT( argc, argv );

    kvs::CommandLine cl( argc, argv );
    cl.addHelpOption();
    cl.addValue( "project name / make options", false );
    // Makefile generation option.
    cl.addOption( "g", "Generate a Makefile. The target name will be set to the specified name.", 1 );
    cl.addOption( "G", "Generate a Makefile. The target name will be automatically set to the current directory name.", 0 );
    // Qt project generation option.
    cl.addOption( "q", "Generate a Qt project file. The target name will be set to the specified name.", 1 );
    cl.addOption( "Q", "Generate a Qt project file. The target name will be automatically set to the current directory name.", 0 );
    cl.addOption( "qtproj", "Generate a Qt project file. The target name will be set to the specified name.", 1 );
    cl.addOption( "Qtproj", "Generate a Qt project file. The target name will be automatically set to the current directory name.", 0 );
//#if defined ( KVS_COMPILER_VC )
    // VC project generation option.
    cl.addOption( "vcproj", "generate a MSVC project file. The target name will be set to the specified name.", 1 );
    cl.addOption( "Vcproj", "generate a MSVC project file. The target name will be automatically set to the current directory name.", 0 );
    cl.addOption( "vcproj_cuda", "generate a VC project file with CUDA option. The target name will be set to the specified name.", 1 );
//#endif
    if ( !cl.parse() ) { exit( EXIT_FAILURE ); }

    if ( cl.hasOption( "g" ) )
    {
        const std::string project_name( cl.optionValue<std::string>( "g" ) );
//        kvsmake::WriteMakefile( project_name );
//        return 0;
        return kvsmake::Makefile( project_name ).start( argc, argv );
    }

    if ( cl.hasOption( "G" ) )
    {
        const kvs::Directory dir( "." );
        const std::string project_name( dir.name() );
//        return kvsmake::WriteMakefile( project_name );
        return kvsmake::Makefile( project_name ).start( argc, argv );
    }

    if ( cl.hasOption( "q" ) || cl.hasOption( "qtproj" ) )
    {
        const std::string project_name( cl.optionValue<std::string>( "qtproj" ) );
//        return kvsmake::WriteQtProject( project_name );
        return kvsmake::QtProject( project_name ).start( argc, argv );
    }

    if ( cl.hasOption( "Q" ) || cl.hasOption( "Qtproj" ) )
    {
        const kvs::Directory dir( "." );
        const std::string project_name( dir.name() );
//        return kvsmake::WriteQtProject( project_name );
        return kvsmake::QtProject( project_name ).start( argc, argv );
    }

    if ( cl.hasOption( "vcproj" ) )
    {
        const std::string project_name( cl.optionValue<std::string>( "vcproj" ) );
#if defined ( KVS_COMPILER_VC )
#if KVS_COMPILER_VERSION_GREATER_OR_EQUAL( 10, 0 )
//        return kvsmake::WriteVCXProject( project_name );
        return kvsmake::VCXProject( project_name ).start( argc, argv );
#else
//        return kvsmake::WriteVCProject( project_name );
        return kvsmake::VCProject( project_name ).start( argc, argv );
#endif
#else // else KVS_COMPILER_VC
//        return kvsmake::WriteVCXProject( project_name );
        return kvsmake::VCXProject( project_name ).start( argc, argv );
#endif
    }

    if ( cl.hasOption( "Vcproj" ) )
    {
        const kvs::Directory dir( "." );
        const std::string project_name( dir.name() );
#if defined ( KVS_COMPILER_VC )
#if KVS_COMPILER_VERSION_GREATER_OR_EQUAL( 10, 0 )
//        return kvsmake::WriteVCXProject( project_name );
        return kvsmake::VCXProject( project_name ).start( argc, argv );
#else
//        return kvsmake::WriteVCProject( project_name );
        return kvsmake::VCProject( project_name ).start( argc, argv );
#endif
#else
//        return kvsmake::WriteVCXProject( project_name );
        return kvsmake::VCXProject( project_name ).start( argc, argv );
#endif
    }

    if ( cl.hasOption( "vcproj_cuda" ) )
    {
        const std::string project_name( cl.optionValue<std::string>( "vcproj_cuda" ) );
//        return kvsmake::WriteVCProjectCUDA( project_name );
        return kvsmake::VCProjectCUDA( project_name ).start( argc, argv );
    }

    if ( !kvs::File( kvsmake::MakefileName ).exists() )
    {
        kvsMessageError( "Cannot find %s.", kvsmake::MakefileName.c_str() );
        exit( EXIT_FAILURE );
    }

    std::string make_arguments;
    for ( int i = 1; i < argc; ++i )
    {
        make_arguments += std::string( " " ) + std::string( argv[i] );
    }

    const std::string command =
        kvsmake::MakeCommand + std::string( " -f " ) +
        kvsmake::MakefileName + make_arguments;

    return system( command.c_str() );
}
