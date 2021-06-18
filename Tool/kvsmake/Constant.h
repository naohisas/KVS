/****************************************************************************/
/**
 *  @file   Constant.h
 *  @author Naohisa Sakamot
 */
/****************************************************************************/
#pragma once
#include <cstdlib>
#include <string>
#include <kvs/Compiler>


namespace kvsmake
{

const std::string KVS_DIR( std::getenv( "KVS_DIR" ) );

#if defined ( KVS_COMPILER_VC )
const std::string MakeCommand( "nmake" );
const std::string MakefileName( "Makefile.vc.kvs" );
const std::string MakefileTemplate ( KVS_DIR + "\\bin\\Makefile.vc.kvs.template" );
const std::string QtProjectTemplate( KVS_DIR + "\\bin\\QtProject.template" );
const std::string VCProjectTemplate( KVS_DIR + "\\bin\\VCProject.template" );
const std::string VCXProjectTemplate( KVS_DIR + "\\bin\\VCXProject.template" );
const std::string VCProjectCUDATemplate( KVS_DIR + "\\bin\\VCProjectCUDA.template" );
#else
const std::string MakeCommand( "make" );
const std::string MakefileName( "Makefile.kvs" );
const std::string MakefileTemplate ( KVS_DIR + "/bin/Makefile.kvs.template" );
const std::string QtProjectTemplate( KVS_DIR + "/bin/QtProject.template" );
const std::string VCProjectTemplate( KVS_DIR + "/bin/VCProject.template" );
const std::string VCXProjectTemplate( KVS_DIR + "/bin/VCXProject.template" );
const std::string VCProjectCUDATemplate( KVS_DIR + "/bin/VCProjectCUDA.template" );
#endif

} // end of namespace kvsmake
