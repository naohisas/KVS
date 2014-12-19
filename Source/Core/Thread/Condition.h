/****************************************************************************/
/**
 *  @file Condition.h
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: Condition.h 1372 2012-11-30 00:27:21Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#ifndef KVS__CONDITION_H_INCLUDE
#define KVS__CONDITION_H_INCLUDE

#include <kvs/Platform>
#include "Mutex.h"

#if defined ( KVS_PLATFORM_WINDOWS )
#include <windows.h>
#endif


namespace kvs
{

/*==========================================================================*/
/**
 *  @brief  Condition class.
 */
/*==========================================================================*/
class Condition
{
public:

#if defined ( KVS_PLATFORM_WINDOWS )
    struct Handler
    {
        enum
        {
            WakeUpOne = 0,
            WakeUpAll,
            NumberOfEvents
        };

        HANDLE event[NumberOfEvents];
    };
#else
    typedef pthread_cond_t Handler;
#endif

private:

    int m_nsleepers; ///< number of sleep threads
    kvs::Mutex m_mutex; ///< mutex
    Handler m_handler; ///< handler

public:

    Condition();
    virtual ~Condition();

    void wakeUpOne();
    void wakeUpAll();
    bool wait( kvs::Mutex* mutex );
    bool wait( kvs::Mutex* mutex, int msec );

protected:

    void create_condition_variable();
    void delete_condition_variable();
};

} // end of namespace kvs

#endif // KVS__CONDITION_H_INCLUDE
