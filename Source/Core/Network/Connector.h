/*****************************************************************************/
/**
 *  @file   Connector.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#ifndef KVS__CONNECTOR_H_INCLUDE
#define KVS__CONNECTOR_H_INCLUDE

#include <kvs/TCPSocket>
#include <kvs/IPAddress>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Connector class.
 */
/*===========================================================================*/
class Connector
{
private:

    kvs::TCPSocket* m_handler; //< handler
    kvs::IPAddress m_ip; //< IP address
    int m_port; //< port number
    size_t m_ntrials; //< number of trials

public:

    Connector();
    Connector( kvs::TCPSocket* handler );
    Connector( const kvs::IPAddress& ip, const int port, const size_t ntrials );
    ~Connector();

    bool open();
    void close();
    bool connect( const kvs::IPAddress& ip, const int port, const size_t ntrials );
    bool reconnect();
    void send( const kvs::MessageBlock& block );

private:

    bool create_handler();
    void delete_handler();
};

} // end of namespace kvs

#endif // KVS__CONNECTOR_H_INCLUDE
