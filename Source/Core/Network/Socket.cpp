/****************************************************************************/
/**
 *  @file Socket.cpp
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: Socket.cpp 1317 2012-09-25 03:13:02Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#include "Socket.h"
#include "IPAddress.h"
#include "SocketSelector.h"
#include "SocketTimer.h"
#include <kvs/Platform>


namespace kvs
{

#if defined( KVS_PLATFORM_WINDOWS )
static WSADATA wsa_data;
#endif

#if defined( KVS_PLATFORM_WINDOWS )
const Socket::id_type Socket::InvalidID  = INVALID_SOCKET;
const int             Socket::ErrorValue = SOCKET_ERROR;
const int             Socket::Timeout    = 1;
#else
const Socket::id_type Socket::InvalidID  = -1;
const int             Socket::ErrorValue = -1;
const int             Socket::Timeout    = ETIME;
#endif


/*==========================================================================*/
/**
 *  Constructor.
 */
/*==========================================================================*/
Socket::Socket():
    m_id( Socket::InvalidID ),
    m_is_open( false ),
    m_is_bound( false ),
    m_is_blocking( true )
{
}

/*==========================================================================*/
/**
 *  Constructor.
 *  @param other [in] socket
 */
/*==========================================================================*/
Socket::Socket( const Socket& other ):
    m_id( other.m_id ),
    m_address( other.m_address ),
    m_is_open( other.m_is_open ),
    m_is_bound( other.m_is_bound ),
    m_is_blocking( other.m_is_blocking )
{
}

/*==========================================================================*/
/**
 *  Constructor.
 *  @param id [in] socket ID
 *  @param address [in] socket address
 */
/*==========================================================================*/
Socket::Socket( const Socket::id_type& id, const kvs::SocketAddress& address ):
    m_id( id )
{
    this->setAddress( address );
}

/*==========================================================================*/
/**
 *  Destructor.
 */
/*==========================================================================*/
Socket::~Socket()
{
    this->close();
}

/*==========================================================================*/
/**
 *  Substitution operator '='.
 *  @param other [in] socket
 */
/*==========================================================================*/
Socket& Socket::operator = ( const Socket& other )
{
    m_id          = other.m_id;
    m_address     = other.m_address;
    m_is_open     = other.m_is_open;
    m_is_bound    = other.m_is_bound;
    m_is_blocking = other.m_is_blocking;

    return( *this );
}

/*==========================================================================*/
/**
 *  Get the socket ID (handle).
 *  @retval socket ID
 */
/*==========================================================================*/
const Socket::id_type& Socket::id() const
{
    return( m_id );
}

/*==========================================================================*/
/**
 *  Get the socket address.
 *  @retval socket address
 */
/*==========================================================================*/
const SocketAddress& Socket::address() const
{
    return( m_address );
}

/*==========================================================================*/
/**
 *  Set a IP address.
 *  @param ip [in] IP address
 */
/*==========================================================================*/
void Socket::setIp( const kvs::IPAddress& ip )
{
    m_address.setIp( ip );
}

/*==========================================================================*/
/**
 *  Set a port number.
 *  @param port [in] port number
 */
/*==========================================================================*/
void Socket::setPort( const int port )
{
    m_address.setPort( port );
}

/*==========================================================================*/
/**
 *  Set a socket address.
 *  @param address [in] socket address
 */
/*==========================================================================*/
void Socket::setAddress( const kvs::SocketAddress& address )
{
    m_address = address;
}

/*==========================================================================*/
/**
 *  Check whether the socket is valid.
 *  @return true, if the socket is valid.
 */
/*==========================================================================*/
bool Socket::isValid() const
{
    return( m_id != Socket::InvalidID );
}

/*==========================================================================*/
/**
 *  Check whether the socket is open.
 *  @return true, if the socket is open.
 */
/*==========================================================================*/
bool Socket::isOpen() const
{
    return( m_is_open );
}

/*==========================================================================*/
/**
 *  Check whether the socket is bound.
 *  @return true, if the socket is bound.
 */
/*==========================================================================*/
bool Socket::isBound() const
{
    return( m_is_bound );
}

/*==========================================================================*/
/**
 *  Check the blocking mode.
 *  @return true, if the blocking mode.
 */
/*==========================================================================*/
bool Socket::isBlocking() const
{
    return( m_is_blocking );
}

