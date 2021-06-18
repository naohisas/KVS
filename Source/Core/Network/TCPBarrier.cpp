/*****************************************************************************/
/**
 *  @file   TCPBarrier.cpp
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#include "TCPBarrier.h"
#include <kvs/DebugNew>
#include <kvs/TCPSocket>

namespace { const int BARRIER_BUFFER_SIZE = 12; }

namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new TCPBarrier class.
 *  @param  ip [in] IP address
 *  @param  port [in] port number
 */
/*===========================================================================*/
TCPBarrier::TCPBarrier( const kvs::IPAddress& ip, int port )
{
    m_client = new kvs::TCPSocket( ip, port );
    if( !m_client->isConnected() )
    {
        kvsMessageError("Cannot connect to the barrier server.");
    }
}

/*===========================================================================*/
/**
 *  @brief  Destroys the TCPBarrier class.
 */
/*===========================================================================*/
TCPBarrier::~TCPBarrier()
{
    if( m_client ) delete m_client;
}

/*===========================================================================*/
/**
 *  @brief  Wait for the connections.
 */
/*===========================================================================*/
void TCPBarrier::wait()
{
    char send_buffer[::BARRIER_BUFFER_SIZE]; sprintf( send_buffer, "BARRIER" );
//        m_client->disableBlocking();
    m_client->send( send_buffer, ::BARRIER_BUFFER_SIZE );

    m_mutex.lock();

    char recv_buffer[::BARRIER_BUFFER_SIZE];
//        m_client->enableBlocking();
    int size = m_client->receive( recv_buffer, ::BARRIER_BUFFER_SIZE );

    if( size > 0 ) m_mutex.unlock();
}

} // end of namespace kvs
