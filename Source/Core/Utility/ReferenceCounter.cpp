/****************************************************************************/
/**
 *  @file ReferenceCounter.cpp
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: ReferenceCounter.cpp 631 2010-10-10 02:15:35Z naohisa.sakamoto $
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
ReferenceCounter::~ReferenceCounter( void )
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
size_t ReferenceCounter::value( void ) const
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
kvs::Mutex& ReferenceCounter::key( void ) const
{
    return( m_key );
}
#endif

/*==========================================================================*/
/**
 *  Increment the renference counter.
 */
/*==========================================================================*/
void ReferenceCounter::increment( void )
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
void ReferenceCounter::decrement( void )
{
#if defined ( KVS_ENABLE_THREAD_SAFE )
    kvs::MutexLocker lock( &m_key );
#endif
    --m_value;
}

} // end of namespace kvs
