/****************************************************************************/
/**
 *  @file   Mutex.h
 *  @author Naohisa Sakamoto
 */
/****************************************************************************/
#ifndef KVS__MUTEX_H_INCLUDE
#define KVS__MUTEX_H_INCLUDE

#include <kvs/Platform>
#if defined ( KVS_PLATFORM_WINDOWS )
#include <windows.h>
#include <errno.h>
#include <process.h>
#else
#include <pthread.h>
#endif


namespace kvs
{

/*==========================================================================*/
/**
 *  Mutex class.
 */
/*==========================================================================*/
class Mutex
{
public:

#if defined ( KVS_PLATFORM_WINDOWS )
    typedef HANDLE Handler;
#else
    typedef pthread_mutex_t Handler;
#endif

private:

    Handler m_handler; ///< mutex handler

public:

    Mutex();
    virtual ~Mutex();

    Handler& handler();
    const Handler& handler() const;

    void lock();
    void unlock();
    bool tryLock();

protected:

    void create_mutex();
    void delete_mutex();
};

} // end of namespace kvs

#endif // KVS_CORE_MUTEX_H_INCLUDE
