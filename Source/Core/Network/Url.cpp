/****************************************************************************/
/**
 *  @file Url.cpp
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: Url.cpp 1317 2012-09-25 03:13:02Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#include "Socket.h"
#include "Url.h"
#include <cstdlib>


namespace kvs
{

/*==========================================================================*/
/**
 *  Constructor.
 */
/*==========================================================================*/
Url::Url():
    m_port( 0 )
{
}

/*==========================================================================*/
/**
 *  Constructor.
 *  @param url [in] URL string
 */
/*==========================================================================*/
Url::Url( const std::string& url ):
    m_port( 0 )
{
    this->parse_url( url );
}

/*==========================================================================*/
/**
 *  output operator.
 */
/*==========================================================================*/
std::ostream& operator << ( std::ostream& os, const Url& other )
{
    os << "protocol: " << other.m_protocol << std::endl
       << "username: " << other.m_username << std::endl
       << "password: " << other.m_password << std::endl
       << "hostname: " << other.m_hostname << std::endl
       << "port:     " << other.m_port << std::endl
       << "path:     " << other.m_path;

    return os;
}

/*==========================================================================*/
/**
 *  Get a protocol.
 *  @return protocol
 */
/*==========================================================================*/
const std::string& Url::protocol() const
{
    return( m_protocol );
}

/*==========================================================================*/
/**
 *  Get a username.
 *  @return username
 */
/*==========================================================================*/
const std::string& Url::username() const
{
    return( m_username );
}

/*==========================================================================*/
/**
 *  Get a password.
 *  @return password
 */
/*==========================================================================*/
const std::string& Url::password() const
{
    return( m_password );
}

/*==========================================================================*/
/**
 *  Get a hostname.
 *  @return hostname
 */
/*==========================================================================*/
const std::string& Url::hostname() const
{
    return( m_hostname );
}

/*==========================================================================*/
/**
 *  Get a port numebr.
 *  @return port number
 */
/*==========================================================================*/
int Url::port() const
{
    return( m_port );
}

/*==========================================================================*/
/**
 *  Get a path.
 *  @return path
 */
/*==========================================================================*/
const std::string& Url::path() const
{
    return( m_path );
}

/*==========================================================================*/
/**
 *  Parse URL string.
 *  @param url [in] URL string
 */
/*==========================================================================*/
void Url::parse_url( const std::string& url )
{
    this->split_protocol( url );
}

/*==========================================================================*/
/**
 *  Split protocol from a given URL string.
 *  @param url [in] URL string
 */
/*==========================================================================*/
void Url::split_protocol( std::string url )
{
    std::string::size_type p = url.find_first_of(":");
    if( p != std::string::npos )
    {
        m_protocol = url.substr(0,p);

        if( m_protocol == "http" )
        {
            struct servent* service = getservbyname( "http", "tcp" );
            m_port = ( service ) ? ntohs( service->s_port ) : 80;
        }
        else if( m_protocol == "ftp"  )
        {
            struct servent* service = getservbyname( "ftp", "tcp" );
            m_port = ( service ) ? ntohs( service->s_port ) : 21;
        }
        else return;

        this->split_authority( url.substr(p+3) );
    }
}

/*==========================================================================*/
/**
 *  Split authority string.
 *  @param sub_url [in] sub URL
 */
/*==========================================================================*/
void Url::split_authority( std::string sub_url )
{
    std::string::size_type p = sub_url.find_first_of("/");
    if( p != std::string::npos )
    {
        m_path  = sub_url.substr(p+1);
        sub_url = sub_url.substr(0,p);
    }

    p = sub_url.find_first_of("@");
    if( p != std::string::npos )
    {
        this->split_user_info( sub_url.substr(0,p) );
    }

    this->split_hostname( sub_url );
}

/*==========================================================================*/
/**
 *  Split user information.
 *  @param sub_url [in] sub URL
 */
/*==========================================================================*/
void Url::split_user_info( std::string sub_url )
{
    std::string::size_type p = sub_url.find_first_of(":");
    if( p != std::string::npos )
    {
        m_username = sub_url.substr(0,p);
        m_password = sub_url.substr(p+1);
    }
    else
    {
        m_username = sub_url;
    }
}

/*==========================================================================*/
/**
 *  Split hostname.
 *  @param sub_url [in] sub URL
 */
/*==========================================================================*/
void Url::split_hostname( std::string sub_url )
{
    std::string::size_type p = sub_url.find_first_of(":");
    if( p != std::string::npos )
    {
        m_hostname = sub_url.substr(0,p);
        m_port     = atoi( sub_url.substr(p+1).c_str() );
    }
    else
    {
        m_hostname = sub_url;
    }
}

} // end of namespace kvs
