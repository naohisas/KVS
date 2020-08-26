/****************************************************************************/
/**
 *  @file   HttpRequestHeader.h
 *  @author Naohisa Sakamoto
 */
/****************************************************************************/
#ifndef KVS__HTTP_REQUEST_HEADER_H_INCLUDE
#define KVS__HTTP_REQUEST_HEADER_H_INCLUDE

#include <string>
#include <vector>


namespace kvs
{

/*==========================================================================*/
/**
 *  HTTP request header class.
 */
/*==========================================================================*/
class HttpRequestHeader
{
public:

    typedef std::vector<std::string> FieldList;

protected:

    std::string m_method; ///< method
    std::string m_path; ///< path
    int m_major_version; ///< HTTP major version
    int m_minor_version; ///< HTTP minor version
    FieldList m_field_list; ///< field (key+value) list

public:

    HttpRequestHeader();
    HttpRequestHeader(
        const std::string& method,
        const std::string& path,
        const int major_version = 1,
        const int minor_version = 0 );

    const std::string& method() const;
    const std::string& path() const;
    int majorVersion() const;
    int minorVersion() const;

    void set(
        const std::string& method,
        const std::string& path,
        const int major_version = 1,
        const int minor_version = 0 );

    void addField( const std::string& key, const std::string& value );
    std::string makeMessage() const;
};

} // end of namespace kvs

#endif // KVS__HTTP_REQUEST_HEADER_H_INCLUDE
