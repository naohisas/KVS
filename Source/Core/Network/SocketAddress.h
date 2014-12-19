/****************************************************************************/
/**
 *  @file SocketAddress.h
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: SocketAddress.h 1317 2012-09-25 03:13:02Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#ifndef KVS__SOCKET_ADDRESS_H_INCLUDE
#define KVS__SOCKET_ADDRESS_H_INCLUDE

#include "IPAddress.h"
#include <iostream>
#include <cstring>


namespace kvs
{

/*==========================================================================*/
/**
 *  @class  Socket address class.
 */
/*==========================================================================*/
class SocketAddress
{
public:

    typedef struct sockaddr_in address_type;

private:

    address_type m_address; ///< IP address and port number
    kvs::IPAddress m_ip; ///< IP address
    int m_port; ///< port number

public:

    SocketAddress();
    SocketAddress( const char* address_name );
    SocketAddress( const kvs::IPAddress& ip, const int port );
    SocketAddress( const SocketAddress& other );

    SocketAddress& operator = ( const SocketAddress& other );
    friend bool operator < ( const SocketAddress& other1, const SocketAddress& other2 );
    friend bool operator > ( const SocketAddress& other1, const SocketAddress& other2 );
    friend bool operator <= ( const SocketAddress& other1, const SocketAddress& other2 );
    friend bool operator >= ( const SocketAddress& other1, const SocketAddress& other2 );
    friend bool operator == ( const SocketAddress& other1, const SocketAddress& other2 );
    friend bool operator != ( const SocketAddress& other1, const SocketAddress& other2 );
    friend std::ostream& operator << ( std::ostream& os, const SocketAddress& other );

    const kvs::IPAddress& ip() const;
    int port() const;
    const address_type& address() const;

    void setIp( const kvs::IPAddress& ip );
    void setPort( const int port );
    void setAddress( const address_type& address );

    static void initialize( address_type* address );

protected:

    void initialize_address( const IPAddress& ip, const int port );
    void copy_address( const address_type& address );
    void split_ip_and_port( const char* address_name );
};

} // end of namespace kvs

#endif // KVS__SOCKET_ADDRESS_H_INCLUDE
