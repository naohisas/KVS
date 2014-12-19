/****************************************************************************/
/**
 *  @file Socket.h
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: Socket.h 1317 2012-09-25 03:13:02Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#ifndef KVS__SOCKET_H_INCLUDE
#define KVS__SOCKET_H_INCLUDE

#include "SocketStandard.h"
#include "SocketAddress.h"
#include "SocketTimer.h"
#include "IPAddress.h"
#include <kvs/Platform>
#include <string>


namespace kvs
{

/*==========================================================================*/
/**
 *  @class  Socket class.
 */
/*==========================================================================*/
class Socket
{
public:

#if defined( KVS_PLATFORM_WINDOWS )
    typedef SOCKET id_type;
    typedef int length_type;
    typedef char FAR option_type;
#else
    typedef int id_type;
    typedef socklen_t length_type;
    typedef void option_type;
#endif

    enum
    {
        TCPType = SOCK_STREAM, ///< TCP socket
        UDPType = SOCK_DGRAM   ///< UDP socket
    };

    static const id_type InvalidID;
    static const int ErrorValue;
    static const int Timeout;

protected:

    id_type m_id;
    SocketAddress m_address;
    bool m_is_open;
    bool m_is_bound;
    bool m_is_blocking;

public:

    Socket();
    Socket( const Socket& other );
    Socket( const id_type& id, const SocketAddress& address );
    virtual ~Socket();

    Socket& operator = ( const Socket& other );

    const id_type& id() const;
    const kvs::SocketAddress& address() const;

    void setIp( const kvs::IPAddress& ip );
    void setPort( const int port );
    void setAddress( const kvs::SocketAddress& address );

    bool isValid() const;
    bool isOpen() const;
    bool isBound() const;
    bool isBlocking() const;
    void open( const int socket_type );
    void close();
    int bind( const kvs::SocketAddress& socket_address );
    void enableBlocking();
    void disableBlocking();
    int error();
    std::string errorString();

protected:

    int set_option( id_type id, int level, int name, void* value, int length );
    int receive_once( id_type id, char* buffer, int length );
    int receive_exact( id_type id, char* buffer, int length );
    int receive_peek( id_type id, char* buffer, int length );
    int receive_line( id_type id, std::string& line );
    int connect_to_host( const kvs::SocketAddress& socket_address, const kvs::SocketTimer* timeout = 0 );
    int connect_complete( const kvs::SocketTimer* timeout );
    void blocking_socket( id_type id );
    void non_blocking_socket( id_type id );
    void close_socket( id_type id );
};

} // end of namespace kvs

#endif // KVS__SOCKET_H_INCLUDE
