/*****************************************************************************/
/**
 *  @file   main.cpp
 *  @brief  Example program of the socket (TCP).
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: main.cpp 1520 2013-04-10 01:22:07Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#include <iostream>
#include <string>
#include <kvs/CommandLine>
#include <kvs/TCPServer>
#include <kvs/TCPSocket>
#include <kvs/SocketAddress>
#include <kvs/MessageBlock>
#include <kvs/IPAddress>


/*===========================================================================*/
/**
 *  @brief  Argument class.
 */
/*===========================================================================*/
class Argument : public kvs::CommandLine
{
public:

    Argument( int argc, char** argv ):
        kvs::CommandLine( argc, argv )
    {
        addHelpOption();
        addOption( "server", "Server program.", 0, false );
        addOption( "client", "Client program.", 0, false );
        addOption( "port", "Port number.", 1, false );
        addOption( "ip", "IP address.", 1, false );
    }
};

/*===========================================================================*/
/**
 *  @brief  Server function.
 *  @param  argument [in] argument
 *  @return true, if the message can be received from the client successfully
 */
/*===========================================================================*/
bool Server( const Argument& argument )
{
    const int port = argument.hasOption("port") ? argument.optionValue<int>("port") : 5000;

    // Open.
    kvs::TCPServer server;
    server.open();

    // Bind.
    if ( server.bind( port ) < 0 )
    {
        const std::string error = server.errorString();
        kvsMessageError( "Cannot bind the port (%d). [%s]", port, error.c_str() );
        return false;
    }

    // Listen.
    if ( !server.listen() )
    {
        const std::string error = server.errorString();
        kvsMessageError( "Cannot listen to the port (%d). [%s]", port, error.c_str() );
        return false;
    }

    // Accept and receive the message.
    kvs::SocketAddress client_address;
    kvs::MessageBlock message;
    if ( server.receive( &message, &client_address ) < 0 )
    {
        const std::string error = server.errorString();
        kvsMessageError( "Cannot receive the message. [%s]", error.c_str() );
        return false;
    }

    if ( message.size() > 0 )
    {
        std::cout << "Client: " << client_address.ip() << std::endl;
        std::cout << "Message: " << message.toString() << std::endl;
    }
    else
    {
        kvsMessageError( "Received message is empty." );
        return false;
    }

    return true;
}

/*===========================================================================*/
/**
 *  @brief  Client function.
 *  @param  argument [in] argument
 *  @return true, if the message can be send to the server successfully
 */
/*===========================================================================*/
bool Client( const Argument& argument )
{
    const kvs::IPAddress ip( argument.hasOption("ip") ? argument.optionValue<std::string>("ip").c_str() : "localhost" );
    const int port = argument.hasOption("port") ? argument.optionValue<int>("port") : 5000;

    // Open.
    kvs::TCPSocket client;
    client.open();

    // Connect to the server.
    if ( !client.connect( ip, port ) )
    {
        const std::string error = client.errorString();
        kvsMessageError( "Cannot connect to the server. [%s]", error.c_str() );
        return false;
    }

    // Send the message.
    kvs::MessageBlock message( std::string("Hello KVS!") );
    if ( client.send( message ) < 0 )
    {
        const std::string error = client.errorString();
        kvsMessageError( "Cannot send the message. [%s]", error.c_str() );
        return false;
    }

    return true;
}

/*===========================================================================*/
/**
 *  @brief  Main function.
 *  @param  argc [in] argument count
 *  @param  argv [in] argument values
 */
/*===========================================================================*/
int main( int argc, char** argv )
{
    Argument argument( argc, argv );
    if ( !argument.parse() ) exit( EXIT_FAILURE );

    if ( argument.hasOption("server") ) return Server( argument );
    else if ( argument.hasOption("client") ) return Client( argument );

    kvsMessageError("Need to specify '-server' or '-client'.");
    return false;
}
