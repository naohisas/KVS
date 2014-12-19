/****************************************************************************/
/**
 *  @file HttpRequestHeader.cpp
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: HttpRequestHeader.cpp 1318 2012-10-01 04:30:36Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#include "HttpRequestHeader.h"
#include <kvs/String>


namespace kvs
{

/*==========================================================================*/
/**
 *  Constructor.
 */
/*==========================================================================*/
HttpRequestHeader::HttpRequestHeader():
    m_major_version( 1 ),
    m_minor_version( 0 ),
    m_field_list( 10 )
{
}

/*==========================================================================*/
/**
 *  Constructor.
 *  @param method [in] method
 *  @param path [in] path
 *  @param major_version [in] major version
 *  @param minor_version [in] minor version
 */
/*==========================================================================*/
HttpRequestHeader::HttpRequestHeader(
    const std::string& method,
    const std::string& path,
    const int          major_version,
    const int          minor_version ):
    m_field_list( 10 )
{
    this->set( method, path, major_version, minor_version );
}

/*==========================================================================*/
/**
 *  Get the method.
 *  @retval method
 */
/*==========================================================================*/
const std::string& HttpRequestHeader::method() const
{
    return( m_method );
}

/*==========================================================================*/
/**
 *  Get the path.
 *  @retval path
 */
/*==========================================================================*/
const std::string& HttpRequestHeader::path() const
{
    return( m_path );
}

/*==========================================================================*/
/**
 *  Get the major version.
 *  @return major version
 */
/*==========================================================================*/
int HttpRequestHeader::majorVersion() const
{
    return( m_major_version );
}

/*==========================================================================*/
/**
 *  Get the minor version.
 *  @return minor version
 */
/*==========================================================================*/
int HttpRequestHeader::minorVersion() const
{
    return( m_minor_version );
}

/*==========================================================================*/
/**
 *  Set header information.
 *  @param method [in] method
 *  @param path [in] path
 *  @param major_version [in] major version
 *  @param minor_version [in] minor version
 */
/*==========================================================================*/
void HttpRequestHeader::set(
    const std::string& method,
    const std::string& path,
    const int          major_version,
    const int          minor_version )
{
    m_method = method;
    m_path = path;
    m_major_version = major_version;
    m_minor_version = minor_version;
}

/*==========================================================================*/
/**
 *  Add field value.
 *  @param key [in] key
 *  @param value [in] value
 */
/*==========================================================================*/
void HttpRequestHeader::addField( const std::string& key, const std::string& value )
{
    m_field_list.push_back( key + ": " + value );
}

/*==========================================================================*/
/**
 *  Make reaquest message.
 *  @return request message
 */
/*==========================================================================*/
std::string HttpRequestHeader::makeMessage() const
{
    std::string request_line;
    std::string major_version = kvs::String::ToString( m_major_version );
    std::string minor_version = kvs::String::ToString( m_minor_version );
    request_line =
        m_method + " /" + m_path +
        " HTTP/" +
        major_version + "." + minor_version +
        "\r\n";

    std::string field_lines;
    FieldList::const_iterator field = m_field_list.begin();
    FieldList::const_iterator last  = m_field_list.end();
    while( field != last ){ field_lines += *field + "\r\n"; ++field; }

    return( request_line + field_lines + "\r\n" );
}

} // end of namespace kvs
