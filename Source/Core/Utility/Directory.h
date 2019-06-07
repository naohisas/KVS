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
#pragma once
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
public:
    static std::string Separator();
    static std::string Absolute( const std::string& directory_path );
    static bool Exists( const std::string& directory_path );
    static bool Make( const std::string& directory_path );
    static bool Remove( const std::string& directory_path );
    static bool Change( const std::string& directory_path );
    static std::string RootPath();
    static std::string HomePath();
    static std::string TempPath();
    static std::string CurrentPath();
    static Directory Root() { return Directory( RootPath() ); }
    static Directory Home() { return Directory( HomePath() ); }
    static Directory Temp() { return Directory( TempPath() ); }
    static Directory Current() { return Directory( CurrentPath() ); }

private:
    std::string m_path; ///< directory path (ex. "/tmp/abc/")
    std::string m_name; ///< directory name (ex. "abc")

public:
    Directory();
    explicit Directory( const std::string& directory_path );
    virtual ~Directory();

    std::string path( bool absolute = false ) const;
    std::string name() const;
    kvs::FileList fileList( const bool sort = true ) const;
    bool isDirectory() const;
    bool exists() const;
    bool parse( const std::string& direcotry_path );

public:
    KVS_DEPRECATED( bool isExisted() const ) { return this->exists(); }
    KVS_DEPRECATED( std::string directoryPath( bool abs = false ) const ) { return this->path( abs ); }
    KVS_DEPRECATED( std::string directoryName() const ) { return this->name(); }
};

} // end of namespace kvs
