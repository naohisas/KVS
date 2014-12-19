/****************************************************************************/
/**
 *  @file Semaphore.h
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: Semaphore.h 1372 2012-11-30 00:27:21Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#ifndef KVS__SEMAPHORE_H_INCLUDE
#define KVS__SEMAPHORE_H_INCLUDE

#include "Mutex.h"
#include "Condition.h"


namespace kvs
{

/*==========================================================================*/
/**
 *  Semaphore class.
 */
/*==========================================================================*/
class Semaphore
{
private:

    int m_available; ///< number of available semaphores
    kvs::Mutex m_mutex; ///< mutex for locker
    kvs::Condition m_condition; ///< condition

public:

    Semaphore( int nresources = 0 );
    virtual ~Semaphore();

    void acquire( int nresources = 1 );
    void release( int nresources = 1 );
    bool tryAcquire( int nresources = 1 );
    int available();
};

} // end of namespace kvs

#endif // KVS__SEMAPHORE_H_INCLUDE
