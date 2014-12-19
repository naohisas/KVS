/****************************************************************************/
/**
 *  @file HttpConnector.h
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: HttpConnector.h 1317 2012-09-25 03:13:02Z naohisa.sakamoto@gmail.com $
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
