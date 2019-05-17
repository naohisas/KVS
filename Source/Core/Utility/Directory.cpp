/****************************************************************************/
/**
 *  @file Directory.cpp
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: Directory.cpp 1597 2013-06-13 01:27:02Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#include "Directory.h"
#include <kvs/Message>
#include <kvs/Platform>
#if defined ( KVS_PLATFORM_WINDOWS )
#include <windows.h>
#include <mbstring.h>
#include <direct.h>
#else
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <pwd.h>
#include <cstring>
#include <cerrno>
#include <climits>
#endif
#include <cstdlib>


namespace
{

#if defined ( KVS_PLATFORM_WINDOWS )
enum MBCharType
{
    MBTypeSB,
    MBTypeDB1,
    MBTypeDB2,
    MBTypeERR
};

/*==========================================================================*/
/**
 *  Test a string to determine whether it is lead-bytes of MB characters.
 *  @param str [in] string
 *  @param num [in] number of characters composed of the string
 *  @return nonezero, if str is lead-bytes of multibyte characters.
 */
/*==========================================================================*/
int IsMBCharLeadByte( const char* str, int num )
{
    int k;
    if ( !_ismbblead( str[num] ) ) { return 0; }
    k = num;
    while ( --k >= 0 && _ismbblead( str[k] ) )
    {
        ;
    }
    return ( num - k ) & 1;
}

/*==========================================================================*/
/**
 *  Get a type of multibyte character.
 *  @param str [in] string
 *  @param num [in] number of characters composed of the string
 *  @return type of multibyte character.
 */
/*==========================================================================*/
MBCharType GetMBCharType( const char* str, int num )
{
    if ( num > 0 && IsMBCharLeadByte( str, num - 1 ) )
    {
        return _ismbbtrail( str[num] ) ? MBTypeDB2 : MBTypeERR;
    }
    else
    {
        return _ismbblead( str[num] ) ? MBTypeDB1 : MBTypeSB;
    }
}
#endif

} // end of namespace


