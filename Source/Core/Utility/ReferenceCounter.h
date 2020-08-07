/****************************************************************************/
/**
 *  @file   ReferenceCounter.h
 *  @author Naohisa Sakamoto
 */
/****************************************************************************/
#pragma once
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
    mutable kvs::Mutex m_key; ///< lock key
#endif
    size_t m_value; ///< counter

public:
    explicit ReferenceCounter( size_t value = 0 );
    ~ReferenceCounter( void );

public:
    void setValue( size_t value );
    size_t value() const;

#if defined ( KVS_ENABLE_THREAD_SAFE )
    kvs::Mutex& key() const;
#endif

public:
    void increment();
    void decrement();
};

} // end of namespace kvs
