/****************************************************************************/
/**
 *  @file   File.h
 *  @author Naohisa Sakamoto
 */
/****************************************************************************/
#pragma once
#include <string>
#include <kvs/Deprecated>


namespace kvs
{

/*==========================================================================*/
/**
 *  File class
 */
/*==========================================================================*/
class File
{
public:
    static bool Exists( const std::string& file_path );

private:
    std::string m_file_path{}; ///< file path (ex. "/tmp/abc/xyz.tar.gz")
    std::string m_path_name{}; ///< path name (ex. "/tmp/abc")
    std::string m_file_name{}; ///< file name (ex. "xyz.tar.gz")

public:
    File() = default;
    File( const std::string& file_path );
    virtual ~File() = default;

    bool operator <( const File& file ) const;
    bool operator ==( const File& file ) const;

    std::string filePath( bool absolute = false ) const;
    std::string pathName( bool absolute = false ) const;
    std::string fileName() const { return m_file_name; }
    std::string baseName( bool complete = true ) const;
    std::string extension( bool complete = false ) const;

    size_t byteSize() const;
    bool isFile() const;
    bool exists() const;
    bool parse( const std::string& file_path );

public:
    KVS_DEPRECATED( bool isExisted() const ) { return this->exists(); }
};

} // end of namespace kvs
