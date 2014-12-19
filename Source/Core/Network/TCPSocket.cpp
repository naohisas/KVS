/****************************************************************************/
/**
 *  @file TCPSocket.cpp
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: TCPSocket.cpp 1536 2013-04-15 02:21:56Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#include "TCPSocket.h"
#include "IPAddress.h"
#include "SocketAddress.h"
#include "SocketTimer.h"
#include "MessageBlock.h"


namespace kvs
{

/*==========================================================================*/
/**
 *  Constructor.
 */
/*==========================================================================*/
TCPSocket::TCPSocket():
    m_is_connected( false )
{
}

/*==========================================================================*/
/**
 *  Constructor.
 *  @param ip [in] IP address
 *  @param port [in] port number
 *  @param timeout [in] timeout value
 */
/*==========================================================================*/
TCPSocket::TCPSocket( const kvs::IPAddress& ip, const int port , const kvs::SocketTimer* timeout ):
    m_is_connected( false )
{
    this->open();
    this->connect( ip, port, timeout );
}

/*==========================================================================*/
/**
 *  Constructor.
 *  @param socket_address [in] address
 *  @param timeout [in] timeout value
 */
/*==========================================================================*/
TCPSocket::TCPSocket( const kvs::SocketAddress& socket_address, const kvs::SocketTimer* timeout ):
    m_is_connected( false )
{
    this->open();
    this->connect( socket_address, timeout );
}

/*==========================================================================*/
/**
 *  Constructor.
 *  @param id [in] socket ID
 *  @param address [in] socket adrees
 */
/*==========================================================================*/
TCPSocket::TCPSocket( const kvs::Socket::id_type& id, const kvs::SocketAddress& address ):
    kvs::Socket( id, address ),
    m_is_connected( false )
{
}

/*==========================================================================*/
/**
 *  Destructor.
 */
/*==========================================================================*/
TCPSocket::~TCPSocket()
{
}

/*==========================================================================*/
/**
 *  Check whether the socket is connect successfully.
 *  @return false, if the connection is failure.
 */
/*==========================================================================*/
bool TCPSocket::isConnected()
{
    return( m_is_connected );
}

/*==========================================================================*/
/**
 *  Opent a TUP socket.
 */
/*==========================================================================*/
void TCPSocket::open()
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
 *  Connect to a host.
 *  @param ip [in] IP address
 *  @param port [in] port number
 *  @param timeout [in] timeout value
 *  @return true, if connection success.
 */
/*==========================================================================*/
bool TCPSocket::connect(
    const kvs::IPAddress&   ip,
    const int               port,
    const kvs::SocketTimer* timeout )
{
    return( this->connect( kvs::SocketAddress( ip, port ), timeout ) );
}

/*==========================================================================*/
/**
 *  Connecto to a host.
 *  @param socket_address [in] remote peer address
 *  @param timeout [in] timeout value
 *  @return true, if connection success.
 */
/*==========================================================================*/
bool TCPSocket::connect(
    const kvs::SocketAddress& socket_address,
    const kvs::SocketTimer*   timeout )
{
    if( !kvs::Socket::isOpen() ) return( false );

//    const kvs::IPAddress none( static_cast<kvs::IPAddress::integer_type>(kvs::IPAddress::None) );
    const kvs::IPAddress none( kvs::IPAddress::None );
    if( socket_address.ip() == none )
    {
        return( false );
    }

    m_is_connected = false;

    if( kvs::Socket::connect_to_host( socket_address, timeout ) != -1 )
    {
        m_is_connected = true;
    }

    return( m_is_connected );
}

/*==========================================================================*/
/**
 *  Connect completely.
 *  @param timeout [in] timeout value
 *  @return true, if it's success.
 */
/*==========================================================================*/
bool TCPSocket::complete( const kvs::SocketTimer* timeout )
{
    if( m_is_connected ) return( true );

    if( !kvs::Socket::isBlocking() )
    {
        if( kvs::Socket::connect_complete( timeout ) > 0 )
        {
            kvs::Socket::enableBlocking();
            return( true );
        }
    }

    return( false );
}

/*==========================================================================*/
/**
 *  Send messages.
 *  @param message [in] pointer to message
 *  @param message_size [in] size of message [byte]
 *  @return size of sent messages
 */
/*==========================================================================*/
int TCPSocket::send( const void* message, const int message_size )
{
    return( ::send( kvs::Socket::id(), (const char*)message, message_size, 0 ) );
}

/*==========================================================================*/
/**
 *  Send messages.
 *  @param message [in] message
 *  @return size of sent message
 */
/*==========================================================================*/
int TCPSocket::send( const kvs::MessageBlock& message )
{
    return( this->send( message.blockData(), message.blockSize() ) );
}

/*==========================================================================*/
/**
 *  Receive messages.
 *  @param  message      [out] pointer to received message
 *  @param message_size [in] size of message [byte]
 *  @return size of received message
 */
/*==========================================================================*/
int TCPSocket::receive( void* message, const int message_size )
{
    return( kvs::Socket::receive_exact( kvs::Socket::id(), (char*)message, message_size ) );
}

/*==========================================================================*/
/**
 *  Receive message exactly.
 *  @param  message [out] pointer to received message
 *  @return size of received message
 */
/*==========================================================================*/
int TCPSocket::receive( MessageBlock* message )
{
    size_t message_size = 0;
    int status = kvs::Socket::receive_peek( kvs::Socket::id(),
                                            (char*)&message_size,
                                            sizeof( size_t ) );
    if( status == -1 ) return( status );

    message->allocate( ntohl( message_size ) );

    return( this->receive( message->blockData(), message->blockSize() ) );
}

/*==========================================================================*/
/**
 *  Receive message at once.
 *  @param  message      [out] pointer to received message
 *  @param message_size [in] size of message [byte]
 *  @return size of received message
 */
/*==========================================================================*/
int TCPSocket::receiveOnce( void* message, const int message_size )
{
    return( kvs::Socket::receive_once( kvs::Socket::id(), (char*)message, message_size ) );
}

/*==========================================================================*/
/**
 *  Receive message.
 *  @param  line [out] line
 *  @return size of received messsage
 */
/*==========================================================================*/
int TCPSocket::receiveLine( std::string& line )
{
    return( kvs::Socket::receive_line( kvs::Socket::id(), line ) );
}

} // end of namespace kvs