/*==========================================================================*/
/**
 *  Open the socket.
 *  @param socket_type [in] socket type
 */
/*==========================================================================*/
void Socket::open( const int socket_type )
{
#if defined( KVS_PLATFORM_WINDOWS )
    WORD version = MAKEWORD( 2, 0 );
    if( WSAStartup( version, &wsa_data ) != 0 )
    {
        m_is_open = false;
        return;
    }
#endif

    m_id = ::socket( AF_INET, socket_type, 0 );
    if( m_id == Socket::InvalidID )
    {
        m_is_open = false;
        return;
    }

    m_is_open = true;
}

/*==========================================================================*/
/**
 *  Close the socket.
 */
/*==========================================================================*/
void Socket::close()
{
    m_is_open = false;
    m_is_bound = false;

    if( m_id != Socket::InvalidID )
    {
        this->close_socket( m_id );
        m_id = Socket::InvalidID;
    }

#if defined( KVS_PLATFORM_WINDOWS )
    WSACleanup();
#endif
}

/*==========================================================================*/
/**
 *  Bind the socket.
 *  @param socket_address [in] socket address
 *  @param socket_type [in] socket type
 *
 *  @return -1, if the error is occurred.
 */
/*==========================================================================*/
int Socket::bind( const SocketAddress& socket_address )
{
    m_is_bound = false;

    if( !this->isValid() ) return( false );

    this->setAddress( socket_address );

    kvs::SocketAddress::address_type address = socket_address.address();

    const length_type length = sizeof( address );
    const sockaddr*   server = reinterpret_cast<const sockaddr*>( &address );

    int status = ::bind( this->id(), server, length );
    if( status == Socket::ErrorValue ) return( status );

    m_is_bound = true;

    return( status );
}

/*==========================================================================*/
/**
 *  Enable blocking mode.
 */
/*==========================================================================*/
void Socket::enableBlocking()
{
    blocking_socket( this->id() );
}

/*==========================================================================*/
/**
 *  Disable blocking mode (enable non-blocking mode).
 */
/*==========================================================================*/
void Socket::disableBlocking()
{
    non_blocking_socket( this->id() );
}

/*==========================================================================*/
/**
 *  Get a error ID.
 *  @return error ID
 */
/*==========================================================================*/
int Socket::error()
{
#if defined(KVS_PLATFORM_WINDOWS)
    return( WSAGetLastError() );
#else
    return( errno );
#endif
}

/*==========================================================================*/
/**
 *  Get a error string.
 *  @return error string
 */
