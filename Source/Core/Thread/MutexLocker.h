/****************************************************************************/
/**
 *  @file MutexLocker.h
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: MutexLocker.h 1372 2012-11-30 00:27:21Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#ifndef KVS__MUTEX_LOCKER_H_INCLUDE
#define KVS__MUTEX_LOCKER_H_INCLUDE


namespace kvs
{

class Mutex;

/*==========================================================================*/
/**
 *  @brief  Mutex locker class.
 */
/*==========================================================================*/
class MutexLocker
{
private:

    kvs::Mutex* m_mutex; ///< pointer to mutex

public:

    MutexLocker( kvs::Mutex* mutex );
    virtual ~MutexLocker();

    void relock();
    void unlock();
    Mutex* mutex();
};

} // end of namespace kvs

#endif // KVS__MUTEX_LOCKER_H_INCLUDE
