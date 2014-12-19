/****************************************************************************/
/**
 *  @file Thread.cpp
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: Thread.cpp 1386 2012-12-04 09:38:12Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#include "Thread.h"
#include <kvs/Message>
#if defined ( KVS_PLATFORM_WINDOWS )
#include <windows.h>
#include <errno.h>
#include <process.h>
#else
#include <pthread.h>
#include <sys/time.h>
#endif


namespace
{

/*==========================================================================*/
/**
 *  @brief  Entry point for kvs::Thread::run method.
 *  @param  arg [in] pointer to arguments of thread function
 *  @return NULL
 */
/*==========================================================================*/
void* EntryPoint( void* arg )
{
    ( (kvs::Thread*)arg )->run();
    return NULL;
}

#if !defined ( KVS_PLATFORM_WINDOWS )
/*==========================================================================*/
/**
 *  @brief  Sleep thread.
 *  @param  t [in] sleep time
 */
/*==========================================================================*/
void SleepThread( struct timespec* t )
{
    pthread_mutex_t m;
    pthread_cond_t  c;

    pthread_mutex_init( &m, 0 );
    pthread_cond_init( &c, 0 );

    pthread_mutex_lock( &m );
    pthread_cond_timedwait( &c, &m, t );
    pthread_mutex_unlock( &m );

    pthread_cond_destroy( &c );
    pthread_mutex_destroy( &m );
}
#endif
} // end of namesapce


namespace kvs
{

/*==========================================================================*/
/**
 *  @brief  Sleep the thread in seconds.
 *  @param  sec [in] second
 */
/*==========================================================================*/
void Thread::Sleep( const int sec )
{
#if defined ( KVS_PLATFORM_WINDOWS )
    ::Sleep( sec * 1000 );
#else
    struct timeval tv;
    gettimeofday( &tv, 0 );
    struct timespec ts;
    ts.tv_sec  = tv.tv_sec + sec;
    ts.tv_nsec = ( tv.tv_usec * 1000 );
    ::SleepThread( &ts );
#endif
}

/*==========================================================================*/
/**
 *  @brief  Sleep the thread in milli-seconds.
 *  @param  msec [in] milli-second
 */
/*==========================================================================*/
void Thread::MilliSleep( const int msec )
{
#if defined ( KVS_PLATFORM_WINDOWS )
    ::Sleep( msec );
#else
    struct timeval tv;
    gettimeofday( &tv, 0 );
    struct timespec ts;
    ts.tv_nsec  = ( tv.tv_usec + ( msec % 1000 ) * 1000 ) * 1000;
    ts.tv_sec   = tv.tv_sec + ( msec / 1000 ) + ( ts.tv_nsec / 1000000000 );
    ts.tv_nsec %= 1000000000;
    ::SleepThread( &ts );
#endif
}

/*==========================================================================*/
/**
 *  @brief  Sleep the thread in micro-seconds.
 *  @param  usec [in] micro-second
 */
/*==========================================================================*/
void Thread::MicroSleep( const int usec )
{
#if defined ( KVS_PLATFORM_WINDOWS )
    ::Sleep( ( usec / 1000 ) + 1 );
#else
    struct timeval tv;
    gettimeofday( &tv, 0 );
    struct timespec ts;
    ts.tv_nsec  = ( tv.tv_usec + ( usec % 1000000 ) ) * 1000;
    ts.tv_sec   = tv.tv_sec + ( usec / 1000000 ) + ( ts.tv_nsec / 1000000000 );
    ts.tv_nsec %= 1000000000;
    ::SleepThread( &ts );
#endif
}

/*==========================================================================*/
/**
 *  @brief  Constructs a new Thread class.
 */
/*==========================================================================*/
Thread::Thread():
    m_is_running( false ),
    m_handler( 0 ),
    m_routine( NULL )
{
}

/*==========================================================================*/
/**
 *  @brief  Constructs a new Thread class.
 *  @param  routine [in] routine function for thread
 */
/*==========================================================================*/
Thread::Thread( Routine routine ):
    m_is_running( false ),
    m_handler( 0 ),
    m_routine( routine )
{
}

/*==========================================================================*/
/**
 *  @brief  Destructs the Thread class.
 */
/*==========================================================================*/
Thread::~Thread()
{
    this->delete_thread();
}

/*==========================================================================*/
/**
 *  @brief  Returns thread hundler.
 *  @return thread hundler
 */
/*==========================================================================*/
Thread::Handler& Thread::handler()
{
    return m_handler;
}

/*==========================================================================*/
/**
 *  @brief  Returns thread hundler.
 *  @return thread hundler
 */
/*==========================================================================*/
const Thread::Handler& Thread::handler() const
{
    return m_handler;
}

/*==========================================================================*/
/**
 *  @brief  Test whether the thread is running.
 *  @return true, if the thread is running
 */
/*==========================================================================*/
bool Thread::isRunning() const
{
    return m_is_running;
}

/*==========================================================================*/
/**
 *  @brief  Start the thread.
 *  @return true, if the process is done succussfully
 */
/*==========================================================================*/
bool Thread::start()
{
    Routine routine;
    if ( m_routine == NULL ) { routine = ( Routine ) ::EntryPoint; }
    else { routine = m_routine; }

    if ( !this->create_thread( routine, this ) )
    {
        kvsMessageError( "Cannot create thread." );
        m_is_running = false;

        return false;
    }

    m_is_running = true;

    return true;
}

/*==========================================================================*/
/**
 *  @brief  Wait the thread.
 *  @return true, if the process is done successfully
 */
/*==========================================================================*/
bool Thread::wait()
{
#if defined ( KVS_PLATFORM_WINDOWS )
    int ret = WaitForSingleObject( m_handler, INFINITE );
    return ret == WAIT_FAILED ? false : true;
#else
    int ret = pthread_join( m_handler, NULL );
    return ret == 0 ? true : false;
#endif
}

/*==========================================================================*/
/**
 *  @brief  Quit the thread.
 */
/*==========================================================================*/
void Thread::quit()
{
    this->delete_thread();
}

/*==========================================================================*/
/**
 *  Create the thread.
 *  @param routine [in] routine function
 *  @param arg [in] argument of the routine function
 *  @return true, if the process is done successfully
 */
/*==========================================================================*/
bool Thread::create_thread( Routine routine, void* arg )
{
#if defined ( KVS_PLATFORM_WINDOWS )
    WinRoutine rt = (WinRoutine)routine;
    m_handler = (HANDLE)_beginthreadex( NULL, 0, rt, arg, 0, NULL );
    return( m_handler == 0 ? false : true );
#else
    int err = pthread_create( &m_handler, NULL, routine, arg );
    return( err == 0 ? true : false );
#endif
}

/*==========================================================================*/
/**
 *  Delete thread.
 */
/*==========================================================================*/
void Thread::delete_thread()
{
#if defined ( KVS_PLATFORM_WINDOWS )
    if ( m_handler ) { CloseHandle( m_handler ); }
#else
#endif
}

} // end of namespace kvs
