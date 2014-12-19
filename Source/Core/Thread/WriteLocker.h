/****************************************************************************/
/**
 *  @file WriteLocker.h
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: WriteLocker.h 1372 2012-11-30 00:27:21Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#ifndef KVS__WRITE_LOCKER_H_INCLUDE
#define KVS__WRITE_LOCKER_H_INCLUDE


namespace kvs
{

class ReadWriteLock;

/*==========================================================================*/
/**
 *  @brief  Write locker class.
 */
/*==========================================================================*/
class WriteLocker
{
private:

    kvs::ReadWriteLock* m_rwlock; ///< pointer to read-write lock

public:

    WriteLocker( kvs::ReadWriteLock* rwlock );
    virtual ~WriteLocker();

    void relock();
    void unlock();
    kvs::ReadWriteLock* readWriteLock();
};

} // end of namespace kvs

#endif // KVS__WRITE_LOCKER_H_INCLUDE
