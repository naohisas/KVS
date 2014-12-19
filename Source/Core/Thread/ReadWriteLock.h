/****************************************************************************/
/**
 *  @file ReadWriteLock.h
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: ReadWriteLock.h 1372 2012-11-30 00:27:21Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#ifndef KVS__READ_WRITE_LOCK_H_INCLUDE
#define KVS__READ_WRITE_LOCK_H_INCLUDE

#include "Mutex.h"
#include "Condition.h"


namespace kvs
{

/*==========================================================================*/
/**
 *  Read/Write lock class.
 */
/*==========================================================================*/
class ReadWriteLock
{
private:

    int m_counter; ///< access counter
    int m_nreaders; ///< number of waiting readers
    int m_nwriters; ///< number of waiting writers
    kvs::Mutex m_mutex; ///< mutex
    kvs::Condition m_reader; ///< condition variable for reader
    kvs::Condition m_writer; ///< condition variable for writer

public:

    ReadWriteLock();
    virtual ~ReadWriteLock();

    void lockRead();
    void lockWrite();
    bool tryLockRead();
    bool tryLockWrite();
    void unlock();
};

} // end of namespace kvs

#endif // KVS__READ_WRITE_LOCK_H_INCLUDE
