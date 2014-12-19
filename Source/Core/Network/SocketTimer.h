/****************************************************************************/
/**
 *  @file SocketTimer.h
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: SocketTimer.h 1317 2012-09-25 03:13:02Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#ifndef KVS__SOCKET_TIMER_H_INCLUDE
#define KVS__SOCKET_TIMER_H_INCLUDE

#include "SocketStandard.h"


namespace kvs
{

/*==========================================================================*/
/**
 *  @class  Socket timer class.
 */
/*==========================================================================*/
class SocketTimer
{
public:

    static const SocketTimer Zero;

private:

    struct timeval m_value; ///< time value

public:

    SocketTimer( const double seconds = 0.0 );

    const SocketTimer& operator = ( const SocketTimer& other );
    friend bool operator == ( const SocketTimer& other1, const SocketTimer& other2 );

    const struct timeval& value() const;
    struct timeval& value();

    void setValue( const double seconds );

    bool isZero() const;
};

} // end of namespace kvs

#endif // KVS__SOCKET_TIMER_H_INCLUDE
