/****************************************************************************/
/**
 *  @file   FileFormatBase.h
 *  @author Naohisa Sakamoto
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
