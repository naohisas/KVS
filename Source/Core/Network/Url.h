/****************************************************************************/
/**
 *  @file Url.h
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: Url.h 1317 2012-09-25 03:13:02Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#ifndef KVS__URL_H_INCLUDE
#define KVS__URL_H_INCLUDE

#include <iostream>
#include <string>


namespace kvs
{

/*==========================================================================*/
/**
 *  URL class.
 */
/*==========================================================================*/
class Url
{
protected:

    std::string m_protocol; ///< protocol (e.g. "http", "ftp")
    std::string m_username; ///< user name
    std::string m_password; ///< password
    std::string m_hostname; ///< hostname
    int m_port; ///< port number
    std::string m_path; ///< path

public:

    Url();
    Url( const std::string& url );

    friend std::ostream& operator << ( std::ostream& os, const Url& other );

    const std::string& protocol() const;
    const std::string& username() const;
    const std::string& password() const;
    const std::string& hostname() const;
    int port() const;
    const std::string& path() const;

private:

    void parse_url( const std::string& url );
    void split_protocol( std::string url );
    void split_authority( std::string sub_url );
    void split_user_info( std::string sub_url );
    void split_hostname( std::string sub_url );
};

} // end of namespace kvs

#endif // KVS__URL_H_INCLUDE
