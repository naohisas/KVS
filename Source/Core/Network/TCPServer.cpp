/****************************************************************************/
/**
 *  @file TCPServer.cpp
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: TCPServer.cpp 1536 2013-04-15 02:21:56Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#include "TCPServer.h"
#include <kvs/DebugNew>
#include "TCPSocket.h"
#include "SocketAddress.h"
#include "SocketTimer.h"
#include "SocketSelector.h"
#include "IPAddress.h"
#include "MessageBlock.h"


namespace kvs
{

/*==========================================================================*/
/**
 *  Constructor.
 */
/*==========================================================================*/
TCPServer::TCPServer():
    m_max_nconnections( 5 )
{
}

/*==========================================================================*/
/**
 *  Constructor.
 *  @param port [in] port number
 *  @param max_nconnections [in] max. number of connect client
 */
/*==========================================================================*/
TCPServer::TCPServer( const int port, const int max_nconnections ):
    m_max_nconnections( max_nconnections )
{
    this->open();
    this->bind( port );
}

/*==========================================================================*/
/**
 *  Destructor.
 */
/*==========================================================================*/
TCPServer::~TCPServer()
{
}

/*==========================================================================*/
/**
 *  Open.
 */
/*==========================================================================*/
void TCPServer::open()
{
    kvs::Socket::open( kvs::Socket::TCPType );
    if( kvs::Socket::isOpen() )
    {
        int nodelay = 0; // 0 = no-buffering, 1 = buffering
        kvs::Socket::set_option( m_id, IPPROTO_TCP, TCP_NODELAY,
                                 &nodelay, sizeof(nodelay) );

        int reuse = 1;
        kvs::Socket::set_option( m_id, SOL_SOCKET, SO_REUSEADDR,
                                 &reuse, sizeof(reuse) );

        struct linger linger_opt;
        linger_opt.l_onoff  = 1; // 0 = off, 1 = on
        linger_opt.l_linger = 1000;
        kvs::Socket::set_option( m_id, SOL_SOCKET, SO_LINGER,
                                 &linger_opt, sizeof(linger_opt) );
    }
}

/*==========================================================================*/
/**
 *  Bind the port.
 *  @param port [in] bound port number
 *  @return -1, if the error is occurred.
 */
/*==========================================================================*/
int TCPServer::bind( const int port )
{
    if( !Socket::isOpen() ) return( false );

    return( Socket::bind( SocketAddress( IPAddress::Any, port ) ) );
}

/*==========================================================================*/
/**
 *  Waiting for the incoming client.
 *  @return true, if the success.
 */
/*==========================================================================*/
bool TCPServer::listen()
{
    if( ::listen( kvs::Socket::id(), m_max_nconnections ) == -1 )
    {
        kvs::Socket::close();
        return( false );
    }

    return( true );
}

/*==========================================================================*/
/**
 *  Accept the connection of the client.
 *  @param  client_address [out] pointer to the client address
 *  @return accepted socket ID
 */
/*==========================================================================*/
kvs::Socket::id_type TCPServer::accept( kvs::SocketAddress* client_address )
{
    if( client_address )
    {
        kvs::SocketAddress::address_type address;
        kvs::SocketAddress::initialize( &address );
        kvs::Socket::length_type length = sizeof( address );

        kvs::Socket::id_type id = ::accept( Socket::id(),
                                            reinterpret_cast<sockaddr*>( &address ),
                                            &length );

        client_address->setAddress( address );

        return( id );
    }

    return( ::accept( kvs::Socket::id(), 0, 0 ) );
}

/*===========================================================================*/
/**
 *  Sets a maximum number of connections.
 *  @param  max_nconnections [in] maximum number of connections
 */
/*===========================================================================*/
void TCPServer::setMaxConnections( const int max_nconnections )
{
    m_max_nconnections = max_nconnections;
}

/*==========================================================================*/
/**
 *  Check for new connections.
 *  @param blocking_time [in] blocking time
 *  @return pointer to the connected socket
 */
