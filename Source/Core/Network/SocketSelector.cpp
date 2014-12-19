/****************************************************************************/
/**
 *  @file SocketSelector.cpp
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: SocketSelector.cpp 1317 2012-09-25 03:13:02Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#include "SocketSelector.h"
#include "SocketTimer.h"
#include <kvs/Compiler>

// FD_SET includes a warning C4127: conditional expression is constant.
#if defined ( KVS_COMPILER_VC )
#pragma warning( push )
#pragma warning( disable: 4127 )
#endif


namespace kvs
{

SocketSelector::SocketSelector()
{
    this->clear();
}

SocketSelector::SocketSelector( const SocketSelector& other ):
    m_readable( other.m_readable ),
    m_writable( other.m_writable )
{
}

SocketSelector::~SocketSelector()
{
}

void SocketSelector::setReadable( const kvs::Socket::id_type& socket_id )
{
    FD_SET( socket_id, &m_readable );
}

void SocketSelector::setWritable( const kvs::Socket::id_type& socket_id )
{
    FD_SET( socket_id, &m_writable );
}

bool SocketSelector::isReadable( const kvs::Socket::id_type& socket_id )
{
    return( FD_ISSET( socket_id, &m_readable ) ? true : false );
}

bool SocketSelector::isWritable( const kvs::Socket::id_type& socket_id )
{
    return( FD_ISSET( socket_id, &m_writable ) ? true : false );
}

void SocketSelector::clearReadable( const kvs::Socket::id_type& socket_id )
{
    FD_CLR( socket_id, &m_readable );
}

void SocketSelector::clearWritable( const kvs::Socket::id_type& socket_id )
{
    FD_CLR( socket_id, &m_writable );
}

void SocketSelector::clear()
{
    FD_ZERO( &m_readable );
    FD_ZERO( &m_writable );
}

int SocketSelector::select( const kvs::SocketTimer& timeout )
{
    kvs::SocketTimer temp = timeout;

    struct timeval* p_timeout;
    if( timeout.isZero() )
    {
        p_timeout = NULL;
    }
    else
    {
        p_timeout = &( temp.value() );
    }

    return( ::select( FD_SETSIZE, &m_readable, &m_writable, NULL, p_timeout ) );
}

} // end of namespace kvs

#if defined ( KVS_COMPILER_VC )
#pragma warning( pop )
#endif
