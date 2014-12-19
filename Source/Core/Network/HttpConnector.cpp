/****************************************************************************/
/**
 *  @file HttpConnector.cpp
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: HttpConnector.cpp 1317 2012-09-25 03:13:02Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#include "HttpConnector.h"
#include "Url.h"
#include "SocketTimer.h"
#include "HttpRequestHeader.h"
#include <kvs/IgnoreUnusedVariable>


namespace kvs
{

/*==========================================================================*/
/**
 *  Constructor.
 */
/*==========================================================================*/
HttpConnector::HttpConnector():
    m_port(80)
{
}

/*==========================================================================*/
/**
 *  Constructor.
 *  @param url [in] URL
 *  @param timeout [in] timeout value
 */
/*==========================================================================*/
HttpConnector::HttpConnector( const kvs::Url& url, const kvs::SocketTimer* timeout )
{
    IgnoreUnusedVariable( timeout );
    this->connect( url );
}

/*==========================================================================*/
/**
 *  Destructor.
 */
/*==========================================================================*/
HttpConnector::~HttpConnector()
{
    this->close();
}

/*==========================================================================*/
/**
 *  Connect to a host which is specified by given URL.
 *  @param url [in] URL
 *  @param timeout [in] timeout value
 *  @return true, if the connection success. false, if not.
 */
/*==========================================================================*/
bool HttpConnector::connect( const kvs::Url& url, const kvs::SocketTimer* timeout )
{
    m_hostname = url.hostname();
    m_port = url.port();

    kvs::SocketAddress host( kvs::IPAddress( m_hostname.c_str() ), url.port() );

    m_connector.open();

    return( m_connector.connect( host, timeout ) );
}

/*==========================================================================*/
/**
 *  Close connection.
 */
/*==========================================================================*/
void HttpConnector::close()
{
    m_connector.close();
}

/*==========================================================================*/
/**
 *  Get messages and save it as a file.
 *  @param path [in] path of remote host
 *  @param filename [in] saved filename
 *  @return true, if the messages is got success. false, if not.
 */
/*==========================================================================*/
bool HttpConnector::get( const std::string& path, const std::string& filename )
{
    std::ofstream ofs( filename.c_str() );
    if( !ofs ) return( false );

    if( !this->get( path, ofs ) ) return( false );

    ofs.close();

    return( true );
}

/*==========================================================================*/
/**
 *  Get messages and output a stream.
 *  @param path [in] path of remote host
 *  @param  output_stream [out] output stream
 *  @return true, if the messages is got success. false, if not.
 */
/*==========================================================================*/
bool HttpConnector::get( const std::string& path, std::ostream& output_stream )
{
    kvs::HttpRequestHeader request( "GET", path, 1, 0 );
    request.addField( "HOST", m_hostname );
    request.addField( "Accept", "*/*" );
    request.addField( "Connection", "close" );

    std::string message = request.makeMessage();
    if( !m_connector.send( message.c_str(), message.size() ) ) return( false );

    std::string header;
    for (;;)
    {
        std::string line;
        int size = m_connector.receiveLine( line );

        if( size > 0 )
        {
            if( line == "\r\n" ) break;
            header += line;
        }
        else return( false );
    }

    for (;;)
    {
        std::string line;
        int size = m_connector.receiveLine( line );

        if( size == -1 ) return( false );
        else
        {
            if( size > 0 )
            {
                output_stream.write( line.c_str(), line.size() );
            }
            else
            {
                break;
            }
        }
    }

    return( true );
}

} // end of namespace kvs
