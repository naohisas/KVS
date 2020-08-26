/****************************************************************************/
/**
 *  @file   SocketTimer.h
 *  @author Naohisa Sakamoto
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
