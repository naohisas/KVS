/*****************************************************************************/
/**
 *  @file   Acceptor.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: Acceptor.cpp 1317 2012-09-25 03:13:02Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#include "Acceptor.h"
#include <kvs/DebugNew>
#include <kvs/Message>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new Acceptor class.
 */
/*===========================================================================*/
Acceptor::Acceptor()
{
    if ( this->create_handler() ) this->open();
    else kvsMessageError("Cannot create handler.");
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new Acceptor class.
 *  @param  port [in] port number
 *  @param  ntrials [in] number of trials
 */
/*===========================================================================*/
Acceptor::Acceptor( const int port, const size_t ntrials )
{
    if ( this->create_handler() )
    {
        if ( this->open() ) this->bind( port, ntrials );
    }
    else
    {
        kvsMessageError("Cannot create handler.");
    }
}

/*===========================================================================*/
/**
 *  @brief  Destroys the Acceptor class.
 */
/*===========================================================================*/
Acceptor::~Acceptor()
{
    this->close();
}

/*===========================================================================*/
/**
 *  @brief  Open port.
 *  @return true if the process is done successfully
 */
/*===========================================================================*/
bool Acceptor::open()
{
    m_handler->open();
    if ( !m_handler->isOpen() )
    {
        kvsMessageError("Cannot open server for acceptor.");
        return( false );
    }

    return( true );
}

/*===========================================================================*/
/**
 *  @brief  Close the port.
 */
/*===========================================================================*/
void Acceptor::close()
{
    m_handler->close();
    this->delete_handler();
}

/*===========================================================================*/
/**
 *  @brief  Binds the port.
 *  @param  port [in] port number
 *  @param  ntrials [in] number of trials
 *  @return true if the process is done successfully
 */
/*===========================================================================*/
bool Acceptor::bind( const int port, const size_t ntrials )
{
    size_t counter = 0;
    while( counter < ntrials )
    {
        m_handler->bind( port );
        if( m_handler->isBound() )
        {
            if ( m_handler->listen() ) return( true );

            kvsMessageError( "Cannot listen port %d.", port );
            return( false );
        }

        counter++;
    }

    return( false );
}

/*===========================================================================*/
/**
 *  @brief  Returns the pointer to the new connection.
 *  @return pointer to the new connection (socket)
 */
/*===========================================================================*/
kvs::TCPSocket* Acceptor::newConnection()
{
    return( m_handler->checkForNewConnection() );
}

/*===========================================================================*/
/**
 *  @brief  Receives the message block.
 *  @param  block [in] pointer to the message block
 *  @param  client_address [in] client address
 *  @return received message size
 */
/*===========================================================================*/
int Acceptor::receive( kvs::MessageBlock* block, kvs::SocketAddress* client_address )
{
    return( m_handler->receive( block, client_address ) );
}

/*===========================================================================*/
/**
 *  @brief  Creates the handler.
 *  @return true if the process is done successfully
 */
/*===========================================================================*/
bool Acceptor::create_handler()
{
    m_handler = new kvs::TCPServer();

    return( m_handler != NULL );
}

/*===========================================================================*/
/**
 *  @brief  Deletes the handler.
 */
/*===========================================================================*/
void Acceptor::delete_handler()
{
    if ( m_handler ) delete m_handler;
}

} // end of namespace kvs
