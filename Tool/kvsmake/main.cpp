/****************************************************************************/
/**
 *  @file   main.cpp
 *  @author Naohisa Sakamoto
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
#include "Build.h"
#include "Constant.h"
#include "Makefile.h"
#include "QtProject.h"
#include "VCProject.h"
#include "VCXProject.h"
#include "VCProjectCUDA.h"

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
    cl.addOption( "g", "Generate a Makefile. The target name will be set to the specified name.", 1 );
    cl.addOption( "G", "Generate a Makefile. The target name will be automatically set to the current directory name.", 0 );
    cl.addOption( "q", "Generate a Qt project file. The target name will be set to the specified name.", 1 );
    cl.addOption( "Q", "Generate a Qt project file. The target name will be automatically set to the current directory name.", 0 );
    cl.addOption( "qtproj", "Generate a Qt project file. The target name will be set to the specified name.", 1 );
    cl.addOption( "Qtproj", "Generate a Qt project file. The target name will be automatically set to the current directory name.", 0 );
    cl.addOption( "vcproj", "generate a MSVC project file. The target name will be set to the specified name.", 1 );
    cl.addOption( "Vcproj", "generate a MSVC project file. The target name will be automatically set to the current directory name.", 0 );
    cl.addOption( "vcproj_cuda", "generate a VC project file with CUDA option. The target name will be set to the specified name.", 1 );
    cl.addOption( "use_mpi", "Use MPI compiler.", 0 );
    if ( !cl.parse() ) { exit( EXIT_FAILURE ); }

    if ( cl.hasOption( "g" ) )
    {
        const std::string project_name( cl.optionValue<std::string>( "g" ) );
        return kvsmake::Makefile( project_name, cl.hasOption( "use_mpi" ) ).run();
    }

    if ( cl.hasOption( "G" ) )
    {
        const kvs::Directory dir( "." );
        const std::string project_name( dir.name() );
        return kvsmake::Makefile( project_name, cl.hasOption( "use_mpi" ) ).run();
    }

    if ( cl.hasOption( "q" ) || cl.hasOption( "qtproj" ) )
    {
        const std::string project_name( cl.optionValue<std::string>( "qtproj" ) );
        return kvsmake::QtProject( project_name ).run();
    }

    if ( cl.hasOption( "Q" ) || cl.hasOption( "Qtproj" ) )
    {
        const kvs::Directory dir( "." );
        const std::string project_name( dir.name() );
        return kvsmake::QtProject( project_name ).run();
    }

    if ( cl.hasOption( "vcproj" ) )
    {
        const std::string project_name( cl.optionValue<std::string>( "vcproj" ) );
#if defined ( KVS_COMPILER_VC )
#if KVS_COMPILER_VERSION_GREATER_OR_EQUAL( 10, 0 )
        return kvsmake::VCXProject( project_name ).run();
#else
        return kvsmake::VCProject( project_name ).run();
#endif
#else // else KVS_COMPILER_VC
        return kvsmake::VCXProject( project_name ).run();
#endif
    }

    if ( cl.hasOption( "Vcproj" ) )
    {
        const kvs::Directory dir( "." );
        const std::string project_name( dir.name() );
#if defined ( KVS_COMPILER_VC )
#if KVS_COMPILER_VERSION_GREATER_OR_EQUAL( 10, 0 )
        return kvsmake::VCXProject( project_name ).run();
#else
        return kvsmake::VCProject( project_name ).run();
#endif
#else
        return kvsmake::VCXProject( project_name ).run();
#endif
    }

    if ( cl.hasOption( "vcproj_cuda" ) )
    {
        const std::string project_name( cl.optionValue<std::string>( "vcproj_cuda" ) );
        return kvsmake::VCProjectCUDA( project_name ).run();
    }

    return kvsmake::Build( argc, argv ).run();
}