/*==========================================================================*/
std::string Socket::errorString()
{
    const char* error_string = NULL;

#if defined( KVS_PLATFORM_WINDOWS )
    switch( this->error() )
    {
    case WSAEINTR:           error_string = "WSAEINTR";           break;
    case WSAEBADF:           error_string = "WSAEBADF";           break;
    case WSAEFAULT:          error_string = "WSAEFAULT";          break;
    case WSAEINVAL:          error_string = "WSAEINVAL";          break;
    case WSAEMFILE:          error_string = "WSAEMFILE";          break;
    case WSAEWOULDBLOCK:     error_string = "WSAEWOULDBLOCK";     break;
    case WSAEINPROGRESS:     error_string = "WSAEINPROGRESS";     break;
    case WSAEALREADY:        error_string = "WSAEALREADY";        break;
    case WSAENOTSOCK:        error_string = "WSAENOTSOCK";        break;
    case WSAEDESTADDRREQ:    error_string = "WSAEDESTADDRREQ";    break;
    case WSAEMSGSIZE:        error_string = "WSAEMSGSIZE";        break;
    case WSAEPROTOTYPE:      error_string = "WSAEPROTOTYPE";      break;
    case WSAENOPROTOOPT:     error_string = "WSAENOPROTOOPT";     break;
    case WSAEPROTONOSUPPORT: error_string = "WSAEPROTONOSUPPORT"; break;
    case WSAESOCKTNOSUPPORT: error_string = "WSAESOCKTNOSUPPORT"; break;
    case WSAEOPNOTSUPP:      error_string = "WSAEOPNOTSUPP";      break;
    case WSAEPFNOSUPPORT:    error_string = "WSAEPFNOSUPPORT";    break;
    case WSAEAFNOSUPPORT:    error_string = "WSAEAFNOSUPPORT";    break;
    case WSAEADDRINUSE:      error_string = "WSAEADDRINUSE";      break;
    case WSAEADDRNOTAVAIL:   error_string = "WSAEADDRNOTAVAIL";   break;
    case WSAENETDOWN:        error_string = "WSAENETDOWN";        break;
    case WSAENETUNREACH:     error_string = "WSAENETUNREACH";     break;
    case WSAENETRESET:       error_string = "WSAENETRESET";       break;
    case WSAECONNABORTED:    error_string = "WSAECONNABORTED";    break;
    case WSAECONNRESET:      error_string = "WSAECONNRESET";      break;
    case WSAENOBUFS:         error_string = "WSAENOBUFS";         break;
    case WSAEISCONN:         error_string = "WSAEISCONN";         break;
    case WSAENOTCONN:        error_string = "WSAENOTCONN";        break;
    case WSAESHUTDOWN:       error_string = "WSAESHUTDOWN";       break;
    case WSAETOOMANYREFS:    error_string = "WSAETOOMANYREFS";    break;
    case WSAETIMEDOUT:       error_string = "WSAETIMEDOUT";       break;
    case WSAECONNREFUSED:    error_string = "WSAECONNREFUSED";    break;
    case WSAELOOP:           error_string = "WSAELOOP";           break;
    case WSAENAMETOOLONG:    error_string = "WSAENAMETOOLONG";    break;
    case WSAEHOSTDOWN:       error_string = "WSAEHOSTDOWN";       break;
    case WSAEHOSTUNREACH:    error_string = "WSAEHOSTUNREACH";    break;
    case WSASYSNOTREADY:     error_string = "WSASYSNOTREADY";     break;
    case WSAVERNOTSUPPORTED: error_string = "WSAVERNOTSUPPORTED"; break;
    case WSANOTINITIALISED:  error_string = "WSANOTINITIALISED";  break;
    case WSAHOST_NOT_FOUND:  error_string = "WSAHOST_NOT_FOUND";  break;
    case WSATRY_AGAIN:       error_string = "WSATRY_AGAIN";       break;
    case WSANO_RECOVERY:     error_string = "WSANO_RECOVERY";     break;
    case WSANO_DATA:         error_string = "WSANO_DATA";         break;
    }
#else
    error_string = strerror( this->error() );
#endif

    return( error_string ? error_string : "Unknown error" );
}

/*==========================================================================*/
/**
 *  Set socket option.
 *  @param id [in] socket ID
 *  @param level [in] protocl level
 *  @param name [in] option name
 *  @param value [in] option value
 *  @param length [in] option length
 *  @return 0, if the proccess is done successfully
 */
/*==========================================================================*/
int Socket::set_option( id_type id, int level, int name, void* value, int length )
{
    return( ::setsockopt( id, level, name, (option_type*)( value ), length ) );
}

/*==========================================================================*/
/**
 *  Receive buffer at once.
 *  @param id [in] socket ID
 *  @param  buffer [out] pointer to buffer
 *  @param length [in] buffer length [byte]
 *  @return received buffer size [byte]
 */
/*==========================================================================*/
int Socket::receive_once( id_type id, char* buffer, int length )
{
    return( ::recv( id, buffer, length, 0 ) );
}

/*==========================================================================*/
/**
 *  Receive buffer exactly.
 *  @param id [in] socket ID
 *  @param  buffer [out] pointer to buffer
 *  @param length [in] buffer length [byte]
 *  @return received buffer size [byte]
 */
/*==========================================================================*/
int Socket::receive_exact( id_type id, char* buffer, int length )
{
    int received_size = 0;

    for (;;)
    {
        int actual_size = ::recv( id, buffer, length - received_size, 0 );
        if( actual_size <= 0 ) break;

        received_size += actual_size;
        buffer += actual_size;

        if( received_size >= length ) break;
    }

    return( received_size );
}

/*==========================================================================*/
/**
 *  Receive a line buffer.
 *  @param id [in] socket ID
 *  @param  line [out] line buffer
 *  @return received buffer size [byte]
 */
/*==========================================================================*/
int Socket::receive_line( id_type id, std::string& line )
{
    for (;;)
    {
        char c;
        int size = ::recv( id, &c, 1, 0 );

        if( size > 0 )
        {
            line += c;
            if( c == '\n' ) break;
        }
        else return( size );
    }

    return( line.size() );
}

