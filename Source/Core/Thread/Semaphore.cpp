/****************************************************************************/
/**
 *  @file Semaphore.cpp
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: Semaphore.cpp 1372 2012-11-30 00:27:21Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#include "Semaphore.h"
#include "MutexLocker.h"
#include "Condition.h"


namespace kvs
{

/*==========================================================================*/
/**
 *  @brief  Constructs a new Semaphore class.
 *  @param  nresources [in] number of resources
 */
/*==========================================================================*/
Semaphore::Semaphore( int nresources ):
    m_available( nresources )
{
}

/*==========================================================================*/
/**
 *  @brief  Destructs the Semaphore class.
 */
/*==========================================================================*/
Semaphore::~Semaphore()
{
}

/*==========================================================================*/
/**
 *  @brief  Acquire the semaphores.
 *  @param  nresources [in] number of resources
 */
/*==========================================================================*/
void Semaphore::acquire( int nresources )
{
    kvs::MutexLocker locker( &m_mutex );

    while ( nresources > m_available )
    {
        m_condition.wait( &m_mutex );
    }

    m_available -= nresources;
}

/*==========================================================================*/
/**
 *  @brief  Release the semaphores.
 *  @param  nresources [in] number of resources
 */
/*==========================================================================*/
void Semaphore::release( int nresources )
{
    kvs::MutexLocker locker( &m_mutex );

    m_condition.wakeUpAll();
    m_available += nresources;
}

/*==========================================================================*/
/**
 *  @brief  Test whether it is posibble to acquire.
 *  @param  nresources [in] number of resources
 *  @return true, if it is possible to acquire
 */
/*==========================================================================*/
bool Semaphore::tryAcquire( int nresources )
{
    kvs::MutexLocker locker( &m_mutex );

    if ( nresources > m_available ) { return false; }

    m_available -= nresources;

    return true;
}

/*==========================================================================*/
/**
 *  @brief  Returns the number of the available resources.
 *  @return number of the available resources
 */
/*==========================================================================*/
int Semaphore::available()
{
    kvs::MutexLocker locker( &m_mutex );

    return m_available;
}

} // end of namespace kvs
