/****************************************************************************/
/**
 *  @file IPAddress.cpp
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: IPAddress.cpp 1317 2012-09-25 03:13:02Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#include "IPAddress.h"


namespace kvs
{

const IPAddress::integer_type IPAddress::Any = INADDR_ANY;
const IPAddress::integer_type IPAddress::Loopback = INADDR_LOOPBACK;
const IPAddress::integer_type IPAddress::Broadcast = INADDR_BROADCAST;
const IPAddress::integer_type IPAddress::None = INADDR_NONE;

/*==========================================================================*/
/**
 *  Constructor.
 *  @param address [in] IP address as integer type
 */
/*==========================================================================*/
IPAddress::IPAddress( IPAddress::integer_type address )
{
    this->set_ip_address( address );
}

/*==========================================================================*/
/**
 *  Constructor.
 *  @param hostname [in] hostname
 */
/*==========================================================================*/
IPAddress::IPAddress( const char* hostname )
{
    this->set_ip_address( hostname );
}

/*==========================================================================*/
/**
 *  Constructor.
 *  @param address [in] IP address
 */
/*==========================================================================*/
IPAddress::IPAddress( const IPAddress::address_type& address ):
    m_address( address )
{
}

/*==========================================================================*/
/**
 *  Constructor.
 *  @param other [in] IP address
 */
/*==========================================================================*/
IPAddress::IPAddress( const IPAddress& other )
{
    this->copy_address( other.m_address );
}

/*==========================================================================*/
/**
 *  '=' operator.
 *  @param hostname [in] hostname
 */
/*==========================================================================*/
IPAddress& IPAddress::operator = ( const char* hostname )
{
    this->set_ip_address( hostname );

    return( *this );
}

/*==========================================================================*/
/**
 *  '=' operator.
 *  @param address [in] IP address as integer type
 */
/*==========================================================================*/
IPAddress& IPAddress::operator = ( IPAddress::integer_type address )
{
    this->set_ip_address( address );

    return( *this );
}

/*==========================================================================*/
/**
 *  Substitution operator.
 *  @param other [in] IP address
 */
/*==========================================================================*/
IPAddress& IPAddress::operator = ( const IPAddress& other )
{
    this->copy_address( other.m_address );

    return( *this );
}

/*==========================================================================*/
/**
 *  '<' operator.
 *  @param other1 [in] IP address
 *  @param other2 [in] IP address
 *  @return true, if the other1 is less than the other2.
 */
/*==========================================================================*/
bool operator < ( const IPAddress& other1, const IPAddress& other2 )
{
    return( other1.addressAsInteger() < other2.addressAsInteger() );
}

/*==========================================================================*/
/**
 *  '>' operator.
 *  @param other1 [in] IP address
 *  @param other2 [in] IP address
 *  @return true, if the other1 is greater than the other2.
 */
/*==========================================================================*/
bool operator > ( const IPAddress& other1, const IPAddress& other2 )
{
    return( other1.addressAsInteger() > other2.addressAsInteger() );
}

/*==========================================================================*/
/**
 *  '<=' operator.
 *  @param other1 [in] IP address
 *  @param other2 [in] IP address
 *  @return true, if the other1 is less equal than the other2.
 */
/*==========================================================================*/
bool operator <= ( const IPAddress& other1, const IPAddress& other2 )
{
    return( !( other1.addressAsInteger() > other2.addressAsInteger() ) );
}

/*==========================================================================*/
/**
 *  '>=' operator.
 *  @param other1 [in] IP address
 *  @param other2 [in] IP address
 *  @return true, if the other1 is greater equal than the other2.
 */
/*==========================================================================*/
bool operator >= ( const IPAddress& other1, const IPAddress& other2 )
{
    return( !( other1.addressAsInteger() < other2.addressAsInteger() ) );
}

/*==========================================================================*/
/**
 *  '==' operator.
 *  @param other1 [in] IP address
 *  @param other2 [in] IP address
 *  @return true, if the other1 is equal to the other2.
 */
/*==========================================================================*/
bool operator == ( const IPAddress& other1, const IPAddress& other2 )
{
    return( other1.addressAsInteger() == other2.addressAsInteger() );
}

/*==========================================================================*/
/**
 *  '!=' operator.
 *  @param other1 [in] IP address
 *  @param other2 [in] IP address
 *  @return true, if the other1 is not equal to the other2.
 */
/*==========================================================================*/
bool operator != ( const IPAddress& other1, const IPAddress& other2 )
{
    return( !( other1.addressAsInteger() == other2.addressAsInteger() ) );
}

/*==========================================================================*/
/**
 *  Output stream.
 *  @param os [in] output stream
 *  @param other [in] IP address
 *  @return output stream
 */
/*==========================================================================*/
std::ostream& operator << ( std::ostream& os, const IPAddress& other )
{
    os << other.addressAsString();

    return( os );
}

/*==========================================================================*/
/**
 *  Get an address.
 *  @return IP address
 */
/*==========================================================================*/
const IPAddress::address_type& IPAddress::address() const
{
    return( m_address );
}

/*==========================================================================*/
/**
 *  Get an address as integer type.
 *  @return IP address
 */
/*==========================================================================*/
IPAddress::integer_type IPAddress::addressAsInteger() const
{
#if defined( KVS_PLATFORM_WINDOWS )
    return( m_address.S_un.S_addr );
#else
    return( m_address.s_addr );
#endif
}

/*==========================================================================*/
/**
 *  Get an address as string type.
 *  @return IP address
 */
/*==========================================================================*/
std::string IPAddress::addressAsString() const
{
    return( std::string( ::inet_ntoa( m_address ) ) );
}

/*==========================================================================*/
/**
 *  Set an IP address.
 *  @param hostname [in] hostname
 */
/*==========================================================================*/
void IPAddress::set_ip_address( const char* hostname )
{
#if defined( KVS_PLATFORM_WINDOWS )
    if( std::string( hostname ) == "localhost" )
    {
        IPAddress::integer_type address = ::inet_addr( "127.0.0.1" );
        this->set_ip_address( address );
        return;
    }
#endif

    IPAddress::integer_type address = ::inet_addr( hostname );
    if( address == IPAddress::None )
    {
        struct hostent* ent;
        ent = ::gethostbyname( hostname );
        if( ent )
        {
            if( ent->h_addr_list[0] )
            {
                address = *(integer_type*)ent->h_addr_list[0];
            }
        }
    }

    this->set_ip_address( address );
}

/*==========================================================================*/
/**
 *  Set an IP address.
 *  @param address [in] IP address
 */
/*==========================================================================*/
void IPAddress::set_ip_address( IPAddress::integer_type address )
{
#if defined( KVS_PLATFORM_WINDOWS )
    m_address.S_un.S_addr = address;
#else
    m_address.s_addr = address;
#endif
}

/*==========================================================================*/
/**
 *  Copy IP address.
 *  @param address [in] IP address
 */
/*==========================================================================*/
void IPAddress::copy_address( const IPAddress::address_type& address )
{
    memcpy( reinterpret_cast<void*>( &m_address ),
            reinterpret_cast<const void*>( &address ),
            sizeof( m_address ) );
}

} // end of namespace kvs
