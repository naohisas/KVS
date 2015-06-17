/*****************************************************************************/
/**
 *  @file   SharedCount.h
 *  @author Naoya Maeda
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id$
 */
/*****************************************************************************/
#ifndef KVS_SHARED_COUNT_H_INCLUDE
#define KVS_SHARED_COUNT_H_INCLUDE

#if 1
  #if defined KVS_COMPILER_GCC
    #define KVS_ATOMIC_INCREMENT( a ) __sync_add_and_fetch( &( a ), 1 )
    #define KVS_ATOMIC_DECREMENT( a ) __sync_sub_and_fetch( &( a ), 1 )
    #define KVS_ATOMIC_COMPARE_SWAP( a, b, c ) __sync_val_compare_and_swap( &( a ), ( b ), ( c ) )
  #elif defined KVS_COMPILER_VC
    #include <intrin.h>
    #define KVS_ATOMIC_INCREMENT( a ) _InterlockedIncrement( &( a ) )
    #define KVS_ATOMIC_DECREMENT( a ) _InterlockedDecrement( &( a ) )
    #define KVS_ATOMIC_COMPARE_SWAP( a, b, c ) _InterlockedCompareExchange( &( a ), ( c ), ( b ) )
  #else
    #define KVS_ATOMIC_INCREMENT( a ) ( ++( a ) )
    #define KVS_ATOMIC_DECREMENT( a ) ( --( a ) )
    #define KVS_ATOMIC_COMPARE_SWAP( a, b, c ) _kvs_non_atomic_compare_swap( &( a ), ( b ), ( c ) )
  #endif
#else
  #define KVS_ATOMIC_INCREMENT( a ) ( ++( a ) )
  #define KVS_ATOMIC_DECREMENT( a ) ( --( a ) )
  #define KVS_ATOMIC_COMPARE_SWAP( a, b, c ) _kvs_non_atomic_compare_swap( &( a ), ( b ), ( c ) )
#endif

#include <typeinfo>

namespace kvs
{

namespace detail
{

namespace shared_pointer_impl
{

inline long _kvs_non_atomic_compare_swap( long* ptr, long oldval, long newval )
{
    long old = *ptr;
    if ( old == oldval ) { *ptr = newval; }
    return old;
}

// Implementation of shared count.
// A instance of this class contain a pointer to an instance and 
// manage the shared count and weak count of it.
class Counter
{
protected:
// Constructs a instance which contain 'ptr' and manages the life-time of the instance to which 'ptr' is indicating.
// The instance to which 'ptr' is indicating is automatically deleted when the shared count becomes 0.
// And the instance of this class is automatcally deleted when the weak count becomes 0.
// In the beginning, the weak count is 1, which is decremented when the shared count becomes 0.
    explicit Counter( void* ptr )
        : m_pointer( ptr )
        , m_count( 1 )
        , m_weak_count( 1 )
    {}

