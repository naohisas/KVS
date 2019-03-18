/****************************************************************************/
/**
 *  @file File.h
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: File.h 1707 2014-01-27 07:37:04Z naohisa.sakamoto@gmail.com $
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
    /*KVS_DEPRECATED*/ static std::string Separator();
    static bool Exists( const std::string& file_path );

private:
    std::string m_file_path; ///< file path (ex. "/tmp/abc/xyz.tar.gz")
    std::string m_path_name; ///< path name (ex. "/tmp/abc")
    std::string m_file_name; ///< file name (ex. "xyz.tar.gz")
    std::string m_base_name; ///< base name (ex. "xyz")
    std::string m_extension; ///< complete extension (ex. "tar.gz")

public:
    File();
    File( const std::string& file_path );
    virtual ~File() {}

    bool operator <( const File& file ) const;
    bool operator ==( const File& file ) const;

    std::string filePath( bool absolute = false ) const;
    std::string pathName( bool absolute = false ) const;
    std::string fileName() const;
    std::string baseName() const;
    std::string extension( bool complete = false ) const;

    size_t byteSize() const;
    bool isFile() const;
    bool exists() const;
    bool parse( const std::string& file_path );

public:
    KVS_DEPRECATED( bool isExisted() const ) { return this->exists(); }
};

} // end of namespace kvs
