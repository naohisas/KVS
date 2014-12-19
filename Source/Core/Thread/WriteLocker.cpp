/****************************************************************************/
/**
 *  @file WriteLocker.cpp
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: WriteLocker.cpp 1372 2012-11-30 00:27:21Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#include "WriteLocker.h"
#include "ReadWriteLock.h"


namespace kvs
{

/*==========================================================================*/
/**
 *  @brief  Constructs a new WriteLocker class.
 *  @param  rwlock [in] pointer to read/write lock
 */
/*==========================================================================*/
WriteLocker::WriteLocker( kvs::ReadWriteLock* rwlock ):
    m_rwlock( rwlock )
{
    this->relock();
}

/*==========================================================================*/
/**
 *  @brief  Destructs the WriteLocker class.
 */
/*==========================================================================*/
WriteLocker::~WriteLocker()
{
    this->unlock();
}

/*==========================================================================*/
/**
 *  @brief  Relock.
 */
/*==========================================================================*/
void WriteLocker::relock()
{
    if ( m_rwlock ) { m_rwlock->lockWrite(); }
}

/*==========================================================================*/
/**
 *  @brief  Unlock.
 */
/*==========================================================================*/
void WriteLocker::unlock()
{
    if ( m_rwlock ) { m_rwlock->unlock(); }
}

/*==========================================================================*/
/**
 *  @brief  Returns pointer to the read/write lock.
 *  @return pointer to the read/write lock
 */
/*==========================================================================*/
kvs::ReadWriteLock* WriteLocker::readWriteLock()
{
    return m_rwlock;
}

} // end of namespace kvs
