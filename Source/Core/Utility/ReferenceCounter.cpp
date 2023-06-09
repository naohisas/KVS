/****************************************************************************/
/**
 *  @file   ReferenceCounter.cpp
 *  @author Naohisa Sakamoto
 */
/****************************************************************************/
#include "ReferenceCounter.h"
#if defined ( KVS_ENABLE_THREAD_SAFE )
#include <kvs/Mutex>
#include <kvs/MutexLocker>
#endif


namespace kvs
{

/*==========================================================================*/
/**
 *  Increment the renference counter.
 */
/*==========================================================================*/
void ReferenceCounter::increment()
{
#if defined ( KVS_ENABLE_THREAD_SAFE )
    kvs::MutexLocker lock( &m_key );
#endif
    ++m_value;
}

/*==========================================================================*/
/**
 *  Decrement the reference counter.
 */
/*==========================================================================*/
void ReferenceCounter::decrement()
{
#if defined ( KVS_ENABLE_THREAD_SAFE )
    kvs::MutexLocker lock( &m_key );
#endif
    --m_value;
}

} // end of namespace kvs
