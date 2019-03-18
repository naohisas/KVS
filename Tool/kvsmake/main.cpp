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
#if defined ( KVS_COMPILER_VC )
#include "WriteVCProject.h"
#include "WriteVCProjectCUDA.h"
#include "WriteVCXProject.h"
#endif

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

    kvs::CommandLine argument( argc, argv );
    argument.addHelpOption();
    argument.addValue( "project_name/make_options", false );
    argument.addOption( "g", "Generate a makefile.", 1 );
    argument.addOption( "G", "Generate a makefile and set it's target name to the current directory name", 0 );
    argument.addOption( "q", "Generate a project file for Qt.", 1 );
    argument.addOption( "Q", "Generate a project file for Qt and set it's target name to the current directory name.", 0 );
    argument.addOption( "qtproj", "Generate a project file for Qt.", 1 );
    argument.addOption( "Qtproj", "Generate a project file for Qt and set it's target name to the current directory name.", 0 );
#if defined ( KVS_COMPILER_VC )
    argument.addOption( "vcproj", "generate a project file for VC and set it's target name to the current directory name.", 0 );
    argument.addOption( "Vcproj", "generate a project file for VC.", 1 );
    argument.addOption( "vcproj_cuda", "generate a project file for VC and CUDA.", 1 );
#endif
    if ( !argument.parse() ) { exit( EXIT_FAILURE ); }

    if ( argument.hasOption( "g" ) )
    {
        const std::string project_name( argument.optionValue<std::string>( "g" ) );
        kvsmake::WriteMakefile( project_name );
        return 0;
    }

    if ( argument.hasOption( "G" ) )
    {
        const kvs::Directory dir( "." );
        const std::string project_name( dir.name() );
        return kvsmake::WriteMakefile( project_name );
    }

    if ( argument.hasOption( "q" ) || argument.hasOption( "qtproj" ) )
    {
        const std::string project_name( argument.optionValue<std::string>( "qtproj" ) );
        return kvsmake::WriteQtProject( project_name );
    }

    if ( argument.hasOption( "Q" ) || argument.hasOption( "Qtproj" ) )
    {
        const kvs::Directory dir( "." );
        const std::string project_name( dir.name() );
        return kvsmake::WriteQtProject( project_name );
    }

#if defined ( KVS_COMPILER_VC )
    if ( argument.hasOption( "vcproj" ) )
    {
        const std::string project_name( argument.optionValue<std::string>( "vcproj" ) );
#if KVS_COMPILER_VERSION_GREATER_OR_EQUAL( 10, 0 )
        return kvsmake::WriteVCXProject( project_name );
#else
        return kvsmake::WriteVCProject( project_name );
#endif
    }

    if ( argument.hasOption( "Vcproj" ) )
    {
        const kvs::Directory dir( "." );
        const std::string project_name( dir.name() );
#if KVS_COMPILER_VERSION_GREATER_OR_EQUAL( 10, 0 )
        return kvsmake::WriteVCXProject( project_name );
#else
        return kvsmake::WriteVCProject( project_name );
#endif
    }

    if ( argument.hasOption( "vcproj_cuda" ) )
    {
        const std::string project_name( argument.optionValue<std::string>( "vcproj_cuda" ) );
        return kvsmake::WriteVCProjectCUDA( project_name );
    }
#endif // KVS_COMPILER_VC

    if ( !kvs::File( kvsmake::Makefile ).exists() )
    {
        kvsMessageError( "Cannot find %s.", kvsmake::Makefile.c_str() );
        exit( EXIT_FAILURE );
    }

    std::string make_arguments;
    for ( int i = 1; i < argc; ++i )
    {
        make_arguments += std::string( " " ) + std::string( argv[i] );
    }

    const std::string command =
        kvsmake::MakeCommand +
        std::string( " -f " ) +
        kvsmake::Makefile +
        make_arguments;

    return system( command.c_str() );
}
