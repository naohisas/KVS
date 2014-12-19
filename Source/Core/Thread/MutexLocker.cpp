/****************************************************************************/
/**
 *  @file MutexLocker.cpp
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: MutexLocker.cpp 1372 2012-11-30 00:27:21Z naohisa.sakamoto@gmail.com $
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
