/*****************************************************************************/
/**
 *  @file   Connector.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: Connector.cpp 1317 2012-09-25 03:13:02Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#include "Connector.h"
#include <kvs/DebugNew>
#include <kvs/Message>

namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new Connector class.
 */
/*===========================================================================*/
Connector::Connector()
{
    if ( this->create_handler() ) this->open();
    else kvsMessageError("Cannot create handler.");
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new Connector class.
 *  @param  handler [in] pointer to the socket
 */
/*===========================================================================*/
Connector::Connector( kvs::TCPSocket* handler ):
    m_handler( handler )
{
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new Connector class.
 *  @param  ip [in] IP address
 *  @param  port [in] port number
 *  @param  ntrials [in] number of trials
 */
/*===========================================================================*/
Connector::Connector( const kvs::IPAddress& ip, const int port, const size_t ntrials )
{
    if ( this->create_handler() )
    {
        if ( this->open() )
        {
            this->connect( ip, port, ntrials );
        }
    }
    else
    {
        kvsMessageError("Cannot create handler.");
    }
}

/*===========================================================================*/
/**
 *  @brief  Destroys the Connector class.
 */
/*===========================================================================*/
Connector::~Connector()
{
    this->close();
    this->delete_handler();
}

/*===========================================================================*/
/**
 *  @brief  Open the socket.
 *  @return true if the process is done successfully
 */
/*===========================================================================*/
bool Connector::open()
{
    m_handler->open();
    if ( !m_handler->isOpen() )
    {
        kvsMessageError("Cannot open socket for connector.");
        return( false );
    }

    return( true );
}

/*===========================================================================*/
/**
 *  @brief  Close the socket.
 */
/*===========================================================================*/
void Connector::close()
{
    m_handler->close();
}

/*===========================================================================*/
/**
 *  @brief  Connect to the server.
 *  @param  ip [in] IP address
 *  @param  port [in] port number
 *  @param  ntrials [in] number of trials
 *  @return true if the proce is done successfully
 */
/*===========================================================================*/
bool Connector::connect( const kvs::IPAddress& ip, const int port, const size_t ntrials )
{
    m_ip      = ip;
    m_port    = port;
    m_ntrials = ntrials;

    size_t counter = 0;
    while ( counter < ntrials )
    {
        m_handler->connect( ip, port );
        if ( m_handler->isConnected() ) return( true );

        counter++;
    }

    return( false );
}

/*===========================================================================*/
/**
 *  @brief  Reconnect to the server.
 *  @return true if the proce is done successfully
 */
/*===========================================================================*/
bool Connector::reconnect()
{
    if ( m_handler->isConnected() )
    {
        this->close();

        if ( this->open() )
        {
            return( this->connect( m_ip, m_port, m_ntrials ) );
        }
    }

    return( false );
}

/*===========================================================================*/
/**
 *  @brief  Send the message block.
 *  @param  block [in] message block
 */
/*===========================================================================*/
void Connector::send( const kvs::MessageBlock& block )
{
    m_handler->send( block );
}

/*===========================================================================*/
/**
 *  @brief  Creates the handler.
 *  @return true if the handler is created successfully
 */
/*===========================================================================*/
bool Connector::create_handler()
{
    m_handler = new kvs::TCPSocket();

    return( m_handler != NULL );
}

/*===========================================================================*/
/**
 *  @brief  Deletes the handler.
 */
/*===========================================================================*/
void Connector::delete_handler()
{
    if ( m_handler ) delete m_handler;
}

} // end of namespace kvs