/*==========================================================================*/
/**
 *  Peek at an incoming buffer.
 *  @param id [in] socket ID
 *  @param  buffer [out] pointer to buffer
 *  @param length [in] buffer length [byte]
 *  @return received buffer size [byte]
 */
/*==========================================================================*/
int Socket::receive_peek( id_type id, char* buffer, int length )
{
    int received_size = 0;

    do
    {
        received_size = ::recv( id, buffer, length, MSG_PEEK );
        if( received_size < 0 )
        {
#if defined( KVS_PLATFORM_WINDOWS )
            if( this->error() == WSAEMSGSIZE )
            {
                received_size = length;
            }
#endif
            break; // Error.
        }
        if( received_size == 0 ) return( 0 );
    }
    while( received_size != length );

    return( received_size );
}

/*==========================================================================*/
/**
 *  Connect to a host machine.
 *  @param socket_address [in] host machine's socket address 
 *  @param timeout [in] time-out
 *  @return -1, if the connection is failure.
 */
/*==========================================================================*/
int Socket::connect_to_host(
    const kvs::SocketAddress& socket_address,
    const kvs::SocketTimer*   timeout )
{
    this->setAddress( socket_address );
    kvs::SocketAddress::address_type address = this->address().address();

    const length_type length = sizeof( sockaddr_in );
    const sockaddr*   server = reinterpret_cast<const sockaddr*>( &address );

    // Blocking connection.
    if( !timeout )
    {
        return( ::connect( this->id(), server, length ) );
    }

    // Non-blocking connection.
    if( timeout->isZero() )
    {
        this->disableBlocking();

        return( ::connect( this->id(), server, length ) );
    }

    // Timed wait connection.
    int status;
    this->disableBlocking();
    {
        status = ::connect( this->id(), server, length );
        if( status == Socket::ErrorValue )
        {
            kvs::SocketSelector selector;
            selector.setWritable( this->id() );
            status = selector.select( *timeout );

            if( status < 0 )
            {
                // Error.
                status = Socket::ErrorValue;
            }
            else if( status == 0 )
            {
                // Timeout.
                errno  = Socket::Timeout;
                status = Socket::ErrorValue;
            }
            else
            {
                // Writable.
                kvs::SocketAddress::address_type name;
                Socket::length_type length = sizeof( name );
                if( ::getpeername( this->id(), (struct sockaddr*)&name, &length ) < 0 )
                {
                    // Connection failure.
                    status = Socket::ErrorValue;
                }
            }
        }
    }
    this->enableBlocking();

    return( status );
}

/*==========================================================================*/
/**
 *  Connect complete.
 *  @param timeout [in] time-out
 *  @return -1, if the connect is failure. 0, if the connection is timeout.
 */
/*==========================================================================*/
int Socket::connect_complete( const SocketTimer* timeout )
{
    SocketSelector selector;
    selector.setReadable( this->id() );
    selector.setWritable( this->id() );

    return( selector.select( *timeout ) );
}

/*==========================================================================*/
/**
 *  Set blocking mode.
 *  @param id [in] socket ID
 */
/*==========================================================================*/
void Socket::blocking_socket( id_type id )
{
    if( m_is_blocking ) return;

    int status;

#if defined( KVS_PLATFORM_WINDOWS )
    u_long flag = 0;
    status = ::ioctlsocket( id, FIONBIO, &flag );
#else
    int flag = 0;
    status = ::ioctl( id, FIONBIO, &flag );
#endif

    if( status != Socket::ErrorValue ) m_is_blocking = true;
}

/*==========================================================================*/
/**
 *  Set non-blocking mode.
 *  @param id [in] socket ID
 */
/*==========================================================================*/
void Socket::non_blocking_socket( id_type id )
{
    if( !m_is_blocking ) return;

    int status;

#if defined( KVS_PLATFORM_WINDOWS )
    u_long flag = 1;
    status = ::ioctlsocket( id, FIONBIO, &flag );
#else
    int flag = 1;
    status = ::ioctl( id, FIONBIO, &flag );
#endif

    if( status != Socket::ErrorValue ) m_is_blocking = false;
}

/*==========================================================================*/
/**
 *  Close the socket.
 *  @param id [in] socket ID
 */
/*==========================================================================*/
void Socket::close_socket( id_type id )
{
#if defined( KVS_PLATFORM_WINDOWS )
    ::closesocket( id );
#else
    ::close( id );
#endif
}

} // end of namespace kvs
