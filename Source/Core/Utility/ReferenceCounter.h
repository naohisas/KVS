/****************************************************************************/
/**
 *  @file ReferenceCounter.h
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: ReferenceCounter.h 1707 2014-01-27 07:37:04Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#ifndef KVS__REFERENCE_COUNTER_H_INCLUDE
#define KVS__REFERENCE_COUNTER_H_INCLUDE

#include <cstddef>
#if defined ( KVS_ENABLE_THREAD_SAFE )
#include <kvs/Mutex>
#endif


namespace kvs
{

/*==========================================================================*/
/**
 *  Reference counter class.
 */
/*==========================================================================*/
class ReferenceCounter
{
private:

#if defined ( KVS_ENABLE_THREAD_SAFE )
    mutable kvs::Mutex m_key;   ///< lock key
#endif
    size_t             m_value; ///< counter

public:

    explicit ReferenceCounter( size_t value = 0 );

    ~ReferenceCounter( void );

public:

    void setValue( size_t value );

    size_t value( void ) const;

#if defined ( KVS_ENABLE_THREAD_SAFE )
    kvs::Mutex& key( void ) const;
#endif

public:

    void increment( void );

    void decrement( void );
};

} // end of namespace kvs

#endif // KVS__REFERENCE_COUNTER_H_INCLUDE
