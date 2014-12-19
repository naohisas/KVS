/*****************************************************************************/
/**
 *  @file   ShaderSource.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: ShaderSource.cpp 1033 2012-02-17 16:16:41Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#include "ShaderSource.h"
#include <kvs/File>
#include <kvs/Directory>
#include <kvs/Message>
#include <kvs/String>
#include <sstream>
#include <fstream>
#include <cstdlib>
#include <vector>


namespace
{


/*===========================================================================*/
/**
 *  @brief  Return KVS shader path ("$KVS_DIR/include/Core/Visualization/Shader").
 *  @return shader path
 */
/*===========================================================================*/
std::string KVSShaderPath()
{
    const std::string sep = kvs::File::Separator();
    const char* kvs_dir = std::getenv("KVS_DIR");
    if ( kvs_dir != NULL )
    {
        std::string path = std::string( kvs_dir ) + sep;
        path += "include" + sep;
        path += "Core" + sep;
        path += "Visualization" + sep;
        path += "Shader" + sep;
        return path;
    }

    return "";
}

/*===========================================================================*/
/**
 *  @brief  Search shader path class.
 */
/*===========================================================================*/
class SearchPath
{
private:

    std::vector<std::string> m_search_path_list;

public:

    SearchPath()
    {
        this->init();
    }

    void init()
    {
        // Add "$KVS_DIR/include/Core/Visualization/Shader".
        const std::string kvs_shader_path = KVSShaderPath();
        if ( !kvs_shader_path.empty() )
        {
            m_search_path_list.push_back( kvs_shader_path );
        }

        // Add current directory (".").
        const std::string sep = kvs::File::Separator();
        m_search_path_list.push_back("." + sep );
    }

    void add( const std::string& path )
    {
        m_search_path_list.push_back( path );
    }

    void del()
    {
        m_search_path_list.clear();
    }

    std::string find( const std::string& source )
    {
        // Search the source file from the m_search_path_list.
        std::vector<std::string>::reverse_iterator path = m_search_path_list.rbegin();
        std::vector<std::string>::reverse_iterator last = m_search_path_list.rend();
        while ( path != last )
        {
            const std::string sep = kvs::File::Separator();
            const std::string filename = *path + sep + source;
            const kvs::File file( filename );
            if ( file.exists() ) { return filename; }
            path++;
        }

        return source;
    }
};

SearchPath search_path;

}

namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Adds search path.
 *  @param  path [in] path
 */
/*===========================================================================*/
void ShaderSource::AddSearchPath( const std::string& path )
{
    ::search_path.add( path );
}

/*===========================================================================*/
/**
 *  @brief  Sets search path (registerd path will be removed).
 *  @param  path [in] path
 */
/*===========================================================================*/
void ShaderSource::SetSearchPath( const std::string& path )
{
    ::search_path.del();
    ::search_path.add( path );
}

/*===========================================================================*/
/**
 *  @brief  Resets the search path (only initial path will be set).
 */
/*===========================================================================*/
void ShaderSource::ResetSearchPath()
{
    ::search_path.del();
    ::search_path.init();
}

/*===========================================================================*/
/**
 *  @brief  Remove the search path (search path will be empty).
 */
