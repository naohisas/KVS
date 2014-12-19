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
#include <cstring>
#include <cerrno>
#endif
#include <kvs/Message>

#ifdef PATH_MAX
#define MAX_PATH_LENGTH PATH_MAX
#else
#define MAX_PATH_LENGTH 4096
#endif


namespace
{

/*==========================================================================*/
/**
 *  Get current path.
 *  @param path [in] path
 *  @return current path (absolute)
 */
/*==========================================================================*/
inline std::string GetCurrentPath()
{
#if defined ( KVS_PLATFORM_WINDOWS )
    char current_path[MAX_PATH_LENGTH];
    _getcwd( current_path, MAX_PATH_LENGTH );
#else
    char current_path[MAX_PATH_LENGTH];
    if ( !getcwd( current_path, MAX_PATH_LENGTH ) )
    {
        kvsMessageError( "%s", strerror( errno ) );
        return ".";
     }
#endif

    return current_path;
}

/*==========================================================================*/
/**
 *  Get absolute path of given path.
 *  @param path [in] path
 *  @return absolute path
 */
/*==========================================================================*/
inline std::string GetAbsolutePath( const std::string& path )
{
#if defined ( KVS_PLATFORM_WINDOWS )
    char absolute_path[MAX_PATH_LENGTH];
    _fullpath( absolute_path, const_cast<char*>( path.c_str() ), MAX_PATH_LENGTH );
#else
    char absolute_path[MAX_PATH_LENGTH];
#if defined ( KVS_PLATFORM_CYGWIN )
    /* WARNING: In the case of the cygwin environment, the realpath function
     * returns NULL as the absolute path when an error occurs, for example,
     * the directory which is not existed is given. In order to deal with this
     * problem, the GetAbsolutePath function returns an uncanonical absolute
     * path (ex. "/aaa/bbb/ccc/../ddd").
     */
    bool is_existed = false;
    struct stat filestat;
    if ( !stat( path.c_str(), &filestat ) )
    {
        is_existed = ( filestat.st_mode & S_IFDIR );
    }

    if ( !is_existed )
    {
        return GetCurrentPath() + "/" + path;
    }
#endif
    if ( !realpath( path.c_str(), absolute_path ) )
    {
        kvsMessageError( "%s", strerror( errno ) );
        return "";
    }
#endif

    return absolute_path;
}

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
    return File::Separator();
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
    return _mkdir( ::GetAbsolutePath( directory_path ).c_str() ) == 0;
#else
    return mkdir( ::GetAbsolutePath( directory_path ).c_str(), 0777 ) == 0;
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
    return _rmdir( ::GetAbsolutePath( directory_path ).c_str() ) == 0;
#else
    return  rmdir( ::GetAbsolutePath( directory_path ).c_str() ) == 0;
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
    return _chdir( ::GetAbsolutePath( directory_path ).c_str() ) == 0;
#else
    return chdir( ::GetAbsolutePath( directory_path ).c_str() ) == 0;
#endif
}

/*==========================================================================*/
/**
 *  Get current directory path.
 *  @return current directory path
 */
/*==========================================================================*/
Directory Directory::Current()
{
    return Directory( ::GetCurrentPath() );
}

/*==========================================================================*/
/**
 *  Constructor.
 */
/*==========================================================================*/
Directory::Directory():
    m_directory_path( "" ),
    m_directory_name( "" ),
    m_file_list()
{
}

/*==========================================================================*/
/**
 *  Constructor.
 *  @param directory_path [in] directory path
 */
/*==========================================================================*/
Directory::Directory( const std::string& directory_path ):
    m_directory_path( directory_path ),
    m_directory_name( "" ),
    m_file_list()
{
    this->parse( directory_path );
    this->sort();
}

/*==========================================================================*/
/**
 *  Destructor.
 */
/*==========================================================================*/
Directory::~Directory()
{
    m_file_list.clear();
}

/*==========================================================================*/
/**
 *  Get directory path.
 *  @param absolute [in] flag for determing whether path is absolute-path
 *  @return directory path
 */
