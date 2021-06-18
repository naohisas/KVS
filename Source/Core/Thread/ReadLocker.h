/****************************************************************************/
/**
 *  @file   ReadLocker.h
 *  @author Naohisa Sakamoto
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
