/****************************************************************************/
/**
 *  @file IPAddress.h
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: IPAddress.h 1317 2012-09-25 03:13:02Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#ifndef KVS__IP_ADDRESS_H_INCLUDE
#define KVS__IP_ADDRESS_H_INCLUDE

#include "SocketStandard.h"
#include <kvs/Platform>
#include <iostream>
#include <string>
#include <cstring>


namespace kvs
{

/*==========================================================================*/
/**
 *  @class  IP address class.
 */
/*==========================================================================*/
class IPAddress
{
public:

#if defined( KVS_PLATFORM_WINDOWS )
    typedef unsigned long integer_type;
#else
    typedef u_int32_t integer_type;
#endif
    typedef struct in_addr address_type;

    static const integer_type Any; // 0.0.0.0
    static const integer_type Loopback; // 127.0.0.1
    static const integer_type Broadcast; // 255.255.255.255
    static const integer_type None; // 255.255.255.255

private:

    address_type m_address; ///< IP address

public:

    IPAddress( integer_type address = IPAddress::None );
    IPAddress( const char* hostname );
    IPAddress( const address_type& address );
    IPAddress( const IPAddress& other );

    IPAddress& operator = ( const char* hostname );
    IPAddress& operator = ( integer_type address );
    IPAddress& operator = ( const IPAddress& other );
    friend bool operator < ( const IPAddress& other1, const IPAddress& other2 );
    friend bool operator > ( const IPAddress& other1, const IPAddress& other2 );
    friend bool operator <= ( const IPAddress& other1, const IPAddress& other2 );
    friend bool operator >= ( const IPAddress& other1, const IPAddress& other2 );
    friend bool operator == ( const IPAddress& other1, const IPAddress& other2 );
    friend bool operator != ( const IPAddress& other1, const IPAddress& other2 );
    friend std::ostream& operator << ( std::ostream& os, const IPAddress& other );

    const address_type& address() const;
    integer_type addressAsInteger() const;
    std::string addressAsString() const;

protected:

    void set_ip_address( const char* hostname );
    void set_ip_address( integer_type address );
    void copy_address( const address_type& address );
};

} // end of namespace kvs

#endif // KVS__IP_ADDRESS_H_INCLUDE
