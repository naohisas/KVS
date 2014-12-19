/****************************************************************************/
/**
 *  @file Condition.cpp
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: Condition.cpp 1372 2012-11-30 00:27:21Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#include "Condition.h"
#include "MutexLocker.h"
#if defined ( KVS_PLATFORM_WINDOWS )
#include <windows.h>
#include <errno.h>
#include <process.h>
#else
#include <pthread.h>
#include <sys/errno.h>
#include <sys/time.h>
#endif


namespace kvs
{

/*==========================================================================*/
/**
 *  @brief  Constructs a new Condition class.
 */
/*==========================================================================*/
Condition::Condition():
    m_nsleepers( 0 )
{
    this->create_condition_variable();
}

/*==========================================================================*/
/**
 *  @brief  Destructs the Condition class.
 */
/*==========================================================================*/
Condition::~Condition()
{
    this->delete_condition_variable();
}

/*==========================================================================*/
/**
 *  @brief  Wake up a thread.
 */
/*==========================================================================*/
void Condition::wakeUpOne()
{
    kvs::MutexLocker locker( &m_mutex );

#if defined ( KVS_PLATFORM_WINDOWS )
    if ( m_nsleepers > 0 ) { SetEvent( m_handler.event[Handler::WakeUpOne] ); }
#else
    pthread_cond_signal( &m_handler );
#endif
}

/*==========================================================================*/
/**
 *  @brief  Wake up all threads.
 */
/*==========================================================================*/
void Condition::wakeUpAll()
{
    kvs::MutexLocker locker( &m_mutex );

#if defined ( KVS_PLATFORM_WINDOWS )
    if ( m_nsleepers > 0 ) { SetEvent( m_handler.event[Handler::WakeUpAll] ); }
#else
    pthread_cond_broadcast( &m_handler );
#endif
}

/*==========================================================================*/
/**
 *  @brief  Wait.
 *  @param  mutex [in] pointer to the mutex
 *  @return true, if the process is done successfully
 */
/*==========================================================================*/
bool Condition::wait( kvs::Mutex* mutex )
{
    if ( !mutex ) { return false; }

    m_mutex.lock();
    m_nsleepers += 1;

    mutex->unlock();

#if defined ( KVS_PLATFORM_WINDOWS )
    m_mutex.unlock();
    int err = WaitForMultipleObjects( 2, m_handler.event, FALSE, INFINITE );

    m_mutex.lock();
    m_nsleepers--;

    int  last = ( err == WAIT_OBJECT_0 + Handler::WakeUpAll ) && ( m_nsleepers == 0 );
    if ( last ) { ResetEvent( m_handler.event[Handler::WakeUpAll] ); }

    bool ret = err != WAIT_FAILED;
#else
    int err = pthread_cond_wait( &m_handler, &m_mutex.handler() );
    m_nsleepers--;

    bool ret = err == 0;
#endif

    m_mutex.unlock();
    mutex->lock();

    return ret;
}

/*==========================================================================*/
/**
 *  @brief  Wait.
 *  @param  mutex [in] pointer to the mutex
 *  @param  msec [in] waiting time in mili-second
 *  @return true, if the process is done successfully
 */
/*==========================================================================*/
bool Condition::wait( kvs::Mutex* mutex, int msec )
{
    if ( !mutex ) { return( false ); }

    m_mutex.lock();
    m_nsleepers++;

    mutex->unlock();

#if defined ( KVS_PLATFORM_WINDOWS )
    m_mutex.unlock();
    int err = WaitForMultipleObjects( 2, m_handler.event, FALSE, msec );

    m_mutex.lock();
    m_nsleepers--;

    int  last = ( err == WAIT_OBJECT_0 + Handler::WakeUpAll ) && ( m_nsleepers == 0 );
    if ( last ) { ResetEvent( m_handler.event[Handler::WakeUpAll] ); }

    bool ret = err != WAIT_FAILED;
#else
    struct timeval tv;
    gettimeofday( &tv, 0 );

    timespec ti;
    ti.tv_nsec  = ( tv.tv_usec + ( msec % 1000 ) * 1000 ) * 1000;
    ti.tv_sec   = tv.tv_sec + ( msec / 1000 ) + ( ti.tv_nsec / 1000000000 );
    ti.tv_nsec %= 1000000000;

    int err = pthread_cond_timedwait( &m_handler, &m_mutex.handler(), &ti );
    m_nsleepers--;

    bool ret = err == 0;
#endif

    m_mutex.unlock();
    mutex->lock();

    return ret;
}

/*==========================================================================*/
/**
 *  @brief  Create condition variable.
 */
/*==========================================================================*/
void Condition::create_condition_variable()
{
#if defined ( KVS_PLATFORM_WINDOWS )
    m_handler.event[Handler::WakeUpOne] = CreateEvent( NULL, FALSE, FALSE, NULL );
    m_handler.event[Handler::WakeUpAll] = CreateEvent( NULL, TRUE,  FALSE, NULL );
#else
    pthread_cond_init( &m_handler, NULL );
#endif
}

/*==========================================================================*/
/**
 *  @brief  Delete condition variable.
 */
/*==========================================================================*/
void Condition::delete_condition_variable()
{
#if defined ( KVS_PLATFORM_WINDOWS )
    if ( m_handler.event[Handler::WakeUpOne] )
    {
        CloseHandle( m_handler.event[Handler::WakeUpOne] );
    }

    if ( m_handler.event[Handler::WakeUpAll] )
    {
        CloseHandle( m_handler.event[Handler::WakeUpAll] );
    }
#else
    pthread_cond_destroy( &m_handler );
#endif
}

} // end of namespace kvs
