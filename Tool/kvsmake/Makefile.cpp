/****************************************************************************/
/**
 *  @file   Makefile.cpp
 *  @author Naohisa Sakamoto
 */
/****************************************************************************/
#include "Makefile.h"
#include <string>
#include <fstream>
#include <kvs/Message>
#include <kvs/Compiler>
#include <kvs/Directory>
#include <kvs/FileList>
#include <kvs/File>
#include <kvs/String>
#include "Constant.h"


namespace
{

/*===========================================================================*/
/**
 *  @brief  Writes a Makefile.
 *  @param  in [in] input stream
 *  @param  out [in] output stream
 *  @param  project_name [in] project name
 *  @param  use_mpi [in] if true, use MPI compiler
 */
/*===========================================================================*/
void Write( std::ifstream& in, std::ofstream& out, const std::string& project_name, const bool use_mpi )
{
#if defined ( KVS_COMPILER_VC )
    // Search cpp files.
    std::string sources( "" );

    const kvs::Directory current_dir( "." );
    const kvs::FileList file_list = current_dir.fileList();

    kvs::FileList::const_iterator iter = file_list.begin();
    const kvs::FileList::const_iterator end = file_list.end();
    while ( iter != end )
    {
        if ( iter->extension() == "cpp" )
        {
            sources += ( iter->fileName() + " \\\n" );
        }
        ++iter;
    }
#endif

    // Write a Makefile.
    while ( !in.eof() )
    {
        std::string line( "" );
        std::getline( in, line );
        line = kvs::String::Replace( line, "PROJECT_NAME_REPLACED_BY_KVSMAKE", project_name );
#if defined ( KVS_COMPILER_VC )
        line = kvs::String::Replace( line, "SOURCES_REPLACED_BY_KVSMAKE", sources );
#endif
        if ( use_mpi )
        {
            line = kvs::String::Replace( line, "(CPP)", "(MPICPP)" );
            line = kvs::String::Replace( line, "(CC)", "(MPICC)" );
            line = kvs::String::Replace( line, "(FC)", "(MPIFC)" );
            line = kvs::String::Replace( line, "(LD)", "(MPILD)" );
        }
        out << line << std::endl;
    }
}

}

namespace kvsmake
{

/*===========================================================================*/
/**
 *  @brief  Executes Makefile generation.
 *  @param  argc [in] argument count (not used)
 *  @param  argv [in] argument values (not used)
 *  @return 0 if the Makefile is generated successfully
 */
/*===========================================================================*/
int Makefile::exec( int /* argc */, char** /* argv */ )
{
    //  Open a template file.
    std::ifstream in( kvsmake::MakefileTemplate.c_str() );
    if ( !in.is_open() )
    {
        kvsMessageError() << "Cannot open " << kvsmake::MakefileTemplate << "." << std::endl;
        return false;
    }

    //  Open a Makefile.
    std::ofstream out( kvsmake::MakefileName.c_str() );
    if ( !out.is_open() )
    {
        kvsMessageError() << "Cannot open " << kvsmake::MakefileName << "." << std::endl;
        return false;
    }

    ::Write( in, out, m_project_name, m_use_mpi );
    return true;
}

} // end of namespace kvsmake