/*==========================================================================*/
kvs::TCPSocket* TCPServer::checkForNewConnection( const kvs::SocketTimer* blocking_time )
{
    if( blocking_time )
    {
        kvs::SocketSelector selector;
        selector.setReadable( this->id() );

        if( selector.select( *blocking_time ) <= 0 ) return( NULL );
        if( !selector.isReadable( this->id() ) ) return( NULL );
    }

    kvs::SocketAddress address;
    kvs::Socket::id_type id = this->accept( &address );

    kvs::TCPSocket* connector = new kvs::TCPSocket( id, address );
    if( connector )
    {
        int nodelay = 0; // 0 = no-buffering, 1 = buffering
        kvs::Socket::set_option( m_id, IPPROTO_TCP, TCP_NODELAY,
                                 &nodelay, sizeof(nodelay) );

        int reuse = 1;
        kvs::Socket::set_option( m_id, SOL_SOCKET, SO_REUSEADDR,
                                 &reuse, sizeof(reuse) );

        struct linger linger_opt;
        linger_opt.l_onoff  = 1; // 0 = off, 1 = on
        linger_opt.l_linger = 1000;
        kvs::Socket::set_option( m_id, SOL_SOCKET, SO_LINGER,
                                 &linger_opt, sizeof(linger_opt) );
    }

    return( connector );
}

/*==========================================================================*/
/**
 *  Send message.
 *  @param buffer [in] pointer to the send message buffer
 *  @param byte_size [in] message buffer size [byte]
 *  @param client_address [in] accept client address
 *  @return send message size
 */
/*==========================================================================*/
int TCPServer::send( const void* buffer, int byte_size, kvs::SocketAddress* client_address )
{
    int size = -1;

    kvs::Socket::id_type id = this->accept( client_address );
    if( id != kvs::Socket::InvalidID )
    {
        size = ::send( id, (const char*)buffer, byte_size, 0 );
        kvs::Socket::close_socket( id );
    }

    return( size );
}

/*==========================================================================*/
/**
 *  Send message.
 *  @param message [in] pointer to the send message
 *  @param client_address [in] accept client address
 *  @return send message size
 */
/*==========================================================================*/
int TCPServer::send( const kvs::MessageBlock& message, kvs::SocketAddress* client_address )
{
    int size = -1;

    kvs::Socket::id_type id = this->accept( client_address );
    if( id != kvs::Socket::InvalidID )
    {
        size = ::send( id,
                       (const char*)message.blockData(),
                       message.blockSize(), 0 );

        kvs::Socket::close_socket( id );
    }

    return( size );
}

/*==========================================================================*/
/**
 *  Receive message.
 *  @param buffer [in] pointer to the received message buffer
 *  @param byte_size [in] message buffer size
 *  @param client_address [in] accept client address
 *  @return received message size
 */
/*==========================================================================*/
int TCPServer::receive( void* buffer, int byte_size, kvs::SocketAddress* client_address )
{
    int size = -1;

    kvs::Socket::id_type id = this->accept( client_address );
    if( id != kvs::Socket::InvalidID )
    {
        size = kvs::Socket::receive_exact( id, (char*)buffer, byte_size );
        kvs::Socket::close_socket( id );
    }

    return( size );
}

/*==========================================================================*/
/**
 *  Receive message.
 *  @param message [in] pointer to the received message
 *  @param client_address [in] accept client address
 *  @return received message size
 */
/*==========================================================================*/
int TCPServer::receive( kvs::MessageBlock* message, kvs::SocketAddress* client_address )
{
    int size = -1;

    kvs::Socket::id_type id = this->accept( client_address );
    if( id != kvs::Socket::InvalidID )
    {
        size_t data_size = 0;
        int status = kvs::Socket::receive_peek( id, (char*)&data_size, sizeof( size_t ) );
        if( status == -1 ) return( status );

        message->allocate( ntohl( data_size ) );

        size = kvs::Socket::receive_exact( id,
                                           (char*)message->blockData(),
                                           message->blockSize() );

        kvs::Socket::close_socket( id );
    }

    return( size );
}

} // end of namespace kvs
