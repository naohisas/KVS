/****************************************************************************/
/**
 *  @file MessageBlock.cpp
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: MessageBlock.cpp 1536 2013-04-15 02:21:56Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#include "SocketStandard.h"
#include "MessageBlock.h"
#include <cstring>
#include <kvs/Type>


namespace
{
const size_t SizeOfHeader = sizeof( kvs::UInt32 ); // 32 bits = 4 bytes
}

namespace kvs
{

/*==========================================================================*/
/**
 *  Constructor.
 */
/*==========================================================================*/
MessageBlock::MessageBlock()
{
}

/*==========================================================================*/
/**
 *  Constructor.
 *  @param message [in] pointer to message
 *  @param message_size [in] size of message [byte]
 */
/*==========================================================================*/
MessageBlock::MessageBlock( const void* message, const size_t message_size )
{
    this->copy( message, message_size );
}

/*==========================================================================*/
/**
 *  Constructor.
 *  @param message [in] message
 */
/*==========================================================================*/
MessageBlock::MessageBlock( const std::string& message )
{
    this->copy( message );
}

/*==========================================================================*/
/**
 *  Constructor.
 *  @param message [in] message
 */
/*==========================================================================*/
template<typename T>
MessageBlock::MessageBlock( const std::vector<T>& message )
{
    this->copy( message );
}

/*==========================================================================*/
/**
 *  Destructor.
 */
/*==========================================================================*/
MessageBlock::~MessageBlock()
{
    this->release();
}

/*==========================================================================*/
/**
 *  Get a size of message.
 *  @return message size [byte]
 */
/*==========================================================================*/
size_t MessageBlock::size() const
{
    if( m_block.size() == 0 ) return( 0 );

    return( m_block.size() == 0 ? 0 : m_block.size() - SizeOfHeader );
}

/*==========================================================================*/
/**
 *  Get a pointer to message.
 *  @return pointer to message
 */
/*==========================================================================*/
void* MessageBlock::data()
{
    return( m_block.data() + SizeOfHeader );
}

/*==========================================================================*/
/**
 *  Get a pointer to message.
 *  @return pointer to message
 */
/*==========================================================================*/
const void* MessageBlock::data() const
{
    return( m_block.data() + SizeOfHeader );
}

/*==========================================================================*/
/**
 *  Get a size of message block.
 *  @return message block size [byte]
 */
/*==========================================================================*/
size_t MessageBlock::blockSize() const
{
    return( m_block.size() );
}

/*==========================================================================*/
/**
 *  Get a point to message block.
 *  @return pointer to message block
 */
/*==========================================================================*/
void* MessageBlock::blockData()
{
    return( m_block.data() );
}

/*==========================================================================*/
/**
 *  Get a point to message block.
 *  @return pointer to message block
 */
/*==========================================================================*/
const void* MessageBlock::blockData() const
{
    return( m_block.data() );
}

/*==========================================================================*/
/**
 *  Get message as string.
 *  @return message
 */
/*==========================================================================*/
std::string MessageBlock::toString() const
{
    const char*  c_str = reinterpret_cast<const char*>( this->data() );
    const size_t size  = this->size();

    return( std::string( c_str, size ) );
}

/*==========================================================================*/
/**
 *  Copy message to block.
 *  @param message [in] message
 *  @param message_size [in] size of message
 */
/*==========================================================================*/
void MessageBlock::copy( const void* message, size_t message_size )
{
    if( this->allocate( message_size ) )
    {
        // Convert host byte-order to network byte-order.
        kvs::UInt32 size = htonl( static_cast<kvs::UInt32>( message_size ) );
//        unsigned int size = htonl( static_cast<unsigned int>( message_size ) );
//        u_long size = htonl( static_cast<u_long>( message_size ) );

        unsigned char* p = m_block.data();
        memcpy( p,                &size,   SizeOfHeader );
        memcpy( p + SizeOfHeader, message, message_size );
    }
}

/*==========================================================================*/
/**
 *  Copy message to block.
 *  @param message [in] message
 */
/*==========================================================================*/
void MessageBlock::copy( const std::string& message )
{
    this->copy( message.c_str(), message.size() );
}

/*==========================================================================*/
/**
 *  Copy message to block.
 *  @param message [in] message
 */
/*==========================================================================*/
template <typename T>
void MessageBlock::copy( const std::vector<T>& message )
{
    this->copy( &message[0], message.size() * sizeof(T) );
}

/*==========================================================================*/
/**
 *  Allocate message block.
 *  @param message_size [in] size of message [byte]
 *  @return pointer to the allocate region
 */
/*==========================================================================*/
void* MessageBlock::allocate( size_t data_size )
{
    m_block.allocate( data_size + SizeOfHeader );
    return( m_block.data() );
}

/*==========================================================================*/
/**
 *  Deallocate.
 */
/*==========================================================================*/
void MessageBlock::release()
{
    m_block.release();
}

} // end of namespace kvs
