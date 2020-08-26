/****************************************************************************/
/**
 *  @file   WriteLocker.cpp
 *  @author Naohisa Sakamoto
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
