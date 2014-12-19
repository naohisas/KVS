/****************************************************************************/
/**
 *  @file Directory.h
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: Directory.h 1707 2014-01-27 07:37:04Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#ifndef KVS__DIRECTORY_H_INCLUDE
#define KVS__DIRECTORY_H_INCLUDE

#include <string>
#include <iostream>
#include <algorithm>
#include <functional>
#include <kvs/FileList>
#include <kvs/Deprecated>


namespace kvs
{

/*==========================================================================*/
/**
 *  Directory class
 */
/*==========================================================================*/
class Directory
{
private:

    std::string m_directory_path; ///< directory path (ex. "/tmp/abc/")
    std::string m_directory_name; ///< directory name (ex. "abc")
    kvs::FileList m_file_list; ///< filename list

public:

    static std::string Separator();
    static bool Make( const std::string& directory_path );
    static bool Remove( const std::string& directory_path );
    static bool Change( const std::string& directory_path );
    static Directory Current();

public:

    Directory();
    explicit Directory( const std::string& directory_path );
    virtual ~Directory();

public:

    std::string directoryPath( bool absolute = false ) const;
    std::string directoryName() const;
    kvs::FileList& fileList();
    const kvs::FileList& fileList() const;
    bool isDirectory() const;
    bool exists() const;
    bool parse( const std::string& direcotry_path );
    void sort();
    kvs::FileList::iterator find( const kvs::File& file );
    kvs::FileList::const_iterator find( const kvs::File& file ) const;

public:
    KVS_DEPRECATED( bool isExisted() const ) { return this->exists(); }
};

} // end of namespace kvs

#endif // KVS__DIRECTORY_H_INCLUDE
