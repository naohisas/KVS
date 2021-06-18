/****************************************************************************/
/**
 *  @file   HttpConnector.h
 *  @author Naohisa Sakamoto
 */
/****************************************************************************/
#ifndef KVS__HTTP_CONNECTOR_H_INCLUDE
#define KVS__HTTP_CONNECTOR_H_INCLUDE

#include "TCPSocket.h"
#include "Url.h"
#include "SocketTimer.h"
#include <iostream>
#include <fstream>


namespace kvs
{

/*==========================================================================*/
/**
 *  HTTP connector class.
 */
/*==========================================================================*/
class HttpConnector
{
protected:

    kvs::TCPSocket m_connector; ///< TCP socket connector
    std::string m_hostname; ///< hostname
    int m_port; ///< port number

public:

    HttpConnector();
    HttpConnector( const kvs::Url& url, const kvs::SocketTimer* timeout = 0 );
    virtual ~HttpConnector();

    bool connect( const kvs::Url& url, const kvs::SocketTimer* timeout = 0 );
    void close();
    bool get( const std::string& path, const std::string& filename );
    bool get( const std::string& path, std::ostream& output_stream );
};

} // end of namespace kvs

#endif // KVS__HTTP_CONNECTOR_H_INCLUDE
