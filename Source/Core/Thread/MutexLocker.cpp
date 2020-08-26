/****************************************************************************/
/**
 *  @file   MutexLocker.cpp
 *  @author Naohisa Sakamoto
 */
/****************************************************************************/
#include "MutexLocker.h"
#include "Mutex.h"


namespace kvs
{

/*==========================================================================*/
/**
 *  @brief  Constructs a new MutexLocker class.
 *  @param  mutex [in] pointer to the mutex
 */
/*==========================================================================*/
MutexLocker::MutexLocker( kvs::Mutex* mutex ):
    m_mutex( mutex )
{
    this->relock();
}

/*==========================================================================*/
/**
 *  @brief  Destructs the MutexLocker class.
 */
/*==========================================================================*/
MutexLocker::~MutexLocker()
{
    this->unlock();
}

/*==========================================================================*/
/**
 *  @brief  Relock.
 */
/*==========================================================================*/
void MutexLocker::relock()
{
    if ( m_mutex ) { m_mutex->lock(); }
}

/*==========================================================================*/
/**
 *  @brief  Unlock.
 */
/*==========================================================================*/
void MutexLocker::unlock()
{
    if ( m_mutex ) { m_mutex->unlock(); }
}

/*==========================================================================*/
/**
 *  @brief  Returns pointer to the mutex.
 *  @return pointer to the mutex
 */
/*==========================================================================*/
Mutex* MutexLocker::mutex()
{
    return m_mutex;
}

} // end of namespace kvs
