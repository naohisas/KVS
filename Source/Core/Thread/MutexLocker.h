/****************************************************************************/
/**
 *  @file   MutexLocker.h
 *  @author Naohisa Sakamoto
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
