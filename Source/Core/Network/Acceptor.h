/*****************************************************************************/
/**
 *  @file   Acceptor.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: Acceptor.h 1317 2012-09-25 03:13:02Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#ifndef KVS__ACCEPTOR_H_INCLUDE
#define KVS__ACCEPTOR_H_INCLUDE

#include <kvs/TCPServer>
#include <kvs/IPAddress>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Acceptor class.
 */
/*===========================================================================*/
class Acceptor
{
private:

    kvs::TCPServer* m_handler; //< handler

public:

    Acceptor();
    Acceptor( const int port, const size_t ntrials );
    ~Acceptor();

    bool open();
    void close();
    bool bind( const int port, const size_t ntrials );
    kvs::TCPSocket* newConnection();
    int receive( kvs::MessageBlock* block, kvs::SocketAddress* client_address = 0 );

private:

    bool create_handler();
    void delete_handler();
};

} // end of namespace kvs

#endif // KVS__ACCEPTOR_H_INCLUDE
