/****************************************************************************/
/**
 *  @file ReadWriteLock.cpp
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: ReadWriteLock.cpp 1372 2012-11-30 00:27:21Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#include "ReadWriteLock.h"
#include "MutexLocker.h"


namespace kvs
{

/*==========================================================================*/
/**
 *  @brief  Constructs a new ReadWriteLock class.
 */
/*==========================================================================*/
ReadWriteLock::ReadWriteLock():
    m_counter( 0 ),
    m_nreaders( 0 ),
    m_nwriters( 0 )
{
}

/*==========================================================================*/
/**
 *  @brief  Destructs the ReadWriteLock class.
 */
/*==========================================================================*/
ReadWriteLock::~ReadWriteLock()
{
}

/*==========================================================================*/
/**
 *  @brief  Lock for read access.
 */
/*==========================================================================*/
void ReadWriteLock::lockRead()
{
    kvs::MutexLocker locker( &m_mutex );

    while ( m_counter < 0 || m_nwriters )
    {
        m_nreaders++;
        m_reader.wait( &m_mutex );
        m_nreaders--;
    }

    m_counter++;
}

/*==========================================================================*/
/**
 *  @brief  Lock for write access.
 */
/*==========================================================================*/
void ReadWriteLock::lockWrite()
{
    kvs::MutexLocker locker( &m_mutex );

    while ( m_counter != 0 )
    {
        m_nwriters++;
        m_writer.wait( &m_mutex );
        m_nwriters--;
    }

    m_counter = -1;
}

/*==========================================================================*/
/**
 *  @brief  Test whether it is possible to lock for reader.
 *  @return true, if it is possible to lock
 */
/*==========================================================================*/
bool ReadWriteLock::tryLockRead()
{
    kvs::MutexLocker locker( &m_mutex );

    if ( m_counter < 0 ) { return false; }

    m_counter++;

    return true;
}

/*==========================================================================*/
/**
 *  @brief  Test whether it is possible to lock for writer.
 *  @return true, if it is possible to lock
 */
/*==========================================================================*/
bool ReadWriteLock::tryLockWrite()
{
    kvs::MutexLocker locker( &m_mutex );

    if ( m_counter != 0 ) { return false; }

    m_counter = -1;

    return true;
}

/*==========================================================================*/
/**
 *  @brief  Unlock.
 */
/*==========================================================================*/
void ReadWriteLock::unlock()
{
    kvs::MutexLocker locker( &m_mutex );

    if ( m_counter < 0 ) { m_counter = 0; }
    else { m_counter--; }

    bool ww = ( m_nwriters && m_counter == 0 );
    bool wr = ( m_nwriters == 0 );

    if ( ww ) { m_writer.wakeUpOne(); }
    else if ( wr ) { m_reader.wakeUpAll(); }
}

} // end of namespace kvs
