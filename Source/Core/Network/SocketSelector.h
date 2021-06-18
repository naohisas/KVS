/****************************************************************************/
/**
 *  @file   SocketSelector.h
 *  @author Naohisa Sakamoto
 */
/****************************************************************************/
#ifndef KVS__SOCKET_SELECTOR_H_INCLUDE
#define KVS__SOCKET_SELECTOR_H_INCLUDE

#include "Socket.h"
#include "SocketTimer.h"


namespace kvs
{

/*==========================================================================*/
/**
 *  Socket select class.
 */
/*==========================================================================*/
class SocketSelector
{
protected:

    fd_set m_readable; ///< readable descriptor
    fd_set m_writable; ///< writable descriptor

public:

    SocketSelector();
    SocketSelector( const SocketSelector& other );
    virtual ~SocketSelector();

public:

    void setReadable( const kvs::Socket::id_type& socket_id );
    void setWritable( const kvs::Socket::id_type& socket_id );
    bool isReadable( const kvs::Socket::id_type& socket_id );
    bool isWritable( const kvs::Socket::id_type& socket_id );
    void clearReadable( const kvs::Socket::id_type& socket_id );
    void clearWritable( const kvs::Socket::id_type& socket_id );
    void clear();
    int select( const kvs::SocketTimer& timeout );
};

} // end of namespace kvs

#endif // KVS__SOCKET_SELECTOR_H_INCLUDE
