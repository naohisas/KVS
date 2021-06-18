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
 *  Constructor.
 *  @param value [in] counter value
 */
/*==========================================================================*/
ReferenceCounter::ReferenceCounter( size_t value )
    : m_value( value )
{
}

/*==========================================================================*/
/**
 *  Destructor.
 */
/*==========================================================================*/
ReferenceCounter::~ReferenceCounter()
{
}

/*==========================================================================*/
/**
 *  Set a counter value.
 *  @param value [in] counter value
 */
/*==========================================================================*/
void ReferenceCounter::setValue( size_t value )
{
    m_value = value;
}

/*==========================================================================*/
/**
 *  Get a counter value.
 *  @return counter value
 */
/*==========================================================================*/
size_t ReferenceCounter::value() const
{
    return( m_value );
}

#if defined ( KVS_ENABLE_THREAD_SAFE )
/*==========================================================================*/
/**
 *  Get a mutex lock key.
 *  @return mutex lock key
 */
/*==========================================================================*/
kvs::Mutex& ReferenceCounter::key() const
{
    return( m_key );
}
#endif

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
