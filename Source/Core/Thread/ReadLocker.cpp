/****************************************************************************/
/**
 *  @file   ReadLocker.cpp
 *  @author Naohisa Sakamoto
 */
/****************************************************************************/
#include "ReadLocker.h"
#include "ReadWriteLock.h"


namespace kvs
{

/*==========================================================================*/
/**
 *  @brief  Constructs a new ReadLocker class.
 *  @param  rwlock [in] pointer to read/write lock
 */
/*==========================================================================*/
ReadLocker::ReadLocker( kvs::ReadWriteLock* rwlock ):
    m_rwlock( rwlock )
{
    this->relock();
}

/*==========================================================================*/
/**
 *  @brief  Destructs the ReadLocker class.
 */
/*==========================================================================*/
ReadLocker::~ReadLocker()
{
    this->unlock();
}

/*==========================================================================*/
/**
 *  @brief  Relock.
 */
/*==========================================================================*/
void ReadLocker::relock()
{
    if ( m_rwlock ) { m_rwlock->lockRead(); }
}

/*==========================================================================*/
/**
 *  @brief  Unlock.
 */
/*==========================================================================*/
void ReadLocker::unlock()
{
    if ( m_rwlock ) { m_rwlock->unlock(); }
}

/*==========================================================================*/
/**
 *  @brief  Returns pointer to the read/write lock.
 *  @return pointer to the read/write lock
 */
/*==========================================================================*/
kvs::ReadWriteLock* ReadLocker::readWriteLock()
{
    return m_rwlock;
}

} // end of namespace kvs