    virtual ~Counter()
    {}

public:
// Returns the count of shared.
    long use_count() const
    {
        return m_count;
    }

// Returns the pointer to the deleter if given type is correct.
    virtual void* deleter( const std::type_info& )
    {
        return NULL;
    }

public:
// Inclements the shared count.
    static void increment( Counter* counter )
    {
        KVS_ATOMIC_INCREMENT( counter->m_count );
    }

// Decrements the shared count.
// When the count become 0, the weak count is also decremented.
    static void decrement( Counter* counter )
    {
        KVS_ATOMIC_DECREMENT( counter->m_count );
        if ( counter->m_count == 0 )
        {
            counter->dispose();
            decrement_weak( counter );
        }
    }

// Increments the weak count.
    static void increment_weak( Counter* counter )
    {
        KVS_ATOMIC_INCREMENT( counter->m_weak_count );
    }

// Decrements the weak count.
// When the weak count becomes 0, this instance is deleted.
    static void decrement_weak( Counter* counter )
    {
        KVS_ATOMIC_DECREMENT( counter->m_weak_count );
        if ( counter->m_weak_count == 0 )
        {
            delete counter;
        }
    }

// Increments the shared count if the instance is not expired.
// Returns true if the count is incremented.
    static bool increment_if_not_expired( Counter* counter )
    {
        for ( ;; )
        {
            long count = ( volatile long& )( counter->m_count );
            if ( count == 0 )
                return false;
            if ( KVS_ATOMIC_COMPARE_SWAP( counter->m_count, count, count + 1 ) == count )
                return true;
        }
    }

private:
    virtual void dispose() = 0;

protected: // for dispose()
    void* m_pointer;   // pointer to a instance

private:
    long m_count;      // number of shared count
    long m_weak_count; // number of weak count + ( m_count != 0 ? 1 : 0 )

private:
    Counter( const Counter& );
    Counter& operator =( const Counter& );
}; // Counter

#undef KVS_ATOMIC_INCREMENT
#undef KVS_ATOMIC_DECREMENT
#undef KVS_ATOMIC_COMPARE_SWAP


template <typename T>
class DefaultCounter : public Counter
{
public:
    DefaultCounter( T* ptr )
        : Counter( reinterpret_cast<void*>( ptr ) )
    {}

private:
// Delete the managing instance using delete.
    void dispose()
    {
        delete reinterpret_cast<T*>( m_pointer );
    }
}; // DefaultCounter



template <typename T, typename D>
class CounterWithDeleter : public Counter
{
public:
// 'deleter' must be copy-constructable.
    CounterWithDeleter( T* ptr, D deleter )
        : Counter( reinterpret_cast<void*>( ptr ) )
        , m_deleter( deleter )
    {}

private:
    void* deleter( const std::type_info& type )
    {
        return type == typeid( D ) ? reinterpret_cast<void*>( &m_deleter ) : NULL;
    }

// Delete the managing instance using deleter specified at constructor.
    void dispose()
    {
        m_deleter( reinterpret_cast<T*>( m_pointer ) );
    }

private:
    D m_deleter;
}; // CounterWithDeleter



class WeakCount;

// Shared Count class
// This class hold a pointer to shared count and manage 
// the operation against the count.
// 
class SharedCount
{
public:
    SharedCount()
        : m_counter( NULL )
    {}

    // delete ptr; must be well-formed.
    template <typename T>
    explicit SharedCount( T* ptr )
    {
        try
        {
            m_counter = new DefaultCounter<T>( ptr );
        }
        catch ( ... )
        {
            delete ptr;
            throw;
        }
    }

    // deleter( ptr ); must be well-formed.
    // 'deleter' must be copy-constructable. Copy of it is used.
    template <typename T, typename D>
    SharedCount( T* ptr, D deleter )
    {
        try
        {
            m_counter = new CounterWithDeleter<T, D>( ptr, deleter );
        }
        catch ( ... )
        {
            deleter( ptr );
            throw;
        }
    }

    SharedCount( const SharedCount& other )
        : m_counter( other.m_counter )
    {
        if ( m_counter ) { Counter::increment( m_counter ); }
    }

    explicit SharedCount( const WeakCount& other );

    ~SharedCount()
    {
        if ( m_counter ) { Counter::decrement( m_counter ); }
    }

public:
    void swap( SharedCount& other )
    {
        std::swap( m_counter, other.m_counter );
    }

    long use_count() const
    {
        return m_counter ? m_counter->use_count() : 0;
    }

    bool is_valid() const
    {
        return m_counter != NULL;
    }

    void* deleter( const std::type_info& type ) const
    {
        return m_counter ? m_counter->deleter( type ) : NULL;
    }

private:
    Counter* m_counter;

    friend class WeakCount;

    SharedCount& operator =( const SharedCount& );
}; // SharedCount



class WeakCount
{
public:
    WeakCount()
        : m_counter( NULL )
    {}

    explicit WeakCount( const SharedCount& other )
        : m_counter( other.m_counter )
    {
        if ( m_counter ) { Counter::increment_weak( m_counter ); }
    }

    WeakCount( const WeakCount& other )
        : m_counter( other.m_counter )
    {
        if ( m_counter ) { Counter::increment_weak( m_counter ); }
    }

    ~WeakCount()
    {
        if ( m_counter ) { Counter::decrement_weak( m_counter ); }
    }

public:
    void swap( WeakCount& other )
    {
        std::swap( m_counter, other.m_counter );
    }

    long use_count() const
    {
        return m_counter ? m_counter->use_count() : 0;
    }

    bool expired() const
    {
        return !m_counter || m_counter->use_count() == 0;
    }

private:
    Counter* m_counter;

    friend class SharedCount;

    WeakCount& operator =( const WeakCount& );
}; // WeakCount



inline SharedCount::SharedCount( const WeakCount& other )
    : m_counter( NULL )
{
    if ( other.m_counter && Counter::increment_if_not_expired( other.m_counter ) )
    {
        // Copy if the instance is not expired.
        m_counter = other.m_counter;
    }
}

} // shared_pointer_impl

} // detail

} // kvs

#endif
