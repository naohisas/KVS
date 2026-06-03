/****************************************************************************/
/**
 *  @file   QtProject.cpp
 *  @author Naohisa Sakamoto
 */
/****************************************************************************/
#include "QtProject.h"
#include <cstdlib>
#include <exception>
#include <fstream>
#include <string>
#include <kvs/Directory>
#include <kvs/FileList>
#include <kvs/File>
#include <kvs/Message>
#include <kvs/String>
#include <kvs/Tokenizer>


namespace
{

struct DefaultValue
{
    bool found = false;
    std::string value{};
};

std::string EnvironmentValue( const char* name )
{
    const char* value = std::getenv( name );
    return value ? std::string( value ) : std::string();
}

std::string Trim( const std::string& value )
{
    const std::string spaces( " \t\r\n" );
    const std::string::size_type first = value.find_first_not_of( spaces );
    if ( first == std::string::npos ) { return std::string(); }

    const std::string::size_type last = value.find_last_not_of( spaces );
    return value.substr( first, last - first + 1 );
}

std::string RemoveComment( const std::string& line )
{
    const std::string::size_type pos = line.find( '#' );
    return pos == std::string::npos ? line : line.substr( 0, pos );
}

std::string JoinPath( const std::string& directory, const std::string& name )
{
    const std::string sep = kvs::Directory::Separator();

    if ( directory.empty() ) { return name; }
    if ( directory.size() >= sep.size() &&
         directory.substr( directory.size() - sep.size() ) == sep )
    {
        return directory + name;
    }

    return directory + sep + name;
}

bool GetAssignmentValue( const std::string& line, const std::string& name, std::string* value )
{
    const std::string stripped = Trim( RemoveComment( line ) );
    if ( stripped.empty() ) { return false; }

    std::string::size_type pos = stripped.find( ":=" );
    std::string::size_type op_length = 2;
    if ( pos == std::string::npos )
    {
        pos = stripped.find( '=' );
        op_length = 1;
    }
    if ( pos == std::string::npos ) { return false; }

    const std::string lhs = Trim( stripped.substr( 0, pos ) );
    if ( lhs != name ) { return false; }

    *value = Trim( stripped.substr( pos + op_length ) );
    return true;
}

DefaultValue ReadMakefileDefault( const std::string& name )
{
    DefaultValue result;

    const std::string kvs_dir = EnvironmentValue( "KVS_DIR" );
    if ( kvs_dir.empty() )
    {
        kvsMessageError() << "KVS_DIR is not defined." << std::endl;
        return result;
    }

    const std::string filename = JoinPath( kvs_dir, "Makefile.def" );
    if ( !kvs::File::Exists( filename ) )
    {
        kvsMessageError() << filename << " does not exist." << std::endl;
        return result;
    }

    std::string contents;
    try
    {
        contents = kvs::String::FromFile( filename );
    }
    catch ( const std::exception& e )
    {
        kvsMessageError() << e.what() << std::endl;
        return result;
    }

    bool in_ifndef = false;
    kvs::Tokenizer lines( contents, "\n" );
    while ( !lines.isLast() )
    {
        const std::string line = Trim( RemoveComment( lines.token() ) );
        if ( line.empty() ) { continue; }

        kvs::Tokenizer tokens( line, " \t" );
        const std::string first = tokens.token();

        if ( !in_ifndef )
        {
            if ( first == "ifndef" && tokens.token() == name )
            {
                in_ifndef = true;
            }
        }
        else
        {
            std::string value;
            if ( GetAssignmentValue( line, name, &value ) )
            {
                result.found = true;
                result.value = value; // Empty value is valid.
                return result;
            }

            if ( first == "endif" )
            {
                in_ifndef = false;
            }
        }
    }

    kvsMessageError() << "Cannot find default value for " << name << " in " << filename << "." << std::endl;
    return result;
}

DefaultValue KVSDefaultValue( const std::string& name )
{
    const std::string value = EnvironmentValue( name.c_str() );
    if ( !value.empty() )
    {
        DefaultValue result;
        result.found = true;
        result.value = value;
        return result;
    }

    return ReadMakefileDefault( name );
}

void Write(
    std::ifstream& in,
    std::ofstream& out,
    const std::string& project_name,
    const std::string& cpp_standard,
    const std::string& macosx_deployment_target )
{
    std::string headers( "" );
    std::string sources( "" );
    std::string qresrcs( "" );

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

    std::string line;
    while ( std::getline( in, line ) )
    {
        line = kvs::String::Replace( line, "PROJECT_NAME_REPLACED_BY_KVSMAKE", project_name );
        line = kvs::String::Replace( line, "HEADERS_REPLACED_BY_KVSMAKE", headers );
        line = kvs::String::Replace( line, "SOURCES_REPLACED_BY_KVSMAKE", sources );
        line = kvs::String::Replace( line, "QRESRCS_REPLACED_BY_KVSMAKE", qresrcs );
        line = kvs::String::Replace( line, "KVS_CPP_STANDARD_REPLACED_BY_KVSMAKE", cpp_standard );
        line = kvs::String::Replace( line, "KVS_MACOSX_DEPLOYMENT_TARGET_REPLACED_BY_KVSMAKE", macosx_deployment_target );
        out << line << std::endl;
    }
}

} // end of namespace


namespace kvsmake
{

int QtProject::exec()
{
    const std::string kvs_dir = EnvironmentValue( "KVS_DIR" );
    if ( kvs_dir.empty() )
    {
        kvsMessageError() << "KVS_DIR is not defined." << std::endl;
        return EXIT_FAILURE;
    }

    const DefaultValue cpp_standard = KVSDefaultValue( "KVS_CPP_STANDARD" );
    if ( !cpp_standard.found || cpp_standard.value.empty() )
    {
        kvsMessageError() << "KVS_CPP_STANDARD is not defined." << std::endl;
        return EXIT_FAILURE;
    }

    const DefaultValue macosx_deployment_target = KVSDefaultValue( "KVS_MACOSX_DEPLOYMENT_TARGET" );
    if ( !macosx_deployment_target.found )
    {
        kvsMessageError() << "KVS_MACOSX_DEPLOYMENT_TARGET is not defined in Makefile.def." << std::endl;
        return EXIT_FAILURE;
    }

    const std::string template_file = JoinPath( JoinPath( kvs_dir, "bin" ), "QtProject.template" );
    std::ifstream in( template_file.c_str() );
    if ( !in.is_open() )
    {
        kvsMessageError() << "Cannot open " << template_file << "." << std::endl;
        return EXIT_FAILURE;
    }

    const std::string filename( m_project_name + ".pro" );
    std::ofstream out( filename.c_str() );
    if ( !out.is_open() )
    {
        kvsMessageError() << "Cannot open " << filename << "." << std::endl;
        return EXIT_FAILURE;
    }

    ::Write(
        in,
        out,
        m_project_name,
        cpp_standard.value,
        macosx_deployment_target.value );

    return EXIT_SUCCESS;
}

} // end of namespace kvsmake