/*==========================================================================*/
std::string Directory::directoryPath( bool absolute ) const
{
    return absolute ? ::GetAbsolutePath( m_directory_path ) : m_directory_path;
}

/*==========================================================================*/
/**
 *  Get directory name.
 *  @return directory name
 */
/*==========================================================================*/
std::string Directory::directoryName() const
{
    return m_directory_name;
}

/*==========================================================================*/
/**
 *  Get file list.
 *  @return file list
 */
/*==========================================================================*/
kvs::FileList& Directory::fileList()
{
    return m_file_list;
}

/*==========================================================================*/
/**
 *  Get file list.
 *  @return file list
 */
/*==========================================================================*/
const kvs::FileList& Directory::fileList() const
{
    return m_file_list;
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
    FindFirstFileA( m_directory_path.c_str(), &find_data );
    return ( find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) != 0;
#else
    struct stat filestat;
    if ( stat( m_directory_path.c_str(), &filestat ) ) { return false; }
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
    m_directory_path = directory_path;

    const std::string absolute_directory_path =
        ::GetAbsolutePath( directory_path );
    const size_t last_sep_pos =
        absolute_directory_path.find_last_of( Directory::Separator() );

    m_directory_name = absolute_directory_path.substr( last_sep_pos + 1 );

#if defined ( KVS_PLATFORM_WINDOWS )
    WIN32_FIND_DATAA find_data;
    HANDLE           hFind;

    char bufname[MAX_PATH_LENGTH];

    strcpy( bufname, m_directory_path.c_str() );
    int len = strlen( bufname );
    /* If len is 0, dirname is empty. so I must get current directory
     * entry(i.e. "*.*") and must not add '\'.
     */
    if ( len )
    {
        if ( bufname[ len - 1 ] != '\\' ||
             ::GetMBCharType( bufname, len - 1 ) == MBTypeDB2 )
        {
            bufname[ len++ ] = '\\';
        }
    }

    strcpy( bufname + len, "*.*" );
    hFind = FindFirstFileA( bufname, &find_data );
    do
    {
        const std::string path( m_directory_path );
        const std::string filename( find_data.cFileName );
        const kvs::File   file( path + kvs::Directory::Separator() + filename );

        if ( file.isFile() )
        {
            m_file_list.push_back( file );
        }
    }
    while ( FindNextFileA( hFind, &find_data ) );

    FindClose( hFind );
#else
    if ( this->exists() )
    {
        DIR* dir = opendir( m_directory_path.c_str() );

        if ( !dir )
        {
            kvsMessageError( "%s is not opened.", m_directory_path.c_str() );
            return false;
        }

        struct dirent* ent;
        while ( ( ent = readdir( dir ) ) != NULL )
        {
            const std::string path( m_directory_path );
            const std::string filename( ent->d_name );
            const kvs::File   file( path + kvs::Directory::Separator() + filename );

            if ( file.isFile() )
            {
                m_file_list.push_back( file );
            }
        }

        closedir( dir );
    }
#endif

    return true;
}

/*==========================================================================*/
/**
 *  Sorting filename list.
 */
/*==========================================================================*/
void Directory::sort()
{
    std::sort( m_file_list.begin(), m_file_list.end() );
}

/*==========================================================================*/
/**
 *  Find file from given directory.
 *  @param file [in] file
 *  @return iterator of the found file
 */
/*==========================================================================*/
kvs::FileList::iterator Directory::find( const File& file )
{
    kvs::FileList::iterator begin = m_file_list.begin();
    kvs::FileList::iterator end   = m_file_list.end();

    return std::find( begin, end, file );
}

/*==========================================================================*/
/**
 *  Find file from given directory.
 *  @param file [in] file
 *  @return iterator of the found file
 */
/*==========================================================================*/
kvs::FileList::const_iterator Directory::find( const File& file ) const
{
    kvs::FileList::const_iterator begin = m_file_list.begin();
    kvs::FileList::const_iterator end   = m_file_list.end();

    return std::find( begin, end, file );
}

} // end of namespace kvs
