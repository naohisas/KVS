/****************************************************************************/
/**
 *  @file ReadLocker.cpp
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: ReadLocker.cpp 1372 2012-11-30 00:27:21Z naohisa.sakamoto@gmail.com $
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
