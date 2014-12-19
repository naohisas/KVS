/****************************************************************************/
/**
 *  @file ReadLocker.h
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: ReadLocker.h 1372 2012-11-30 00:27:21Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#ifndef KVS__READ_LOCKER_H_INCLUDE
#define KVS__READ_LOCKER_H_INCLUDE


namespace kvs
{

class ReadWriteLock;

/*==========================================================================*/
/**
 *  Read locker class.
 */
/*==========================================================================*/
class ReadLocker
{
private:

    kvs::ReadWriteLock* m_rwlock; ///< pointer to read-write lock

public:

    ReadLocker( kvs::ReadWriteLock* rwlock );
    virtual ~ReadLocker();

    void relock();
    void unlock();
    kvs::ReadWriteLock* readWriteLock();
};

} // end of namespace kvs

#endif // KVS__READ_LOCKER_H_INCLUDE
