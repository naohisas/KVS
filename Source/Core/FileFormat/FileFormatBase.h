/****************************************************************************/
/**
 *  @file   FileFormatBase.h
 *  @author Naohisa Sakamoto
 */
/****************************************************************************/
#pragma once
#include <string>
#include <cstdlib>


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
    std::string m_filename = ""; ///< Filename.
    bool m_is_success = false; ///< Whether the reading is success or not.

public:
    static size_t Read( void* buf, size_t size, size_t n, FILE* fp )
    {
        const auto ret = fread( buf, size, n, fp );
        return ret;
    }

    static size_t Seek( FILE* fp, long offset, int origin )
    {
        const auto ret = fseek( fp, offset, origin );
        return ret;
    }

public:
    FileFormatBase() = default;
    virtual ~FileFormatBase() = default;

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
