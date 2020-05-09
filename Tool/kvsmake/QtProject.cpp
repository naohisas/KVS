/****************************************************************************/
/**
 *  @file   QtProject.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: WriteQtProject.cpp 1409 2012-12-21 15:19:08Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#include "QtProject.h"
#include <string>
#include <fstream>
#include <kvs/Message>
#include <kvs/Directory>
#include <kvs/FileList>
#include <kvs/File>
#include <kvs/String>
#include "Constant.h"


namespace
{

/*===========================================================================*/
/**
 *  @brief  Writes a Qt project file.
 *  @param  in [in] input stream
 *  @param  out [in] output stream
 *  @param  project_name [in] project name
 */
/*===========================================================================*/
void Write( std::ifstream& in, std::ofstream& out, const std::string& project_name )
{
    //  Search the project's condition.
    std::string headers( "" );
    std::string sources( "" );
    std::string qresrcs( "" );

    // Search cpp files and h files.
    const kvs::Directory current_dir( "." );
    const kvs::FileList file_list = current_dir.fileList();

    kvs::FileList::const_iterator iter = file_list.begin();
    const kvs::FileList::const_iterator end = file_list.end();

    while ( iter != end )
    {
        if ( iter->extension() == "h" )
        {
            headers += ( iter->fileName() + " \\\n" );
        }
        else if ( iter->extension() == "cpp" )
        {
            sources += ( iter->fileName() + " \\\n" );
        }
        else if ( iter->extension() == "qrc" )
        {
            qresrcs += ( iter->fileName() + " \\\n" );
        }

        ++iter;
    }

    // Write a project file.
    while ( !in.eof() )
    {
        std::string line( "" );
        std::getline( in, line );
        line = kvs::String::Replace( line, "PROJECT_NAME_REPLACED_BY_KVSMAKE", project_name );
        line = kvs::String::Replace( line, "HEADERS_REPLACED_BY_KVSMAKE", headers );
        line = kvs::String::Replace( line, "SOURCES_REPLACED_BY_KVSMAKE", sources );
        line = kvs::String::Replace( line, "QRESRCS_REPLACED_BY_KVSMAKE", qresrcs );
        out << line << std::endl;
    }
}

}

namespace kvsmake
{

/*===========================================================================*/
/**
 *  @brief  Executes Qt project file generation.
 *  @param  argc [in] argument count (not used)
 *  @param  argv [in] argument values (not used)
 *  @return 0 if the project file is generated successfully
 */
/*===========================================================================*/
int QtProject::exec( int /* argc */, char** /* argv */ )
{
    //  Open a template file.
    std::ifstream in( kvsmake::QtProjectTemplate.c_str() );
    if ( !in.is_open() )
    {
        kvsMessageError() << "Cannot open " << kvsmake::QtProjectTemplate << "." << std::endl;
        return false;
    }
    //  Open a project file.
    const std::string filename( m_project_name + ".pro" );
    std::ofstream out( filename.c_str() );
    if ( !out.is_open() )
    {
        kvsMessageError() << "Cannot open " << filename << "." << std::endl;;
        return false;
    }

    ::Write( in, out, m_project_name );
    return true;
}

} // end of namespace kvsmake
