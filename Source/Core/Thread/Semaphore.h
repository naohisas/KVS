/****************************************************************************/
/**
 *  @file   Semaphore.h
 *  @author Naohisa Sakamoto
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
