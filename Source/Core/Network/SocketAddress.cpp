/****************************************************************************/
/**
 *  @file SocketAddress.cpp
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: SocketAddress.cpp 1318 2012-10-01 04:30:36Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#include "SocketAddress.h"
#include <cstdlib>


namespace kvs
{

/*==========================================================================*/
/**
 *  Constructor.
 */
/*==========================================================================*/
SocketAddress::SocketAddress():
    m_ip( kvs::IPAddress() ),
    m_port( 0 )
{
    this->initialize( &m_address );
}

/*==========================================================================*/
/**
 *  Constructor.
 *  @param address_name [in] address as string
 */
/*==========================================================================*/
SocketAddress::SocketAddress( const char* address_name ):
    m_ip( kvs::IPAddress() ),
    m_port( 0 )
{
    this->split_ip_and_port( address_name );
    this->initialize( &m_address );
}

/*==========================================================================*/
/**
 *  Constructor.
 *  @param ip [in] IP address
 *  @param port [in] port number
 */
/*==========================================================================*/
SocketAddress::SocketAddress( const kvs::IPAddress& ip, const int port ):
    m_ip( ip ),
    m_port( port )
{
    this->initialize_address( ip, port );
}

/*==========================================================================*/
/**
 *  Constructor.
 *  @param other [in] socket address
 */
/*==========================================================================*/
SocketAddress::SocketAddress( const SocketAddress& other ):
    m_ip( other.m_ip ),
    m_port( other.m_port )
{
    this->copy_address( other.m_address );
}

/*==========================================================================*/
/**
 *  Substitution operator '='.
 *  @param other [in] socket address
 */
/*==========================================================================*/
SocketAddress& SocketAddress::operator = ( const SocketAddress& other )
{
    m_ip = other.m_ip;
    m_port = other.m_port;
    this->copy_address( other.m_address );

    return( *this );
}

/*==========================================================================*/
/**
 *  Comparison operator '<'
 *  @param other1 [in] socket address 1
 *  @param other2 [in] socket address 2
 */
/*==========================================================================*/
bool operator < ( const SocketAddress& other1, const SocketAddress& other2 )
{
    if( other1.ip() < other2.ip() ) return( true );
    if( other1.ip() > other2.ip() ) return( false );

    return( other1.port() < other2.port() );
}

/*==========================================================================*/
/**
 *  Comparison operator '>'
 *  @param other1 [in] socket address 1
 *  @param other2 [in] socket address 2
 */
/*==========================================================================*/
bool operator > ( const SocketAddress& other1, const SocketAddress& other2 )
{
    return( other1 < other2 );
}

/*==========================================================================*/
/**
 *  Comparison operator '<='
 *  @param other1 [in] socket address 1
 *  @param other2 [in] socket address 2
 */
/*==========================================================================*/
bool operator <= ( const SocketAddress& other1, const SocketAddress& other2 )
{
    return( !( other1 > other2 ) );
}

/*==========================================================================*/
/**
 *  Comparison operator '>='
 *  @param other1 [in] socket address 1
 *  @param other2 [in] socket address 2
 */
/*==========================================================================*/
bool operator >= ( const SocketAddress& other1, const SocketAddress& other2 )
{
    return( !( other1 < other2 ) );
}

/*==========================================================================*/
/**
 *  Equal operator '=='.
 *  @param other1 [in] socket address 1
 *  @param other2 [in] socket address 2
 */
/*==========================================================================*/
bool operator == ( const SocketAddress& other1, const SocketAddress& other2 )
{
    return( ( other1.ip() == other2.ip() ) && ( other1.port() == other2.port() ) );
}

/*==========================================================================*/
/**
 *  Not-equal perator '!='
 *  @param other1 [in] socket address 1
 *  @param other2 [in] socket address 2
 */
/*==========================================================================*/
bool operator != ( const SocketAddress& other1, const SocketAddress& other2 )
{
    return( !( other1 == other2 ) );
}

/*==========================================================================*/
/**
 *  Operator '<<'.
 *  @param os [in] output stream
 *  @param other [in] socket address
 */
/*==========================================================================*/
std::ostream& operator << ( std::ostream& os, const SocketAddress& other )
{
    os << other.ip() << ":" << other.port();

    return( os );
}

/*==========================================================================*/
/**
 *  Get the IP address.
 *  @retval IP address
 */
/*==========================================================================*/
const kvs::IPAddress& SocketAddress::ip() const
{
    return( m_ip );
}

/*==========================================================================*/
/**
 *  Get the port number.
 *  @return port number
 */
/*==========================================================================*/
int SocketAddress::port() const
{
    return( m_port );
}

/*==========================================================================*/
/**
 *  Get the socket address.
 *  @retval socket address
 */
/*==========================================================================*/
const SocketAddress::address_type& SocketAddress::address() const
{
    return( m_address );
}

/*==========================================================================*/
/**
 *  Set the IP address.
 *  @param ip [in] IP address
 */
/*==========================================================================*/
void SocketAddress::setIp( const kvs::IPAddress& ip )
{
    m_ip = ip;
}

/*==========================================================================*/
/**
 *  Set the port number.
 *  @param port [in] port number
 */
/*==========================================================================*/
void SocketAddress::setPort( const int port )
{
    m_port = port;
}

/*==========================================================================*/
/**
 *  Set the socket address.
 *  @param address [in] socket address
 */
/*==========================================================================*/
void SocketAddress::setAddress( const address_type& address )
{
    this->copy_address( address );

    m_ip   = kvs::IPAddress( address.sin_addr );
    m_port = ntohs( address.sin_port );
}

/*==========================================================================*/
/**
 *  Initialize.
 *  @param address [in] pointer to the socket address
 */
/*==========================================================================*/
void SocketAddress::initialize( address_type* address )
{
    memset( reinterpret_cast<void*>( address ), 0, sizeof( *address ) );
}

/*==========================================================================*/
/**
 *  Initialize socket address.
 *  @param ip [in] IP address
 *  @param port [in] port number
 */
/*==========================================================================*/
void SocketAddress::initialize_address( const kvs::IPAddress& ip, const int port )
{
    this->initialize( &m_address );

    m_address.sin_family = AF_INET;
    m_address.sin_port   = htons( static_cast<unsigned short>( port ) );
    m_address.sin_addr   = ip.address();
}

/*==========================================================================*/
/**
 *  Copy the socket address.
 *  @param address [in] socket address
 */
/*==========================================================================*/
void SocketAddress::copy_address( const SocketAddress::address_type& address )
{
    memcpy( reinterpret_cast<void*>( &m_address ),
            reinterpret_cast<const void*>( &address ),
            sizeof( m_address ) );
}

/*==========================================================================*/
/**
 *  Spilt IP adress and port number from the given string.
 *  @param address_name [in] socket address as string
 */
/*==========================================================================*/
void SocketAddress::split_ip_and_port( const char* address_name )
{
    const std::string delim(":");
    std::string ip_and_port( address_name );

    std::string::size_type p = ip_and_port.find_first_of( delim );
    if( p != std::string::npos )
    {
        m_ip = ip_and_port.substr(0,p).c_str();
        ip_and_port = ip_and_port.substr(p+1);
        m_port = atoi( ip_and_port.c_str() );
    }
}

} // end of namespace kvs
