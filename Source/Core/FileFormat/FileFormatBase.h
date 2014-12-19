/****************************************************************************/
/**
 *  @file FileFormatBase.h
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: FileFormatBase.h 1796 2014-08-01 15:39:29Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#ifndef KVS__FILE_FORMAT_BASE_H_INCLUDE
#define KVS__FILE_FORMAT_BASE_H_INCLUDE

#include <string>


namespace kvs
{

/*==========================================================================*/
/**
 *  FileFormatBase.
 */
/*==========================================================================*/
class FileFormatBase
{
private:

    std::string m_filename; ///< Filename.
    bool m_is_success; ///< Whether the reading is success or not.

public:

    FileFormatBase(): m_filename(""), m_is_success( false ) {}
    virtual ~FileFormatBase() {}

    const std::string& filename() const { return m_filename; }
    bool isSuccess() const { return m_is_success; }
    bool isFailure() const { return !m_is_success; }
    virtual bool read( const std::string& filename ) = 0;
    virtual bool write( const std::string& filename ) = 0;

protected:

    void setFilename( const std::string& filename ) { m_filename = filename; }
    void setSuccess( const bool success ) { m_is_success = success; }
};

} // end of namespace kvs

#endif // KVS__FILE_FORMAT_BASE_H_INCLUDE