namespace kvs
{

/*==========================================================================*/
/**
 *  Get the separator.
 *  @return separator
 */
/*==========================================================================*/
std::string Directory::Separator()
{
#if defined ( KVS_PLATFORM_WINDOWS )
    return "\\";
#else
    return "/";
#endif
}

/*===========================================================================*/
/**
 *  @brief  Returns absolute path for the specified directory path.
 *  @param  directory_path [in] directory path
 *  @return absolute path
 */
/*===========================================================================*/
std::string Directory::Absolute( const std::string& directory_path )
{
    std::string path = directory_path;
    if ( path[0] == '~' ) { path = kvs::Directory::HomePath() + path.substr(1); }

#if defined ( KVS_PLATFORM_WINDOWS )
    char absolute_path[ _MAX_PATH ];
    if ( _fullpath( absolute_path, path.c_str(), _MAX_PATH ) )
#else
    char absolute_path[ PATH_MAX ];
    if ( realpath( path.c_str(), absolute_path ) )
#endif
    {
        return absolute_path;
    }

    return "";
}

/*===========================================================================*/
/**
 *  @brief  Returns true if the specified directory exists.
 *  @param  directory_path [in] directory_path (directory name)
 *  @return true if the directory exists
 */
/*===========================================================================*/
bool Directory::Exists( const std::string& directory_path )
{
    kvs::Directory dir( directory_path );
    return dir.exists();
}

/*==========================================================================*/
/**
 *  Make directory.
 *  @param directory_path [in] directory path
 *  @return true, if the directory is maked successfully
 */
/*==========================================================================*/
bool Directory::Make( const std::string& directory_path )
{
#if defined ( KVS_PLATFORM_WINDOWS )
    return _mkdir( Directory::Absolute( directory_path ).c_str() ) == 0;
#else
    return mkdir( Directory::Absolute( directory_path ).c_str(), 0777 ) == 0;
#endif
}

/*==========================================================================*/
/**
 *  Remove directory.
 *  @param directory_path [in] directory path
 *  @return true, if the directory is removed successfully
 */
/*==========================================================================*/
bool Directory::Remove( const std::string& directory_path )
{
#if defined ( KVS_PLATFORM_WINDOWS )
    return _rmdir( Directory::Absolute( directory_path ).c_str() ) == 0;
#else
    return rmdir( Directory::Absolute( directory_path ).c_str() ) == 0;
#endif
}

/*==========================================================================*/
/**
 *  Change directory.
 *  @param directory_path [in] directory path
 *  @return true, if the directory is changed successfully
 */
/*==========================================================================*/
bool Directory::Change( const std::string& directory_path )
{
#if defined ( KVS_PLATFORM_WINDOWS )
    return _chdir( Directory::Absolute( directory_path ).c_str() ) == 0;
#else
    return chdir( Directory::Absolute( directory_path ).c_str() ) == 0;
#endif
}

/*===========================================================================*/
/**
 *  @brief  Returns root directory path.
 *  @return root directory path
 */
/*===========================================================================*/
std::string Directory::RootPath()
{
#if defined ( KVS_PLATFORM_WINDOWS )
    const char* system_drive = std::getenv( "SystemDrive" );
    if ( system_drive ) { return std::string( system_drive ); }
    return std::string("C:");
#else
    return std::string("/");
#endif
}

/*===========================================================================*/
/**
 *  @brief  Returns home directory path.
 *  @return home directory path
 */
/*===========================================================================*/
std::string Directory::HomePath()
{
#if defined ( KVS_PLATFORM_WINDOWS )
    const char* user_profile = std::getenv( "USERPROFILE" );
    if ( user_profile ) { return std::string( user_profile ); }

    const char* home_drive = std::getenv( "HOMEDRIVE" );
    const char* home_path = std::getenv( "HOMEPATH" );
    if ( home_drive && home_path ) { return std::string( home_drive ) + std::string( home_path ); }

    const char* home = std::getenv( "HOME" );
    if ( home ) { return std::string( home ); }
#else
    const char* home = std::getenv( "HOME" );
    if ( home ) { return std::string( home ); }

    struct passwd* pwd = getpwuid( getuid() );
    if ( pwd ) { return std::string( pwd->pw_dir ); }
#endif
    return Directory::RootPath();
}

/*===========================================================================*/
/**
 *  @brief  Returns temp directory path.
 *  @return temp directory path
 */
/*===========================================================================*/
std::string Directory::TempPath()
{
#if defined ( KVS_PLATFORM_WINDOWS )
    const char* temp = std::getenv( "TEMP" );
    if ( temp ) { return std::string( temp ); }

    const char* tmp = std::getenv( "TMP" );
    if ( tmp ) { return std::string( tmp ); }

    return Directory::RootPath();
#else
    const char* tmp_dir = std::getenv( "TMPDIR" );
    if ( tmp_dir ) { return std::string( tmp_dir ); }

    return std::string("/tmp");
#endif
}

/*===========================================================================*/
/**
 *  @brief  Returns current directory path.
 *  @return current directory path
 */
/*===========================================================================*/
std::string Directory::CurrentPath()
{
#if defined ( KVS_PLATFORM_WINDOWS )
    char current_path[ _MAX_PATH ];
    if ( _getcwd( current_path, _MAX_PATH ) )
#else
    char current_path[ PATH_MAX ];
    if ( getcwd( current_path, PATH_MAX ) )
#endif
    {
        return current_path;
    }

    return ".";
}

/*==========================================================================*/
/**
 *  Constructor.
 */
/*==========================================================================*/
Directory::Directory():
    m_path( "" ),
    m_name( "" )
{
}

/*==========================================================================*/
/**
 *  Constructor.
 *  @param directory_path [in] directory path
 */
/*==========================================================================*/
Directory::Directory( const std::string& directory_path ):
    m_path( "" ),
    m_name( "" )
{
    this->parse( directory_path );
}

/*==========================================================================*/
/**
 *  Destructor.
 */
/*==========================================================================*/
Directory::~Directory()
{
}

/*==========================================================================*/
/**
 *  Get directory path.
 *  @param absolute [in] flag for determing whether path is absolute-path
 *  @return directory path
 */
/*==========================================================================*/
std::string Directory::path( bool absolute ) const
{
    return absolute ? Directory::Absolute( m_path ) : m_path;
}

/*==========================================================================*/
/**
 *  Get directory name.
 *  @return directory name
 */
/*==========================================================================*/
std::string Directory::name() const
{
    return m_name;
}

/*===========================================================================*/
/**
 *  @brief  Returns file list contained in the directory.
 *  @param  sort [in] flag for sorting files
 *  @return file list
 */
/*===========================================================================*/
kvs::FileList Directory::fileList( const bool sort ) const
{
    kvs::FileList file_list;

#if defined ( KVS_PLATFORM_WINDOWS )
    WIN32_FIND_DATAA find_data;
    HANDLE hFind;

    char bufname[ _MAX_PATH ];
    strcpy( bufname, m_path.c_str() );
    int len = strlen( bufname );
    /* If len is 0, dirname is empty. so I must get current directory
     * entry(i.e. "*.*") and must not add '\'.
     */
    if ( len )
    {
        if ( bufname[ len - 1 ] != '\\' || ::GetMBCharType( bufname, len - 1 ) == MBTypeDB2 )
        {
            bufname[ len++ ] = '\\';
        }
    }

    strcpy( bufname + len, "*.*" );
    hFind = FindFirstFileA( bufname, &find_data );
    do
    {
        const std::string path( m_path );
        const std::string filename( find_data.cFileName );
        const kvs::File file( path + Directory::Separator() + filename );

        if ( file.isFile() )
        {
            file_list.push_back( file );
        }
    }
    while ( FindNextFileA( hFind, &find_data ) );

    FindClose( hFind );
#else
    if ( this->exists() )
    {
        DIR* dir = opendir( m_path.c_str() );
        if ( !dir )
        {
            kvsMessageError( "%s is not opened.", m_path.c_str() );
            return file_list;
        }

        struct dirent* ent;
        while ( ( ent = readdir( dir ) ) != NULL )
        {
            const std::string path( m_path );
            const std::string filename( ent->d_name );
            const kvs::File file( path + Directory::Separator() + filename );
            if ( file.isFile() ) { file_list.push_back( file ); }
        }

        closedir( dir );
    }
#endif

    if ( sort ) { std::sort( file_list.begin(), file_list.end() ); }
    return file_list;
}

/*==========================================================================*/
/**
 *  Test to determine whether given directory is a directory.
 *  @return true, if given directory is directory
 */
/*==========================================================================*/
bool Directory::isDirectory() const
{
#if defined ( KVS_PLATFORM_WINDOWS )
    WIN32_FIND_DATAA find_data;
    FindFirstFileA( m_path.c_str(), &find_data );
    return ( find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) != 0;
#else
    struct stat filestat;
    if ( stat( m_path.c_str(), &filestat ) ) { return false; }
    return filestat.st_mode & S_IFDIR;
#endif
}

/*==========================================================================*/
/**
 *  Test to determin whether given directory is existed.
 *  @return true, if given directory is existed
 */
/*==========================================================================*/
bool Directory::exists() const
{
    return this->isDirectory();
}

/*==========================================================================*/
/**
 *  Parse given directory path.
 *  @param directory_path [in] directory path
 *  @return true, if the parse process is done successfully
 */
/*==========================================================================*/
bool Directory::parse( const std::string& directory_path )
{
    std::string path = directory_path;
    if ( path[0] == '~' ) { path = kvs::Directory::HomePath() + path.substr(1); }
    m_path = path;

    const std::string abs_dir_path = Directory::Absolute( directory_path );
    const size_t last_sep_pos = abs_dir_path.find_last_of( Directory::Separator() );
    m_name = abs_dir_path.substr( last_sep_pos + 1 );

    return true;
}

} // end of namespace kvs