/*===========================================================================*/
void ShaderSource::RemoveSearchPath()
{
    ::search_path.del();
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new ShaderSource class.
 *  @param  source [in] filename of shader source
 */
/*===========================================================================*/
ShaderSource::ShaderSource( const std::string& source )
{
    const kvs::File file( source );
    const std::string extension = file.extension();
    if ( extension == "vert" || extension == "geom" || extension == "frag" )
    {
        if ( file.exists() )
        {
            const std::string filename( source );
            this->read( filename );
        }
        else
        {
            const std::string filename = ::search_path.find( source );
            if ( filename.empty() ) { kvsMessageError( "Cannot find '%s'.", source.c_str() ); }
            else { this->read( filename ); }
        }
    }
    else
    {
        const std::string code( source );
        this->setCode( code );
    }
}

/*===========================================================================*/
/**
 *  @brief  Returns the shade code.
 */
/*===========================================================================*/
const std::string& ShaderSource::code() const
{
    return m_code;
}

/*===========================================================================*/
/**
 *  @brief  Sets the shader code.
 *  @param  code [in] shade code
 */
/*===========================================================================*/
void ShaderSource::setCode( const std::string& code )
{
    m_code = code;
}

/*===========================================================================*/
/**
 *  @brief  Clears the shader code.
 */
/*===========================================================================*/
void ShaderSource::clearCode()
{
    m_code.erase();
}

/*===========================================================================*/
/**
 *  @brief  Defines a macro in the shader code.
 *  @param  name [in] name of macro
 */
/*===========================================================================*/
void ShaderSource::define( const std::string& name )
{
    const std::string define = "#define " + name + "\n";

    std::string::size_type version = m_code.find("#version");
    if ( version != std::string::npos )
    {
        m_code.insert( m_code.find( "\n", version ) + 1, define );
    }
    else
    {
        m_code = define + m_code;
    }
}

/*===========================================================================*/
/**
 *  @brief  Reads shader code from file.
 *  @param  filename [in] filename of shader code
 *  @return true if the reading process is done successfully
 */
/*===========================================================================*/
bool ShaderSource::read( const std::string& filename )
{
    std::string code;
    std::string content = kvs::String::FromFile( filename );
    if ( this->read_code( filename, content, code ) )
    {
        this->setCode( code );
        return true;
    }

    return false;
}

/*===========================================================================*/
/**
 *  @brief  Reads shader code and included header file.
 *  @param  filename [in] filename of shader code
 *  @param  content [in] content of shader code (included header file contents)
 *  @param  code [in] read shader code
 *  @return true if the reading process is done successfully
 */
/*===========================================================================*/
bool ShaderSource::read_code( const std::string& filename, const std::string& content, std::string& code )
{
    std::string line;
    std::istringstream source( content );
    while ( std::getline( source, line ) )
    {
        std::string::size_type p = line.find("#include");
        std::string::size_type c1 = line.find("//");
        std::string::size_type c2 = line.find("/*");
        bool comment = ( c1 != std::string::npos && c1 < p ) || ( c2 != std::string::npos && c2 < p );

        if ( p != std::string::npos && !comment )
        {
            // Found "#include" directive.
            std::string included_filename;
            std::string::size_type p1 = line.find( "\"", p + 1 );
            std::string::size_type p2 = line.find( "\"", p1 + 1 );
            if ( p1 != std::string::npos )
            {
                // Search the file from the specified path in search_path.
                included_filename = std::string( line, p1 + 1, p2 - p1 - 1 ); // without the path
                included_filename = ::search_path.find( included_filename ); // with the path
            }
            else
            {
                // Search the file from the KVS shader path.
                p1 = line.find( "<", p + 1 );
                p2 = line.find( ">", p1 + 1 );
                if ( p1 != std::string::npos )
                {
                    included_filename = std::string( line, p1 + 1, p2 - p1 - 1 ); // without the path
                    included_filename = ::KVSShaderPath() + included_filename; // with the path
                }
            }

            kvs::File included_file( included_filename );
            if ( !included_file.exists() )
            {
                kvsMessageError("Cannot find \'%s\'.", included_file.filePath().c_str() );
                return false;
            }

            std::string included_content = kvs::String::FromFile( included_file.filePath() );
            if ( !included_content.empty() && included_content[ included_content.size() - 1 ] != '\n' )
            {
                included_content += "\n";
            }

            if ( !this->read_code( filename, included_content, code ) )
            {
                return false;
            }
        }
        else
        {
            code += line + "\n";
        }
    }

    return true;
}

} // end of namespace kvs
