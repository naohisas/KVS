/*****************************************************************************/
/**
 *  @file   Acceptor.h
 *  @author Naohisa Sakamoto
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
