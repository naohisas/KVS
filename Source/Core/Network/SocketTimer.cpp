/****************************************************************************/
/**
 *  @file SocketTimer.cpp
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: SocketTimer.cpp 1317 2012-09-25 03:13:02Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#include "SocketTimer.h"


namespace kvs
{

const SocketTimer SocketTimer::Zero;

SocketTimer::SocketTimer( const double seconds )
{
    this->setValue( seconds );
}

const SocketTimer& SocketTimer::operator = ( const SocketTimer& other )
{
    m_value.tv_sec  = other.m_value.tv_sec;
    m_value.tv_usec = other.m_value.tv_usec;

    return( *this );
}

bool operator == ( const SocketTimer& other1, const SocketTimer& other2 )
{
    return( other1.m_value.tv_sec  == other2.m_value.tv_sec &&
            other1.m_value.tv_usec == other2.m_value.tv_usec );
}

const struct timeval& SocketTimer::value() const
{
    return( m_value );
}

struct timeval& SocketTimer::value()
{
    return( m_value );
}

void SocketTimer::setValue( double seconds )
{
    m_value.tv_sec  = static_cast<long>( seconds );
    m_value.tv_usec = static_cast<long>( ( seconds - m_value.tv_sec ) * 1000000 );
}

bool SocketTimer::isZero() const
{
    return( m_value.tv_sec == 0 && m_value.tv_usec == 0 );
}

} // end of namespace kvs
