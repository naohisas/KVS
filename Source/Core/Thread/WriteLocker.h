/****************************************************************************/
/**
 *  @file   WriteLocker.h
 *  @author Naohisa Sakamoto
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
