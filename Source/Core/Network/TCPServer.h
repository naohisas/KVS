/****************************************************************************/
/**
 *  @file   TCPServer.h
 *  @author Naohisa Sakamoto
 */
/****************************************************************************/
#ifndef KVS__TCP_SERVER_H_INCLUDE
#define KVS__TCP_SERVER_H_INCLUDE

#include "Socket.h"
#include "SocketTimer.h"
#include "SocketAddress.h"
#include "MessageBlock.h"
#include "TCPSocket.h"


namespace kvs
{

/*==========================================================================*/
/**
 *  TCP server class.
 */
/*==========================================================================*/
class TCPServer : public kvs::Socket
{
protected:

    int m_max_nconnections; ///< max. number of connection client

public:

    TCPServer();
    TCPServer( const int port, const int max_nconnections = 5 );
    virtual ~TCPServer();

    void open();
    int bind( const int port );
    bool listen();
    kvs::Socket::id_type accept( kvs::SocketAddress* socket_address = 0 );

    void setMaxConnections( const int max_nconnections );
    kvs::TCPSocket* checkForNewConnection( const kvs::SocketTimer* blocking_time = 0 );

    int send( const void* buffer, int byte_size, kvs::SocketAddress* client_address = 0 );
    int send( const kvs::MessageBlock& message, kvs::SocketAddress* client_address = 0 );
    int receive( void* buffer, int byte_size, kvs::SocketAddress* client_address = 0 );
    int receive( kvs::MessageBlock* message, kvs::SocketAddress* client_address = 0 );
};

} // end of namespace kvs

#endif // KVS__TCP_SERVER_H_INCLUDE
