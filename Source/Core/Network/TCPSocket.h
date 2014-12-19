/****************************************************************************/
/**
 *  @file TCPSocket.h
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: TCPSocket.h 1317 2012-09-25 03:13:02Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#ifndef KVS__TCP_SOCKET_H_INCLUDE
#define KVS__TCP_SOCKET_H_INCLUDE

#include "Socket.h"
#include "IPAddress.h"
#include "SocketAddress.h"
#include "SocketTimer.h"
#include "MessageBlock.h"


namespace kvs
{

/*==========================================================================*/
/**
 *  TCP socket class.
 */
/*==========================================================================*/
class TCPSocket : public kvs::Socket
{
protected:

    bool m_is_connected; ///< check flag for connection

public:

    TCPSocket();
    TCPSocket( const kvs::IPAddress& ip, const int port, const kvs::SocketTimer* timeout = 0 );
    TCPSocket( const kvs::SocketAddress& socket_address, const kvs::SocketTimer* timeout = 0 );
    TCPSocket( const kvs::Socket::id_type& id, const kvs::SocketAddress& address );
    virtual ~TCPSocket();

    bool isConnected();
    void open();
    bool connect( const kvs::IPAddress& ip, const int port, const kvs::SocketTimer* timeout = 0 );
    bool connect( const kvs::SocketAddress& socket_address, const kvs::SocketTimer* timeout = 0 );
    bool complete( const kvs::SocketTimer* timer = 0 );
    int send( const void* message, const int message_size );
    int send( const kvs::MessageBlock& message );
    int receive( void* message, const int message_size );
    int receive( kvs::MessageBlock* message );
    int receiveOnce( void* message, const int message_size );
    int receiveLine( std::string& line );
};

} // end of namespace kvs

#endif // KVS__TCP_SOCKET_H_INCLUDE
