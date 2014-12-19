/****************************************************************************/
/**
 *  @file Mutex.cpp
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: Mutex.cpp 1372 2012-11-30 00:27:21Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#include "Mutex.h"
#include <kvs/Message>
#if defined ( KVS_PLATFORM_WINDOWS )
#include <windows.h>
#include <errno.h>
#include <process.h>
#else
#include <pthread.h>
#include <sys/time.h>
#endif


namespace kvs
{

/*==========================================================================*/
/**
 *  @brief  Constructs a new Mutex class.
 */
/*==========================================================================*/
Mutex::Mutex()
{
    this->create_mutex();
}

/*==========================================================================*/
/**
 *  @brief  Destructs the Mutex class.
 */
/*==========================================================================*/
Mutex::~Mutex()
{
    this->delete_mutex();
}

/*==========================================================================*/
/**
 *  @brief  Returns mutex hundler.
 *  @return mutex hundler
 */
/*==========================================================================*/
Mutex::Handler& Mutex::handler()
{
    return m_handler;
}

/*==========================================================================*/
/**
 *  @brief  Returns mutex hundler.
 *  @return mutex hundler
 */
/*==========================================================================*/
const Mutex::Handler& Mutex::handler() const
{
    return m_handler;
}

/*==========================================================================*/
/**
 *  @brief  Lock.
 */
/*==========================================================================*/
void Mutex::lock()
{
#if defined ( KVS_PLATFORM_WINDOWS )
    WaitForSingleObject( m_handler, INFINITE );
#else
    pthread_mutex_lock( &m_handler );
#endif
}

/*==========================================================================*/
/**
 *  @brief  Unlock.
 */
/*==========================================================================*/
void Mutex::unlock()
{
#if defined ( KVS_PLATFORM_WINDOWS )
    ReleaseMutex( m_handler );
#else
    pthread_mutex_unlock( &m_handler );
#endif
}

/*==========================================================================*/
/**
 *  @brief  Test whether it is possible to lock.
 *  @return true, if it is possible to lock
 */
/*==========================================================================*/
bool Mutex::tryLock()
{
#if defined ( KVS_PLATFORM_WINDOWS )
    if ( WaitForSingleObject( m_handler, 0 ) != WAIT_OBJECT_0 )
    {
        kvsMessageError( "Mutex lock test failure." );
        return false;
    }
#else
    if ( pthread_mutex_trylock( &m_handler ) != 0 )
    {
        kvsMessageError( "Mutex lock test failure." );
        return false;
    }
#endif
    return true;
}

/*==========================================================================*/
/**
 *  @brief  Create the mutex.
 */
/*==========================================================================*/
void Mutex::create_mutex()
{
#if defined ( KVS_PLATFORM_WINDOWS )
    m_handler = CreateMutex( NULL, FALSE, NULL );
#else
    pthread_mutex_init( &m_handler, NULL );
#endif
}

/*==========================================================================*/
/**
 *  @brief  Delete the mutex.
 */
/*==========================================================================*/
void Mutex::delete_mutex()
{
#if defined ( KVS_PLATFORM_WINDOWS )
    if ( m_handler ) { CloseHandle( m_handler ); }
#else
    pthread_mutex_destroy( &m_handler );
#endif
}

} // end of namespace kvs
